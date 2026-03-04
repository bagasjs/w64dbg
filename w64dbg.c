#define DASM_IMPLEMENTATION
#include "tp/x86_dasm_headeronly.h"

#include <windows.h>
#include <dbghelp.h>
#include <stdio.h>
#include <stdint.h>
#include "utils.h"

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
    DWORD64 sym_module_base;
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
    return TRUE;
}

BOOL dbg_set_breakpoint(LPVOID address)
{
    BYTE original_byte = 0;
    if(!ReadProcessMemory(dbg.process, address, &original_byte, 1, NULL)) {
        fprintf(stderr, "ERROR: failed to read byte at breakpoint address. code: %d\n", (int)GetLastError());
        return FALSE;
    }
    BYTE int3 = 0xCC;
    if (!WriteProcessMemory(dbg.process, address, &int3, 1, NULL)) {
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
                {
                    LPVOID base = event.u.CreateProcessInfo.lpBaseOfImage;
                    SymSetOptions(SYMOPT_UNDNAME | SYMOPT_DEFERRED_LOADS);
                    if(!SymInitialize(dbg.process, NULL, FALSE)) {
                        fprintf(stderr, "WARN: failed to load debug symbols from %s\n", dbg.file);
                    }
                    dbg.sym_module_base = SymLoadModuleEx(
                            dbg.process,
                            NULL,
                            dbg.file,
                            NULL,
                            (DWORD64)base,
                            0,
                            NULL,
                            0);
                    if (!dbg.sym_module_base) {
                        printf("ERROR: SymLoadModuleEx failed\n");
                    }
                    dbg_set_breakpoint(event.u.CreateProcessInfo.lpStartAddress);
                } break;
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
                        exit(-1);
                    }
                } break;
            default:
                break;
        }
        ContinueDebugEvent(event.dwProcessId, event.dwThreadId, DBG_CONTINUE);
    }
}


typedef struct EnumSymProcUserContext {
    StringView symbol; // symbol to find
    ULONG64 found_address;
} EnumSymProcUserContext;

BOOL CALLBACK EnumSymProc(PSYMBOL_INFO pSymInfo, ULONG SymbolSize, PVOID UserContext)
{
    EnumSymProcUserContext *ctx = UserContext;
    StringView  haystack = sv_from_cstr(pSymInfo->Name);
    if(sv_eq(ctx->symbol, haystack)) {
        ctx->found_address = pSymInfo->Address;
    }
    return TRUE;
}

void dbg_step_out(DWORD thread_id)
{
    HANDLE hThread = OpenThread(THREAD_ALL_ACCESS, FALSE, thread_id);
    CONTEXT ctx = {0};
    ctx.ContextFlags = CONTEXT_ALL;
    GetThreadContext(hThread, &ctx);
    LPVOID retaddr = 0;
    if(!ReadProcessMemory(dbg.process, (LPVOID)ctx.Rsp, &retaddr, sizeof(retaddr), NULL)) {
        fprintf(stderr, "WARN: couldn't read return address\n");
        return;
    }
    BYTE step_out_orig_byte = 0;
    if(!ReadProcessMemory(dbg.process, retaddr, &step_out_orig_byte, 1, NULL)) {
        fprintf(stderr, "WARNING: failed to read byte at breakpoint address. code: %d\n", (int)GetLastError());
        return;
    }
    BYTE int3 = 0xCC;
    if (!WriteProcessMemory(dbg.process, retaddr, &int3, 1, NULL)) {
        fprintf(stderr, "WARNING: Failed to write INT 3 instruction. code: %d\n", (int)GetLastError());
        return;
    }
    CloseHandle(hThread);

    DEBUG_EVENT event;
    while(1) {
        ContinueDebugEvent(dbg.process_info.dwProcessId, thread_id, DBG_CONTINUE);
        WaitForDebugEvent(&event, INFINITE);
        if(event.dwDebugEventCode == EXCEPTION_DEBUG_EVENT) {
            DWORD code = event.u.Exception.ExceptionRecord.ExceptionCode;
            if(code == EXCEPTION_BREAKPOINT) {
                HANDLE hThread = OpenThread(THREAD_ALL_ACCESS, FALSE, event.dwThreadId);
                CONTEXT ctx = {0};
                ctx.ContextFlags = CONTEXT_ALL;
                GetThreadContext(hThread, &ctx);
                LPVOID address = event.u.Exception.ExceptionRecord.ExceptionAddress;
                if(retaddr == address) {
                    WriteProcessMemory(dbg.process, address, &step_out_orig_byte, 1, NULL);
                    ctx.Rip -= 1;
                    SetThreadContext(hThread, &ctx);
                    CloseHandle(hThread);
                    break;
                }
                CloseHandle(hThread);
            } else if (code == EXCEPTION_SINGLE_STEP) {
                printf("Exception: SINGLE_STEP\n");
            } else {
                printf("Exception: 0x%x\n", (unsigned int)code);
                exit(-1);
            }
        }
        ContinueDebugEvent(event.dwProcessId, event.dwThreadId, DBG_CONTINUE);
    }
    dbg_single_step(thread_id);
    ContinueDebugEvent(dbg.process_info.dwProcessId, thread_id, DBG_CONTINUE);
}

void dbg_step_over(DWORD thread_id)
{
    HANDLE hThread = OpenThread(THREAD_ALL_ACCESS, FALSE, thread_id);
    CONTEXT ctx = {0};
    ctx.ContextFlags = CONTEXT_ALL;
    GetThreadContext(hThread, &ctx);

    // Disassemble current instruction
    BYTE buf[16];
    if(!ReadProcessMemory(dbg.process, (LPVOID)ctx.Rip, buf, sizeof(buf), NULL)) {
        fprintf(stderr, "WARN: couldn't read Rip\n");
        return;
    }
    x86_set_buffer(&dbg.disasm, buf);
    x86_set_dmode(&dbg.disasm, X86_DMODE_64BIT);
    x86_set_ip(&dbg.disasm, ctx.Rip);
    if(x86_dasm(&dbg.disasm) < 0) {
        fprintf(stderr, "ERROR: failed to disassemble\n");
        CloseHandle(hThread);
        return;
    }
    DWORD64 rip = ctx.Rip;

    // If the instruction is a call, set a temporary breakpoint at next instruction
    if(dbg.disasm.mnem == X86_MN_CALL) { // near call relative
        LPVOID next_instr = (LPVOID)(rip + dbg.disasm.len);
        BYTE step_over_original_byte = 0;
        if(!ReadProcessMemory(dbg.process, next_instr, &step_over_original_byte, 1, NULL)) {
            fprintf(stderr, "WARNING: failed to read byte at breakpoint address. code: %d\n", (int)GetLastError());
            return;
        }
        BYTE int3 = 0xCC;
        if (!WriteProcessMemory(dbg.process, next_instr, &int3, 1, NULL)) {
            fprintf(stderr, "WARNING: Failed to write INT 3 instruction. code: %d\n", (int)GetLastError());
            return;
        }
        CloseHandle(hThread);

        DEBUG_EVENT event;
        ContinueDebugEvent(dbg.process_info.dwProcessId, thread_id, DBG_CONTINUE);
        while(1) {
            WaitForDebugEvent(&event, INFINITE);
            if(event.dwDebugEventCode == EXCEPTION_DEBUG_EVENT) {
                DWORD code = event.u.Exception.ExceptionRecord.ExceptionCode;
                if(code == EXCEPTION_BREAKPOINT) {
                    HANDLE hThread = OpenThread(THREAD_ALL_ACCESS, FALSE, event.dwThreadId);
                    CONTEXT ctx = {0};
                    ctx.ContextFlags = CONTEXT_ALL;
                    GetThreadContext(hThread, &ctx);
                    LPVOID address = event.u.Exception.ExceptionRecord.ExceptionAddress;
                    if(next_instr == address) {
                        WriteProcessMemory(dbg.process, address, &step_over_original_byte, 1, NULL);
                        ctx.Rip -= 1;
                        SetThreadContext(hThread, &ctx);
                        CloseHandle(hThread);
                        break;
                    }
                    CloseHandle(hThread);
                }
            }
            ContinueDebugEvent(event.dwProcessId, event.dwThreadId, DBG_CONTINUE);
        }
        dbg_single_step(thread_id);
        ContinueDebugEvent(dbg.process_info.dwProcessId, thread_id, DBG_CONTINUE);
    } else {
        // Otherwise just single-step
        ctx.EFlags |= 0x100; // Trap flag
        SetThreadContext(hThread, &ctx);
        CloseHandle(hThread);
        ContinueDebugEvent(dbg.process_info.dwProcessId, thread_id, DBG_CONTINUE);
    }
}

StringView sv_chop_symbol(StringView *source)
{
    *source = sv_strip(*source);
    size_t i = 0;
    if(!isalpha(source->items[i])) return (StringView){0};
    while(isalnum(source->items[i])) i += 1;
    StringView result = (StringView) { .items = source->items, .count = i };
    source->items += i;
    source->count -= i;
    return result;
}

char sv_next_char(StringView *source) {
    if(source->count <= 0) return 0;
    char ch = *source->items;
    source->items += 1;
    source->count -= 1;
    return ch;
}

bool sv_expect_char(StringView *source, char ch) {
    if(sv_next_char(source) != ch) {
        fprintf(stderr, "WARNING: expecting next character to be '%c'", ch);
        return false;
    }
    return true;
}

DWORD64 sv_next_number(StringView *source) {
    DWORD64 value = 0;

    if (source->count >= 2 && source->items[0] == '0' && (source->items[1] == 'x' || source->items[1] == 'X')) {
        // Hexadecimal
        source->items += 2;
        source->count -= 2;
        while (source->count > 0) {
            char ch = *source->items;
            if (ch >= '0' && ch <= '9') value = (value << 4) | (ch - '0');
            else if (ch >= 'a' && ch <= 'f') value = (value << 4) | (ch - 'a' + 10);
            else if (ch >= 'A' && ch <= 'F') value = (value << 4) | (ch - 'A' + 10);
            else break;
            source->items++;
            source->count--;
        }
    } else {
        // Decimal
        while (source->count > 0 && isdigit((unsigned char)*source->items)) {
            value = value * 10 + (*source->items - '0');
            source->items++;
            source->count--;
        }
    }

    return value;
}

static DWORD64 eval_expr(StringView *source, CONTEXT ctx)
{
    StringView symbol = sv_chop_symbol(source);
    DWORD64 value = 0;
    if(sv_eq(symbol, SV("rax"))) value = ctx.Rax;
    else if(sv_eq(symbol, SV("rbx"))) value = ctx.Rbx;
    else if(sv_eq(symbol, SV("rip"))) value = ctx.Rip;
    else if(sv_eq(symbol, SV("rcx"))) value = ctx.Rcx;
    else if(sv_eq(symbol, SV("rdi"))) value = ctx.Rdi;
    else if(sv_eq(symbol, SV("rsi"))) value = ctx.Rsi;
    else if(sv_eq(symbol, SV("rbp"))) value = ctx.Rbp;
    else if(sv_eq(symbol, SV("rsp"))) value = ctx.Rsp;
    else if(sv_eq(symbol, SV("r8"))) value = ctx.R8;
    else if(sv_eq(symbol, SV("r9"))) value = ctx.R9;
    else {
        int bytes = 0;
        if(sv_eq(symbol, SV("dword"))) bytes = 4;
        else if(sv_eq(symbol, SV("qword"))) bytes = 8;
        else if(sv_eq(symbol, SV("word")))  bytes = 2;
        else if(sv_eq(symbol, SV("byte")))  bytes = 1;
        if(bytes == 0) {
            fprintf(stderr, "WARN: Unknown primitive "SV_FMT"\n", SV_ARG(symbol));
            return 0;
        }

        if(!sv_expect_char(source, '[')) return 0;
        DWORD64 addr = eval_expr(source, ctx);
        if(!sv_expect_char(source, ']')) return 0;
        BYTE buf[8] = {0};
        if(!ReadProcessMemory(dbg.process, (LPVOID)addr, &buf, bytes, NULL)) {
            fprintf(stderr, "WARNING: failed to read byte at breakpoint address. code: %d\n", (int)GetLastError());
            return 0;
        }

        switch(bytes) {
            case 1: value = *(BYTE*)buf; break;
            case 2: value = *(WORD*)buf; break;
            case 4: value = *(DWORD*)buf; break;
            case 8: value = *(DWORD64*)buf; break;
        }
    }

    while(source->count > 0) {
        char ch = *source->items;
        if(ch == ']') break;
        sv_next_char(source);
        DWORD64 off = sv_next_number(source);
        switch(ch) {
        case '+':
            value += off;
            break;
        case '-':
            value -= off;
            break;
        default:
            fprintf(stderr, "Unknown offset operator '%c'\n", ch);
            break;
        }
    }
    return value;
}

void dbg_print_expr(DWORD thread_id, StringView source)
{
    HANDLE hThread = OpenThread(THREAD_ALL_ACCESS, FALSE, thread_id);
    CONTEXT ctx = {0};
    ctx.ContextFlags = CONTEXT_ALL;
    GetThreadContext(hThread, &ctx);
    StringView orig = source;
    printf(SV_FMT" = %016llx\n", SV_ARG(orig), eval_expr(&source, ctx));
    CloseHandle(hThread);
}

char prev[256];
char buf[256];
void dbg_repl(DWORD thread_id)
{
    while(1) {
        printf("(w64dbg) ");
        if(!fgets(buf, sizeof(buf), stdin)) exit(0);
        StringView prompt = sv_rstrip(sv_from_cstr(buf));
        StringView cmd = sv_chop_until_space(&prompt);
        if(cmd.count == 0) {
            prompt = sv_rstrip(sv_from_cstr(prev));
            cmd = sv_chop_until_space(&prompt);
        } else {
            memcpy(prev, buf, sizeof(prev));
        }

        if(sv_eq(cmd, SV("quit")) || sv_eq(cmd, SV("q"))) {
            exit(0);
        } else if(sv_eq(cmd, SV("break")) || sv_eq(cmd, SV("b"))) {
            StringView symbol = sv_chop_until_space(&prompt);
            EnumSymProcUserContext ctx = {0};
            ctx.symbol = symbol;
            if (!SymEnumSymbols(dbg.process, dbg.sym_module_base, "*", EnumSymProc, &ctx)) {
                printf("ERROR: SymEnumSymbols failed\n");
            }
            if(ctx.found_address) {
                if(dbg_set_breakpoint((LPVOID)ctx.found_address)) {
                    printf("INFO: Set breakpoint at symbol "SV_FMT" on address 0x%llx\n", SV_ARG(symbol), ctx.found_address);
                } else {
                    fprintf(stderr, "WARNING: Failed to set breakpoint\n");
                }
            } else {
                fprintf(stderr, "WARNING: Failed to find the address of symbol "SV_FMT" \n", SV_ARG(symbol));
            }
        } else if(sv_eq(cmd, SV("symbols")) || sv_eq(cmd, SV("sym"))) {
            StringView symbol = sv_chop_until_space(&prompt);
            EnumSymProcUserContext ctx = {0};
            ctx.symbol = symbol;
            if (!SymEnumSymbols(dbg.process, dbg.sym_module_base, "*", EnumSymProc, &ctx)) {
                printf("ERROR: SymEnumSymbols failed\n");
            }
            if(ctx.found_address) {
                printf("Found symbol "SV_FMT" on address 0x%llx\n", SV_ARG(symbol), ctx.found_address);
            }
        } else if(sv_eq(cmd, SV("step")) || sv_eq(cmd, SV("s"))) {
            dbg_print_instruction(thread_id);
            dbg_single_step(thread_id);
            ContinueDebugEvent(dbg.process_info.dwProcessId, thread_id, DBG_CONTINUE);
            return;
        } else if(sv_eq(cmd, SV("next")) || sv_eq(cmd, SV("n"))) {
            dbg_print_instruction(thread_id);
            dbg_step_over(thread_id);
            return;
        } else if(sv_eq(cmd, SV("finish")) || sv_eq(cmd, SV("f"))) {
            dbg_print_instruction(thread_id);
            dbg_step_out(thread_id);
            return;
        } else if(sv_eq(cmd, SV("resume")) || sv_eq(cmd, SV("r"))) {
            ContinueDebugEvent(dbg.process_info.dwProcessId, thread_id, DBG_CONTINUE);
            return;
        } else if(sv_eq(cmd, SV("regs"))) {
            dbg_print_registers(thread_id);
        } else if(sv_eq(cmd, SV("print")) || sv_eq(cmd, SV("p"))) {
            dbg_print_expr(thread_id, prompt);
        } else if(sv_eq(cmd, SV("help"))) {
            printf("(w64dbg) help\n");
            printf("List of Commands available in w64dbg\n");
            printf("help            Show this list\n");
            printf("regs            Print all registers\n");
            printf("step      s     Step to the next assembly instruction\n");
            printf("resume    r     Resume execution until breakpoint or any exception\n");
            printf("break     b     Set a breakpoint\n");
            printf("symbols   sym   List of all symbols in the executable\n");
            printf("eval      e     (todo) Evaluate expression. Syntax is as follows\n");
            printf("                'e rax'           This will shows the content of register rax\n");
            printf("                'e DWORD[rax]'   This will shows the DWORD content of memory addressed by rax\n");
            printf("                'e DWORD[rax+8]' This will shows the DWORD content of memory addressed by rax with offset +8\n");
            printf("                (todo) 'e DWORD[rax:8]' This will shows the 8 DWORDs content of start by the address of rax.\n");

        } else {
            printf("Unknown command %.*s\n", (int)cmd.count, cmd.items);
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
