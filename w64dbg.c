#define DASM_IMPLEMENTATION
#include "tp/x86_dasm_headeronly.h"

#include <windows.h>
#include <dbghelp.h>
#include <stdio.h>
#include <stdint.h>

typedef struct {
    LPVOID address;
    BYTE   original_byte;
} Breakpoint;

typedef struct {
    const char *file;
    PROCESS_INFORMATION process_info;
    HANDLE process;
    Breakpoint bp;
    BOOL stepping_over_breakpoint;
    DWORD stepping_thread;
    x86_dasm_context_t disasm;
} Debugger;
static Debugger dbg = {0};

BOOL dbg_open(const char *filepath)
{
    dbg.file = filepath;
    STARTUPINFOA startup_info = {0};
    startup_info.cb = sizeof(startup_info);
    if (!CreateProcessA(
        filepath,
        NULL,
        NULL,
        NULL,
        FALSE,
        DEBUG_PROCESS,
        NULL,
        NULL,
        &startup_info,
        &dbg.process_info))
    {
        fprintf(stderr, "ERROR: failed to create process from %s\n", filepath);
        return FALSE;
    }
    dbg.process = dbg.process_info.hProcess;
    SymSetOptions(SYMOPT_UNDNAME | SYMOPT_DEFERRED_LOADS);
    if(!SymInitialize(dbg.process, NULL, FALSE)) {
        fprintf(stderr, "WARN: failed to load debug symbols from %s\n", filepath);
    }
    return TRUE;
}

BOOL dbg_set_breakpoint(LPVOID address)
{
    BYTE original_byte = 0;
    if(!ReadProcessMemory(dbg.process_info.hProcess, address, &original_byte, 1, NULL)) {
        fprintf(stderr, "ERROR: failed to read byte at breakpoint address. code: %d\n", (int)GetLastError());
        return FALSE;
    }
    BYTE int3 = 0xCC;
    if (!WriteProcessMemory(dbg.process_info.hProcess, address, &int3, 1, NULL)) {
        printf("ERROR: Failed to write INT 3 instruction. code: %d\n", (int)GetLastError());
        return FALSE;
    }

    dbg.bp.address = address;
    dbg.bp.original_byte = original_byte;
    return TRUE;
}

BOOL dbg_restore_breakpoint(void)
{
    if(!WriteProcessMemory(
            dbg.process_info.hProcess, 
            dbg.bp.address,
            &dbg.bp.original_byte,
            1,
            NULL)) {
        printf("ERROR: Failed to restore original byte. code: %d\n",
                (int)GetLastError());
        return FALSE;
    }
    // FlushInstructionCache(dbg.process_info.hProcess, (LPCVOID)dbg.bp.address, 1);
    return TRUE;
}

void dbg_print_symbol(DWORD64 address)
{
    char buffer[sizeof(SYMBOL_INFO) + MAX_SYM_NAME];
    PSYMBOL_INFO symbol = (PSYMBOL_INFO)buffer;

    symbol->SizeOfStruct = sizeof(SYMBOL_INFO);
    symbol->MaxNameLen = MAX_SYM_NAME;

    DWORD64 displacement = 0;

    if (SymFromAddr(dbg.process, address, &displacement, symbol)) {
        printf("%s+0x%llx", symbol->Name, (unsigned long long)displacement);
    } else {
        printf("0x%llx", address);
    }
}

void dbg_print_current_location(DWORD threadId)
{
    HANDLE hThread = OpenThread(THREAD_ALL_ACCESS, FALSE, threadId);

    CONTEXT ctx = {0};
    ctx.ContextFlags = CONTEXT_ALL;
    GetThreadContext(hThread, &ctx);

    printf("%016llx",
           (unsigned long long)ctx.Rip);

    dbg_print_symbol(ctx.Rip);

    printf("\n");

    CloseHandle(hThread);
}

BOOL dbg_reapply_breakpoint(void)
{
    BYTE int3 = 0xCC;
    if (!WriteProcessMemory(dbg.process_info.hProcess, dbg.bp.address, &int3, 1, NULL)) {
        printf("ERROR: Failed to write INT 3 instruction. code: %d\n", (int)GetLastError());
        return FALSE;
    }
    return TRUE;
}

void dbg_single_step(DWORD threadId)
{
    HANDLE hThread = OpenThread(THREAD_ALL_ACCESS, FALSE, threadId);
    CONTEXT ctx = {0};
    ctx.ContextFlags = CONTEXT_ALL;
    GetThreadContext(hThread, &ctx);
    ctx.EFlags |= 0x100; // Trap flag
    SetThreadContext(hThread, &ctx);
    CloseHandle(hThread);
}

void dbg_print_registers(DWORD threadId)
{
    HANDLE hThread = OpenThread(THREAD_ALL_ACCESS, FALSE, threadId);
    CONTEXT ctx = {0};
    ctx.ContextFlags = CONTEXT_ALL;

    GetThreadContext(hThread, &ctx);

    printf("RIP: %016llx\n", ctx.Rip);
    printf("RSP: %016llx\n", ctx.Rsp);
    printf("RBP: %016llx\n", ctx.Rbp);
    printf("RAX: %016llx\n", ctx.Rax);
    printf("RBX: %016llx\n", ctx.Rbx);
    printf("RCX: %016llx\n", ctx.Rcx);
    printf("RDX: %016llx\n", ctx.Rdx);

    CloseHandle(hThread);
}

BOOL dbg_print_instruction(DWORD thread_id) 
{
    HANDLE hThread = OpenThread(THREAD_ALL_ACCESS, FALSE, thread_id);
    CONTEXT ctx = {0};
    ctx.ContextFlags = CONTEXT_ALL;
    GetThreadContext(hThread, &ctx);
    BYTE buf[16];
    if(!ReadProcessMemory(dbg.process_info.hProcess, (LPVOID)ctx.Rip, buf, sizeof(buf), NULL)) {
        fprintf(stderr, "ERROR: failed to read memory of process. code: %d\n", (int)GetLastError());
        return FALSE;
    }

    x86_set_buffer(&dbg.disasm, buf);
    x86_set_dmode(&dbg.disasm, X86_DMODE_64BIT);
    x86_set_ip(&dbg.disasm, ctx.Rip);
    if(x86_dasm(&dbg.disasm) < 0) {
        fprintf(stderr, "ERROR: Failed to disassemble\n");
        return FALSE;
    } else {
        dbg_print_symbol(ctx.Rip);
        printf(" | ");
        /* print the bytes */
        for(int j = 0; j < 16 - dbg.disasm.len; j++)
            printf("   ");
        for(int j = 0; j < dbg.disasm.len; j++)
            printf("%02X ", dbg.disasm.buffer[j]);
        /* print the decoded instruction */
        printf(" | %s\n", dbg.disasm.inst_str);
    }
    CloseHandle(hThread);
    return TRUE;
}

BOOL dbg_wait_for_debug_event(DWORD *stopped_thread)
{
    DEBUG_EVENT event;
    while(1) {
        WaitForDebugEvent(&event, INFINITE);
        DWORD c = DBG_CONTINUE;
        switch(event.dwDebugEventCode) {
            case CREATE_PROCESS_DEBUG_EVENT:
                dbg_set_breakpoint(event.u.CreateProcessInfo.lpStartAddress);
                break;
            case EXIT_PROCESS_DEBUG_EVENT:
                printf("Process exited.\n");
                return FALSE;
            case EXCEPTION_DEBUG_EVENT:
                {
                    DWORD code = event.u.Exception.ExceptionRecord.ExceptionCode;
                    if(code == EXCEPTION_BREAKPOINT) {
                        HANDLE hThread = OpenThread(THREAD_ALL_ACCESS, FALSE, event.dwThreadId);
                        CONTEXT ctx = {0};
                        ctx.ContextFlags = CONTEXT_ALL;
                        GetThreadContext(hThread, &ctx);
                        /*LPVOID address = (LPVOID)ctx.Rip - 1;*/
                        LPVOID address = event.u.Exception.ExceptionRecord.ExceptionAddress;
                        if(dbg.bp.address == address) {
                            dbg_print_instruction(event.dwThreadId);
                            printf("INFO: Breakpoint hit at address %p\n", address);
                            dbg_restore_breakpoint();
                            ctx.Rip -= 1;
                            SetThreadContext(hThread, &ctx);
                            CloseHandle(hThread);

                            dbg.stepping_over_breakpoint = TRUE;
                            dbg.stepping_thread = event.dwThreadId;
                            *stopped_thread = event.dwThreadId;
                            return TRUE;
                        }
                        CloseHandle(hThread);
                    } else if (code == EXCEPTION_SINGLE_STEP) {
                        if(dbg.stepping_over_breakpoint && event.dwThreadId == dbg.stepping_thread) {
                            dbg_reapply_breakpoint();
                            dbg.stepping_over_breakpoint = FALSE;
                        }
                        *stopped_thread = event.dwThreadId;
                        return TRUE;
                    } else {
                        printf("Exception: 0x%x\n", (unsigned int)code);
                    }
                } break;
            default:
                break;
        }
        ContinueDebugEvent(event.dwProcessId, event.dwThreadId, DBG_CONTINUE);
    }
}

BOOL CALLBACK EnumSymProc(
    PSYMBOL_INFO pSymInfo,
    ULONG SymbolSize,
    PVOID UserContext)
{
    printf("0x%llx %s\n",
        (unsigned long long)pSymInfo->Address,
        pSymInfo->Name);
    return TRUE;
}

char cmd[256];
void dbg_repl(DWORD thread_id)
{
    while(1) {
        printf("(w64dbg) ");
        if(!fgets(cmd, sizeof(cmd), stdin)) exit(0);
        if(strncmp(cmd, "quit", 4) == 0 || strncmp(cmd, "q", 1) == 0) {
            exit(0);
        } else if(strncmp(cmd, "continue", 8) == 0 || strncmp(cmd, "c", 1) == 0) {
            dbg_print_instruction(thread_id);
            ContinueDebugEvent(dbg.process_info.dwProcessId, thread_id, DBG_CONTINUE);
            return;
        } else if(strncmp(cmd, "symbols", 7) == 0 || strncmp(cmd, "sym", 3) == 0) {
            DWORD64 base = SymLoadModuleEx(
                dbg.process,
                NULL,
                dbg.file,
                NULL,
                0,
                0,
                NULL,
                0);
            if (!base) {
                printf("ERROR: SymLoadModuleEx failed\n");
            }
            // Enumerate ALL symbols
            if (!SymEnumSymbols(dbg.process, base, "*", EnumSymProc, NULL)) {
                printf("ERROR: SymEnumSymbols failed\n");
            }
            SymCleanup(dbg.process);
        } else if(strncmp(cmd, "step", 4) == 0 || strncmp(cmd, "s", 1) == 0) {
            dbg_print_instruction(thread_id);
            dbg_single_step(thread_id);
            ContinueDebugEvent(dbg.process_info.dwProcessId, thread_id, DBG_CONTINUE);
            return;
        } else if(strncmp(cmd, "regs", 4) == 0) {
            dbg_print_registers(thread_id);
        } else if(strncmp(cmd, "help", 4) == 0) {
            printf("(w64dbg) help\n");
            printf("List of Commands available in w64dbg\n");
            printf("help            Show this list\n");
            printf("regs            Print all registers\n");
            printf("step      s     Step to the next assembly instruction\n");
            printf("continue  c     Continue execution until breakpoint or any exception\n");
            printf("break     b     (todo) Set a breakpoint\n");
            printf("symbols   sym   List of all symbols in the executable\n");
            printf("eval      e     (todo) Evaluate expression. Syntax is as follows\n");
            printf("                'e rax'           This will shows the content of register rax\n");
            printf("                'e DWORD [rax]'   This will shows the DWORD content of memory addressed by rax\n");
            printf("                'e DWORD [rax+8]' This will shows the DWORD content of memory addressed by rax with offset +8\n");
            printf("                'e DWORD [rax:8]' This will shows the 8 DWORDs content of start by the address of rax.\n");

        } else {
            printf("Unknown command %.*s\n", (int)strlen(cmd) - 1, cmd);
        }
    }
}

int main(int argc, char *argv[])
{
    if(argc < 2) {
        fprintf(stderr, "ERROR: please provide the file to debug\n");
        fprintf(stderr, "USAGE: %s main.exe\n", argv[0]);
        return -1;
    }
    if(!dbg_open(argv[1])) return -1;

    DWORD thread_id;
    while(dbg_wait_for_debug_event(&thread_id)) {
        dbg_repl(thread_id);
    }

    return 0;
}
