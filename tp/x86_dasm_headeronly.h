/*

    This file is part of x86_dasm.

    x86_dasm is a compact x86-64 disassembling library
    
    Copyright 2014 / the`janitor / < email: base64dec(dGhlLmphbml0b3JAcHJvdG9ubWFpbC5jb20=) >

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

        http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.

*/
#ifndef _TYPES_H_
#define _TYPES_H_

#if defined(_MSC_VER)

    #include <windows.h>

    typedef LARGE_INTEGER           large_int_t;

    #if(_MSC_VER < 1300)

        typedef SIZE_T              size_t;
        typedef signed char         int8_t;
        typedef signed short        int16_t;
        typedef signed int          int32_t;
        typedef long long           int64_t

        typedef unsigned char       uint8_t;
        typedef unsigned short      uint16_t;
        typedef unsigned int        uint32_t;
        typedef unsigned long long  uint64_t;

    #else

        typedef signed __int8       int8_t;
        typedef signed __int16      int16_t;
        typedef signed __int32      int32_t;
        typedef signed __int64      int64_t;

        typedef unsigned __int8     uint8_t;
        typedef unsigned __int16    uint16_t;
        typedef unsigned __int32    uint32_t;
        typedef unsigned __int64    uint64_t;

        typedef signed long         long_t;
        typedef unsigned long       ulong_t;
        typedef unsigned char       uchar_t;
        typedef unsigned short      ushort_t;

    #endif

    #if defined(_WIN64)

        typedef signed __int64      intptr_t;
        typedef unsigned __int64    uintptr_t;
        typedef unsigned __int64    size_t;

    #else

        typedef signed int          intptr_t;
        typedef unsigned int        uintptr_t;

    #endif

#else

    #include <sys/types.h>
    #include <stdint.h>

    typedef signed long         long_t;
    typedef unsigned long       ulong_t;
    typedef unsigned char       uchar_t;
    typedef unsigned short      ushort_t;

#endif

typedef unsigned int bool_t;

#define true    1
#define false   0

#ifndef __cplusplus
    #define NULL    ((void *)0)
#endif

#if 0
#ifndef offsetof
#define offsetof(s,m) (size_t)&(((s *)0)->m)
#endif
#endif

#ifndef countof
#define countof(a) (sizeof(a) / sizeof(a[0]))
#endif

#define roundup(a, b) \
    (((ulong_t)(a) + (ulong_t)(b) - 1) & ~((ulong_t)(b) - 1))

/* token paste */
#define PASTE_INTERNAL(a, b) a##b 

/* level of indirection for recursive expansion */
#define PASTE_TOKEN(a, b) PASTE_INTERNAL(a, b) 

#endif //_TYPES_H_

/*

    This file is part of x86_dasm.

    x86_dasm is a compact x86-64 disassembling library
    
    Copyright 2014 / the`janitor / < email: base64dec(dGhlLmphbml0b3JAcHJvdG9ubWFpbC5jb20=) >

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

        http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.

*/

/*

    This file is part of x86_dasm.

    x86_dasm is a compact x86-64 disassembling library
    
    Copyright 2014 / the`janitor / < email: base64dec(dGhlLmphbml0b3JAcHJvdG9ubWFpbC5jb20=) >

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

        http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.

*/
#ifndef _DASM_TABLES_H_
#define _DASM_TABLES_H_

/* 
    #define X86_DASM_DEBUG      : enable debug output
    #define X86_DASM_NOFMT      : disable all formatting/strings
    #define X86_DASM_NLE        : if not on a little-endian system
*/

#ifdef __cplusplus
    extern "C" {
#endif

/****************************************************************************

    Instruction format

****************************************************************************/

/* 
    the total number of bytes in an instruction encoding must be less than
    or equal to 15
*/
#define MAX_INST_LENGTH 15

#define ESCAPE_OPCODE_2B    0x0f
#define ESCAPE_OPCODE_3B_1  0x38
#define ESCAPE_OPCODE_3B_2  0x3a

/*
    MODRM byte

    bits 0-2: rm  (rex.b, vex.b, xop.b extend this field to 4 bits)
    bits 3-5: reg (rex.r, vex.r, xop.r extend this field to 4 bits)
    bits 6-7: mod
*/
#define MODRM_RM(b)         ((b >> 0) & 7)
#define MODRM_REG(b)        ((b >> 3) & 7)
#define MODRM_MOD(b)        ((b >> 6) & 3)

/*
    SIB byte

    bits 0-2: base  (rex.b extends this field to 4 bits)
    bits 3-5: index (rex.x extends this field to 4 bits)
    bits 6-7: scale (00 = 1, 01 = 2, 10 = 4, 11 = 8)

    effective_address = scale * index + base + disp8/16/32
*/
#define SIB_BASE(b)         ((b >> 0) & 7)
#define SIB_INDEX(b)        ((b >> 3) & 7)
#define SIB_SCALE(b)        ((b >> 6) & 3)

/*
    REX byte

    bit 0: b (msb extension of modrm.rm or sib.base or opcode reg field)
    bit 1: x (msb extension of sib.index)
    bit 2: r (msb extension of modrm.reg)
    bit 3: w (0 = default operand size, 1 = 64-bit operand size)
*/
#define REX_PREFIX(b)       (((b >> 4) & 0x0f) == 4)
#define _REX_B(b)           ((b >> 0) & 1)
#define _REX_X(b)           ((b >> 1) & 1)
#define _REX_R(b)           ((b >> 2) & 1)
#define _REX_W(b)           ((b >> 3) & 1)

/*
    VEX 2-byte
*/
#define _VEX_2B_PP(b)       ((b >> 0) & 3)
#define _VEX_2B_L(b)        ((b >> 2) & 1)
#define _VEX_2B_VVVV(b)     ((~(b >> 3)) & 0x0f)
#define _VEX_2B_R(b)        ((~(b >> 7)) & 1)

#define VEX_2B_PM(b)        (MODRM_MOD(b) == 3) /* LDS must have modrm.mod = !11b */

/*
    VEX 3-byte
*/
#define _VEX_3B_MMMMM(b)    ((b >> 0) & 0x1f)
#define _VEX_3B_B(b)        ((~(b >> 5)) & 1)
#define _VEX_3B_X(b)        ((~(b >> 6)) & 1)
#define _VEX_3B_R(b)        ((~(b >> 7)) & 1)

#define _VEX_3B_PP(b)       ((b >> 0) & 3)
#define _VEX_3B_L(b)        ((b >> 2) & 1)
#define _VEX_3B_VVVV(b)     ((~(b >> 3)) & 0x0f)
#define _VEX_3B_W(b)        ((b >> 7) & 1)

#define VEX_3B_PM(b)        (MODRM_MOD(b) == 3) /* LES must have modrm.mod = !11b */

/*
    XOP
*/
#define _XOP_MMMMM(b)       ((b >> 0) & 0x1f)
#define _XOP_B(b)           ((~(b >> 5)) & 1)
#define _XOP_X(b)           ((~(b >> 6)) & 1)
#define _XOP_R(b)           ((~(b >> 7)) & 1)

#define _XOP_PP(b)          ((b >> 0) & 3)
#define _XOP_L(b)           ((b >> 2) & 1)
#define _XOP_VVVV(b)        ((~(b >> 3)) & 0x0f)
#define _XOP_W(b)           ((b >> 7) & 1)

#define XOP_PM(b)           (MODRM_MOD(b) == 3)
#define XOP_VALID(b)        (_XOP_MMMMM(b) > 7 && _XOP_MMMMM(b) < 11)

/*
    EVEX
*/
#define _EVEX_B(b)          ((~(b >> 5)) & 1)
#define _EVEX_X(b)          ((~(b >> 6)) & 1) 
#define _EVEX_R(b)          ((~(b >> 7)) & 1)
#define _EVEX_RR(b)         ((~(b >> 4)) & 1) /* R' */
#define _EVEX_MM(b)         ((b >> 0) & 3)

#define _EVEX_PP(b)         ((b >> 0) & 3)
#define _EVEX_W(b)          ((b >> 7) & 1)
#define _EVEX_VVVV(b)       ((~(b >> 3)) & 0x0f)

#define _EVEX_Z(b)          ((b >> 7) & 1) 
#define _EVEX_LL(b)         ((b >> 6) & 1) /* L' */
#define _EVEX_L(b)          ((b >> 5) & 1) 
#define _EVEX_BB(b)         ((b >> 4) & 1)
#define _EVEX_VV(b)         ((~(b >> 3)) & 1) /* V' */
#define _EVEX_AAA(b)        ((b >> 0) & 7)

#define EVEX_P0(b)          (((b >> 2) & 3) == 0) /* check on P0, else #UD */
#define EVEX_P1(b)          (((b >> 2) & 1) == 1) /* check on P1, else #UD */
#define EVEX_PM(b)          (MODRM_MOD(b) == 3) /* BOUND must have modrm.mod = !11b */


/*
    prefixes
*/
#define PREFIX_OSIZE        0x66 /* operand-size override */
#define PREFIX_ASIZE        0x67 /* address-size override */
#define PREFIX_SEGOVRD_CS   0x2e /* segment override, ignored in 64-bit mode */
#define PREFIX_SEGOVRD_DS   0x3e /* segment override, ignored in 64-bit mode */
#define PREFIX_SEGOVRD_ES   0x26 /* segment override, ignored in 64-bit mode */
#define PREFIX_SEGOVRD_FS   0x64 /* segment override */
#define PREFIX_SEGOVRD_GS   0x65 /* segment override */
#define PREFIX_SEGOVRD_SS   0x36 /* segment override, ignored in 64-bit mode */
#define PREFIX_LOCK         0xf0 /* lock rw atomically */
#define PREFIX_REPNE        0xf2
#define PREFIX_REP          0xf3
#define PREFIX_BRT          0x2e /* branch taken (hint), only with jcc */
#define PREFIX_BRNT         0x3e /* branch not taken (hint), only with jcc */
#define PREFIX_VEX_3B       0xc4
#define PREFIX_VEX_2B       0xc5
#define PREFIX_EVEX         0x62
#define PREFIX_XOP          0x8f

/****************************************************************************

    addressing methods

    Intel 64 and IA-32 Architectures Software Developer�s Manual, Volume 2 
    Order Number: 325383-055US
    June 2015

****************************************************************************/

enum
{
    AM_NONE = 0,

/* 
    direct address: the instruction has no ModR/M byte; the address of the 
    operand is encoded in the instruction. No base register, index register, 
    or scaling factor can be applied (for example, far JMP (EA)).
*/
    AM_A,

/* 
    the VEX.vvvv field of the VEX prefix selects a general purpose register. 
*/
    AM_B,

/* 
    the reg field of the ModR/M byte selects a control register 
    (for example, MOV (0F20, 0F22)).
*/
    AM_C,

/* 
    the reg field of the ModR/M byte selects a debug register 
    (for example, MOV (0F21, 0F23)). 
*/
    AM_D,       

/* 
    a ModR/M byte follows the opcode and specifies the operand. The operand 
    is either a general-purpose register or a memory address. If it is a 
    memory address, the address is computed from a segment register and any 
    of the following values: a base register, an index register, a scaling 
    factor, a displacement.
*/
    AM_E,           

/* 
    EFLAGS/RFLAGS register 
*/
    AM_F,

/* 
    the reg field of the ModR/M byte selects a general register 
    (for example, AX (000)).
*/
    AM_G,   

/* 
    the VEX.vvvv field of the VEX prefix selects a 128-bit XMM register or a 
    256-bit YMM register, determined by operand type. For legacy SSE 
    encodings this operand does not exist, changing the instruction to
    destructive form.
*/
    AM_H,

/* 
    immediate data: the operand value is encoded in subsequent bytes of the 
    instruction.
*/
    AM_I,

/* 
    the instruction contains a relative offset to be added to the instruction 
    pointer register (for example, JMP (0E9), LOOP).
*/
    AM_J,   

/*
    the upper 4 bits of the 8-bit immediate selects a 128-bit XMM register or 
    a 256-bit YMM register, determined by operand type. 
    (the MSB is ignored in 32-bit mode)
*/
    AM_L,   

/* 
    the ModR/M byte may refer only to memory (for example, BOUND, LES, LDS, 
    LSS, LFS, LGS, CMPXCHG8B).
*/
    AM_M,   

/* 
    the R/M field of the ModR/M byte selects a packed-quadword, 
    MMX technology register.
*/
    AM_N,       

/* 
    the instruction has no ModR/M byte. The offset of the operand is coded as 
    a word or double word (depending on address size attribute) in the 
    instruction. No base register, index register, or scaling factor can be 
    applied (for example, MOV (A0�A3)).
*/
    AM_O,       

/* 
    the reg field of the ModR/M byte selects a packed quadword 
    MMX technology register.
*/
    AM_P,       

/* 
    a ModR/M byte follows the opcode and specifies the operand. The operand 
    is either an MMX technology register or a memory address. If it is a 
    memory address, the address is computed from a segment register and any 
    of the following values: a base register, an index register, a scaling 
    factor, and a displacement.
*/
    AM_Q,

/* 
    the R/M field of the ModR/M byte may refer only to a general register 
    (for example, MOV (0F20-0F23)).
*/
    AM_R,

/* 
    the reg field of the ModR/M byte selects a segment register 
    (for example, MOV (8C,8E)).
*/
    AM_S,

/* 
    the R/M field of the ModR/M byte selects a 128-bit XMM register or a 
    256-bit YMM register, determined by operand type.
*/
    AM_U,

/*
    the reg field of the ModR/M byte selects a 128-bit XMM register or a 
    256-bit YMM register, determined by operand type.
*/
    AM_V,

/* 
    a ModR/M byte follows the opcode and specifies the operand. The operand 
    is either a 128-bit XMM register, a 256-bit YMM register (determined by 
    operand type), or a memory address. If it is a memory address, the 
    address is computed from a segment register and any of the following 
    values: a base register, an index register, a scaling factor, and a 
    displacement.
*/
    AM_W,       

/* 
    memory addressed by the DS:rSI register pair (for example, MOVS, CMPS, 
    OUTS, or LODS).
*/
    AM_X,

/* 
    memory addressed by the ES:rDI register pair (for example, MOVS, CMPS, 
    INS, STOS, or SCAS).
*/
    AM_Y,

/* 
    implicit immediate data of value 1 
*/
    AM_I1,

/* 
    implicit immediate data of value 3 
*/
    AM_I3,          

/* 
    register RAX/EAX/AX depending on the operand's size attribute
    (no REX.B extension) 
*/
    AM_rAX,
    AM_rCX,
    AM_rDX,
    AM_rBX,
    AM_rSP,
    AM_rBP,
    AM_rSI,
    AM_rDI,

/* 
    register RAX/EAX/AX, R8/R8D/R8W depending on the operand's size 
    attribute (with REX.B extension) 

    when any REX prefix is used, SPL, BPL, SIL and DIL are used. 
    otherwise, without any REX prefix AH, CH, DH and BH are used. 
*/
    AM_rAX_r8,
    AM_rCX_r9,
    AM_rDX_r10,
    AM_rBX_r11, 
    AM_rSP_r12,
    AM_rBP_r13,
    AM_rSI_r14,
    AM_rDI_r15,

/* 
    register ST(0), FPU/x87 extension 
*/
    AM_ST0,     
    AM_ST1,     
    AM_ST2, 
    AM_ST3, 
    AM_ST4, 
    AM_ST5, 
    AM_ST6, 
    AM_ST7, 

/* 
    register CS/DS/ES/... segment registers 
*/
    AM_ES,  
    AM_CS,
    AM_SS,  
    AM_DS,
    AM_FS,
    AM_GS,

    AM_MAX
};

/****************************************************************************

    operand types

    Intel 64 and IA-32 Architectures Software Developer�s Manual, Volume 2 
    Order Number: 325383-055US
    June 2015

****************************************************************************/

enum 
{
    OT_NONE = 0,

/* 
    two one-word operands in memory or two double-word operands in memory, 
    depending on operand-size attribute (used only by the BOUND instruction).
*/
    OT_A,

/* 
    byte, regardless of operand-size attribute. 
*/
    OT_B,

/*  
    byte or word, depending on operand-size attribute.

    OT_C,
*/
    
/* 
    doubleword, regardless of operand-size attribute.
*/
    OT_D,

/* 
    double-quadword, regardless of operand-size attribute. 
*/
    OT_DQ,

/* 
    32-bit, 48-bit, or 80-bit pointer, depending on operand-size attribute.
*/
    OT_P,

/* 
    128-bit or 256-bit packed double-precision floating-point data.
*/
    OT_PD,

/* 
    quadword MMX technology register (for example: mm0).
*/
    OT_PI,  

/* 
    128-bit or 256-bit packed single-precision floating-point data.
*/
    OT_PS,  

/* 
    quadword, regardless of operand-size attribute.
*/
    OT_Q,

/* 
    quad-quadword (256-bits), regardless of operand-size attribute.
*/
    OT_QQ,

/* 
    6-byte or 10-byte pseudo-descriptor.
*/
    OT_S,

/* 
    scalar element of a 128-bit double-precision floating data.
*/
    OT_SD,

/* 
    scalar element of a 128-bit single-precision floating data.
*/
    OT_SS,

/* 
    doubleword integer register (for example: eax).
*/
    OT_SI,

/* 
    10-byte data, regardless of operand-size attribute.
    tbyte, dt
*/
    OT_T,   

/* 
    word, doubleword or quadword (in 64-bit mode), depending on operand-size 
    attribute.
*/
    OT_V,

/* 
    word, regardless of operand-size attribute. 
*/
    OT_W,

/* 
    dq or qq based on the operand-size attribute.
*/
    OT_X,

/* 
    doubleword or quadword (in 64-bit mode), depending on operand-size 
    attribute.
*/
    OT_Y,

/* 
    word for 16-bit operand-size or doubleword for 32 or 64-bit operand-size.   
*/
    OT_Z,

/* 
    64- or 128-bit depending on VEX.L   
*/
    OT_OQ,

    OT_MAX
};

/****************************************************************************

    operands

****************************************************************************/

#define O_NONE          AM_NONE, OT_NONE

/*
    general-purpose register operands
*/
#define O_AL            AM_rAX,         OT_B
#define O_CL            AM_rCX,         OT_B
#define O_AX            AM_rAX,         OT_W
#define O_DX            AM_rDX,         OT_W
#define O_eAX           AM_rAX,         OT_Z
#define O_eCX           AM_rCX,         OT_Z
#define O_eDX           AM_rDX,         OT_Z
#define O_eBX           AM_rBX,         OT_Z
#define O_eSP           AM_rSP,         OT_Z
#define O_eBP           AM_rBP,         OT_Z
#define O_eSI           AM_rSI,         OT_Z
#define O_eDI           AM_rDI,         OT_Z
#define O_rAX           AM_rAX,         OT_V
#define O_rDX           AM_rDX,         OT_V
#define O_rAX_r8        AM_rAX_r8,      OT_V
#define O_rCX_r9        AM_rCX_r9,      OT_V
#define O_rDX_r10       AM_rDX_r10,     OT_V
#define O_rBX_r11       AM_rBX_r11,     OT_V
#define O_rSP_r12       AM_rSP_r12,     OT_V
#define O_rBP_r13       AM_rBP_r13,     OT_V
#define O_rSI_r14       AM_rSI_r14,     OT_V
#define O_rDI_r15       AM_rDI_r15,     OT_V
#define O_AL_r8L        AM_rAX_r8,      OT_B
#define O_CL_r9L        AM_rCX_r9,      OT_B
#define O_DL_r10L       AM_rDX_r10,     OT_B
#define O_BL_r11L       AM_rBX_r11,     OT_B
#define O_AH_r12L       AM_rSP_r12,     OT_B
#define O_CH_r13L       AM_rBP_r13,     OT_B
#define O_DH_r14L       AM_rSI_r14,     OT_B
#define O_BH_r15L       AM_rDI_r15,     OT_B

/*
    segment register operands
*/
#define O_ES            AM_ES,          OT_NONE
#define O_CS            AM_CS,          OT_NONE
#define O_SS            AM_SS,          OT_NONE
#define O_DS            AM_DS,          OT_NONE
#define O_FS            AM_FS,          OT_NONE
#define O_GS            AM_GS,          OT_NONE

/*
    FPU register operands
*/
#define O_ST0           AM_ST0,         OT_NONE
#define O_ST1           AM_ST1,         OT_NONE
#define O_ST2           AM_ST2,         OT_NONE
#define O_ST3           AM_ST3,         OT_NONE
#define O_ST4           AM_ST4,         OT_NONE
#define O_ST5           AM_ST5,         OT_NONE
#define O_ST6           AM_ST6,         OT_NONE
#define O_ST7           AM_ST7,         OT_NONE

#define O_Ap            AM_A,           OT_P
#define O_By            AM_B,           OT_Y
#define O_Cy            AM_C,           OT_Y
#define O_Dy            AM_D,           OT_Y
#define O_Eb            AM_E,           OT_B
#define O_Ed            AM_E,           OT_D
#define O_Ep            AM_E,           OT_P
#define O_Ev            AM_E,           OT_V
#define O_Ew            AM_E,           OT_W
#define O_Ey            AM_E,           OT_Y
#define O_Fv            AM_F,           OT_V
#define O_Gb            AM_G,           OT_B
#define O_Gd            AM_G,           OT_D
#define O_Gv            AM_G,           OT_V
#define O_Gw            AM_G,           OT_W
#define O_Gy            AM_G,           OT_Y
#define O_Gz            AM_G,           OT_Z
#define O_Gq            AM_G,           OT_Q
#define O_Hdq           AM_H,           OT_DQ
#define O_Hpd           AM_H,           OT_PD
#define O_Hps           AM_H,           OT_PS
#define O_Hq            AM_H,           OT_Q
#define O_Hqq           AM_H,           OT_QQ
#define O_Hsd           AM_H,           OT_SD
#define O_Hss           AM_H,           OT_SS
#define O_Hx            AM_H,           OT_X
#define O_I1            AM_I1,          OT_B
#define O_I3            AM_I3,          OT_B
#define O_Ib            AM_I,           OT_B
#define O_Id            AM_I,           OT_D
#define O_Iv            AM_I,           OT_V
#define O_Iw            AM_I,           OT_W
#define O_Iz            AM_I,           OT_Z
#define O_Jb            AM_J,           OT_B
#define O_Jz            AM_J,           OT_Z
#define O_Lx            AM_L,           OT_X
#define O_Lss           AM_L,           OT_SS
#define O_M             AM_M,           OT_NONE
#define O_Ma            AM_M,           OT_A
#define O_Mb            AM_M,           OT_B
#define O_Md            AM_M,           OT_D
#define O_Mdq           AM_M,           OT_DQ
#define O_Mp            AM_M,           OT_P
#define O_Mpd           AM_M,           OT_PD
#define O_Mps           AM_M,           OT_PS
#define O_Mq            AM_M,           OT_Q
#define O_Moq           AM_M,           OT_OQ
#define O_Ms            AM_M,           OT_S
#define O_Mt            AM_M,           OT_T
#define O_Mv            AM_M,           OT_V
#define O_Mw            AM_M,           OT_W
#define O_Mx            AM_M,           OT_X
#define O_My            AM_M,           OT_Y
#define O_Nq            AM_N,           OT_Q
#define O_Ob            AM_O,           OT_B
#define O_Ov            AM_O,           OT_V
#define O_Ppi           AM_P,           OT_PI
#define O_Pq            AM_P,           OT_Q
#define O_Py            AM_P,           OT_Y
#define O_Qd            AM_Q,           OT_D
#define O_Qpi           AM_Q,           OT_PI
#define O_Qq            AM_Q,           OT_Q
#define O_Rd            AM_R,           OT_D
#define O_Rb            AM_R,           OT_B
#define O_Rw            AM_R,           OT_W
#define O_Rq            AM_R,           OT_Q
#define O_Ry            AM_R,           OT_Y
#define O_Rdq           AM_R,           OT_DQ
#define O_Rv            AM_R,           OT_V
#define O_Ry            AM_R,           OT_Y
#define O_Sw            AM_S,           OT_W
#define O_Udq           AM_U,           OT_DQ
#define O_Upd           AM_U,           OT_PD
#define O_Ups           AM_U,           OT_PS
#define O_Uq            AM_U,           OT_Q
#define O_Ux            AM_U,           OT_X
#define O_Uss           AM_U,           OT_SS
#define O_Vdq           AM_V,           OT_DQ
#define O_Vpd           AM_V,           OT_PD
#define O_Vpi           AM_V,           OT_PI
#define O_Vps           AM_V,           OT_PS
#define O_Vq            AM_V,           OT_Q
#define O_Vqq           AM_V,           OT_QQ
#define O_Vsd           AM_V,           OT_SD
#define O_Vss           AM_V,           OT_SS
#define O_Vx            AM_V,           OT_X
#define O_Vy            AM_V,           OT_Y
#define O_Wd            AM_W,           OT_D
#define O_Wdq           AM_W,           OT_DQ
#define O_Wpd           AM_W,           OT_PD
#define O_Wpi           AM_W,           OT_PI
#define O_Wps           AM_W,           OT_PS
#define O_Wq            AM_W,           OT_Q
#define O_Woq           AM_W,           OT_OQ
#define O_Wqq           AM_W,           OT_QQ
#define O_Wsd           AM_W,           OT_SD
#define O_Wss           AM_W,           OT_SS
#define O_Wx            AM_W,           OT_X
#define O_Xb            AM_X,           OT_B
#define O_Xv            AM_X,           OT_V
#define O_Xz            AM_X,           OT_Z
#define O_Yb            AM_Y,           OT_B
#define O_Yv            AM_Y,           OT_V
#define O_Yz            AM_Y,           OT_Z

/****************************************************************************

    property flags

****************************************************************************/

enum
{
    PF_NONE = 0,

/* 
    the instruction is invalid or not encodable in 64-bit mode. 40 through 
    4F (single-byte INC and DEC) are REX prefix combinations when in 64-bit 
    mode (use FE/FF Grp 4 and 5 for INC and DEC).
*/
    PF_I64 = (1 << 0),

/*
    instruction is only available when in 64-bit mode.
*/
    PF_O64 = (1 << 1),

/*
    the operand size is forced to a 64-bit operand size when in 64-bit mode 
    (prefixes that change operand size are ignored for this instruction in 
    64-bit mode).

*/
    PF_F64 = (1 << 2),

/*
    when in 64-bit mode, instruction defaults to 64-bit operand size and 
    cannot encode 32-bit operand size.
*/
    PF_D64 = (1 << 3),

/*
    can accept a LOCK prefix.
*/
    PF_LOCK = (1 << 4),

/*
    can accept a REP prefix.
*/
    PF_REP = (1 << 5),

/*
    can accept a REP prefix.
*/
    PF_REPE_REPNE = (1 << 6),

/*
    can accept VEX prefix.
*/
    PF_VEX = (1 << 7),

/*
    can only accept a 128-bit VEX prefix.
*/
    PF_VEX128 = (1 << 8),

/*
    can only accept a 256-bit VEX prefix.
*/
    PF_VEX256 = (1 << 9),

/*
    requires VEX prefix.
*/
    PF_OVEX = (1 << 10),

/*
    last byte encodes a sse/vex condition code
*/
    PF_CC = (1 << 11),

/*
    last byte encodes a xop condition code
*/
    PF_XOPCC = (1 << 12),

/*
    the instruction has prefixes N/A
*/
    PF_PFXNA = (1 << 13),

/*
    the instruction supports vectored sib vm32x (AVX2+)
*/
    PF_VSIB_X = (1 << 14),

/*
    the instruction supports vectored sib vm32y (AVX2+)
*/
    PF_VSIB_Y = (1 << 15),

/*
    the instruction supports vectored sib vm32z (AVX2+)
*/
    PF_VSIB_Z = (1 << 16),

/*
    the instruction supports vectored sib vm32x/y/z based on L (AVX2+)
*/
    PF_VSIB_L = (1 << 17),

/*
    sign-extension for imm8/16/32
*/
    PF_SIGN = (1 << 18)

};

/****************************************************************************

    table indexes and lookup methods

****************************************************************************/

#define TABLE_ID(i) TABLE_ID_##i
#define TABLE_LOOKUP_METHOD(i) TABLE_LM_##i

enum
{
    /* addressing size: 0 = 16-bits, 1 = 32-bits, 2 = 64-bits */
    TABLE_LOOKUP_METHOD(ASIZE) = 0, 

    /* operand size: 0 = 16-bits, 1 = 32-bits, 2 = 64-bits */
    TABLE_LOOKUP_METHOD(OSIZE),

    /* operand size: 0 = vex.w0, 1 = vex.w1 */
    TABLE_LOOKUP_METHOD(OSIZE_VEX),

    /* operand size: 0 = xop.w0, 1 = xop.w1 */
    TABLE_LOOKUP_METHOD(OSIZE_XOP),

    /* dasm mode:  0 = 16-bits, 1 = 32-bits, 2 = 64-bits */
    TABLE_LOOKUP_METHOD(DMODE),

    /* modr/m byte reg/nnn field */
    TABLE_LOOKUP_METHOD(MODRM_REG),

    /* modr/m byte r/m field */
    TABLE_LOOKUP_METHOD(MODRM_RM),  

    /* modr/m byte mod field:  0 = !11b, 1 = 11b */
    TABLE_LOOKUP_METHOD(MODRM_MOD), 

    /* modr/m byte:  index = modr/m byte - C0h */
    TABLE_LOOKUP_METHOD(FPU),

    /* last immediate byte is the opcode */
    TABLE_LOOKUP_METHOD(3DNOW),

    /* byte */
    TABLE_LOOKUP_METHOD(BYTE)
};

enum
{
    /* 1-byte table ids */
    TABLE_ID(1B) = 0,
    TABLE_ID(60),
    TABLE_ID(61),
    TABLE_ID(63),
    TABLE_ID(6D),
    TABLE_ID(6F),
    TABLE_ID(80),
    TABLE_ID(81),
    TABLE_ID(82),
    TABLE_ID(83),
    TABLE_ID(8F),
    TABLE_ID(98),
    TABLE_ID(99),
    TABLE_ID(9C),
    TABLE_ID(9D),
    TABLE_ID(A5),
    TABLE_ID(A7),
    TABLE_ID(AB),
    TABLE_ID(AD),
    TABLE_ID(AF),
    TABLE_ID(C0),
    TABLE_ID(C1),
    TABLE_ID(C6),
    TABLE_ID(C7),
    TABLE_ID(CF),
    TABLE_ID(D0),
    TABLE_ID(D1),
    TABLE_ID(D2),
    TABLE_ID(D3),
    TABLE_ID(D8),
    TABLE_ID(D9),
    TABLE_ID(DA),
    TABLE_ID(DB),
    TABLE_ID(DC),
    TABLE_ID(DD),
    TABLE_ID(DE),
    TABLE_ID(DF),
    TABLE_ID(E3),
    TABLE_ID(F6),
    TABLE_ID(F7),
    TABLE_ID(FE),
    TABLE_ID(FF),
    TABLE_ID(D8_MOD_N11),
    TABLE_ID(D8_MOD_11),
    TABLE_ID(D9_MOD_N11),
    TABLE_ID(D9_MOD_11),
    TABLE_ID(DA_MOD_N11),
    TABLE_ID(DA_MOD_11),
    TABLE_ID(DB_MOD_N11),
    TABLE_ID(DB_MOD_11),
    TABLE_ID(DC_MOD_N11),
    TABLE_ID(DC_MOD_11),
    TABLE_ID(DD_MOD_N11),
    TABLE_ID(DD_MOD_11),
    TABLE_ID(DE_MOD_N11),
    TABLE_ID(DE_MOD_11),
    TABLE_ID(DF_MOD_N11),
    TABLE_ID(DF_MOD_11),

    /* 2-byte table ids */
    TABLE_ID(0F),
    TABLE_ID(0F_00),
    TABLE_ID(0F_01),
    TABLE_ID(0F_0D),
    TABLE_ID(0F_0F), /* 3DNow! */
    TABLE_ID(0F_12),
    TABLE_ID(0F_16),
    TABLE_ID(0F_18),
    TABLE_ID(0F_19),
    TABLE_ID(0F_1C),
    TABLE_ID(0F_1D),
    TABLE_ID(0F_1E),
    TABLE_ID(0F_1F),
    TABLE_ID(0F_6E),
    TABLE_ID(0F_71),
    TABLE_ID(0F_71_REG2),
    TABLE_ID(0F_71_REG4),
    TABLE_ID(0F_71_REG6),
    TABLE_ID(0F_72),
    TABLE_ID(0F_72_REG2),
    TABLE_ID(0F_72_REG4), 
    TABLE_ID(0F_72_REG6),
    TABLE_ID(0F_73),
    TABLE_ID(0F_73_REG2),
    TABLE_ID(0F_73_REG6),
    TABLE_ID(0F_7E),
    TABLE_ID(0F_AE),
    TABLE_ID(0F_B9),
    TABLE_ID(0F_BA),
    TABLE_ID(0F_C4),
    TABLE_ID(0F_C7),
    TABLE_ID(0F_00_REG0),
    TABLE_ID(0F_00_REG1),
    TABLE_ID(0F_01_REG0),
    TABLE_ID(0F_01_REG1),
    TABLE_ID(0F_01_REG2),
    TABLE_ID(0F_01_REG3),
    TABLE_ID(0F_01_REG4),
    TABLE_ID(0F_01_REG7),
    TABLE_ID(0F_01_REG0_RM),
    TABLE_ID(0F_01_REG1_RM),
    TABLE_ID(0F_01_REG2_RM),
    TABLE_ID(0F_01_REG3_RM),
    TABLE_ID(0F_01_REG7_RM),
    TABLE_ID(0F_18_REG0),
    TABLE_ID(0F_18_REG1),
    TABLE_ID(0F_18_REG2),
    TABLE_ID(0F_18_REG3),
    TABLE_ID(0F_AE_REG0),
    TABLE_ID(0F_AE_REG1),
    TABLE_ID(0F_AE_REG2),
    TABLE_ID(0F_AE_REG3),
    TABLE_ID(0F_AE_REG4),
    TABLE_ID(0F_AE_REG5),
    TABLE_ID(0F_AE_REG6),
    TABLE_ID(0F_AE_REG7),
    TABLE_ID(0F_C7_REG1),
    TABLE_ID(0F_C7_REG3),
    TABLE_ID(0F_C7_REG4),
    TABLE_ID(0F_C7_REG5),
    TABLE_ID(0F_C7_REG6),
    TABLE_ID(0F_C7_REG7),
    TABLE_ID(66_0F),
    TABLE_ID(66_0F_19),
    TABLE_ID(66_0F_1C),
    TABLE_ID(66_0F_1D),
    TABLE_ID(66_0F_1E),
    TABLE_ID(66_0F_1F),
    TABLE_ID(66_0F_6E),
    TABLE_ID(66_0F_71),
    TABLE_ID(66_0F_71_REG2),
    TABLE_ID(66_0F_71_REG4),
    TABLE_ID(66_0F_71_REG6),
    TABLE_ID(66_0F_72),
    TABLE_ID(66_0F_72_REG2),
    TABLE_ID(66_0F_72_REG4),
    TABLE_ID(66_0F_72_REG6),
    TABLE_ID(66_0F_73),
    TABLE_ID(66_0F_73_REG2),
    TABLE_ID(66_0F_73_REG3),
    TABLE_ID(66_0F_73_REG6),
    TABLE_ID(66_0F_73_REG7),
    TABLE_ID(66_0F_78),
    TABLE_ID(66_0F_7E),
    TABLE_ID(66_0F_AE),
    TABLE_ID(66_0F_C4),
    TABLE_ID(66_0F_C7), 
    TABLE_ID(66_0F_C7_REG6),
    TABLE_ID(F3_0F),
    TABLE_ID(F3_0F_01),
    TABLE_ID(F3_0F_01_REG5),
    TABLE_ID(F3_0F_01_REG5_RM),
    TABLE_ID(F3_0F_19),
    TABLE_ID(F3_0F_1C),
    TABLE_ID(F3_0F_1D),
    TABLE_ID(F3_0F_1E),
    TABLE_ID(F3_0F_1E_REG1),
    TABLE_ID(F3_0F_1E_REG7_RM),
    TABLE_ID(F3_0F_1F),
    TABLE_ID(F3_0F_71),
    TABLE_ID(F3_0F_72),
    TABLE_ID(F3_0F_73),
    TABLE_ID(F3_0F_AE),
    TABLE_ID(F3_0F_AE_REG5),
    TABLE_ID(F3_0F_C7),
    TABLE_ID(F3_0F_C7_REG6),
    TABLE_ID(F3_0F_C7_REG7),
    TABLE_ID(F2_0F),
    TABLE_ID(F2_0F_19),
    TABLE_ID(F2_0F_1C),
    TABLE_ID(F2_0F_1D),
    TABLE_ID(F2_0F_1E),
    TABLE_ID(F2_0F_1F),
    TABLE_ID(F2_0F_71),
    TABLE_ID(F2_0F_72),
    TABLE_ID(F2_0F_73),
    TABLE_ID(F2_0F_AE),

    /* 3-byte table ids */
    TABLE_ID(0F_38),
    TABLE_ID(0F_38_F3),
    TABLE_ID(0F_38_F6),
    TABLE_ID(0F_3A),
    TABLE_ID(66_0F_38),
    TABLE_ID(66_0F_38_20),
    TABLE_ID(66_0F_38_21),
    TABLE_ID(66_0F_38_22),
    TABLE_ID(66_0F_38_23),
    TABLE_ID(66_0F_38_24),
    TABLE_ID(66_0F_38_25),
    TABLE_ID(66_0F_38_30),
    TABLE_ID(66_0F_38_31),
    TABLE_ID(66_0F_38_32),
    TABLE_ID(66_0F_38_33),
    TABLE_ID(66_0F_38_34),
    TABLE_ID(66_0F_38_35),
    TABLE_ID(66_0F_38_45),
    TABLE_ID(66_0F_38_47),
    TABLE_ID(66_0F_38_8C),
    TABLE_ID(66_0F_38_8E),
    TABLE_ID(66_0F_38_90),
    TABLE_ID(66_0F_38_91),
    TABLE_ID(66_0F_38_92),
    TABLE_ID(66_0F_38_93),
    TABLE_ID(66_0F_38_96),
    TABLE_ID(66_0F_38_97),
    TABLE_ID(66_0F_38_98),
    TABLE_ID(66_0F_38_99),
    TABLE_ID(66_0F_38_9A),
    TABLE_ID(66_0F_38_9B),
    TABLE_ID(66_0F_38_9C),
    TABLE_ID(66_0F_38_9D),
    TABLE_ID(66_0F_38_9E),
    TABLE_ID(66_0F_38_9F),
    TABLE_ID(66_0F_38_A6),
    TABLE_ID(66_0F_38_A7),
    TABLE_ID(66_0F_38_A8),
    TABLE_ID(66_0F_38_A9),
    TABLE_ID(66_0F_38_AA),
    TABLE_ID(66_0F_38_AB),
    TABLE_ID(66_0F_38_AC),
    TABLE_ID(66_0F_38_AD),
    TABLE_ID(66_0F_38_AE),
    TABLE_ID(66_0F_38_AF),
    TABLE_ID(66_0F_38_B6),
    TABLE_ID(66_0F_38_B7),
    TABLE_ID(66_0F_38_B8),
    TABLE_ID(66_0F_38_B9),
    TABLE_ID(66_0F_38_BA),
    TABLE_ID(66_0F_38_BB),
    TABLE_ID(66_0F_38_BC),
    TABLE_ID(66_0F_38_BD),
    TABLE_ID(66_0F_38_BE),
    TABLE_ID(66_0F_38_BF),
    TABLE_ID(66_0F_38_F5),
    TABLE_ID(66_0F_3A),
    TABLE_ID(66_0F_3A_14),
    TABLE_ID(66_0F_3A_15),
    TABLE_ID(66_0F_3A_16),
    TABLE_ID(66_0F_3A_20),
    TABLE_ID(66_0F_3A_21),
    TABLE_ID(66_0F_3A_22),
    TABLE_ID(F3_0F_38),
    TABLE_ID(F3_0F_3A),
    TABLE_ID(F2_0F_38),
    TABLE_ID(F2_0F_3A),

    /* XOP */
    TABLE_ID(XOP_8),
    TABLE_ID(XOP_8_A2),
    TABLE_ID(XOP_8_A3),
    TABLE_ID(XOP_9), 
    TABLE_ID(XOP_9_1),
    TABLE_ID(XOP_9_2),
    TABLE_ID(XOP_9_12),
    TABLE_ID(XOP_9_90), 
    TABLE_ID(XOP_9_91),
    TABLE_ID(XOP_9_92),
    TABLE_ID(XOP_9_93),
    TABLE_ID(XOP_9_94),
    TABLE_ID(XOP_9_95),
    TABLE_ID(XOP_9_96),
    TABLE_ID(XOP_9_97),
    TABLE_ID(XOP_9_98),
    TABLE_ID(XOP_9_99),
    TABLE_ID(XOP_9_9A),
    TABLE_ID(XOP_9_9B), 
    TABLE_ID(XOP_A),
    TABLE_ID(XOP_A_12),

    /* FMA4 */
    TABLE_ID(66_0F_3A_69),

    TABLE_ID(66_0F_3A_5C),
    TABLE_ID(66_0F_3A_5D),
    TABLE_ID(66_0F_3A_5E),
    TABLE_ID(66_0F_3A_5F),
    TABLE_ID(66_0F_3A_68),
    TABLE_ID(66_0F_3A_6A),
    TABLE_ID(66_0F_3A_6B),
    TABLE_ID(66_0F_3A_6C),
    TABLE_ID(66_0F_3A_6D),
    TABLE_ID(66_0F_3A_6E),
    TABLE_ID(66_0F_3A_6F),
    TABLE_ID(66_0F_3A_78),
    TABLE_ID(66_0F_3A_79),
    TABLE_ID(66_0F_3A_7A),
    TABLE_ID(66_0F_3A_7B),
    TABLE_ID(66_0F_3A_7C),
    TABLE_ID(66_0F_3A_7D),
    TABLE_ID(66_0F_3A_7E),
    TABLE_ID(66_0F_3A_7F),

    /* do not use to dereference! */
    TABLE_ID(NONE)
};

/****************************************************************************

    table structs

****************************************************************************/

typedef struct
{
    uint8_t am; /* addressing method */
    uint8_t ot; /* operand type */

} x86_table_operand_t;

typedef struct
{
    uint16_t                table_id;       /* table id */
    uint8_t                 iset;           /* instruction set */
    uint16_t                mnem_id;        /* mnemonic id */

#if !defined(X86_DASM_NOFMT) || defined(X86_DASM_DEBUG)

    char*                   mnem_str;       /* mnemonic string */

#endif

    uint32_t                properties;     /* properties */
    x86_table_operand_t     operands[4];    /* operands */
    
} x86_table_entry_t;

typedef struct
{

#if defined(X86_DASM_DEBUG)

    char*                   table_name;

#endif

    const x86_table_entry_t*    table;
    uint8_t                     lookup_method;

} x86_table_link_t;

/****************************************************************************

    macro acrobatics to overload the OP() macro

****************************************************************************/

#define EXPAND(x) x
#define OPERAND(x) {x}

#define OP_0 { \
    OPERAND(O_NONE), \
    OPERAND(O_NONE), \
    OPERAND(O_NONE), \
    OPERAND(O_NONE) }

#define OP_1(_1) { \
    OPERAND(O_##_1), \
    OPERAND(O_NONE), \
    OPERAND(O_NONE), \
    OPERAND(O_NONE) }

#define OP_2(_1, _2) { \
    OPERAND(O_##_1), \
    OPERAND(O_##_2), \
    OPERAND(O_NONE), \
    OPERAND(O_NONE) }

#define OP_3(_1, _2, _3) { \
    OPERAND(O_##_1), \
    OPERAND(O_##_2), \
    OPERAND(O_##_3), \
    OPERAND(O_NONE) }

#define OP_4(_1, _2, _3, _4) { \
    OPERAND(O_##_1), \
    OPERAND(O_##_2), \
    OPERAND(O_##_3), \
    OPERAND(O_##_4) }

#define PP_NARG(...) EXPAND(PP_NARG_(__VA_ARGS__, PP_RSEQ_N()))
#define PP_NARG_(...) EXPAND(PP_ARG_N(__VA_ARGS__))
#define PP_ARG_N(_1, _2, _3, _4, N, ...) N
#define PP_RSEQ_N() 4, 3, 2, 1, 0

#define OP_(N) OP_##N
#define OP_EVAL(N) OP_(N)

#define OP(...) \
    EXPAND(OP_EVAL(EXPAND(PP_NARG(__VA_ARGS__)))(__VA_ARGS__))

/****************************************************************************

    table macros

****************************************************************************/

#define TABLE_NAME(_1) table_##_1 

#define TABLE_BEGIN(_1) \
    static const x86_table_entry_t TABLE_NAME(_1)[] = {

#define TABLE_END(_1) \
    };

#if !defined(X86_DASM_NOFMT) || defined(X86_DASM_DEBUG)

#define TABLE_ENTRY(_1, _2, _3, _4, _5) { \
    /* table id  */         TABLE_ID(NONE), \
    /* instruction set */   _4, \
    /* mnemonic id */       _2, \
    /* mnemonic string */   _1, \
    /* properties */        _3, \
    /* operands */          _5, }   

#define TABLE_ENTRY_INVALID() { \
    /* table id  */         TABLE_ID(NONE), \
    /* instruction set */   X86_ISET_NONE, \
    /* mnemonic id */       X86_MN_INVALID, \
    /* mnemonic string */   NULL, \
    /* properties */        PF_NONE, \
    /* operands */          OP_0 }

#define TABLE_ENTRY_ESCAPE(_1) { \
    /* table id  */         TABLE_ID(_1), \
    /* instruction set */   X86_ISET_NONE, \
    /* mnemonic id */       X86_MN_INVALID, \
    /* mnemonic string */   NULL, \
    /* properties */        PF_NONE, \
    /* operands */          OP_0 }

#else

#define TABLE_ENTRY(_1, _2, _3, _4, _5) { \
    /* table id  */         TABLE_ID(NONE), \
    /* instruction set */   _4, \
    /* mnemonic id */       _2, \
    /* properties */        _3, \
    /* operands */          _5, }   

#define TABLE_ENTRY_INVALID() { \
    /* table id  */         TABLE_ID(NONE), \
    /* instruction set */   X86_ISET_NONE, \
    /* mnemonic id */       X86_MN_INVALID, \
    /* properties */        PF_NONE, \
    /* operands */          OP_0 }

#define TABLE_ENTRY_ESCAPE(_1) { \
    /* table id  */         TABLE_ID(_1), \
    /* instruction set */   X86_ISET_NONE, \
    /* mnemonic id */       X86_MN_INVALID, \
    /* properties */        PF_NONE, \
    /* operands */          OP_0 }

#endif

#if defined(X86_DASM_DEBUG)

#define TABLE_LINK(_1, _2) { \
    /* table_name */        #_1, \
    /* table */             TABLE_NAME(_1), \
    /* lookup method */     TABLE_LOOKUP_METHOD(_2) }

#else

#define TABLE_LINK(_1, _2) { \
    /* table */             TABLE_NAME(_1), \
    /* lookup method */     TABLE_LOOKUP_METHOD(_2) }

#endif

#define EFLAGS_ENTRY(_1, _2, _3, _4, _5, _6, _7, _8) \
    { X86_FLAG_##_1, X86_FLAG_##_2, X86_FLAG_##_3, X86_FLAG_##_4, \
      X86_FLAG_##_5, X86_FLAG_##_6, X86_FLAG_##_7, X86_FLAG_##_8 }

/****************************************************************************

    declarations / exports

****************************************************************************/

typedef struct
{
    uint8_t o : 3; /* overflow flag */
    uint8_t d : 3; /* direction flag */
    uint8_t i : 3; /* interrupt flag */
    uint8_t s : 3; /* sign flag */
    uint8_t z : 3; /* zero flag */
    uint8_t a : 3; /* aux carry flag */
    uint8_t p : 3; /* parity flag */
    uint8_t c : 3; /* carry flag */

} x86_eflags_t;

extern const x86_table_link_t x86_table_links[];
extern const x86_eflags_t x86_eflags_table[];

x86_eflags_t x86_eflags_get(uint16_t mnem);

#ifdef __cplusplus
    }
#endif

#endif //_DASM_H_
#ifndef _DASM_H_
#define _DASM_H_

#include <limits.h>

#ifdef __cplusplus
    extern "C" {
#endif

/* 
    #define X86_DASM_DEBUG      : enable debug output
    #define X86_DASM_NOFMT      : disable all formatting/strings
    #define X86_DASM_NLE        : if not on a little-endian system
*/

#define BIT_MASK(__TYPE__, __ONE_COUNT__) \
    ((__TYPE__) (-((__ONE_COUNT__) != 0))) \
    & (((__TYPE__) -1) >> ((sizeof(__TYPE__) * CHAR_BIT) - (__ONE_COUNT__)))

/****************************************************************************

    x86 registers

****************************************************************************/

enum 
{
    /* 64-bit general-purpose registers */
    X86_REG_RAX = 0,
    X86_REG_RCX,
    X86_REG_RDX,    
    X86_REG_RBX,
    X86_REG_RSP,    
    X86_REG_RBP,    
    X86_REG_RSI,
    X86_REG_RDI,
    X86_REG_R8, 
    X86_REG_R9,     
    X86_REG_R10,    
    X86_REG_R11,
    X86_REG_R12,    
    X86_REG_R13,    
    X86_REG_R14,    
    X86_REG_R15,
    X86_REG_RIP,

    /* 32-bit general-purpose registers */
    X86_REG_EAX,    
    X86_REG_ECX,    
    X86_REG_EDX,    
    X86_REG_EBX,
    X86_REG_ESP,    
    X86_REG_EBP,    
    X86_REG_ESI,    
    X86_REG_EDI,
    X86_REG_R8D,    
    X86_REG_R9D,    
    X86_REG_R10D,
    X86_REG_R11D,
    X86_REG_R12D,   
    X86_REG_R13D,   
    X86_REG_R14D,   
    X86_REG_R15D,
    X86_REG_EIP,

    /* 16-bit general-purpose registers */
    X86_REG_AX,     
    X86_REG_CX,     
    X86_REG_DX,     
    X86_REG_BX,
    X86_REG_SP,     
    X86_REG_BP, 
    X86_REG_SI,     
    X86_REG_DI,
    X86_REG_R8W,    
    X86_REG_R9W,    
    X86_REG_R10W,
    X86_REG_R11W,
    X86_REG_R12W,   
    X86_REG_R13W,   
    X86_REG_R14W,   
    X86_REG_R15W,

    /* 8-bit general-purpose registers */
    X86_REG_AL, 
    X86_REG_CL,     
    X86_REG_DL,     
    X86_REG_BL,
    X86_REG_AH, /* not addressable in REX prefix instruction forms */       
    X86_REG_CH, /* not addressable in REX prefix instruction forms */       
    X86_REG_DH, /* not addressable in REX prefix instruction forms */       
    X86_REG_BH, /* not addressable in REX prefix instruction forms */
    X86_REG_SPL,    /* only addressable in REX prefix instruction forms */  
    X86_REG_BPL,    /* only addressable in REX prefix instruction forms */  
    X86_REG_SIL,    /* only addressable in REX prefix instruction forms */  
    X86_REG_DIL,    /* only addressable in REX prefix instruction forms */
    X86_REG_R8L,    
    X86_REG_R9L,    
    X86_REG_R10L,
    X86_REG_R11L,
    X86_REG_R12L,   
    X86_REG_R13L,   
    X86_REG_R14L,   
    X86_REG_R15L,

    /* segment registers */
    X86_REG_ES, /* ignored in long 64-bit mode */
    X86_REG_CS,
    X86_REG_SS, /* ignored in long 64-bit mode */
    X86_REG_DS, /* ignored in long 64-bit mode */
    X86_REG_FS,
    X86_REG_GS,

    /* FPU x87 registers, FPR0-FPR7 */
    X86_REG_ST0,
    X86_REG_ST1,    
    X86_REG_ST2,    
    X86_REG_ST3,
    X86_REG_ST4,    
    X86_REG_ST5,    
    X86_REG_ST6,    
    X86_REG_ST7, 

    /* MMX registers */
    X86_REG_MM0,    
    X86_REG_MM1,
    X86_REG_MM2,    
    X86_REG_MM3,
    X86_REG_MM4,    
    X86_REG_MM5,    
    X86_REG_MM6,    
    X86_REG_MM7,

    /* XMM registers */
    X86_REG_XMM0,
    X86_REG_XMM1,
    X86_REG_XMM2,   
    X86_REG_XMM3,
    X86_REG_XMM4,
    X86_REG_XMM5,
    X86_REG_XMM6,
    X86_REG_XMM7,
    X86_REG_XMM8,
    X86_REG_XMM9,
    X86_REG_XMM10,
    X86_REG_XMM11,
    X86_REG_XMM12,  
    X86_REG_XMM13,  
    X86_REG_XMM14,  
    X86_REG_XMM15,
    X86_REG_XMM16, /* AVX-512 */
    X86_REG_XMM17, /* AVX-512 */
    X86_REG_XMM18, /* AVX-512 */
    X86_REG_XMM19, /* AVX-512 */
    X86_REG_XMM20, /* AVX-512 */
    X86_REG_XMM21, /* AVX-512 */
    X86_REG_XMM22, /* AVX-512 */
    X86_REG_XMM23, /* AVX-512 */
    X86_REG_XMM24, /* AVX-512 */
    X86_REG_XMM25, /* AVX-512 */
    X86_REG_XMM26, /* AVX-512 */
    X86_REG_XMM27, /* AVX-512 */
    X86_REG_XMM28, /* AVX-512 */
    X86_REG_XMM29, /* AVX-512 */
    X86_REG_XMM30, /* AVX-512 */
    X86_REG_XMM31, /* AVX-512 */

    /* YMM registers */
    X86_REG_YMM0,
    X86_REG_YMM1,
    X86_REG_YMM2,   
    X86_REG_YMM3,
    X86_REG_YMM4,
    X86_REG_YMM5,
    X86_REG_YMM6,
    X86_REG_YMM7,
    X86_REG_YMM8,
    X86_REG_YMM9,
    X86_REG_YMM10,
    X86_REG_YMM11,
    X86_REG_YMM12,  
    X86_REG_YMM13,  
    X86_REG_YMM14,  
    X86_REG_YMM15,
    X86_REG_YMM16, /* AVX-512 */
    X86_REG_YMM17, /* AVX-512 */
    X86_REG_YMM18, /* AVX-512 */
    X86_REG_YMM19, /* AVX-512 */
    X86_REG_YMM20, /* AVX-512 */
    X86_REG_YMM21, /* AVX-512 */
    X86_REG_YMM22, /* AVX-512 */
    X86_REG_YMM23, /* AVX-512 */
    X86_REG_YMM24, /* AVX-512 */
    X86_REG_YMM25, /* AVX-512 */
    X86_REG_YMM26, /* AVX-512 */
    X86_REG_YMM27, /* AVX-512 */
    X86_REG_YMM28, /* AVX-512 */
    X86_REG_YMM29, /* AVX-512 */
    X86_REG_YMM30, /* AVX-512 */
    X86_REG_YMM31, /* AVX-512 */

    /* ZMM registers */
    X86_REG_ZMM0,
    X86_REG_ZMM1,
    X86_REG_ZMM2,   
    X86_REG_ZMM3,
    X86_REG_ZMM4,
    X86_REG_ZMM5,
    X86_REG_ZMM6,
    X86_REG_ZMM7,
    X86_REG_ZMM8,
    X86_REG_ZMM9,
    X86_REG_ZMM10,
    X86_REG_ZMM11,
    X86_REG_ZMM12,  
    X86_REG_ZMM13,  
    X86_REG_ZMM14,  
    X86_REG_ZMM15,
    X86_REG_ZMM16, /* AVX-512 */
    X86_REG_ZMM17, /* AVX-512 */
    X86_REG_ZMM18, /* AVX-512 */
    X86_REG_ZMM19, /* AVX-512 */
    X86_REG_ZMM20, /* AVX-512 */
    X86_REG_ZMM21, /* AVX-512 */
    X86_REG_ZMM22, /* AVX-512 */
    X86_REG_ZMM23, /* AVX-512 */
    X86_REG_ZMM24, /* AVX-512 */
    X86_REG_ZMM25, /* AVX-512 */
    X86_REG_ZMM26, /* AVX-512 */
    X86_REG_ZMM27, /* AVX-512 */
    X86_REG_ZMM28, /* AVX-512 */
    X86_REG_ZMM29, /* AVX-512 */
    X86_REG_ZMM30, /* AVX-512 */
    X86_REG_ZMM31, /* AVX-512 */

    /* debug registers */
    X86_REG_DR0,
    X86_REG_DR1,    
    X86_REG_DR2,    
    X86_REG_DR3,
    X86_REG_DR4,    
    X86_REG_DR5,    
    X86_REG_DR6,
    X86_REG_DR7,
    X86_REG_DR8,    /* reserved: undefined opcode (#UD) exception */    
    X86_REG_DR9,    /* reserved: undefined opcode (#UD) exception */
    X86_REG_DR10,   /* reserved: undefined opcode (#UD) exception */
    X86_REG_DR11,   /* reserved: undefined opcode (#UD) exception */
    X86_REG_DR12,   /* reserved: undefined opcode (#UD) exception */
    X86_REG_DR13,   /* reserved: undefined opcode (#UD) exception */
    X86_REG_DR14,   /* reserved: undefined opcode (#UD) exception */
    X86_REG_DR15,   /* reserved: undefined opcode (#UD) exception */

    /* control registers */
    X86_REG_CR0,    
    X86_REG_CR1,    /* reserved: undefined opcode (#UD) exception */    
    X86_REG_CR2,    
    X86_REG_CR3,
    X86_REG_CR4,    
    X86_REG_CR5,    /* reserved: undefined opcode (#UD) exception */    
    X86_REG_CR6,    /* reserved: undefined opcode (#UD) exception */    
    X86_REG_CR7,    /* reserved: undefined opcode (#UD) exception */
    X86_REG_CR8,
    X86_REG_CR9,    /* reserved: undefined opcode (#UD) exception */
    X86_REG_CR10,   /* reserved: undefined opcode (#UD) exception */
    X86_REG_CR11,   /* reserved: undefined opcode (#UD) exception */
    X86_REG_CR12,   /* reserved: undefined opcode (#UD) exception */     
    X86_REG_CR13,   /* reserved: undefined opcode (#UD) exception */
    X86_REG_CR14,   /* reserved: undefined opcode (#UD) exception */    
    X86_REG_CR15,   /* reserved: undefined opcode (#UD) exception */

    /* used only internally! */
    X86_REG_GPR,
    X86_REG_NONE /* invalid or not present */
};

/****************************************************************************

    x86 instruction sets

****************************************************************************/

enum
{
    X86_ISET_NONE = 0,
    X86_ISET_GP,    /* general-purpose instructions */
    X86_ISET_BMI1,  /* bit manipulation instructions */
    X86_ISET_BMI2,  
    X86_ISET_FPU,   /* floating-point unit (x87 co-processor) instructions */
    X86_ISET_MMX,   /* single instruction, multiple data (SIMD) instructions */
    X86_ISET_SSE,   
    X86_ISET_SSE2,
    X86_ISET_SSE3,  
    X86_ISET_SSSE3,
    X86_ISET_SSE4,  
    X86_ISET_SSE41, 
    X86_ISET_SSE42, 
    X86_ISET_SSE4A, 
    X86_ISET_AESNI, /* advanced encryption standard new instructions */
    X86_ISET_VMX,   /* intel virtual-machine extension instructions */
    X86_ISET_SVM,   /* amd secure virtual machine instructions */
    X86_ISET_FMA,   /* fused multiply add (fma) instructions */
    X86_ISET_FMA4,
    X86_ISET_F16C,
    X86_ISET_PCLMULQDQ,
    X86_ISET_ADX,
    X86_ISET_AVX,   /* advanced vector extension instructions */
    X86_ISET_AVX2,  
    X86_ISET_AVX512,
    X86_ISET_XOP,
    X86_ISET_3DNOW,
    X86_ISET_TSX,
    X86_ISET_CET, /* control-flow enforcement technology */
    X86_ISET_UNKNOWN
};

/****************************************************************************

    x86 mnemonics

****************************************************************************/

enum
{
    X86_MN_INVALID = 0,

    X86_MN_AAA,
    X86_MN_AAD,
    X86_MN_AAM,
    X86_MN_AAS,
    X86_MN_ADC,
    X86_MN_ADCX,
    X86_MN_ADD,
    X86_MN_ADDPD,
    X86_MN_ADDPS,
    X86_MN_ADDSD,
    X86_MN_ADDSS,
    X86_MN_ADDSUBPD,
    X86_MN_ADDSUBPS,
    X86_MN_ADOX,
    X86_MN_ADX,
    X86_MN_AESDEC,
    X86_MN_AESDECLAST,
    X86_MN_AESENC,
    X86_MN_AESENCLAST,
    X86_MN_AESIMC,
    X86_MN_AESKEYGEN,
    X86_MN_AESKEYGENASSIST,
    X86_MN_AMX,
    X86_MN_AND,
    X86_MN_ANDN,
    X86_MN_ANDNPD,
    X86_MN_ANDNPS,
    X86_MN_ANDPD,
    X86_MN_ANDPS,
    X86_MN_ARPL,
    X86_MN_BEXTR,
    X86_MN_BLENDPD,
    X86_MN_BLENDPS,
    X86_MN_BLENDVPD,
    X86_MN_BLENDVPS,
    X86_MN_BLSI,
    X86_MN_BLSMSK,
    X86_MN_BLSR,
    X86_MN_BOUND,
    X86_MN_BROADCASTF128,
    X86_MN_BROADCASTI128,
    X86_MN_BROADCASTSD, 
    X86_MN_BROADCASTSS,
    X86_MN_BSF,
    X86_MN_BSR,
    X86_MN_BSWAP,
    X86_MN_BT,
    X86_MN_BTC,
    X86_MN_BTR,
    X86_MN_BTS,
    X86_MN_BZHI,
    X86_MN_CALL,
    X86_MN_CALLF,
    X86_MN_CBW,
    X86_MN_CDQ,
    X86_MN_CDQE,
    X86_MN_CLAC,
    X86_MN_CLC,
    X86_MN_CLD,
    X86_MN_CLFLUSH,
    X86_MN_CLGI,
    X86_MN_CLI,
    X86_MN_CLTS,
    X86_MN_CMC,
    X86_MN_CMOVA,
    X86_MN_CMOVNC,
    X86_MN_CMOVC,
    X86_MN_CMOVNA,
    X86_MN_CMOVZ,
    X86_MN_CMOVG,
    X86_MN_CMOVNL,
    X86_MN_CMOVL,
    X86_MN_CMOVNG,
    X86_MN_CMOVNZ,
    X86_MN_CMOVNO,
    X86_MN_CMOVNP,
    X86_MN_CMOVNS,
    X86_MN_CMOVO,
    X86_MN_CMOVP,
    X86_MN_CMOVPO,
    X86_MN_CMOVS,
    X86_MN_CMP,
    X86_MN_CMPPD,
    X86_MN_CMPPS,
    X86_MN_CMPS,
    X86_MN_CMPSB,
    X86_MN_CMPSW,
    X86_MN_CMPSQ,
    X86_MN_CMPSD,
    X86_MN_CMPSS,
    X86_MN_CMPXCHG,
    X86_MN_CMPXCHG16B,
    X86_MN_CMPXCHG8B,
    X86_MN_COMISD,
    X86_MN_COMISS,
    X86_MN_CPUID,
    X86_MN_CQO,
    X86_MN_CRC32,
    X86_MN_CVTDQ2PD,
    X86_MN_CVTDQ2PS,
    X86_MN_CVTPD2DQ,
    X86_MN_CVTPD2PI,
    X86_MN_CVTPD2PS,
    X86_MN_CVTPH2PS,
    X86_MN_CVTPI2PD,
    X86_MN_CVTPI2PS,
    X86_MN_CVTPS2DQ,
    X86_MN_CVTPS2PD,
    X86_MN_CVTPS2PH,
    X86_MN_CVTPS2PI,
    X86_MN_CVTSD2SI,
    X86_MN_CVTSD2SS,
    X86_MN_CVTSI2SD,
    X86_MN_CVTSI2SS,
    X86_MN_CVTSS2SD,
    X86_MN_CVTSS2SI,
    X86_MN_CVTTPD2DQ,
    X86_MN_CVTTPD2PI,
    X86_MN_CVTTPS2DQ,
    X86_MN_CVTTPS2PI,
    X86_MN_CVTTSD2SI,
    X86_MN_CVTTSS2SI,
    X86_MN_CWD,
    X86_MN_CWDE,
    X86_MN_DAA,
    X86_MN_DAS,
    X86_MN_DEC,
    X86_MN_DIV,
    X86_MN_DIVPD,
    X86_MN_DIVPS,
    X86_MN_DIVSD,
    X86_MN_DIVSS,
    X86_MN_DPPD,
    X86_MN_DPPS,
    X86_MN_EMMS,
    X86_MN_ENTER,
    X86_MN_ENDBR32,
    X86_MN_ENDBR64,
    X86_MN_EXTRACTF128,
    X86_MN_EXTRACTI128,
    X86_MN_EXTRACTPS,
    X86_MN_EXTRQ,
    X86_MN_F2XM1,
    X86_MN_FABS,
    X86_MN_FADD,
    X86_MN_FADDP,
    X86_MN_FBLD,
    X86_MN_FBSTP,
    X86_MN_FCHS,
    X86_MN_FCLEX,
    X86_MN_FCMOVB,
    X86_MN_FCMOVBE,
    X86_MN_FCMOVE,
    X86_MN_FCMOVNB,
    X86_MN_FCMOVNBE,
    X86_MN_FCMOVNE,
    X86_MN_FCMOVNU,
    X86_MN_FCMOVU,
    X86_MN_FCOM,
    X86_MN_FCOM2,
    X86_MN_FCOMI,
    X86_MN_FCOMIP,
    X86_MN_FCOMP,
    X86_MN_FCOMP3,
    X86_MN_FCOMP5,
    X86_MN_FCOMPP,
    X86_MN_FCOS,
    X86_MN_FDECSTP,
    X86_MN_FDIV,
    X86_MN_FDIVP,
    X86_MN_FDIVR,
    X86_MN_FDIVRP,
    X86_MN_FEMMS,
    X86_MN_FFREE,
    X86_MN_FIADD,
    X86_MN_FICOM,
    X86_MN_FICOMP,
    X86_MN_FIDIV,
    X86_MN_FIDIVR,
    X86_MN_FILD,
    X86_MN_FIMUL,
    X86_MN_FINCSTP,
    X86_MN_FINIT,
    X86_MN_FIST,
    X86_MN_FISTP,
    X86_MN_FISTTP,
    X86_MN_FISUB,
    X86_MN_FISUBR,
    X86_MN_FLD,
    X86_MN_FLD1,
    X86_MN_FLDCW,
    X86_MN_FLDENV,
    X86_MN_FLDL2E,
    X86_MN_FLDL2T,
    X86_MN_FLDLG2,
    X86_MN_FLDLN2,
    X86_MN_FLDPI,
    X86_MN_FLDZ,
    X86_MN_FMADDPD,
    X86_MN_FMADD132PD,
    X86_MN_FMADD132PS,
    X86_MN_FMADD132SD,
    X86_MN_FMADD132SS,
    X86_MN_FMADD213PD,
    X86_MN_FMADD213PS,
    X86_MN_FMADD213SD,
    X86_MN_FMADD213SS,
    X86_MN_FMADD231PD,
    X86_MN_FMADD231PS,
    X86_MN_FMADD231SD,
    X86_MN_FMADD231SS,
    X86_MN_FMADDSUB132PD,
    X86_MN_FMADDSUB132PS,
    X86_MN_FMADDSUB213PD,
    X86_MN_FMADDSUB213PS,
    X86_MN_FMADDSUB231PD,
    X86_MN_FMADDSUB231PS,
    X86_MN_FMSUB132PD,
    X86_MN_FMSUB132PS,
    X86_MN_FMSUB132SD,
    X86_MN_FMSUB132SS,
    X86_MN_FMSUB213PD,
    X86_MN_FMSUB213PS,
    X86_MN_FMSUB213SD,
    X86_MN_FMSUB213SS,
    X86_MN_FMSUB231PD,
    X86_MN_FMSUB231PS,
    X86_MN_FMSUB231SD,
    X86_MN_FMSUB231SS,
    X86_MN_FMSUBADD132PD,
    X86_MN_FMSUBADD132PS,
    X86_MN_FMSUBADD213PD,
    X86_MN_FMSUBADD213PS,
    X86_MN_FMSUBADD231PD,
    X86_MN_FMSUBADD231PS,
    X86_MN_FMUL,
    X86_MN_FMULP,
    X86_MN_FNCLEX,
    X86_MN_FNDISI,
    X86_MN_FNENI,
    X86_MN_FNINIT,
    X86_MN_FNMADD132PD,
    X86_MN_FNMADD132PS,
    X86_MN_FNMADD132SD,
    X86_MN_FNMADD132SS,
    X86_MN_FNMADD213PD,
    X86_MN_FNMADD213PS,
    X86_MN_FNMADD213SD,
    X86_MN_FNMADD213SS,
    X86_MN_FNMADD231PD,
    X86_MN_FNMADD231PS,
    X86_MN_FNMADD231SD,
    X86_MN_FNMADD231SS,
    X86_MN_FNMSUB132PD,
    X86_MN_FNMSUB132PS,
    X86_MN_FNMSUB132SD,
    X86_MN_FNMSUB132SS,
    X86_MN_FNMSUB213PD,
    X86_MN_FNMSUB213PS,
    X86_MN_FNMSUB213SD,
    X86_MN_FNMSUB213SS,
    X86_MN_FNMSUB231PD,
    X86_MN_FNMSUB231PS,
    X86_MN_FNMSUB231SD,
    X86_MN_FNMSUB231SS,
    X86_MN_FNOP,
    X86_MN_FNSAVE,
    X86_MN_FNSETPM,
    X86_MN_FNSTCW,
    X86_MN_FNSTENV,
    X86_MN_FNSTSW,
    X86_MN_FPATAN,
    X86_MN_FPREM,
    X86_MN_FPREM1,
    X86_MN_FPTAN,
    X86_MN_FRNDINT,
    X86_MN_FRSTOR,
    X86_MN_FSAVE,
    X86_MN_FSCALE,
    X86_MN_FSIN,
    X86_MN_FSINCOS,
    X86_MN_FSQRT,
    X86_MN_FST,
    X86_MN_FSTCW,
    X86_MN_FSTENV,
    X86_MN_FSTP,
    X86_MN_FSTP1,
    X86_MN_FSTP8,
    X86_MN_FSTP9,
    X86_MN_FSTSW,
    X86_MN_FSUB,
    X86_MN_FSUBP,
    X86_MN_FSUBR,
    X86_MN_FSUBRP,
    X86_MN_FTST,
    X86_MN_FUCOM,
    X86_MN_FUCOMI,
    X86_MN_FUCOMIP,
    X86_MN_FUCOMP,
    X86_MN_FUCOMPP,
    X86_MN_FWAIT,
    X86_MN_FXAM,
    X86_MN_FXCH,
    X86_MN_FXRSTOR,
    X86_MN_FXRSTOR64,
    X86_MN_FXSAVE,
    X86_MN_FXSAVE64,
    X86_MN_FXTRACT,
    X86_MN_FYL2X,
    X86_MN_FYL2XP1,
    X86_MN_GATHERDD,
    X86_MN_GATHERDPD,   
    X86_MN_GATHERDPS,
    X86_MN_GATHERDQ,
    X86_MN_GATHERQD,
    X86_MN_GATHERQPD,
    X86_MN_GATHERQPS,   
    X86_MN_GATHERQQ,
    X86_MN_GETSEC,
    X86_MN_HADDPD,
    X86_MN_HADDPS,
    X86_MN_HINTNOP,
    X86_MN_HLT,
    X86_MN_HSUBPD,
    X86_MN_HSUBPS,
    X86_MN_ICEBP,
    X86_MN_IDIV,
    X86_MN_IMUL,
    X86_MN_IN,
    X86_MN_INC,
    X86_MN_INS,
    X86_MN_INSB,
    X86_MN_INSW,
    X86_MN_INSD,
    X86_MN_INSERTF128,
    X86_MN_INSERTI128,
    X86_MN_INSERTPS,
    X86_MN_INSERTQ,
    X86_MN_INT,
    X86_MN_INT3,
    X86_MN_INTO,
    X86_MN_INVD,
    X86_MN_INVEPT,
    X86_MN_INVLPG,
    X86_MN_INVLPGA,
    X86_MN_INVPCID,
    X86_MN_INVVPID,
    X86_MN_IRET,
    X86_MN_IRETW,
    X86_MN_IRETD,
    X86_MN_IRETQ,
    X86_MN_JA,
    X86_MN_JC,
    X86_MN_JNG,
    X86_MN_JZ,
    X86_MN_JG,
    X86_MN_JNL,
    X86_MN_JL,
    X86_MN_JNA,
    X86_MN_JMP,
    X86_MN_JMPF,
    X86_MN_JNC,
    X86_MN_JNZ,
    X86_MN_JNO,
    X86_MN_JNP,
    X86_MN_JNS,
    X86_MN_JO,
    X86_MN_JP,
    X86_MN_JS,
    X86_MN_JCXZ,
    X86_MN_JECXZ,
    X86_MN_JRCXZ,
    X86_MN_LAHF,
    X86_MN_LAR,
    X86_MN_LDDQU,
    X86_MN_LDMXCSR,
    X86_MN_LDS,
    X86_MN_LEA,
    X86_MN_LEAVE,
    X86_MN_LES,
    X86_MN_LFENCE,
    X86_MN_LFS,
    X86_MN_LGDT,
    X86_MN_LGS,
    X86_MN_LIDT,
    X86_MN_LLDT,
    X86_MN_LMSW,
    X86_MN_LODS,
    X86_MN_LODSB,
    X86_MN_LODSW,
    X86_MN_LODSD,
    X86_MN_LODSQ,
    X86_MN_LOOP,
    X86_MN_LOOPZ,
    X86_MN_LOOPNZ,
    X86_MN_LSL,
    X86_MN_LSS,
    X86_MN_LTR,
    X86_MN_LZCNT,
    X86_MN_MASKMOVDQU,
    X86_MN_MASKMOVPD,
    X86_MN_MASKMOVPS,
    X86_MN_MASKMOVQ,
    X86_MN_MAXPD,
    X86_MN_MAXPS,
    X86_MN_MAXSD,
    X86_MN_MAXSS,
    X86_MN_MFENCE,
    X86_MN_MINPD,
    X86_MN_MINPS,
    X86_MN_MINSD,
    X86_MN_MINSS,
    X86_MN_MONITOR,
    X86_MN_MOV,
    X86_MN_MOVAPD,
    X86_MN_MOVAPS,
    X86_MN_MOVBE,
    X86_MN_MOVD,
    X86_MN_MOVDDUP,
    X86_MN_MOVDQ2Q,
    X86_MN_MOVDQA,
    X86_MN_MOVDQU,
    X86_MN_MOVHLPS,
    X86_MN_MOVHPD,
    X86_MN_MOVHPS,
    X86_MN_MOVLHPS,
    X86_MN_MOVLPD,
    X86_MN_MOVLPS,
    X86_MN_MOVMSKPD,
    X86_MN_MOVMSKPS,
    X86_MN_MOVNTDQ,
    X86_MN_MOVNTDQA,
    X86_MN_MOVNTI,
    X86_MN_MOVNTPD,
    X86_MN_MOVNTSS,
    X86_MN_MOVNTSD,
    X86_MN_MOVNTPS,
    X86_MN_MOVNTQ,
    X86_MN_MOVQ,
    X86_MN_MOVQ2DQ,
    X86_MN_MOVS,
    X86_MN_MOVSB,
    X86_MN_MOVSW,
    X86_MN_MOVSD,
    X86_MN_MOVSQ,
    X86_MN_MOVSHDUP,
    X86_MN_MOVSLDUP,
    X86_MN_MOVSS,
    X86_MN_MOVSX,
    X86_MN_MOVSXD,
    X86_MN_MOVUPD,
    X86_MN_MOVUPS,
    X86_MN_MOVZX,
    X86_MN_MPSADBW,
    X86_MN_MUL,
    X86_MN_MULPD,
    X86_MN_MULPS,
    X86_MN_MULSD,
    X86_MN_MULSS,
    X86_MN_MULX,
    X86_MN_MWAIT,
    X86_MN_NEG,
    X86_MN_NOP,
    X86_MN_NOT,
    X86_MN_OR,
    X86_MN_ORPD,
    X86_MN_ORPS,
    X86_MN_OUT,
    X86_MN_OUTS,
    X86_MN_OUTSB,
    X86_MN_OUTSW,
    X86_MN_OUTSD,
    X86_MN_PABSB,
    X86_MN_PABSD,
    X86_MN_PABSW,
    X86_MN_PACKSSDB,
    X86_MN_PACKSSDW,
    X86_MN_PACKSSWB,
    X86_MN_PACKUSDB,
    X86_MN_PACKUSDW,
    X86_MN_PACKUSWB,
    X86_MN_PADDB,
    X86_MN_PADDD,
    X86_MN_PADDQ,
    X86_MN_PADDSB,
    X86_MN_PADDSW,
    X86_MN_PADDUSB,
    X86_MN_PADDUSW,
    X86_MN_PADDW,
    X86_MN_PALIGNR,
    X86_MN_PAND,
    X86_MN_PANDN,
    X86_MN_PAUSE,
    X86_MN_PAVGB,
    X86_MN_PAVGW,
    X86_MN_PBLENDD,
    X86_MN_PBLENDVB,
    X86_MN_PBLENDW,
    X86_MN_PBROADCASTB,
    X86_MN_PBROADCASTD,
    X86_MN_PBROADCASTQ,
    X86_MN_PBROADCASTW,
    X86_MN_PCLMULQDQ,
    X86_MN_PCMPEQB,
    X86_MN_PCMPEQD,
    X86_MN_PCMPEQQ,
    X86_MN_PCMPEQW,
    X86_MN_PCMPESTRI,
    X86_MN_PCMPESTRM,
    X86_MN_PCMPGTB,
    X86_MN_PCMPGTD,
    X86_MN_PCMPGTQ,
    X86_MN_PCMPGTW,
    X86_MN_PCMPISTRI,
    X86_MN_PCMPISTRM,
    X86_MN_PDEP,
    X86_MN_PERM2F128,
    X86_MN_PERM2I128,
    X86_MN_PERMD,
    X86_MN_PERMILPD,
    X86_MN_PERMILPS,
    X86_MN_PERMPD,
    X86_MN_PERMPS,
    X86_MN_PERMQ,
    X86_MN_PEXT,
    X86_MN_PEXTRB,
    X86_MN_PEXTRD,
    X86_MN_PEXTRQ,
    X86_MN_PEXTRW,
    X86_MN_PFRCPIT2,
    X86_MN_PHADDD,
    X86_MN_PHADDSW,
    X86_MN_PHADDW,
    X86_MN_PHMINPOSUW,
    X86_MN_PHSUBD,
    X86_MN_PHSUBSW,
    X86_MN_PHSUBW,
    X86_MN_PINSRB,
    X86_MN_PINSRD,
    X86_MN_PINSRQ,
    X86_MN_PINSRW,
    X86_MN_PMADDUBSW,
    X86_MN_PMADDWD,
    X86_MN_PMADDWS,
    X86_MN_PMASKMOVD,
    X86_MN_PMASKMOVQ,   
    X86_MN_PMAXSB,
    X86_MN_PMAXSD,
    X86_MN_PMAXSW,
    X86_MN_PMAXUB,
    X86_MN_PMAXUD,
    X86_MN_PMAXUW,
    X86_MN_PMINS,
    X86_MN_PMINSB,
    X86_MN_PMINSD,
    X86_MN_PMINSW,
    X86_MN_PMINUB,
    X86_MN_PMINUD,
    X86_MN_PMINUW,
    X86_MN_PMOVMSKB,
    X86_MN_PMOVSX,
    X86_MN_PMOVSXBD,
    X86_MN_PMOVSXBQ,    
    X86_MN_PMOVSXBW,    
    X86_MN_PMOVSXDQ,
    X86_MN_PMOVSXWD,    
    X86_MN_PMOVSXWQ,
    X86_MN_PMOVZX,
    X86_MN_PMOVZXBD,
    X86_MN_PMOVZXBQ,    
    X86_MN_PMOVZXBW,    
    X86_MN_PMOVZXDQ,
    X86_MN_PMOVZXWD,    
    X86_MN_PMOVZXWQ,
    X86_MN_PMUL,
    X86_MN_PMULDQ,
    X86_MN_PMULHRSW,
    X86_MN_PMULHUW,
    X86_MN_PMULHW,
    X86_MN_PMULLD,
    X86_MN_PMULLW,
    X86_MN_PMULUDQ,
    X86_MN_POP,
    X86_MN_POPA,
    X86_MN_POPAD,
    X86_MN_POPCNT,
    X86_MN_POPF,
    X86_MN_POPFD,
    X86_MN_POPFQ,
    X86_MN_POR,
    X86_MN_PREFETCH,
    X86_MN_PREFETCHWT1,
    X86_MN_PREFETCHW,
    X86_MN_PREFETCHT0,
    X86_MN_PREFETCHT1,
    X86_MN_PREFETCHT2,
    X86_MN_PREFETCHNTA,
    X86_MN_PSADBW,
    X86_MN_PSHUFB,
    X86_MN_PSHUFD,
    X86_MN_PSHUFHW,
    X86_MN_PSHUFLW,
    X86_MN_PSHUFW,
    X86_MN_PSIGN,
    X86_MN_PSIGNB,
    X86_MN_PSIGND,
    X86_MN_PSIGNW,
    X86_MN_PSLLD,
    X86_MN_PSLLDQ,
    X86_MN_PSLLQ,
    X86_MN_PSLLVD,
    X86_MN_PSLLVQ,
    X86_MN_PSLLW,
    X86_MN_PSRAD,
    X86_MN_PSRAVD,
    X86_MN_PSRAW,
    X86_MN_PSRLD,
    X86_MN_PSRLDQ,
    X86_MN_PSRLQ,
    X86_MN_PSRLVD,
    X86_MN_PSRLVQ,
    X86_MN_PSRLW,
    X86_MN_PSUB,
    X86_MN_PSUBB,
    X86_MN_PSUBD,
    X86_MN_PSUBQ,
    X86_MN_PSUBSB,
    X86_MN_PSUBSW,
    X86_MN_PSUBUSB,
    X86_MN_PSUBUSW,
    X86_MN_PSUBW,
    X86_MN_PTEST,
    X86_MN_PTWRITE,
    X86_MN_PUNPCKHBW,
    X86_MN_PUNPCKHDQ,
    X86_MN_PUNPCKHQDQ,
    X86_MN_PUNPCKHWD,
    X86_MN_PUNPCKLBW,
    X86_MN_PUNPCKLDQ,
    X86_MN_PUNPCKLQDQ,
    X86_MN_PUNPCKLWD,
    X86_MN_PUSH,
    X86_MN_PUSHA,
    X86_MN_PUSHAD,
    X86_MN_PUSHF,
    X86_MN_PUSHFD,
    X86_MN_PUSHFQ,
    X86_MN_PXOR,
    X86_MN_RCL,
    X86_MN_RCPPS,
    X86_MN_RCPSS,
    X86_MN_RCR,
    X86_MN_RDFSBASE,
    X86_MN_RDGSBASE,
    X86_MN_RDMSR,
    X86_MN_RDPMC,
    X86_MN_RDRAND,
    X86_MN_RDSEED,
    X86_MN_RDSSPD,
    X86_MN_RDSSPQ,
    X86_MN_RDTSC,
    X86_MN_RDTSCP,
    X86_MN_RETF,
    X86_MN_RETN,
    X86_MN_ROL,
    X86_MN_ROR,
    X86_MN_RORX,
    X86_MN_ROUND,
    X86_MN_ROUNDPD,
    X86_MN_ROUNDPS,
    X86_MN_ROUNDSD,
    X86_MN_ROUNDSS,
    X86_MN_RSM,
    X86_MN_RSQRTPS,
    X86_MN_RSQRTSS,
    X86_MN_SAHF,
    X86_MN_SAL,
    X86_MN_SALC,
    X86_MN_SAR,
    X86_MN_SARX,
    X86_MN_SBB,
    X86_MN_SCAS,
    X86_MN_SCASB,
    X86_MN_SCASW,
    X86_MN_SCASD,
    X86_MN_SCASQ,
    X86_MN_SETA,
    X86_MN_SETC,
    X86_MN_SETNC,
    X86_MN_SETZ,
    X86_MN_SETG,
    X86_MN_SETNG,
    X86_MN_SETL,
    X86_MN_SETNL,
    X86_MN_SETNA,
    X86_MN_SETNZ,
    X86_MN_SETNO,
    X86_MN_SETNP,
    X86_MN_SETNS,
    X86_MN_SETO,
    X86_MN_SETP,
    X86_MN_SETPO,
    X86_MN_SETS,
    X86_MN_SFENCE,
    X86_MN_SGDT,
    X86_MN_SHL,
    X86_MN_SHLD,
    X86_MN_SHLX,
    X86_MN_SHR,
    X86_MN_SHRD,
    X86_MN_SHRX,
    X86_MN_SHUFPD,
    X86_MN_SHUFPS,
    X86_MN_SIDT,
    X86_MN_SKINIT,
    X86_MN_SLDT,
    X86_MN_SMSW,
    X86_MN_SQRT,
    X86_MN_SQRTPD,
    X86_MN_SQRTPS,
    X86_MN_SQRTSD,
    X86_MN_SQRTSS,
    X86_MN_STAC,
    X86_MN_STC,
    X86_MN_STD,
    X86_MN_STGI,
    X86_MN_STI,
    X86_MN_STMXCSR,
    X86_MN_STOS,
    X86_MN_STOSB,
    X86_MN_STOSW,
    X86_MN_STOSD,
    X86_MN_STOSQ,
    X86_MN_STR,
    X86_MN_SUB,
    X86_MN_SUBPD,
    X86_MN_SUBPS,
    X86_MN_SUBSD,
    X86_MN_SUBSS,
    X86_MN_SWAPGS,
    X86_MN_SYSCALL,
    X86_MN_SYSENTER,
    X86_MN_SYSEXIT,
    X86_MN_SYSRET,
    X86_MN_TEST,
    X86_MN_TESTPD,
    X86_MN_TESTPS,
    X86_MN_TZCNT,
    X86_MN_UCOMISD,
    X86_MN_UCOMISS,
    X86_MN_UD,
    X86_MN_UD1,
    X86_MN_UD2,
    X86_MN_UNPCKHPD,
    X86_MN_UNPCKHPS,
    X86_MN_UNPCKLPD,
    X86_MN_UNPCKLPS,
    X86_MN_VERR,
    X86_MN_VERW,
    X86_MN_VMCALL,
    X86_MN_VMCLEAR,
    X86_MN_VMFUNC,
    X86_MN_VMLAUNCH,
    X86_MN_VMLOAD,
    X86_MN_VMPTRLD,
    X86_MN_VMPTRST,
    X86_MN_VMREAD,
    X86_MN_VMRESUME,
    X86_MN_VMRUN,
    X86_MN_VMSAVE,
    X86_MN_VMWRITE,
    X86_MN_VMXOFF,
    X86_MN_VMXON,
    X86_MN_WAIT,
    X86_MN_WBINVD,
    X86_MN_WRFSBASE,
    X86_MN_WRGSBASE,
    X86_MN_WRMSR,
    X86_MN_XADD,
    X86_MN_XCHG,
    X86_MN_XEND,
    X86_MN_XGETBV,
    X86_MN_XLAT,
    X86_MN_XOR,
    X86_MN_XORPD,
    X86_MN_XORPS,
    X86_MN_XORSS,
    X86_MN_XRSTOR,
    X86_MN_XRSTOR64,
    X86_MN_XRSTORS,
    X86_MN_XRSTORS64,
    X86_MN_XSAVE,
    X86_MN_XSAVE64,
    X86_MN_XSAVEC,
    X86_MN_XSAVEC64,
    X86_MN_XSAVES,
    X86_MN_XSAVES64,
    X86_MN_XSAVEOPT,
    X86_MN_XSAVEOPT64,
    X86_MN_XSETBV,
    X86_MN_XTEST,
    /* XOP */
    X86_MN_VPMACSSWW,
    X86_MN_VPMACSSWD,
    X86_MN_VPMACSSDQL,
    X86_MN_VPMACSSDD,
    X86_MN_VPMACSSDQH,
    X86_MN_VPMACSWW,
    X86_MN_VPMACSWD,
    X86_MN_VPMACSDQL,
    X86_MN_VPMACSDD,
    X86_MN_VPMACSDQH,
    X86_MN_VPMADCSSWD,
    X86_MN_VPMADCSWD,
    X86_MN_VPROTB,
    X86_MN_VPROTW,
    X86_MN_VPROTD,
    X86_MN_VPROTQ,
    X86_MN_VPCOMB,
    X86_MN_VPCOMW,
    X86_MN_VPCOMD,
    X86_MN_VPCOMQ,
    X86_MN_VPCOMUB,
    X86_MN_VPCOMUW,
    X86_MN_VPCOMUD,
    X86_MN_VPCOMUQ,
    X86_MN_VPPERM,
    X86_MN_VPCMOV,
    X86_MN_VFRCZPS,
    X86_MN_VFRCZPD,
    X86_MN_VFRCZSS,
    X86_MN_VFRCZSD,
    X86_MN_VPHADDBW,
    X86_MN_VPHADDBD,
    X86_MN_VPHADDBQ,
    X86_MN_VPHADDWD,
    X86_MN_VPHADDWQ,
    X86_MN_VPHADDDQ,
    X86_MN_VPHADDUBWD,
    X86_MN_VPHADDUBD,
    X86_MN_VPHADDUBQ,
    X86_MN_VPHADDUWD,
    X86_MN_VPHADDUWQ,
    X86_MN_VPHADDUDQ,
    X86_MN_VPHSUBBW,
    X86_MN_VPHSUBWD,
    X86_MN_VPHSUBDQ,
    X86_MN_VZEROUPPER,
    X86_MN_VZEROALL,
    X86_MN_BLCFILL,
    X86_MN_BLSFILL,
    X86_MN_BLCS,
    X86_MN_TZMSK,
    X86_MN_BLCIC,
    X86_MN_BLCSIC,
    X86_MN_T1MSKC,
    X86_MN_BLCMSK,
    X86_MN_LLWPCB,
    X86_MN_SLWPCB,
    X86_MN_VPSHLB,
    X86_MN_VPSHLW,
    X86_MN_VPSHLD,
    X86_MN_VPSHLQ,
    X86_MN_VPSHAB,
    X86_MN_VPSHAW,
    X86_MN_VPSHAD,
    X86_MN_VPSHAQ,
    X86_MN_LWPINS,
    X86_MN_LWPVAL,
    /* 3DNow */
    X86_MN_PFCMPGE,
    X86_MN_PI2FW,
    X86_MN_PI2FD,
    X86_MN_PF2IW,
    X86_MN_PF2ID,
    X86_MN_PFNACC,
    X86_MN_PFPNACC,
    X86_MN_PFMIN,
    X86_MN_PFRCP,
    X86_MN_PFRSQRT,
    X86_MN_PFSUB,
    X86_MN_PFADD,
    X86_MN_PFCMPGT,
    X86_MN_PFMAX,
    X86_MN_PFRCPIT1,
    X86_MN_PFRSQIT1,
    X86_MN_PFSUBR,
    X86_MN_PFACC,
    X86_MN_PFCMPEQ,
    X86_MN_PFMUL,
    X86_MN_PMULHRW,
    X86_MN_PSWAPD,
    X86_MN_PAVGUSB,
    X86_MN_FFREEP,
    X86_MN_FXCH4,
    X86_MN_FSTDW,
    X86_MN_FSTSG,
    X86_MN_FXCH7,
    /* FMA4 */
    X86_MN_FMADDSUBPS,
    X86_MN_FMADDSUBPD,
    X86_MN_FMSUBADDPS,
    X86_MN_FMSUBADDPD,
    X86_MN_FMADDPS,
    X86_MN_FMADDSS,
    X86_MN_FMADDSD, 
    X86_MN_FMSUBPS,
    X86_MN_FMSUBPD,
    X86_MN_FMSUBSS,
    X86_MN_FMSUBSD,
    X86_MN_FNMADDPS,
    X86_MN_FNMADDPD,
    X86_MN_FNMADDSS,
    X86_MN_FNMADDSD,
    X86_MN_FNMSUBPS,
    X86_MN_FNMSUBPD,
    X86_MN_FNMSUBSS,
    X86_MN_FNMSUBSD,
    /* TSX */
    X86_MN_XBEGIN,
    X86_MN_XABORT,
    /* CET */
    X86_MN_WRUSSD,
    X86_MN_WRUSSQ,
    X86_MN_WRSSD,
    X86_MN_WRSSQ,
    X86_MN_INCSSPD,
    X86_MN_INCSSPQ,
    X86_MN_SAVEPREVSSP,
    X86_MN_RSTORSSP,
    X86_MN_CLRSSBSY,
    X86_MN_SETSSBSY,

    X86_MN_ANY
};

/***************************************************************************/

enum 
{
    X86_FLAG_NA = 0,    /* flag not affected */
    X86_FLAG_T,         /* flag tested */
    X86_FLAG_M,         /* flag modified */
    X86_FLAG_TM,        /* flag tested and modified */
    X86_FLAG_CLR,       /* flag cleared */
    X86_FLAG_SET        /* flag set */
};

enum
{
    /* dasm mode */
    X86_DMODE_16BIT = 0,    /* real mode / virtual 8086 mode (16-bit) */
    X86_DMODE_32BIT,        /* protected mode / long compatibility mode (32-bit) */
    X86_DMODE_64BIT,        /* long mode (64-bit) */

    /* operand size */
    X86_OSIZE_16BIT = 0,
    X86_OSIZE_32BIT,
    X86_OSIZE_64BIT,

    /* addressing size */
    X86_ASIZE_16BIT = 0,
    X86_ASIZE_32BIT,
    X86_ASIZE_64BIT,

    /* operand type */
    X86_OPTYPE_NONE = 0,    /* operand invalid */
    X86_OPTYPE_REG,         /* register */
    X86_OPTYPE_IMM,         /* immediate */
    X86_OPTYPE_MEXPR,       /* memory expression: base+index*scale+disp */
    X86_OPTYPE_MOFFS,       /* memory offset: A0-A3 */
    X86_OPTYPE_ABS,         /* absolute address: jmp far seg:offset */
    X86_OPTYPE_REL,         /* relative to rIP */

    /* operand size (effective) */
    X86_OPSIZE_NONE = 0,
    X86_OPSIZE_BYTE,        /* 1 bytes  (8 bit) */
    X86_OPSIZE_WORD,        /* 2 bytes  (16 bit) */
    X86_OPSIZE_DWORD,       /* 4 bytes  (32 bit) */
    X86_OPSIZE_FWORD,       /* 6 bytes  (48 bit) */
    X86_OPSIZE_QWORD,       /* 8 bytes  (64 bit) */
    X86_OPSIZE_TWORD,       /* 10 bytes (80 bit) */
    X86_OPSIZE_OWORD,       /* 16 bytes (128 bit) */
    X86_OPSIZE_YWORD,       /* 32 bytes (256 bit) */
    X86_OPSIZE_ZWORD,       /* 64 bytes (512 bit) */

    /* addressing size */
    X86_ASIZE_NONE = 0,
    X86_ASIZE_WORD,         /* 2 bytes  (16 bit) */
    X86_ASIZE_DWORD,        /* 4 bytes  (32 bit) */
    X86_ASIZE_QWORD,        /* 8 bytes  (64 bit) */

    /* displacement size */
    X86_DISP_NONE = 0,      /* no displacement */
    X86_DISP_8,             /* byte displacement */
    X86_DISP_16,            /* word displacement */
    X86_DISP_32             /* dword displacement */
};

typedef union 
{ 
    uint8_t     u8;
    uint16_t    u16;
    uint32_t    u32;
    uint64_t    u64;
    int8_t      s8;
    int16_t     s16; 
    int32_t     s32;
    int64_t     s64;

} data_type;

typedef data_type x86_reg;

typedef struct
{

#if !defined(X86_DASM_NOFMT) || defined(X86_DASM_DEBUG)

    char str[64];

#endif

    uint8_t type;       /* X86_OPTYPE_ */
    uint8_t size;       /* X86_OPSIZE_ */
    uint8_t size_orig;  /* X86_OPSIZE_, orig. before fixes/sign-extension */
    uint8_t asize;      /* X86_ASIZE_ */
    uint8_t seg;        /* X86_REG_ES/DS/SS/FS/GS/CS */
    uint8_t pos;        /* position of IMM/REL/MOFFS/DISP in buffer */

    union 
    {
        /* X86_OPTYPE_ABS */
        struct
        {
            uint16_t seg;
            data_type offs;

        } abs;

        /* X86_OPTYPE_REG */
        uint8_t reg;            /* X86_REG_ */

        /* X86_OPTYPE_MEXPR */
        struct
        {
            uint8_t scale;      /* 1, 2, 4, 8 */
            uint8_t index;      /* X86_REG_ */
            uint8_t base;       /* X86_REG_ */
            uint8_t disp_size;  /* X86_DISP_ */
            data_type disp;

        } mexpr;

        /* X86_OPTYPE_IMM */
        /* X86_OPTYPE_MOFFS */
        /* X86_OPTYPE_REL */
        data_type data;
    };

} x86_operand_t;

typedef struct
{
    uint8_t buffer[32]; /* buffer to disasm */

#if !defined(X86_DASM_NOFMT) || defined(X86_DASM_DEBUG)

    char inst_str[128]; /* full formatted instruction */
    char mnem_str[32];  /* mnemonic string */

#endif

    union 
    {
        uint8_t pos;    /* position in buffer (cursor) */
        uint8_t len;    /* instruction lenght */
    };
    
    uint8_t dmode;      /* X86_DMODE_ */
    uint8_t osize;      /* X86_OSIZE_ */
    uint8_t asize;      /* X86_ASIZE_ */
    uint8_t iset;       /* X86_ISET_ */
    uint16_t mnem;      /* X86_MN_ */

    uint8_t modrm;
    uint8_t sib;
    uint8_t vsib_base;

    /* prefixes */
    uint8_t pfx_rex;
    uint8_t pfx_last;
    uint8_t pfx_seg;
    uint8_t pfx_mandatory;

    union 
    {
        uint8_t pfx_vex[3];
        uint8_t pfx_xop[3];
    };

    /* positions in buffer */
    uint8_t pos_modrm : 4;
    uint8_t pos_sib : 4;
    uint8_t pos_rex : 4;
    uint8_t pos_opcode : 4;

    /* count of operand-size ovverride prefixes */
    uint8_t pfx_c_osize : 4;
    
    /* prefixes present */
    uint8_t pfx_p_rex : 1;
    uint8_t pfx_p_seg : 1;
    uint8_t pfx_p_osize : 1;
    uint8_t pfx_p_asize : 1;
    uint8_t pfx_p_lock : 1;
    uint8_t pfx_p_rep : 1;
    uint8_t pfx_p_repne : 1;
    uint8_t pfx_p_vex2b : 1;
    uint8_t pfx_p_vex3b : 1;
    uint8_t pfx_p_evex : 1;
    uint8_t pfx_p_xop : 1;

    /* bytes present */
    uint8_t p_modrm : 1;
    uint8_t p_sib : 1;
    uint8_t p_vsib : 1;

    /* current table link */
    const x86_table_link_t* table_link;
    
    /* operands */
    x86_operand_t operands[4];

    /* EFLAGS */
    x86_eflags_t eflags;

    /* current instruction pointer */
    x86_reg rip;

    /* destination instruction pointer (for relative inst.) */
    x86_reg dest_rip;

} x86_dasm_context_t;

typedef struct
{
    x86_reg rax;
    x86_reg rcx;
    x86_reg rdx;
    x86_reg rbx;
    x86_reg rsp;
    x86_reg rbp;
    x86_reg rsi;
    x86_reg rdi;

    /* 64-bit mode only */
    x86_reg r8;
    x86_reg r9;
    x86_reg r10;
    x86_reg r11;
    x86_reg r12;
    x86_reg r13;
    x86_reg r14;
    x86_reg r15;

    x86_reg rip;

    /* base address of the segments, -- not the segment selector! -- */
    x86_reg es;
    x86_reg cs;
    x86_reg ss;
    x86_reg ds;
    x86_reg fs;
    x86_reg gs;

} x86_regs_t;

/* utility functions */
void x86_set_buffer(x86_dasm_context_t* x86_dctx, void* code);

void x86_set_ip(x86_dasm_context_t* x86_dctx, uint64_t ip);

void x86_set_dmode(x86_dasm_context_t* x86_dctx, int dmode);

bool_t x86_is_jmp(x86_dasm_context_t* x86_dctx);

bool_t x86_is_jmpcc(x86_dasm_context_t* x86_dctx);

bool_t x86_is_ret(x86_dasm_context_t* x86_dctx);

bool_t x86_resolve_rip(x86_dasm_context_t* x86_dctx, ulong_t i, uint64_t* addr);

uint64_t x86_resolve_op(x86_dasm_context_t* x86_dctx, x86_regs_t* regs, ulong_t i);

/* disassembling function */
int x86_dasm(x86_dasm_context_t* x86_dctx);

#ifdef __cplusplus
    }
#endif

#endif //_DASM_H_

/*

    This file is part of x86_dasm.

    x86_dasm is a compact x86-64 disassembling library
    
    Copyright 2021 / the`janitor / < email: base64dec(dGhlLmphbml0b3JAcHJvdG9ubWFpbC5jb20=) >

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

        http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.

*/
#ifndef _DASM_MACROS_H_
#define _DASM_MACROS_H_

/*
    This file provides some helper macros in an attempt to simplify conditional expressions.

    The core macros are part of the P99 macro suite for C99 by Jens Gustedt, with some slight modifications.
        - https://gitlab.inria.fr/gustedt/p99
    
*/

/***********************************************************************************************************************
    
    Core

***********************************************************************************************************************/

#define EXPAND(x) x

#define PP_CONCAT_(v1, v2) v1 ## v2
#define PP_CONCAT(v1, v2) PP_CONCAT_(v1, v2)

#define PP_CONCAT5_(_0, _1, _2, _3, _4) _0 ## _1 ## _2 ## _3 ## _4

#define PP_IDENTITY_(x) x
#define PP_IDENTITY(x) PP_IDENTITY_(x)

#define PP_VA_ARGS_(...) __VA_ARGS__
#define PP_VA_ARGS(...) PP_VA_ARGS_(__VA_ARGS__)

#define PP_IDENTITY_VA_ARGS_(x, ...) x, __VA_ARGS__
#define PP_IDENTITY_VA_ARGS(x, ...) PP_IDENTITY_VA_ARGS_(x, __VA_ARGS__)

#define PP_IIF_0(x, ...) __VA_ARGS__
#define PP_IIF_1(x, ...) x
#define PP_IIF(c) PP_CONCAT_(PP_IIF_, c)

#define PP_HAS_COMMA(...) \
    PP_IDENTITY(PP_VA_ARGS_TAIL(__VA_ARGS__, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0))

#define PP_IS_EMPTY_TRIGGER_PARENTHESIS_(...) ,

#define PP_IS_EMPTY(...) PP_IS_EMPTY_( \
    /* test if there is just one argument, eventually an empty one */ \
    PP_HAS_COMMA(__VA_ARGS__),                                \
    /* test if _TRIGGER_PARENTHESIS_ together with the argument adds a comma */ \
    PP_HAS_COMMA(PP_IS_EMPTY_TRIGGER_PARENTHESIS_ __VA_ARGS__), \
    /* test if the argument together with a parenthesis adds a comma */ \
    PP_HAS_COMMA(__VA_ARGS__ ()),                             \
    /* test if placing it between _TRIGGER_PARENTHESIS_ and the parenthesis adds a comma */ \
    PP_HAS_COMMA(PP_IS_EMPTY_TRIGGER_PARENTHESIS_ __VA_ARGS__ ()))

#define PP_IS_EMPTY_(_0, _1, _2, _3) PP_HAS_COMMA(PP_CONCAT5_(PP_IS_EMPTY_IS_EMPTY_CASE_, _0, _1, _2, _3))
#define PP_IS_EMPTY_IS_EMPTY_CASE_0001 ,

#define PP_VA_ARGS_SIZE(...) PP_IIF(PP_IS_EMPTY(__VA_ARGS__))(0, PP_VA_ARGS_SIZE_(__VA_ARGS__, PP_VA_ARGS_SEQ64()))
#define PP_VA_ARGS_SIZE_(...) PP_IDENTITY(PP_VA_ARGS_TAIL(__VA_ARGS__))

#define PP_VA_ARGS_TAIL(_0,_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15,_16,_17,_18,_19,_20,_21,_22, x, ...) x
#define PP_VA_ARGS_SEQ64() 23,22,21,20,19,18,17,16,15,14,13,12,11,10,9,8,7,6,5,4,3,2,1,0

/***********************************************************************************************************************
    
    Register Operand

    Examples: REG(), REG32(), REG(RAX), REG(R8L)

***********************************************************************************************************************/

#define REG_CHECK_TYPE() type == X86_OPTYPE_REG
#define REG_CHECK_SIZE(_1) size == X86_OPSIZE_##_1
#define REG_0() REG_CHECK_TYPE(), REG_CHECK_TYPE(), REG_CHECK_TYPE()
#define REG_1(_1) reg == REG_##_1, REG_CHECK_TYPE(), REG_CHECK_TYPE()
#define REG_(N) REG_##N
#define REG_EVAL(N) REG_(N)
#define REG_INTERNAL(...) EXPAND(REG_EVAL(EXPAND(PP_VA_ARGS_SIZE(__VA_ARGS__)))(__VA_ARGS__))

#define REG(...) REG_INTERNAL(__VA_ARGS__) , REG_CHECK_TYPE() , REG_CHECK_TYPE()
#define REG8(...) REG_INTERNAL(__VA_ARGS__) , REG_CHECK_SIZE(BYTE) , REG_CHECK_TYPE()
#define REG16(...) REG_INTERNAL(__VA_ARGS__) , REG_CHECK_SIZE(WORD) , REG_CHECK_TYPE()
#define REG32(...) REG_INTERNAL(__VA_ARGS__) , REG_CHECK_SIZE(DWORD) , REG_CHECK_TYPE()
#define REG64(...) REG_INTERNAL(__VA_ARGS__) , REG_CHECK_SIZE(QWORD) , REG_CHECK_TYPE()

/***********************************************************************************************************************
    
    Immediate Operand

    Examples: IMM(), IMM8(), IMMS16(-2), IMM32(0xFFFFFFF8)

***********************************************************************************************************************/

#define IMM_CHECK_TYPE() type == X86_OPTYPE_IMM
#define IMM_CHECK_SIZE(_1) size == X86_OPSIZE_##_1
#define IMM_0() IMM_CHECK_TYPE()
#define IMM_1(_1) data.u64 == (uint64_t)_1
#define IMM_2(_1, _2) data.##_1 == _2
#define IMM_(N) IMM_##N
#define IMM_EVAL(N) IMM_(N)
#define IMM_INTERNAL(...) EXPAND(IMM_EVAL(EXPAND(PP_VA_ARGS_SIZE(__VA_ARGS__)))(__VA_ARGS__)) , \
    IMM_CHECK_TYPE() , IMM_CHECK_TYPE() , IMM_CHECK_TYPE()

#define IMM(...) IMM_INTERNAL(__VA_ARGS__) , IMM_CHECK_TYPE()
    
#define IMMU8(...) PP_IIF(PP_IS_EMPTY(__VA_ARGS__))(IMM_INTERNAL(__VA_ARGS__), IMM_INTERNAL(u8, __VA_ARGS__)) , \
    IMM_CHECK_SIZE(BYTE)

#define IMMU16(...) PP_IIF(PP_IS_EMPTY(__VA_ARGS__))(IMM_INTERNAL(__VA_ARGS__), IMM_INTERNAL(u16, __VA_ARGS__)) , \
    IMM_CHECK_SIZE(WORD)

#define IMMU32(...) PP_IIF(PP_IS_EMPTY(__VA_ARGS__))(IMM_INTERNAL(__VA_ARGS__), IMM_INTERNAL(u32, __VA_ARGS__)) , \
    IMM_CHECK_SIZE(DWORD)

#define IMMU64(...) PP_IIF(PP_IS_EMPTY(__VA_ARGS__))(IMM_INTERNAL(__VA_ARGS__), IMM_INTERNAL(u64, __VA_ARGS__)) , \
    IMM_CHECK_SIZE(QWORD)

#define IMMS8(...) PP_IIF(PP_IS_EMPTY(__VA_ARGS__))(IMM_INTERNAL(__VA_ARGS__), IMM_INTERNAL(s8, __VA_ARGS__)) , \
    IMM_CHECK_SIZE(BYTE)

#define IMMS16(...) PP_IIF(PP_IS_EMPTY(__VA_ARGS__))(IMM_INTERNAL(__VA_ARGS__), IMM_INTERNAL(s16, __VA_ARGS__)) , \
    IMM_CHECK_SIZE(WORD)

#define IMMS32(...) PP_IIF(PP_IS_EMPTY(__VA_ARGS__))(IMM_INTERNAL(__VA_ARGS__), IMM_INTERNAL(s32, __VA_ARGS__)) , \
    IMM_CHECK_SIZE(DWORD)

#define IMMS64(...) PP_IIF(PP_IS_EMPTY(__VA_ARGS__))(IMM_INTERNAL(__VA_ARGS__), IMM_INTERNAL(s64, __VA_ARGS__)) , \
    IMM_CHECK_SIZE(QWORD)

#define IMM8(...) IMMU8(__VA_ARGS__)
#define IMM16(...) IMMU16(__VA_ARGS__)
#define IMM32(...) IMMU32(__VA_ARGS__)
#define IMM64(...) IMMU64(__VA_ARGS__)

/***********************************************************************************************************************
    
    Memory Offset Operand

    Examples: MOFFS32(), MOFFS(0xFF800000)

***********************************************************************************************************************/

#define MOFFS_CHECK_TYPE() type == X86_OPTYPE_MOFFS
#define MOFFS_CHECK_SIZE(_1) size == X86_OPSIZE_##_1
#define MOFFS_0() MOFFS_CHECK_TYPE()
#define MOFFS_1(_1) data.u64 == (uint64_t)_1
#define MOFFS_2(_1, _2) data.##_1 == _2
#define MOFFS_(N) MOFFS_##N
#define MOFFS_EVAL(N) MOFFS_(N)
#define MOFFS_INTERNAL(...) EXPAND(MOFFS_EVAL(EXPAND(PP_VA_ARGS_SIZE(__VA_ARGS__)))(__VA_ARGS__)) , \
    MOFFS_CHECK_TYPE() , MOFFS_CHECK_TYPE() , MOFFS_CHECK_TYPE()

#define MOFFS(...) MOFFS_INTERNAL(__VA_ARGS__) , MOFFS_CHECK_TYPE()
    
#define MOFFS16(...) PP_IIF(PP_IS_EMPTY(__VA_ARGS__))(MOFFS_INTERNAL(__VA_ARGS__), MOFFS_INTERNAL(u16, __VA_ARGS__)) , \
    MOFFS_CHECK_SIZE(WORD)
    
#define MOFFS32(...) PP_IIF(PP_IS_EMPTY(__VA_ARGS__))(MOFFS_INTERNAL(__VA_ARGS__), MOFFS_INTERNAL(u32, __VA_ARGS__)) , \
    MOFFS_CHECK_SIZE(DWORD)
    
#define MOFFS64(...) PP_IIF(PP_IS_EMPTY(__VA_ARGS__))(MOFFS_INTERNAL(__VA_ARGS__), MOFFS_INTERNAL(u64, __VA_ARGS__)) , \
    MOFFS_CHECK_SIZE(QWORD)

/***********************************************************************************************************************
 
    Absolute Address Operand

    Examples: ABS32(0x33, 0x77546009), ABS64()

***********************************************************************************************************************/

#define ABS_CHECK_TYPE() type == X86_OPTYPE_ABS
#define ABS_CHECK_SIZE(_1) size == X86_OPSIZE_##_1
#define ABS_0() ABS_CHECK_TYPE(), ABS_CHECK_TYPE()
#define ABS_1(_1) abs.seg == _1, ABS_CHECK_TYPE()
#define ABS_3(_1, _2, _3) abs.seg == _2, abs.offs.##_1 == _3
#define ABS_(N) ABS_##N
#define ABS_EVAL(N) ABS_(N)
#define ABS_INTERNAL(...) EXPAND(ABS_EVAL(EXPAND(PP_VA_ARGS_SIZE(__VA_ARGS__)))(__VA_ARGS__)) , \
    ABS_CHECK_TYPE() , ABS_CHECK_TYPE()

#define ABS(...) ABS_INTERNAL(__VA_ARGS__) , ABS_CHECK_TYPE()
    
#define ABS16(...) PP_IIF(PP_IS_EMPTY(__VA_ARGS__))(ABS_INTERNAL(__VA_ARGS__), ABS_INTERNAL(u16, __VA_ARGS__)) , \
    ABS_CHECK_SIZE(DWORD)
    
#define ABS32(...) PP_IIF(PP_IS_EMPTY(__VA_ARGS__))(ABS_INTERNAL(__VA_ARGS__), ABS_INTERNAL(u32, __VA_ARGS__)) , \
    ABS_CHECK_SIZE(FWORD)
    
#define ABS64(...) PP_IIF(PP_IS_EMPTY(__VA_ARGS__))(ABS_INTERNAL(__VA_ARGS__), ABS_INTERNAL(u64, __VA_ARGS__)) , \
    ABS_CHECK_SIZE(TWORD)

/***********************************************************************************************************************
    
    Relative Operand

    Examples: REL8(), REL(-2), RELDST(0xBADF00D)

***********************************************************************************************************************/

#define REL_CHECK_TYPE() type == X86_OPTYPE_REL
#define REL_CHECK_SIZE(_1) size == X86_OPSIZE_##_1
#define REL_0() REL_CHECK_TYPE()
#define REL_1(_1) data.u64 == (uint64_t)_1
#define REL_2(_1, _2) data.##_1 == _2
#define REL_(N) REL_##N
#define REL_EVAL(N) REL_(N)
#define REL_INTERNAL(...) EXPAND(REL_EVAL(EXPAND(PP_VA_ARGS_SIZE(__VA_ARGS__)))(__VA_ARGS__)) , \
    REL_CHECK_TYPE() , REL_CHECK_TYPE() , REL_CHECK_TYPE()

#define REL(...) REL_INTERNAL(__VA_ARGS__) , REL_CHECK_TYPE()
    
#define REL8(...) PP_IIF(PP_IS_EMPTY(__VA_ARGS__))(REL_INTERNAL(__VA_ARGS__), REL_INTERNAL(s8, __VA_ARGS__)) , \
    REL_CHECK_SIZE(BYTE)
    
#define REL16(...) PP_IIF(PP_IS_EMPTY(__VA_ARGS__))(REL_INTERNAL(__VA_ARGS__), REL_INTERNAL(s16, __VA_ARGS__)) , \
    REL_CHECK_SIZE(WORD)
    
#define REL32(...) PP_IIF(PP_IS_EMPTY(__VA_ARGS__))(REL_INTERNAL(__VA_ARGS__), REL_INTERNAL(s32, __VA_ARGS__)) , \
    REL_CHECK_SIZE(DWORD)

#define RELDST(_1) dest_rip.u64 == (uint64_t)_1

/***********************************************************************************************************************
    
    Memory Expression Operand

    Examples: MEXPR(EAX), MEXPR(RAX, RCX, 2, 0x1BADB002)

***********************************************************************************************************************/

#define MEXPR_CHECK_TYPE() type == X86_OPTYPE_MEXPR

#define MEXPR_0() \
    MEXPR_CHECK_TYPE(), \
    MEXPR_CHECK_TYPE(), \
    MEXPR_CHECK_TYPE(), \
    MEXPR_CHECK_TYPE(), \
    MEXPR_CHECK_TYPE()

#define MEXPR_1(_1) \
    mexpr.base == X86_REG_##_1, \
    MEXPR_CHECK_TYPE(), \
    MEXPR_CHECK_TYPE(), \
    MEXPR_CHECK_TYPE(), \
    MEXPR_CHECK_TYPE()

#define MEXPR_2(_1, _2) \
    mexpr.base == X86_REG_##_1, \
    mexpr.index == X86_REG_##_2, \
    MEXPR_CHECK_TYPE(), \
    MEXPR_CHECK_TYPE(), \
    MEXPR_CHECK_TYPE()

#define MEXPR_3(_1, _2, _3) \
    mexpr.base == X86_REG_##_1, \
    mexpr.index == X86_REG_##_2, \
    mexpr.scale == _3, \
    MEXPR_CHECK_TYPE(), \
    MEXPR_CHECK_TYPE()

#define MEXPR_4(_1, _2, _3, _4) \
    mexpr.base == X86_REG_##_1, \
    mexpr.index == X86_REG_##_2, \
    mexpr.scale == _3, \
    mexpr.disp.u64 == (uint64_t)_4, \
    MEXPR_CHECK_TYPE()

#define MEXPR_(N) MEXPR_##N
#define MEXPR_EVAL(N) MEXPR_(N)
#define MEXPR(...) EXPAND(MEXPR_EVAL(EXPAND(PP_VA_ARGS_SIZE(__VA_ARGS__)))(__VA_ARGS__))

/***********************************************************************************************************************
    
    Matching Functions

    Examples: 
        
        X86M(&ctx, ANY, REG(), REG())
        X86M(&ctx, MOV, REG8(), IMM8())
        X86M(&ctx, CALL, RELDST(0xD00D2BAD))
        X86M(&ctx, JMP, REL(-2))

***********************************************************************************************************************/

#define OP_MATCH(_1, _2, _3, _4, _5, _6, _7) \
    ((_1)->operands[_2]._3 && (_1)->operands[_2]._4 && \
    (_1)->operands[_2]._5 && (_1)->operands[_2]._6 && \
    (_1)->operands[_2]._7)

#define INST_MATCH_0() ,
#define INST_MATCH_1(_1) ,
#define INST_MATCH_2(_1, _2) ((_1)->mnem == X86_MN_##_2 || X86_MN_##_2 == X86_MN_ANY)
#define INST_MATCH_3(_1, _2, _3) (IMATCH_2(_1, _2) && ((_1)->_3))
    
#define INST_MATCH_7(_1, _2, _3, _4, _5, _6, _7) \
    (INST_MATCH_2(_1, _2) && \
    OP_MATCH(_1, 0, _3, _4, _5, _6, _7))
    
#define INST_MATCH_12(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12) \
    (INST_MATCH_7(_1, _2, _3, _4, _5, _6, _7) && \
    OP_MATCH(_1, 1, _8, _9, _10, _11, _12))
    
#define INST_MATCH_17(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17) \
    (INST_MATCH_12(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12) && \
    OP_MATCH(_1, 2, _13, _14, _15, _16, _17))
    
#define INST_MATCH_22(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, _22) \
    (INST_MATCH_17(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17) && \
    OP_MATCH(_1, 3, _18, _19, _20, _21, _22))
    
#define INST_MATCH_(N) INST_MATCH_##N
#define INST_MATCH_EVAL(N) INST_MATCH_(N)

#define X86M(...) EXPAND(INST_MATCH_EVAL(EXPAND(PP_VA_ARGS_SIZE(__VA_ARGS__)))(__VA_ARGS__))

#endif //_DASM_MACROS_H_

#ifdef DASM_IMPLEMENTATION

/*

    This file is part of x86_dasm.

    x86_dasm is a compact x86-64 disassembling library
    
    Copyright 2014 / the`janitor / < email: base64dec(dGhlLmphbml0b3JAcHJvdG9ubWFpbC5jb20=) >

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

        http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.

*/
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <inttypes.h>

#define dbg_print(msg, ...)             \
    do {                                \
        printf(msg, ## __VA_ARGS__);    \
    } while(false)

#if defined(X86_DASM_DEBUG)

    #define dbg_msg(msg, ...) \
        dbg_print("[*] %s: " msg "\n", __FUNCTION__, ##__VA_ARGS__) 

    #define dbg_err(msg, ...) \
        dbg_print("[!] %s: " msg "\n", __FUNCTION__, ##__VA_ARGS__) 

#else

    #define dbg_msg(msg, ...)
    #define dbg_err(msg, ...)

#endif 

#if 0
    #define strcpy      <define function>
    #define strcat      <define function>
    #define strstr      <define function>
    #define strncat     <define function>
    #define strchr      <define function>
    #define sprintf     <define function>
    #define memset      <define function>
    #define memcpy      <define function>
#endif

/****************************************************************************

    shortcuts

****************************************************************************/

#define REX_B               _REX_B(x86_dctx->pfx_rex)
#define REX_X               _REX_X(x86_dctx->pfx_rex)
#define REX_R               _REX_R(x86_dctx->pfx_rex)
#define REX_W               _REX_W(x86_dctx->pfx_rex)

#define VEX_2B_PP           _VEX_2B_PP(x86_dctx->pfx_vex[0])
#define VEX_2B_L            _VEX_2B_L(x86_dctx->pfx_vex[0])
#define VEX_2B_VVVV         _VEX_2B_VVVV(x86_dctx->pfx_vex[0])
#define VEX_2B_R            _VEX_2B_R(x86_dctx->pfx_vex[0])

#define VEX_3B_MMMMM        _VEX_3B_MMMMM(x86_dctx->pfx_vex[0])
#define VEX_3B_B            _VEX_3B_B(x86_dctx->pfx_vex[0])
#define VEX_3B_X            _VEX_3B_X(x86_dctx->pfx_vex[0])
#define VEX_3B_R            _VEX_3B_R(x86_dctx->pfx_vex[0])
#define VEX_3B_PP           _VEX_3B_PP(x86_dctx->pfx_vex[1])
#define VEX_3B_L            _VEX_3B_L(x86_dctx->pfx_vex[1])
#define VEX_3B_VVVV         _VEX_3B_VVVV(x86_dctx->pfx_vex[1])
#define VEX_3B_W            _VEX_3B_W(x86_dctx->pfx_vex[1])

#define XOP_MMMMM           _XOP_MMMMM(x86_dctx->pfx_xop[0])
#define XOP_B               _XOP_B(x86_dctx->pfx_xop[0])
#define XOP_X               _XOP_X(x86_dctx->pfx_xop[0])
#define XOP_R               _XOP_R(x86_dctx->pfx_xop[0])
#define XOP_PP              _XOP_PP(x86_dctx->pfx_xop[1])
#define XOP_L               _XOP_L(x86_dctx->pfx_xop[1])
#define XOP_VVVV            _XOP_VVVV(x86_dctx->pfx_xop[1])
#define XOP_W               _XOP_W(x86_dctx->pfx_xop[1])

#define EVEX_B              _EVEX_B(x86_dctx->pfx_vex[0])
#define EVEX_X              _EVEX_X(x86_dctx->pfx_vex[0])
#define EVEX_R              _EVEX_R(x86_dctx->pfx_vex[0])
#define EVEX_RR             _EVEX_RR(x86_dctx->pfx_vex[0])
#define EVEX_MM             _EVEX_MM(x86_dctx->pfx_vex[0])
#define EVEX_PP             _EVEX_PP(x86_dctx->pfx_vex[1])
#define EVEX_W              _EVEX_W(x86_dctx->pfx_vex[1])
#define EVEX_VVVV           _EVEX_VVVV(x86_dctx->pfx_vex[1])
#define EVEX_Z              _EVEX_Z(x86_dctx->pfx_vex[2])
#define EVEX_LL             _EVEX_LL(x86_dctx->pfx_vex[2])
#define EVEX_L              _EVEX_L(x86_dctx->pfx_vex[2])
#define EVEX_BB             _EVEX_BB(x86_dctx->pfx_vex[2])
#define EVEX_VV             _EVEX_VV(x86_dctx->pfx_vex[2])
#define EVEX_AAA            _EVEX_AAA(x86_dctx->pfx_vex[2])

/****************************************************************************

    tables and vars

****************************************************************************/

static const x86_table_link_t* const vex_links[4][4] = 
{
    {
    /* pp: 00    */ &x86_table_links[TABLE_ID(0F)],
                    &x86_table_links[TABLE_ID(0F)], 
                    &x86_table_links[TABLE_ID(0F_38)], 
                    &x86_table_links[TABLE_ID(0F_3A)]
    },
    {
    /* pp: 01 66 */ &x86_table_links[TABLE_ID(66_0F)],
                    &x86_table_links[TABLE_ID(66_0F)], 
                    &x86_table_links[TABLE_ID(66_0F_38)], 
                    &x86_table_links[TABLE_ID(66_0F_3A)]
    },
    {
    /* pp: 10 F3 */ &x86_table_links[TABLE_ID(F3_0F)],
                    &x86_table_links[TABLE_ID(F3_0F)], 
                    &x86_table_links[TABLE_ID(F3_0F_38)], 
                    &x86_table_links[TABLE_ID(F3_0F_3A)]
    },
    {
    /* pp: 11 F2 */ &x86_table_links[TABLE_ID(F2_0F)],
                    &x86_table_links[TABLE_ID(F2_0F)], 
                    &x86_table_links[TABLE_ID(F2_0F_38)], 
                    &x86_table_links[TABLE_ID(F2_0F_3A)]
    }
};

static const x86_table_link_t* const xop_links[4][4] = 
{
    {
    /* pp: 00    */ &x86_table_links[TABLE_ID(XOP_8)],
                    &x86_table_links[TABLE_ID(XOP_9)], 
                    &x86_table_links[TABLE_ID(XOP_A)],
                    NULL
    },
    {
    /* pp: 01 66 */ &x86_table_links[TABLE_ID(XOP_8)],
                    &x86_table_links[TABLE_ID(XOP_9)], 
                    &x86_table_links[TABLE_ID(XOP_A)],
                    NULL
    },
    {
    /* pp: 10 F3 */ &x86_table_links[TABLE_ID(XOP_8)],
                    &x86_table_links[TABLE_ID(XOP_9)], 
                    &x86_table_links[TABLE_ID(XOP_A)],
                    NULL
    },
    {
    /* pp: 11 F2 */ &x86_table_links[TABLE_ID(XOP_8)],
                    &x86_table_links[TABLE_ID(XOP_9)], 
                    &x86_table_links[TABLE_ID(XOP_A)],
                    NULL
    }
};

/* effective address, addressing size 16-bit */
static const uint8_t ea_asize16[8][2] =
{
    /* 0 */ {X86_REG_BX, X86_REG_SI},
    /* 1 */ {X86_REG_BX, X86_REG_DI},
    /* 2 */ {X86_REG_BP, X86_REG_SI},
    /* 3 */ {X86_REG_BP, X86_REG_DI},
    /* 4 */ {X86_REG_SI, X86_REG_NONE},
    /* 5 */ {X86_REG_DI, X86_REG_NONE},
    /* 6 */ {X86_REG_BP, X86_REG_NONE},
    /* 7 */ {X86_REG_BX, X86_REG_NONE}
};

#if !defined(X86_DASM_NOFMT) || defined(X86_DASM_DEBUG)

/* registers strings, same order as defined in the enum */
static const char* const regs_str[] =
{
    "rax", "rcx", "rdx", "rbx", "rsp", "rbp", "rsi", "rdi", "r8", 
    "r9", "r10", "r11", "r12", "r13", "r14", "r15", "rip",

    "eax", "ecx", "edx", "ebx","esp", "ebp", "esi", "edi",
    "r8d", "r9d", "r10d", "r11d", "r12d", "r13d", "r14d", "r15d", "eip",

    "ax", "cx", "dx", "bx", "sp", "bp", "si", "di",
    "r8w", "r9w", "r10w", "r11w", "r12w", "r13w", "r14w", "r15w",

    "al", "cl", "dl", "bl", "ah", "ch", "dh", "bh",
    "spl", "bpl", "sil", "dil", "r8l", "r9l", "r10l", "r11l", "r12l",   
    "r13l", "r14l", "r15l",

    "es", "cs", "ss", "ds", "fs", "gs",

    "st0", "st1", "st2", "st3", "st4", "st5", "st6", "st7", 

    "mm0", "mm1", "mm2", "mm3", "mm4", "mm5", "mm6", "mm7",

    "xmm0", "xmm1", "xmm2", "xmm3", "xmm4", "xmm5", "xmm6", "xmm7",
    "xmm8", "xmm9", "xmm10", "xmm11", "xmm12", "xmm13", "xmm14", "xmm15",
    "xmm16", "xmm17", "xmm18", "xmm19", "xmm20", "xmm21", "xmm22", "xmm23",
    "xmm24", "xmm25", "xmm26", "xmm27", "xmm28", "xmm29", "xmm30", "xmm31",

    "ymm0", "ymm1", "ymm2", "ymm3", "ymm4", "ymm5", "ymm6", "ymm7",
    "ymm8", "ymm9", "ymm10", "ymm11", "ymm12", "ymm13", "ymm14", "ymm15",
    "ymm16", "ymm17", "ymm18", "ymm19", "ymm20", "ymm21", "ymm22", "ymm23",
    "ymm24", "ymm25", "ymm26", "ymm27", "ymm28", "ymm29", "ymm30", "ymm31",

    "zmm0", "zmm1", "zmm2", "zmm3", "zmm4", "zmm5", "zmm6", "zmm7",
    "zmm8", "zmm9", "zmm10", "zmm11", "zmm12", "zmm13", "zmm14", "zmm15",
    "zmm16", "zmm17", "zmm18", "zmm19", "zmm20", "zmm21", "zmm22", "zmm23",
    "zmm24", "zmm25", "zmm26", "zmm27", "zmm28", "zmm29", "zmm30", "zmm31",

    "dr0", "dr1", "dr2", "dr3", "dr4", "dr5", "dr6", "dr7",
    "dr8", "dr9", "dr10", "dr11", "dr12", "dr13", "dr14", "dr15",

    "cr0", "cr1", "cr2", "cr3", "cr4", "cr5", "cr6", "cr7",
    "cr8", "cr9", "cr10", "cr11", "cr12", "cr13", "cr14", "cr15"
};

/* operand size strings, used for memory pointers */
static const char* const opsize_str[] =
{
    "", "byte", "word", "dword", "fword", "qword", "tword",
    "oword", "yword", "zword"
};

static
const char* const
get_opsize_str(uint8_t a)
{
    return opsize_str[a];
}

#endif

/****************************************************************************

    decoding functions

****************************************************************************/

static 
uint8_t
get_uint8(x86_dasm_context_t* x86_dctx, int i)
{
    return (i == -1 ? x86_dctx->buffer[x86_dctx->pos++] : x86_dctx->buffer[i]);
}

static 
uint16_t
get_uint16(x86_dasm_context_t* x86_dctx, int i)
{
    uint16_t r;

    uint8_t* p = (i == -1 ? 
        &x86_dctx->buffer[x86_dctx->pos] : 
        &x86_dctx->buffer[i]);

#if defined(X86_DASM_NLE)

    r = 0;
    r |= ((uint16_t)p[0]);
    r |= (((uint16_t)p[1]) << 8);

#else

    r = *(uint16_t *)p;

#endif

    /* update the position in the buffer */
    if(i == -1) 
    {
        x86_dctx->pos += sizeof(uint16_t);
    }

    return r;
}

static 
uint32_t
get_uint32(x86_dasm_context_t* x86_dctx, int i)
{
    uint32_t r;

    uint8_t* p = (i == -1 ? 
        &x86_dctx->buffer[x86_dctx->pos] : 
        &x86_dctx->buffer[i]);

#if defined(X86_DASM_NLE)

    r = 0;
    r |= ((uint32_t)p[0]);
    r |= (((uint32_t)p[1]) << 8);
    r |= (((uint32_t)p[2]) << 16);
    r |= (((uint32_t)p[3]) << 24);

#else

    r = *(uint32_t *)p;

#endif

    /* update the position in the buffer */
    if(i == -1) 
    {
        x86_dctx->pos += sizeof(uint32_t);
    }

    return r;
}

static 
uint64_t
get_uint64(x86_dasm_context_t* x86_dctx, int i)
{
    uint64_t r;

    uint8_t* p = (i == -1 ? 
        &x86_dctx->buffer[x86_dctx->pos] : 
        &x86_dctx->buffer[i]);

#if defined(X86_DASM_NLE)

    r = 0;
    r |= ((uint64_t)p[0]);
    r |= (((uint64_t)p[1]) << 8);
    r |= (((uint64_t)p[2]) << 16);
    r |= (((uint64_t)p[3]) << 24);
    r |= (((uint64_t)p[4]) << 32);
    r |= (((uint64_t)p[5]) << 40);
    r |= (((uint64_t)p[6]) << 48);
    r |= (((uint64_t)p[7]) << 56);

#else

    r = *(uint64_t *)p;

#endif

    /* update the position in the buffer */
    if(i == -1) 
    {
        x86_dctx->pos += sizeof(uint64_t);
    }

    return r;
}

/***************************************************************************/

static int decode_modrm_mod_rm(x86_dasm_context_t* x86_dctx, 
    x86_operand_t* op, uint8_t base_reg);

/***************************************************************************/

static 
uint8_t
get_modrm(x86_dasm_context_t* x86_dctx)
{
    if(!x86_dctx->p_modrm)
    {
        x86_dctx->p_modrm = true;
        x86_dctx->pos_modrm = x86_dctx->pos;
        x86_dctx->modrm = get_uint8(x86_dctx, -1);
    }

    return x86_dctx->modrm;
}

static 
uint8_t
get_sib(x86_dasm_context_t* x86_dctx)
{
    if(!x86_dctx->p_sib)
    {
        x86_dctx->p_sib = true;
        x86_dctx->pos_sib = x86_dctx->pos;
        x86_dctx->sib = get_uint8(x86_dctx, -1);
    }

    return x86_dctx->sib;
}

static 
void
check_modrm_mod_rm(x86_dasm_context_t* x86_dctx, const x86_table_entry_t* te)
{
    x86_operand_t tmp;

    /* check all the operands (currently max 4) */
    for(int i = 0; i < 4; i++)
    {
        memset(&tmp, 0, sizeof(tmp));

        tmp.size = X86_OPSIZE_BYTE;
        tmp.reg = 0;

        switch(te->operands[i].am)
        {
            case AM_E:
            case AM_M:
            case AM_N:
            case AM_Q:
            case AM_R:
            case AM_U:
            case AM_W:
                /* get the modrm byte in advance */
                get_modrm(x86_dctx);
                decode_modrm_mod_rm(x86_dctx, &tmp, X86_REG_GPR);
                return;

            default:
                break;
            
        }
    }
}

static 
void
check_modrm_reg(x86_dasm_context_t* x86_dctx, const x86_table_entry_t* te)
{
    /* check all the operands (currently max 4) */
    for(int i = 0; i < 4; i++)
    {
        switch(te->operands[i].am)
        {
            case AM_C:
            case AM_D:
            case AM_G:
            case AM_P:
            case AM_S:
            case AM_V:
                /* get the modrm byte in advance */
                get_modrm(x86_dctx);
                return;

            default:
                break;  
        }
    }
}

static 
uint8_t
get_osize(x86_dasm_context_t* x86_dctx, uint16_t flags)
{
    /*
        D (default) field of the code-segment (CS) is assumed to be 32-bit 
        for protected mode / compatibility mode and 16-bit for real mode /
        virtual 8086 mode
    */

    /* operand-size override prefix present */
    uint8_t p = x86_dctx->pfx_p_osize;

    switch(x86_dctx->dmode)
    {
        case X86_DMODE_16BIT:

            /* assumed default operand size: 16 bit */
            x86_dctx->osize = (p ? X86_OSIZE_32BIT : X86_OSIZE_16BIT);
            break;

        case X86_DMODE_32BIT:

            /* assumed default operand size: 32 bit */
            x86_dctx->osize = (p ? X86_OSIZE_16BIT : X86_OSIZE_32BIT);
            break;

        case X86_DMODE_64BIT:

            if(flags & PF_F64)
            {
                /* operand size forced to 64-bit */
                x86_dctx->osize = X86_OSIZE_64BIT;
            }
            else
            {
                if( (REX_W) || 
                    (x86_dctx->pfx_p_vex3b && VEX_3B_W) ||
                    (x86_dctx->pfx_p_evex && EVEX_W) ||
                    (x86_dctx->pfx_p_xop && XOP_W) )
                {
                    /* if rex.w = 1 then prefix 66h is ignored, or vex.w=1 */
                    x86_dctx->osize = X86_OSIZE_64BIT;
                }
                else
                {
                    if(flags & PF_D64)
                    {
                        /* 
                            default operand size is 64 bit and cannot encode 
                            32-bit operand size 
                        */  
                        x86_dctx->osize = (p ? X86_OSIZE_16BIT : X86_OSIZE_64BIT);
                    }
                    else
                    {
                        /* default operand size: 32 bit*/
                        x86_dctx->osize = (p ? X86_OSIZE_16BIT : X86_OSIZE_32BIT);
                    }
                }
            }

            break;

        default:
            break;
    }

    dbg_msg("%i", x86_dctx->osize);

    return x86_dctx->osize;
}

static 
uint8_t
get_asize(x86_dasm_context_t* x86_dctx)
{
    /*
        D (default) field of the code-segment (CS) is assumed to be 32-bit 
        for protected mode / compatibility mode and 16-bit for real mode /
        virtual 8086 mode
    */

    /* address-size override prefix present */
    uint8_t p = x86_dctx->pfx_p_asize;

    switch(x86_dctx->dmode)
    {
        case X86_DMODE_16BIT:

            /* assumed default address size: 16 bit */
            x86_dctx->asize = (p ? X86_ASIZE_32BIT : X86_ASIZE_16BIT);
            break;

        case X86_DMODE_32BIT:

            /* assumed default address size: 32 bit */
            x86_dctx->asize = (p ? X86_ASIZE_16BIT : X86_ASIZE_32BIT);
            break;

        case X86_DMODE_64BIT:

            /* assumed default address size: 64 bit */
            x86_dctx->asize = (p ? X86_OSIZE_32BIT : X86_OSIZE_64BIT);
            break;

        default:
            break;
    }

    dbg_msg("%i", x86_dctx->asize);

    return x86_dctx->asize;
}

static 
int
decode_prefixes(x86_dasm_context_t* x86_dctx)
{
    int i;

    uint8_t* b = &x86_dctx->buffer[0];
    
    /* loop only until max instruction length */
    for(i = 0; i < MAX_INST_LENGTH; i++)
    {
        /* check for REX prefix separately */
        if(REX_PREFIX(b[i]))
        {
            if(x86_dctx->dmode == X86_DMODE_64BIT)
            {
                x86_dctx->pfx_last = b[i];
                x86_dctx->pfx_rex = b[i];
                x86_dctx->pos_rex = i;
                x86_dctx->pfx_p_rex = true;
            }
            else
            {
                /* 
                    not in long mode, so the opcode must be an inc/dec 
                    instruction and we are done decoding prefixes 
                */
                goto decode_prefixes_done;
            }
        }
        else
        {
            /* check for all the other prefixes */
            switch(b[i])
            {
                case PREFIX_OSIZE:

                    x86_dctx->pfx_last = b[i];
                    x86_dctx->pfx_c_osize++;
                    x86_dctx->pfx_p_osize = true;
                    
                    /* 
                        if the prefixes F2h, F3h are present, then it seems 
                        that the prefix 66h is ignored and between F2h and
                        F3h the one that comes later has precedence
                    */
                    if( x86_dctx->pfx_mandatory != PREFIX_REPNE &&
                        x86_dctx->pfx_mandatory != PREFIX_REP)
                    {
                        x86_dctx->pfx_mandatory = b[i];
                    }

                    break;
                    
                case PREFIX_ASIZE:
                    x86_dctx->pfx_last = b[i];
                    x86_dctx->pfx_p_asize = true;
                    break;

                case PREFIX_SEGOVRD_CS:

                    x86_dctx->pfx_seg = X86_REG_CS;

                    goto decode_prefixes_1;

                case PREFIX_SEGOVRD_DS:

                    x86_dctx->pfx_seg = X86_REG_DS;

                    goto decode_prefixes_1;

                case PREFIX_SEGOVRD_ES:

                    x86_dctx->pfx_seg = X86_REG_ES;

                    goto decode_prefixes_1;

                case PREFIX_SEGOVRD_SS:

                    x86_dctx->pfx_seg = X86_REG_SS;

decode_prefixes_1:

                    /* these segment override pfxs are ignored in long mode */
                    if(x86_dctx->dmode != X86_DMODE_64BIT)
                    {
                        /* last segment override prefix wins */
                        x86_dctx->pfx_last = b[i];
                        x86_dctx->pfx_p_seg = true;
                    }

                    break;

                case PREFIX_SEGOVRD_FS:

                    x86_dctx->pfx_seg = X86_REG_FS;

                    goto decode_prefixes_2;

                case PREFIX_SEGOVRD_GS:

                    x86_dctx->pfx_seg = X86_REG_GS;

decode_prefixes_2:

                    /* last segment override prefix wins */
                    x86_dctx->pfx_last = b[i];
                    x86_dctx->pfx_p_seg = true;
                    break;

                case PREFIX_LOCK:
                    x86_dctx->pfx_last = b[i];
                    x86_dctx->pfx_p_lock = true;
                    break;

                case PREFIX_REPNE:
                    x86_dctx->pfx_last = b[i];
                    x86_dctx->pfx_mandatory = b[i];
                    x86_dctx->pfx_p_repne = true;
                    break;

                case PREFIX_REP:
                    x86_dctx->pfx_last = b[i];
                    x86_dctx->pfx_mandatory = b[i];
                    x86_dctx->pfx_p_rep = true;
                    break;
                
                default:

                    goto decode_prefixes_done;
            }
        }
    }

    dbg_err("instruction's size exceeded the limit!");
    return -1;

decode_prefixes_done:

    /*
        REX prefix
    */
    if(x86_dctx->dmode == X86_DMODE_64BIT)
    {
        /* 
            REX prefix is only valid in long mode (64-bit mode) and if used, 
            the REX prefix byte must immediately precede the opcode byte or 
            the escape opcode byte (0FH). When a REX prefix is used in 
            conjunction with an instruction containing a mandatory prefix, 
            the mandatory prefix must come before the REX so the REX prefix 
            can be immediately preceding the opcode or the escape byte. Other 
            placements are ignored. The instruction-size limit of 15 bytes 
            still applies to instructions with a REX prefix.
        */
        if(!REX_PREFIX(x86_dctx->pfx_last))
        {
            /* ignore it */
            x86_dctx->pfx_rex = 0;
            x86_dctx->pos_rex = 0;
            x86_dctx->pfx_p_rex = false;
        }
    }

    /*
        VEX, EVEX, XOP prefixes
    */
    if(b[i] == PREFIX_VEX_3B && (x86_dctx->dmode == X86_DMODE_64BIT || 
        VEX_3B_PM(b[i+1])))
    {
        if( i > (MAX_INST_LENGTH - 5) || 
            x86_dctx->pfx_p_osize || x86_dctx->pfx_p_rex ||
            x86_dctx->pfx_p_lock || x86_dctx->pfx_p_rep ||
            x86_dctx->pfx_p_repne )
        {
            /* 
                any VEX-encoded instruction with a REX, LOCK, 66H, F2H, 
                or F3H prefix preceding VEX will #UD
            */
            dbg_err("VEX-encoded instruction will #UD!");
            return -1;
        }
        else
        {
            dbg_msg("VEX3B");

            x86_dctx->pfx_vex[0] = b[i+1]; /* P0 */
            x86_dctx->pfx_vex[1] = b[i+2]; /* P1 */
            x86_dctx->pfx_p_vex3b = true;

            /* check if the m-mmmm value is invalid */
            if(VEX_3B_MMMMM == 0 || VEX_3B_MMMMM > 3)
            {
                dbg_err("VEX-encoded instruction will #UD!");
                return -1;
            }

            x86_dctx->table_link = vex_links[VEX_3B_PP][VEX_3B_MMMMM];

            /* update the position */
            i += 3;
        }
    }
    else
    if(b[i] == PREFIX_VEX_2B && (x86_dctx->dmode == X86_DMODE_64BIT || VEX_2B_PM(b[i+1])))
    {
        if( i > (MAX_INST_LENGTH - 4) || 
            x86_dctx->pfx_p_osize || x86_dctx->pfx_p_rex ||
            x86_dctx->pfx_p_lock || x86_dctx->pfx_p_rep ||
            x86_dctx->pfx_p_repne )
        {
            /* 
                any VEX-encoded instruction with a REX, LOCK, 66H, F2H, 
                or F3H prefix preceding VEX will #UD
            */
            dbg_err("VEX-encoded instruction will #UD!");

            return -1;
        }
        else
        {
            dbg_msg("VEX2B");

            x86_dctx->pfx_vex[0] = b[i+1]; /* P0 */
            x86_dctx->pfx_p_vex2b = true;

            x86_dctx->table_link = vex_links[VEX_2B_PP][1];

            /* update the position */
            i += 2;
        }
    }
    else
    if(b[i] == PREFIX_EVEX && (x86_dctx->dmode == X86_DMODE_64BIT || EVEX_PM(b[i+1])))
    {
        /* 
            currently dont support EVEX prefix until better 
            documentation is released...and I stop being lazy
        */
        dbg_err("EVEX currently not supported!");
        return -1;

        if( i > (MAX_INST_LENGTH - 6) || 
            x86_dctx->pfx_p_osize || x86_dctx->pfx_p_rex ||
            x86_dctx->pfx_p_lock || x86_dctx->pfx_p_rep ||
            x86_dctx->pfx_p_repne )
        {
            /* 
                any VEX-encoded instruction with a REX, LOCK, 66H, F2H, 
                or F3H prefix preceding VEX will #UD
            */
            dbg_err("VEX-encoded instruction will #UD!");
            return -1;
        }
        else
        {
            if(EVEX_P0(b[i+1]) && EVEX_P1(b[i+2]))
            {
                dbg_msg("EVEX");

                x86_dctx->pfx_vex[0] = b[i+1]; /* P0 */
                x86_dctx->pfx_vex[1] = b[i+2]; /* P1 */
                x86_dctx->pfx_vex[2] = b[i+3]; /* P2 */
                x86_dctx->pfx_p_evex = true;

                x86_dctx->table_link = vex_links[EVEX_PP][EVEX_MM];

                /* update the position */
                i += 4;
            }
        }
    }
    else
    if(b[i] == PREFIX_XOP && (x86_dctx->dmode == X86_DMODE_64BIT || XOP_PM(b[i+1])) && XOP_VALID(b[i+1]))
    {
        if( i > (MAX_INST_LENGTH - 5) || 
            x86_dctx->pfx_p_osize || x86_dctx->pfx_p_rex ||
            x86_dctx->pfx_p_lock || x86_dctx->pfx_p_rep ||
            x86_dctx->pfx_p_repne )
        {
            /* 
                any XOP-encoded instruction with a REX, LOCK, 66H, F2H, 
                or F3H prefix preceding VEX will #UD
            */
            dbg_err("XOP-encoded instruction will #UD!");
            return -1;
        }
        else
        {
            dbg_msg("XOP");

            x86_dctx->pfx_xop[0] = b[i+1]; /* P0 */
            x86_dctx->pfx_xop[1] = b[i+2]; /* P1 */
            x86_dctx->pfx_p_xop = true;

            /* check if the pp value is invalid */
            if(XOP_PP != 0)
            {
                dbg_err("XOP-encoded instruction will #UD!");
                return -1;
            }

            x86_dctx->table_link = xop_links[XOP_PP][XOP_MMMMM - 8];

            /* update the position */
            i += 3;
        }
    }
    else
    if(b[i] == ESCAPE_OPCODE_2B)
    {
        /* later on we will decide if it really is a mandatory prefix or not */
        switch(x86_dctx->pfx_mandatory)
        {
            case PREFIX_OSIZE:          
                x86_dctx->table_link = &x86_table_links[TABLE_ID(66_0F)];
                break;

            case PREFIX_REP:
                x86_dctx->table_link = &x86_table_links[TABLE_ID(F3_0F)];
                break;

            case PREFIX_REPNE:
                x86_dctx->table_link = &x86_table_links[TABLE_ID(F2_0F)];
                break;

            default:
                x86_dctx->table_link = &x86_table_links[TABLE_ID(0F)];
                break;
        }

        /* update the position */
        i++;
    }
    else
    {
        /* one-byte table */
        x86_dctx->pfx_mandatory = 0;
        x86_dctx->table_link = &x86_table_links[TABLE_ID(1B)];
    }

    /* update the position in the buffer */
    x86_dctx->pos = i;
    x86_dctx->pos_opcode = x86_dctx->pos;

    dbg_msg(
        "\n"
        "\t pfx_seg         : %02Xh\n"
        "\t pfx_rex         : %02Xh\n"
        "\t pfx_last        : %02Xh\n"
        "\t pfx_mandatory   : %02Xh\n"
        "\t pfx_vex         : %02Xh, %02Xh\n"
        "\t pfx_p_osize     : %i\n"
        "\t x86_dctx->pos   : %i\n",
        x86_dctx->pfx_seg,
        x86_dctx->pfx_rex,
        x86_dctx->pfx_last,
        x86_dctx->pfx_mandatory,
        x86_dctx->pfx_vex[0],
        x86_dctx->pfx_vex[1],
        x86_dctx->pfx_p_osize,
        x86_dctx->pos );

    return i;
}

static 
int
decode_opcode(x86_dasm_context_t* x86_dctx, const x86_table_entry_t** te_out)
{
    int done = false;

    uint8_t j = 0;
    uint8_t* b = &x86_dctx->buffer[0];

    /* needed to decode 3DNow! instructions */
    x86_operand_t op;
    
    const x86_table_link_t* link = x86_dctx->table_link; 
    const x86_table_entry_t* te = link->table;

    /* loop until we have a valid instruction entry */
    while(!done)
    {
        dbg_msg("table: %s, method: %i", 
            link->table_name, link->lookup_method);

        switch(link->lookup_method)
        {
            case TABLE_LOOKUP_METHOD(BYTE):

                dbg_msg("opcode: %02X", b[x86_dctx->pos]);

                /* current byte is an opcode */
                if(te[b[x86_dctx->pos]].table_id == TABLE_ID(NONE))
                {
                    /* get the instruction */
                    te = &te[b[x86_dctx->pos]];
                    done = true;
                }
                else
                {
                    /* get the new table link */
                    link = &x86_table_links[te[b[x86_dctx->pos]].table_id];
                    te = link->table;
                }

                /* update the position */
                x86_dctx->pos++;
                break;

            case TABLE_LOOKUP_METHOD(3DNOW):

                /* decode the modr/m and sib (if present) */
                decode_modrm_mod_rm(x86_dctx, &op, X86_REG_MM0);

                /* now the current byte is the opcode */
                te = &te[b[x86_dctx->pos]];
                done = true;

                /* restore */
                x86_dctx->pos_opcode = x86_dctx->pos;
                x86_dctx->pos++;
                break;

            case TABLE_LOOKUP_METHOD(OSIZE):

                /* 
                    recalculate the actual operand size by taking into 
                    account also the following instruction's property flags: 
                    D64 and F64
                */
                j = x86_dctx->osize;
                j = get_osize(x86_dctx, te[j].properties);

                goto decode_opcode_1;

            case TABLE_LOOKUP_METHOD(ASIZE):

                j = get_asize(x86_dctx);

                goto decode_opcode_1;

            case TABLE_LOOKUP_METHOD(DMODE):

                j = x86_dctx->dmode;

                goto decode_opcode_1;

            case TABLE_LOOKUP_METHOD(FPU):

                /* current byte is modrm */
                j = get_modrm(x86_dctx) - 0xC0;

                goto decode_opcode_1;

            case TABLE_LOOKUP_METHOD(OSIZE_XOP):

                /* sanity check */
                if(x86_dctx->pfx_p_xop == false)
                {
                    dbg_err("XOP prefix missing!");
                    return -1;
                }

                j = XOP_W;
                
                goto decode_opcode_1;

            case TABLE_LOOKUP_METHOD(OSIZE_VEX):

                /* sanity check */
                if( x86_dctx->pfx_p_vex3b == false &&
                    x86_dctx->pfx_p_evex == false )
                {
                    dbg_err("VEX prefix missing!");
                    return -1;
                }

                j = (x86_dctx->pfx_p_vex3b ? VEX_3B_W : j);
                j = (x86_dctx->pfx_p_evex ? EVEX_W : j);

                goto decode_opcode_1;

            case TABLE_LOOKUP_METHOD(MODRM_MOD):

                /* current byte is modrm */
                j = MODRM_MOD(get_modrm(x86_dctx)) == 3 ? 1 : 0;

                dbg_msg("MODRM_MOD %02X -> %i", get_modrm(x86_dctx), j);

                goto decode_opcode_1;

            case TABLE_LOOKUP_METHOD(MODRM_RM):

                /* current byte is modrm */
                j = MODRM_RM(get_modrm(x86_dctx));

                dbg_msg("MODRM_RM %02X -> %i", get_modrm(x86_dctx), j);

                goto decode_opcode_1;

            case TABLE_LOOKUP_METHOD(MODRM_REG):

                /* current byte is modrm */
                j = MODRM_REG(get_modrm(x86_dctx));

                dbg_msg("MODRM_REG %02X -> %i", get_modrm(x86_dctx), j);

decode_opcode_1:

                if(te[j].table_id == TABLE_ID(NONE))
                {
                    /* get the instruction */
                    te = &te[j];
                    done = true;
                }
                else
                {
                    /* get the new table link */
                    link = &x86_table_links[te[j].table_id];
                    te = link->table;
                }

                break;

            default:
                dbg_err("undefined lookup method!");
                return -1;
        }
    }

    *te_out = te;

    x86_dctx->iset = te->iset;
    x86_dctx->mnem = te->mnem_id;

    /* check if the instruction supports vectored sib */
    if( (te->properties & PF_VSIB_X) != 0 || 
        (te->properties & PF_VSIB_Y) != 0 ||
        (te->properties & PF_VSIB_Z) != 0 ||
        (te->properties & PF_VSIB_L) != 0 )
    {
        x86_dctx->p_vsib = true;

        if((te->properties & PF_VSIB_X) != 0)
        {
            x86_dctx->vsib_base = X86_REG_XMM0;
        }
        else
        if((te->properties & PF_VSIB_Y) != 0)
        {
            x86_dctx->vsib_base = X86_REG_YMM0;
        }
        else
        if((te->properties & PF_VSIB_Z) != 0)
        {
            x86_dctx->vsib_base = X86_REG_ZMM0;
        }
        else
        {
            x86_dctx->vsib_base = X86_REG_NONE;
        }
    }

    /* remove prefixes because they are used as mandatory */
    /* this was made to resolve the jcc bug, a 66 prefix on a jcc mods the osize */
    if((te->properties & PF_PFXNA) == 0)
    {
        switch(x86_dctx->pfx_mandatory)
        {
            case PREFIX_OSIZE:
                x86_dctx->pfx_p_osize = false;
                break;

            case PREFIX_REP:
                x86_dctx->pfx_p_rep = false;
                break;

            case PREFIX_REPNE:
                x86_dctx->pfx_p_repne = false;
                break;

            default:
                break;
        }
    }

#if !defined(X86_DASM_NOFMT) || defined(X86_DASM_DEBUG)

    if(x86_dctx->mnem != X86_MN_INVALID)
    {
        /* copy the mnemonic string */
        strcpy(x86_dctx->mnem_str, te->mnem_str);
    
        dbg_msg("%s", te->mnem_str);
    }

#endif

    check_modrm_mod_rm(x86_dctx, te);
    check_modrm_reg(x86_dctx, te);

    return 0;
}

#define decode_imm(ctx, op)     decode_data(ctx, op, X86_OPTYPE_IMM)
#define decode_rel(ctx, op)     decode_data(ctx, op, X86_OPTYPE_REL)
#define decode_moffs(ctx, op)   decode_data(ctx, op, X86_OPTYPE_MOFFS)

static 
int
decode_data(x86_dasm_context_t* x86_dctx, x86_operand_t* op, uint8_t type)
{
    uint8_t size = op->size;
    int32_t rel = 0;

    op->type = type;
    op->pos = x86_dctx->pos;

    /* offset type needs to be decoded by addressing size */
    if(op->type == X86_OPTYPE_MOFFS)
    {
        switch(x86_dctx->asize)
        {
            case X86_ASIZE_16BIT: 
                op->asize = X86_ASIZE_WORD; size = X86_OPSIZE_WORD; 
                break;

            case X86_ASIZE_32BIT: 
                op->asize = X86_ASIZE_DWORD; size = X86_OPSIZE_DWORD; 
                break;

            case X86_ASIZE_64BIT: 
                op->asize = X86_ASIZE_QWORD; size = X86_OPSIZE_QWORD; 
                break;

            default:
                break;
        }
    }

    /* get/read the data */
    switch(size)
    {
        case X86_OPSIZE_BYTE: 
            op->data.u8 = get_uint8(x86_dctx, -1);
            rel = op->data.s8;
            break;

        case X86_OPSIZE_WORD:
            op->data.u16 = get_uint16(x86_dctx, -1);
            rel = op->data.s16;
            break;

        case X86_OPSIZE_DWORD:
            op->data.u32 = get_uint32(x86_dctx, -1);
            rel = op->data.s32;
            break;

        case X86_OPSIZE_QWORD:
            op->data.u64 = get_uint64(x86_dctx, -1);
            break;

        default:
            dbg_err("undefined opsize for imm!");
            return -1;
    }

    /* compute the destination address for relative operands */
    if(op->type == X86_OPTYPE_REL)
    {
        /* instruction length */
        uint8_t len = x86_dctx->len;

        if(x86_dctx->dmode == X86_DMODE_64BIT)
        {
            /* operand size ignored (66h) */
            x86_dctx->dest_rip.u64 = (x86_dctx->rip.u64 + len) + rel;
        }
        else
        {
            /* X86_DMODE_32BIT and X86_DMODE_16BIT */
            x86_dctx->dest_rip.u32 = (x86_dctx->rip.u32 + len) + rel;
            
            if(x86_dctx->osize == X86_OSIZE_16BIT)
            {
                x86_dctx->dest_rip.u32 &= 0xffff;
            }
        }
    }

    return 0;
}

static 
int
decode_gpr(x86_dasm_context_t* x86_dctx, uint8_t opsize, uint8_t reg, uint8_t* result)
{
    /* by default assume invalid register */
    *result = X86_REG_NONE;

    switch(opsize)
    {
        case X86_OPSIZE_BYTE:

            if(x86_dctx->dmode == X86_DMODE_64BIT && x86_dctx->pfx_p_rex)
            {
                /* 
                    SPL, BPL, SIL, DIL only addressable in long mode  
                    instead of AH, CH, DH, BH   
                */
                *result = (reg > 3 ? X86_REG_SPL + reg-4 : X86_REG_AL + reg);
            }
            else
            {
                *result = (X86_REG_AL + reg);
            }

            break;

        case X86_OPSIZE_WORD:   *result = (X86_REG_AX + reg); break;
        case X86_OPSIZE_DWORD:  *result = (X86_REG_EAX + reg); break;
        case X86_OPSIZE_QWORD:  *result = (X86_REG_RAX + reg); break;

        default:
            dbg_err("undefined opsize for gpr!");
            return -1;
    }

    return 0;
}

static 
int
decode_reg(x86_dasm_context_t* x86_dctx, x86_operand_t* op, uint8_t base_reg, uint8_t reg)
{
    op->type = X86_OPTYPE_REG;
    op->reg = X86_REG_NONE;

    switch(base_reg)
    {
        case X86_REG_DR0:

            /* valid debug register: DR0-DR7 */
            if(reg > 7)
            {
                dbg_err("invalid debug register!");
                return -1;
            }
            
            goto decode_reg_1;

        case X86_REG_CR0:

            /* valid control register: CR0, CR2-CR4, CR8 */
            if(!(reg == 0 || (reg > 1 && reg < 5) || reg == 8))
            {
                dbg_err("invalid control register!");
                return -1;
            }

            goto decode_reg_1;

        case X86_REG_ES:

            /* valid segment register */
            if((reg & 7) == 6 || (reg & 7) == 7)
            {
                dbg_err("invalid segment register!");
                return -1;
            }

            goto decode_reg_1;

        case X86_REG_XMM0:
        case X86_REG_YMM0:
        case X86_REG_ZMM0:

decode_reg_1:

            /* add reg to base_reg like an offset */
            op->reg = base_reg + reg;
            break;

        case X86_REG_ST0:
        case X86_REG_MM0:   
            /* valid mmx register: MM0-MM7, even with extension */
            op->reg = base_reg + (reg & 7); 
            break;

        case X86_REG_GPR:
            return decode_gpr(x86_dctx, op->size, reg, &op->reg);

        default:
            dbg_err("invalid base register!");
            return -1;
    }

    return 0;
}

static 
void
decode_sib(x86_dasm_context_t* x86_dctx, x86_operand_t* op)
{
    uint8_t i = 0;

    uint8_t base = SIB_BASE(get_sib(x86_dctx));
    uint8_t index = SIB_INDEX(get_sib(x86_dctx));
    uint8_t mod = MODRM_MOD(get_modrm(x86_dctx));

    dbg_msg("base: %i, index: %i, mod: %i", base, index, mod);

    /* scaling factor is 2^(sib.scale) */
    op->mexpr.scale = 1 << SIB_SCALE(get_sib(x86_dctx));

    if(x86_dctx->dmode == X86_DMODE_64BIT)
    {
        /*
            in long mode the REX.X/VEX.X/XOP.X fields extend the SIB.I field, 
            the REX.B/VEX.B/XOP.B fields extend the SIB.B field, and 
            currently there can be no REX prefix if a VEX prefix is 
            present (result is #UD)
        */
        if(x86_dctx->pfx_p_rex)
        {
            base |= (REX_B << 3);
            index |= (REX_X << 3);
        }
        else
        if(x86_dctx->pfx_p_vex3b)
        {
            base |= (VEX_3B_B << 3);
            index |= (VEX_3B_X << 3);
        }
        else 
        if(x86_dctx->pfx_p_evex)
        {
            base |= (EVEX_B << 3);
            index |= (EVEX_X << 3);

            /* 
                need to check if instruction supports VSIB:
            
                index |= (VEX_EVEX_X(x86_dctx->pfx_vex[0]) << 3);
                index |= (VEX_EVEX_VV(x86_dctx->pfx_vex[2]) << 4);

                [X/Y/ZMM0*scale+base]
            */
        }
        else
        if(x86_dctx->pfx_p_xop)
        {
            base |= (XOP_B << 3);
            index |= (XOP_X << 3);
        }
    }

    /* check for vectored sib support */
    if(x86_dctx->p_vsib)
    {
        /* 
            VSIB = Vector SIB

            Specific AVX2 instructions utilize a vectorized form of 
            indexed register-indirect addressing called vector SIB (VSIB) 
            addressing. In contrast to the standard indexed 
            register-indirect address mode, which generates a single 
            effective address to access a single memory operand, VSIB 
            addressing generates an array of effective addresses which 
            is used to access data from multiple memory locations in
            a single operation.
        */
        if(x86_dctx->vsib_base == X86_REG_NONE)
        {
            /* the reg size depends on the L field */
            if(op->size == X86_OPSIZE_ZWORD)
            {
                i = X86_REG_ZMM0;
            }
            else
            if(op->size == X86_OPSIZE_YWORD)
            {
                i = X86_REG_YMM0;
            }
            else
            {
                i = X86_REG_XMM0;
            }
        }
        else
        {
            i = x86_dctx->vsib_base;
        }
    }

    /* effective address depends on addressing size */
    switch(x86_dctx->asize)
    {
        case X86_ASIZE_32BIT:

            op->mexpr.base = X86_REG_EAX + base;
            op->mexpr.index = (x86_dctx->p_vsib ? i : X86_REG_EAX) + index;

            goto decode_sib_1;

        case X86_ASIZE_64BIT:

            op->mexpr.base = X86_REG_RAX + base;
            op->mexpr.index = (x86_dctx->p_vsib ? i : X86_REG_RAX) + index;

decode_sib_1:

            if( op->mexpr.index == X86_REG_ESP ||
                op->mexpr.index == X86_REG_RSP )
            {
                /* no index register is encoded */
                op->mexpr.index = X86_REG_NONE;
            }

            if( op->mexpr.base == X86_REG_EBP || 
                op->mexpr.base == X86_REG_R13D ||
                op->mexpr.base == X86_REG_RBP || 
                op->mexpr.base == X86_REG_R13 )
            {
                switch(mod)
                {
                    case 0:
                        op->mexpr.base = X86_REG_NONE;
                        op->mexpr.disp_size = X86_DISP_32; 
                        break;

                    /* base preserved */
                    /* if EVEX, X86_DISP_8 * N */
                    case 1: op->mexpr.disp_size = X86_DISP_8; break; 
                    case 2: op->mexpr.disp_size = X86_DISP_32; break;

                    default:
                        break;
                }
            }

            break;

        default:
            break;
    }
}

static 
int
decode_modrm_reg(x86_dasm_context_t* x86_dctx, x86_operand_t* op, uint8_t base_reg)
{
    uint8_t reg = MODRM_REG(get_modrm(x86_dctx));

    dbg_msg("reg: %i", reg);

    if(x86_dctx->dmode == X86_DMODE_64BIT && base_reg != X86_REG_ES)
    {
        /*
            in long mode the REX.R/VEX.R/XOP.R fields extend the MODRM.REG 
            field, and currently there can be no REX prefix if a VEX prefix 
            is present (result is #UD)
        */
        if(x86_dctx->pfx_p_rex)
        {
            reg |= (REX_R << 3);
        }
        else
        if(x86_dctx->pfx_p_vex2b)
        {
            /* VEX2B.R is like REX.R */
            reg |= (VEX_2B_R << 3);
        }
        else
        if(x86_dctx->pfx_p_vex3b)
        {
            /* VEX3B.R is like REX.R */
            reg |= (VEX_3B_R << 3);
        }
        else
        if(x86_dctx->pfx_p_evex)
        {
            /* EVEX.R': combine with EVEX.R and ModR/M.reg  */
            reg |= (EVEX_R << 3);
            reg |= (EVEX_RR << 4);
        }
        else
        if(x86_dctx->pfx_p_xop)
        {
            /* XOP.R is like REX.R */
            reg |= (XOP_R << 3);
        }
    }
    
    return decode_reg(x86_dctx, op, base_reg, reg);
}

static 
int
decode_modrm_mod_rm(x86_dasm_context_t* x86_dctx, x86_operand_t* op, uint8_t base_reg)
{
    uint8_t mod = MODRM_MOD(get_modrm(x86_dctx));
    uint8_t rm = MODRM_RM(get_modrm(x86_dctx));

    dbg_msg("mod: %i, rm: %i", mod, rm);

    if(x86_dctx->dmode == X86_DMODE_64BIT)
    {
        /*
            in long mode the REX.B/VEX.B/XOP.B fields extend the MODRM.RM 
            field, and currently there can be no REX prefix if a VEX prefix 
            is present (result is #UD)
        */
        if(x86_dctx->pfx_p_rex)
        {
            rm |= (REX_B << 3);
        }
        else
        if(x86_dctx->pfx_p_vex3b)
        {
            rm |= (VEX_3B_B << 3);
        }
        else
        if(x86_dctx->pfx_p_evex)
        {
            rm |= (EVEX_B << 3);

            /* if register-direct then EVEX.X also extends r/m */
            if(mod == 3)
            {
                rm |= (EVEX_X << 4);
            }
        }
        else
        if(x86_dctx->pfx_p_xop)
        {
            rm |= (XOP_B << 3);
        }
    }
    
    /* mod = 11b, register-direct addressing mode */
    if(mod == 3)
    {
        /* register: GPR/MMX/XMM/YMM/ZMM */
        return decode_reg(x86_dctx, op, base_reg, rm);
    }

    /* mod = !11b, register-indirect addressing */
    op->type = X86_OPTYPE_MEXPR;
    op->mexpr.base = X86_REG_NONE;
    op->mexpr.index = X86_REG_NONE;
    op->mexpr.disp_size = X86_DISP_NONE;
    op->mexpr.scale = 1;

    /* effective address depends on addressing size */
    switch(x86_dctx->asize)
    {
        case X86_ASIZE_16BIT:

            /* REX.B/VEX.B/XOP.B is ignored */
            rm &= 7;

            op->mexpr.base = ea_asize16[rm][0];
            op->mexpr.index = ea_asize16[rm][1];
            
            if(mod == 0 && rm == 6)
            {
                op->mexpr.base = X86_REG_NONE;
                op->mexpr.disp_size = X86_DISP_16;
            }
            else
            {
                op->mexpr.disp_size = (mod == 1 ? X86_DISP_8 : op->mexpr.disp_size);
                op->mexpr.disp_size = (mod == 2 ? X86_DISP_16 : op->mexpr.disp_size);
            }

            break;

        case X86_ASIZE_32BIT:

            op->mexpr.base = X86_REG_EAX + rm;

            /* REX.B/VEX.B/XOP.B is ignored */
            if(mod == 0 && (rm & 7) == 5)
            {
                if(x86_dctx->dmode == X86_DMODE_64BIT)
                {
                    /*
                        RIP-relative addressing is enabled by 64-bit mode, 
                        not by a 64-bit address-size. The use of the 
                        address-size prefix does not disable RIP-relative 
                        addressing. The effect of the address-size prefix is 
                        to truncate and zero-extend the computed effective 
                        address to 32 bits.
                    */
                    op->mexpr.base = X86_REG_EIP;
                }
                else
                {
                    op->mexpr.base = X86_REG_NONE;
                }
                
                op->mexpr.disp_size = X86_DISP_32;
            }

            goto decode_modrm_mod_rm_1;

        case X86_ASIZE_64BIT:

            op->mexpr.base = X86_REG_RAX + rm;

            /* REX.B/VEX.B/XOP.B is ignored */
            if(mod == 0 && (rm & 7) == 5)
            {
                op->mexpr.base = X86_REG_RIP;
                op->mexpr.disp_size = X86_DISP_32;
            }

decode_modrm_mod_rm_1:
            
            /* if EVEX, X86_DISP_8 * N */
            op->mexpr.disp_size = (mod == 1 ? X86_DISP_8 : op->mexpr.disp_size); 
            op->mexpr.disp_size = (mod == 2 ? X86_DISP_32 : op->mexpr.disp_size);

            /* REX.B/VEX.B/XOP.B is ignored */
            if((rm & 7) == 4)
            {
                /* SIB byte follows the ModR/M byte */
                decode_sib(x86_dctx, op);
            }

            break;

        default:
            break;
    
    }

    /* used to consume or peek the data in the buffer */
    int disp_offset = -1;

    /* save the displacement position */
    op->pos = x86_dctx->pos;

    if(x86_dctx->p_sib)
    {
        if(x86_dctx->pos != x86_dctx->pos_sib + 1)
        {
            op->pos = disp_offset = x86_dctx->pos_sib + 1;
        }
    }
    else
    if(x86_dctx->p_modrm)
    {
        if(x86_dctx->pos != x86_dctx->pos_modrm + 1)
        {
            op->pos = disp_offset = x86_dctx->pos_modrm + 1;
        }
    }

    /* get the displacement */
    switch(op->mexpr.disp_size)
    {
        case X86_DISP_8:
            op->mexpr.disp.u8 = get_uint8(x86_dctx, disp_offset); 
            break;

        case X86_DISP_16: 
            op->mexpr.disp.u16 = get_uint16(x86_dctx, disp_offset); 
            break;

        case X86_DISP_32: 
            op->mexpr.disp.u32 = get_uint32(x86_dctx, disp_offset); 
            break;

        default:
            break;
    }

    return 0;
}

static 
int
decode_opsize(x86_dasm_context_t* x86_dctx, const x86_table_entry_t* te, int i)
{
    x86_operand_t* op = &x86_dctx->operands[i];

    /* decode the operand type */
    switch(te->operands[i].ot)
    {
        case OT_NONE:
            /* no operand type */
            op->size = X86_OPSIZE_NONE;
            break;

        case OT_B:
            /* byte, regardless of osize */
            op->size = X86_OPSIZE_BYTE;
            break;

        case OT_S:
            /* 6-byte or 10-byte pseudo-descriptor */
            switch(x86_dctx->osize)
            {
                case X86_OSIZE_64BIT:
                    op->size = X86_OPSIZE_TWORD;
                    break;

                default:
                    op->size = X86_OPSIZE_FWORD;
                    break;
            }

            break;

        case OT_D:
            /* doubleword, regardless of osize */
        case OT_SI:
            /* doubleword integer register (eax,...) */
            op->size = X86_OPSIZE_DWORD;
            break;

        case OT_DQ:
            /* double-quadword, regardless of osize */
            op->size = X86_OPSIZE_OWORD;
            break;

        case OT_P:
            /* 32-bit, 48-bit, or 80-bit ptr, depending on osize */
            switch(x86_dctx->osize)
            {
                case X86_OSIZE_16BIT:
                    op->size = X86_OPSIZE_DWORD;
                    break;
                case X86_OSIZE_32BIT:
                    op->size = X86_OPSIZE_FWORD;
                    break;
                case X86_OSIZE_64BIT:
                    op->size = X86_OPSIZE_TWORD;
                    break;

                default:
                    break;
            }
            
            break;

        case OT_PS:
            /* 128-bit or 256-bit packed single floating-point */
        case OT_PD:
            /* 128-bit or 256-bit packed double floating-point */
        case OT_X:
            /* dq or qq based on the osize */
            if( (x86_dctx->pfx_p_vex2b && VEX_2B_L) ||
                (x86_dctx->pfx_p_vex3b && VEX_3B_L) ||
                (x86_dctx->pfx_p_xop && XOP_L) )
            {
                /* L: vector length (0: 128-bit, 1: 256-bit) */
                op->size = X86_OPSIZE_YWORD;
            }
            else
            {
                /* default assume L=0 (128-bit) */
                op->size = X86_OPSIZE_OWORD;
            }

            break;

        case OT_PI:
            /* quadword mmx technology register */
            op->size = X86_OPSIZE_QWORD;
            break;

        case OT_Q:
            /* quadword, regardless of osize */
            op->size = X86_OPSIZE_QWORD;
            break;

        case OT_OQ:
            /* 64- or 128-bit depending on VEX.L */
            if( (x86_dctx->pfx_p_vex2b && VEX_2B_L) ||
                (x86_dctx->pfx_p_vex3b && VEX_3B_L) ||
                (x86_dctx->pfx_p_xop && XOP_L) )
            {
                op->size = X86_OPSIZE_OWORD;
            }
            else
            {
                /* default assume L=0 (64-bit) */
                op->size = X86_OPSIZE_QWORD;
            }

            break;

        case OT_QQ:
            /* quad-quadword, regardless of osize */
            op->size = X86_OPSIZE_YWORD;
            break;

        case OT_SD:
            /* scalar element of a 128-bit double floating-point */
        case OT_SS:
            /* scalar element of a 128-bit single floating-point */
            op->size = X86_OPSIZE_OWORD;
            break;

        case OT_V:
            /* word, dword or qword (in 64-bit mode), depending on osize */
            switch(x86_dctx->osize)
            {
                case X86_OSIZE_16BIT:
                    op->size = X86_OPSIZE_WORD;
                    break;
                case X86_OSIZE_32BIT:
                    op->size = X86_OPSIZE_DWORD;
                    break;
                case X86_OSIZE_64BIT:
                    op->size = X86_OPSIZE_QWORD;
                    break;

                default:
                    break;
            }
            
            break;

        case OT_T:
            /* tbyte, regardless of osize */
            op->size = X86_OPSIZE_TWORD;
            break;

        case OT_W:
            /* word, regardless of osize */
            op->size = X86_OPSIZE_WORD;
            break;

        case OT_Y:
            /* dword or qword (in 64-bit mode), depending on osize */
            switch(x86_dctx->osize)
            {
                case X86_OSIZE_64BIT:
                    op->size = X86_OPSIZE_QWORD;
                    break;

                default:
                    op->size = X86_OPSIZE_DWORD;
                    break;
            }

            break;

        case OT_A: 
            /* bound r16, m16&16 / bound r32, m32&32 */
        case OT_Z:
            /* word for 16-bit osize or dword for 32 or 64-bit osize */
            switch(x86_dctx->osize)
            {
                case X86_OSIZE_16BIT:
                    op->size = X86_OPSIZE_WORD;
                    break;

                default:
                    op->size = X86_OPSIZE_DWORD;
                    break;
            }

            break;

        default:
            dbg_err("undefined operand type!");
            return -1;
    }

    dbg_msg("%s", get_opsize_str(op->size));

    return 0;
}

static 
int
decode_optype(x86_dasm_context_t* x86_dctx, const x86_table_entry_t* te, int i)
{
    uint8_t j, x;
    
    x86_operand_t* op = &x86_dctx->operands[i];

    /* default init */
    op->seg = X86_REG_DS;
    op->type = X86_OPTYPE_NONE;

    /* check for segment prefix override */
    if(x86_dctx->pfx_p_seg)
    {
        op->seg = x86_dctx->pfx_seg;
    }

    /* decode the addressing method */
    switch(te->operands[i].am)
    {
        case AM_NONE:
            /* no addressing method */
            op->type = X86_OPTYPE_NONE;
            break;

        case AM_A:
            /* 
                direct address: the instruction has no ModR/M byte; the 
                address of the operand is encoded in the instruction. No base 
                register, index register, or scaling factor can be applied 
                
                EA 1E277072 3300 -> JMP FAR 0033:7270271E
            */
            op->type = X86_OPTYPE_ABS;
            op->pos = x86_dctx->pos;

            switch(op->size)
            {
                case X86_OPSIZE_DWORD:
                    op->abs.offs.u16 = get_uint16(x86_dctx, -1);
                    break;
                case X86_OPSIZE_FWORD:
                    op->abs.offs.u32 = get_uint32(x86_dctx, -1);
                    break;
                case X86_OPSIZE_TWORD:
                    op->abs.offs.u64 = get_uint64(x86_dctx, -1);
                    break;
                default:
                    break;
            }

            op->abs.seg = get_uint16(x86_dctx, -1);

            break;

        case AM_B:
            /* 
                the VEX.vvvv field of the VEX prefix selects a general 
                purpose register
            */
            if( x86_dctx->pfx_p_vex2b || x86_dctx->pfx_p_vex3b || 
                x86_dctx->pfx_p_evex || x86_dctx->pfx_p_xop )
            {
                j = 0;
                j = (x86_dctx->pfx_p_vex2b ? VEX_2B_VVVV : j);
                j = (x86_dctx->pfx_p_vex3b ? VEX_3B_VVVV : j);
                j = (x86_dctx->pfx_p_evex ? EVEX_VVVV : j);
                j = (x86_dctx->pfx_p_xop ? XOP_VVVV : j);

                return decode_reg(x86_dctx, op, X86_REG_GPR, j);
            }
            else
            {
                dbg_err("VEX/XOP prefix missing!");
                return -1;
            }

        case AM_C:
            /* the reg field of the ModR/M byte selects a control register */
            return decode_modrm_reg(x86_dctx, op, X86_REG_CR0);

        case AM_D:
            /* the reg field of the ModR/M byte selects a debug register */
            return decode_modrm_reg(x86_dctx, op, X86_REG_DR0);

        case AM_E:
            /* 
                a ModR/M byte follows the opcode and specifies the operand. 
                The operand is either a general-purpose register or a memory 
                address. If it is a memory address, the address is computed 
                from a segment register and any of the following values: a 
                base register, an index register, a scaling factor, a 
                displacement
            */
            return decode_modrm_mod_rm(x86_dctx, op, X86_REG_GPR);

        case AM_F:
            /* EFLAGS/RFLAGS register */
            op->type = X86_OPTYPE_NONE;
            break;

        case AM_G:
            /* the reg field of the ModR/M byte selects a general register */
            return decode_modrm_reg(x86_dctx, op, X86_REG_GPR);

        case AM_H:
            /*
                the VEX.vvvv field of the VEX prefix selects a 128-bit XMM 
                register or a 256-bit YMM register, determined by operand 
                type. For legacy SSE encodings this operand does not exist, 
                changing the instruction to destructive form
            */
            if(x86_dctx->pfx_p_vex2b || x86_dctx->pfx_p_vex3b || x86_dctx->pfx_p_xop)
            {
                j = 0;
                j = (x86_dctx->pfx_p_vex2b ? VEX_2B_VVVV : j);
                j = (x86_dctx->pfx_p_vex3b ? VEX_3B_VVVV : j);
                j = (x86_dctx->pfx_p_xop ? XOP_VVVV : j);

                /* operand size defined by VEX.L field */
                x = (op->size == X86_OPSIZE_YWORD ? X86_REG_YMM0 : X86_REG_XMM0);

                return decode_reg(x86_dctx, op, x, j);
            }

            /* legacy encoding: ignore the operand */

            break;

        case AM_I:
            /*
                immediate data: the operand value is encoded in subsequent 
                bytes of the instruction
            */
            return decode_imm(x86_dctx, op);

        case AM_J:
            /*
                the instruction contains a relative offset to be added to the 
                instruction pointer register
            */
            return decode_rel(x86_dctx, op);

        case AM_L:
            /*
                the upper 4 bits of the 8-bit immediate selects a 128-bit XMM 
                register or a 256-bit YMM register, determined by operand 
                type (the MSB is ignored in 32-bit mode)
            */
            j = (get_uint8(x86_dctx, -1) >> 4) & 0x0f;

            /* ignore the MSB in 32-bit mode */
            j = (x86_dctx->dmode == X86_DMODE_64BIT ? j : j & 7);

            /* operand size defined by VEX.L field */
            x = (op->size == X86_OPSIZE_YWORD ? X86_REG_YMM0 : X86_REG_XMM0);

            return decode_reg(x86_dctx, op, x, j);

        case AM_M:
            /*
                the ModR/M byte may refer only to memory (for example, 
                BOUND, LES, LDS, LSS, LFS, LGS, CMPXCHG8B)
            */
            if(MODRM_MOD(get_modrm(x86_dctx)) == 3)
            {
                dbg_err("the ModR/M byte may refer only to memory!");
                return -1;
            }

            return decode_modrm_mod_rm(x86_dctx, op, X86_REG_GPR);

        case AM_N:
            /*
                the R/M field of the ModR/M byte selects a packed-quadword, 
                MMX technology register
            */
            if(MODRM_MOD(get_modrm(x86_dctx)) != 3)
            {
                dbg_err(
                    "the R/M field of the ModR/M byte may refer only "
                    "to a MMX technology register!");

                return -1;
            }

            return decode_modrm_mod_rm(x86_dctx, op, X86_REG_MM0);

        case AM_O:
            /*
                the instruction has no ModR/M byte. The offset of the operand 
                is coded as a word or double word (depending on address size 
                attribute) in the instruction. No base register, index 
                register, or scaling factor can be applied 
            */
            return decode_moffs(x86_dctx, op);

        case AM_P:
            /*
                the reg field of the ModR/M byte selects a packed quadword 
                MMX technology register
            */
            return decode_modrm_reg(x86_dctx, op, X86_REG_MM0);

        case AM_Q:
            /* 
                a ModR/M byte follows the opcode and specifies the operand. 
                The operand is either a MMX technology register or a memory 
                address. If it is a memory address, the address is computed 
                from a segment register and any of the following values: a 
                base register, an index register, a scaling factor, a 
                displacement
            */
            return decode_modrm_mod_rm(x86_dctx, op, X86_REG_MM0);

        case AM_R:
            /*
                the R/M field of the ModR/M byte may refer only to a 
                general register
            */
            if(MODRM_MOD(get_modrm(x86_dctx)) != 3)
            {
                dbg_err(
                    "the R/M field of the ModR/M byte may refer only "
                    "to a general register!");

                return -1;
            }

            return decode_modrm_mod_rm(x86_dctx, op, X86_REG_GPR);

        case AM_S:
            /* the reg field of the ModR/M byte selects a segment register */
            return decode_modrm_reg(x86_dctx, op, X86_REG_ES);

        case AM_U:
            /*
                the R/M field of the ModR/M byte selects a 128-bit XMM 
                register or a 256-bit YMM register, determined by operand 
                type
            */
            if(MODRM_MOD(get_modrm(x86_dctx)) != 3)
            {
                dbg_err(
                    "the R/M field of the ModR/M byte may refer only "
                    "to a XMM or YMM register!");

                return -1;
            }

            /* operand size defined by VEX.L field */
            j = (op->size == X86_OPSIZE_YWORD ? X86_REG_YMM0 : X86_REG_XMM0);

            return decode_modrm_mod_rm(x86_dctx, op, j);

        case AM_V:
            /*
                the reg field of the ModR/M byte selects a 128-bit XMM 
                register or a 256-bit YMM register, determined by operand 
                type
            */

            /* operand size defined by VEX.L field */
            j = (op->size == X86_OPSIZE_YWORD ? X86_REG_YMM0 : X86_REG_XMM0);

            return decode_modrm_reg(x86_dctx, op, j);

        case AM_W:
            /*
                a ModR/M byte follows the opcode and specifies the operand. 
                The operand is either a 128-bit XMM register, a 256-bit YMM 
                register (determined by operand type), or a memory address. 
                If it is a memory address, the address is computed from a 
                segment register and any of the following values: a base 
                register, an index register, a scaling factor, and a 
                displacement
            */

            /* operand size defined by VEX.L field */
            j = (op->size == X86_OPSIZE_YWORD ? X86_REG_YMM0 : X86_REG_XMM0);

            return decode_modrm_mod_rm(x86_dctx, op, j);

        case AM_X:
            /*
                memory addressed by the DS:rSI register pair;
                DS can be overridden by a segment prefix!
            */
            op->type = X86_OPTYPE_MEXPR;
            op->seg = (x86_dctx->pfx_p_seg ? x86_dctx->pfx_seg : X86_REG_DS);

            op->mexpr.index = X86_REG_NONE;
            op->mexpr.disp_size = X86_DISP_NONE;
            op->mexpr.scale = 1;

            switch(x86_dctx->asize)
            {
                case X86_ASIZE_16BIT: op->mexpr.base = X86_REG_SI; break;
                case X86_ASIZE_32BIT: op->mexpr.base = X86_REG_ESI; break;
                case X86_ASIZE_64BIT: op->mexpr.base = X86_REG_RSI; break;
                
                default:
                    break;
            }

            break;

        case AM_Y:
            /*
                memory addressed by the ES:rDI register pair;
                ES cannot be overridden by a segment prefix!
            */
            op->type = X86_OPTYPE_MEXPR;
            op->seg = X86_REG_ES;

            op->mexpr.index = X86_REG_NONE;
            op->mexpr.disp_size = X86_DISP_NONE;
            op->mexpr.scale = 1;

            switch(x86_dctx->asize)
            {
                case X86_ASIZE_16BIT: op->mexpr.base = X86_REG_DI; break;
                case X86_ASIZE_32BIT: op->mexpr.base = X86_REG_EDI; break;
                case X86_ASIZE_64BIT: op->mexpr.base = X86_REG_RDI; break;
                
                default:
                    break;
            }

            break;

        case AM_I1:
            /* implicit immediate data of value 1 */
            op->type = X86_OPTYPE_IMM;
            op->data.u8 = 1;
            break;

        case AM_I3:
            /* implicit immediate data of value 3 */
            op->type = X86_OPTYPE_IMM;
            op->data.u8 = 3;
            break;

        case AM_rAX:
        case AM_rCX:
        case AM_rDX:
        case AM_rBX:
        case AM_rSP:
        case AM_rBP:
        case AM_rSI:
        case AM_rDI:
            /* 
                general-purpose register depending on the operand's size 
                attribute (no REX.B extension) 
            */
            j = te->operands[i].am - AM_rAX;

            return decode_reg(x86_dctx, op, X86_REG_GPR, j);

        case AM_rAX_r8:
        case AM_rCX_r9:
        case AM_rDX_r10:
        case AM_rBX_r11:
        case AM_rSP_r12:
        case AM_rBP_r13:
        case AM_rSI_r14:
        case AM_rDI_r15:
            /* 
                general-purpose register depending on the operand's size 
                attribute (with REX.B extension) 
            */
            j = te->operands[i].am - AM_rAX_r8;

            if(x86_dctx->dmode == X86_DMODE_64BIT && x86_dctx->pfx_p_rex)
            {
                j |= (REX_B << 3);
            }

            return decode_reg(x86_dctx, op, X86_REG_GPR, j);

        case AM_ST0:        
        case AM_ST1:        
        case AM_ST2:    
        case AM_ST3:    
        case AM_ST4:    
        case AM_ST5:    
        case AM_ST6:    
        case AM_ST7:
            /* floating-point registers ST(x) */
            j = te->operands[i].am - AM_ST0;
            
            return decode_reg(x86_dctx, op, X86_REG_ST0, j);

        case AM_ES: 
        case AM_CS:
        case AM_SS: 
        case AM_DS:
        case AM_FS:
        case AM_GS:
            /* segment registers */
            j = te->operands[i].am - AM_ES;
            
            return decode_reg(x86_dctx, op, X86_REG_ES, j);

        default:
            dbg_err("undefined addressing method!");
            return -1;
    }

    return 0;
}

static 
int
decode_fix(x86_dasm_context_t* x86_dctx)
{
    x86_operand_t* op = &x86_dctx->operands[0];

    /* check if the base reg is a stack reg -> def. seg is ss */
    for(int i = 0; i < 4; i++)
    {
        if(op[i].type == X86_OPTYPE_MEXPR)
        {
            if( op[i].mexpr.base == X86_REG_SP || 
                op[i].mexpr.base == X86_REG_BP ||
                op[i].mexpr.base == X86_REG_ESP ||
                op[i].mexpr.base == X86_REG_EBP ||
                op[i].mexpr.base == X86_REG_RSP ||
                op[i].mexpr.base == X86_REG_RBP )
            {
                op[i].seg = X86_REG_SS;
            }
        }
    }

    /* check for mov CR8,reg / mov reg, CR8 using LOCK prefix */
    if(x86_dctx->mnem == X86_MN_MOV && x86_dctx->pfx_p_lock)
    {
        if( op[0].type == X86_OPTYPE_REG && 
            op[1].type == X86_OPTYPE_REG )
        {
            if(op[0].reg >= X86_REG_CR0 && op[0].reg <= X86_REG_CR15)
            {
                op[0].reg = X86_REG_CR8;
            }
            else
            if(op[1].reg >= X86_REG_CR0 && op[1].reg <= X86_REG_CR15)
            {
                op[1].reg = X86_REG_CR8;
            }
        }
    }

    /* fix for mov []. seg */
    if(x86_dctx->mnem == X86_MN_MOV)
    {
        if( op[0].type == X86_OPTYPE_MEXPR && 
            op[1].type == X86_OPTYPE_REG )
        {
            if(op[1].reg >= X86_REG_ES && op[1].reg <= X86_REG_GS)
            {
                op[0].size = X86_OPSIZE_WORD;
            }
        }
    }

    /* fix emms, vzeroupper, vzeroall */
    if( x86_dctx->mnem == X86_MN_EMMS &&
        (x86_dctx->pfx_p_vex2b || x86_dctx->pfx_p_vex3b) )
    {
        if(VEX_2B_L || VEX_3B_L)
        {
            /* 256-bit */
            x86_dctx->mnem = X86_MN_VZEROALL;

#if !defined(X86_DASM_NOFMT) || defined(X86_DASM_DEBUG)

            strcpy(x86_dctx->mnem_str, "zeroall");

#endif

        }
        else
        {
            /* 128-bit */
            x86_dctx->mnem = X86_MN_VZEROUPPER;

#if !defined(X86_DASM_NOFMT) || defined(X86_DASM_DEBUG)

            strcpy(x86_dctx->mnem_str, "zeroupper");

#endif

        }
    }

    /* fix xsaveopt */
    if( x86_dctx->mnem == X86_MN_XSAVEOPT && 
        x86_dctx->dmode == X86_DMODE_64BIT && REX_W )
    {
            x86_dctx->mnem = X86_MN_XSAVEOPT64;

#if !defined(X86_DASM_NOFMT) || defined(X86_DASM_DEBUG)

            strcpy(x86_dctx->mnem_str, "xsaveopt64");

#endif

    }

    /* fix xrstor */
    if( x86_dctx->mnem == X86_MN_XRSTOR && 
        x86_dctx->dmode == X86_DMODE_64BIT && REX_W )
    {
            x86_dctx->mnem = X86_MN_XRSTOR64;

#if !defined(X86_DASM_NOFMT) || defined(X86_DASM_DEBUG)

            strcpy(x86_dctx->mnem_str, "xrstor64");

#endif

    }

    /* fix xsave */
    if( x86_dctx->mnem == X86_MN_XSAVE && 
        x86_dctx->dmode == X86_DMODE_64BIT && REX_W )
    {
            x86_dctx->mnem = X86_MN_XSAVE64;

#if !defined(X86_DASM_NOFMT) || defined(X86_DASM_DEBUG)

            strcpy(x86_dctx->mnem_str, "xsave64");

#endif

    }

    /* fix fxsave */
    if( x86_dctx->mnem == X86_MN_FXSAVE && 
        x86_dctx->dmode == X86_DMODE_64BIT && REX_W )
    {
            x86_dctx->mnem = X86_MN_FXSAVE64;

#if !defined(X86_DASM_NOFMT) || defined(X86_DASM_DEBUG)

            strcpy(x86_dctx->mnem_str, "fxsave64");

#endif

    }

    /* fix fxrstor */
    if( x86_dctx->mnem == X86_MN_FXRSTOR && 
        x86_dctx->dmode == X86_DMODE_64BIT && REX_W )
    {
            x86_dctx->mnem = X86_MN_FXRSTOR64;

#if !defined(X86_DASM_NOFMT) || defined(X86_DASM_DEBUG)

            strcpy(x86_dctx->mnem_str, "fxrstor64");

#endif

    }

    /* fix nop or pause */
    if( x86_dctx->mnem == X86_MN_XCHG &&
        x86_dctx->pfx_p_rex == false &&
        x86_dctx->buffer[x86_dctx->pos_opcode] == 0x90 )
    {
        if(x86_dctx->pfx_p_rep)
        {
            x86_dctx->mnem = X86_MN_PAUSE;

#if !defined(X86_DASM_NOFMT) || defined(X86_DASM_DEBUG)

            strcpy(x86_dctx->mnem_str, "pause");

#endif

        }
        else
        {
            x86_dctx->mnem = X86_MN_NOP;

#if !defined(X86_DASM_NOFMT) || defined(X86_DASM_DEBUG)

            strcpy(x86_dctx->mnem_str, "nop");

#endif

        }

        /* erase the operands */
        op[0].type = X86_OPTYPE_NONE;
        op[1].type = X86_OPTYPE_NONE;
    }

    /* fix immediate sign extensions */
    if(x86_dctx->iset == X86_ISET_GP)
    {
        /* should be valid for any GP instruction */

        int i;
        uint8_t tos = X86_OPSIZE_NONE; /* target operand size */

        if(x86_dctx->mnem == X86_MN_PUSH && op[0].type == X86_OPTYPE_IMM)
        {
            // 6A ib PUSH imm8 
            // 68 iw PUSH imm16 
            // 68 id PUSH imm32 

            /*
                Stack-address size. Outside of 64-bit mode, the B flag in 
                the current stack-segment descriptor determines the size of 
                the stack pointer (16 or 32 bits); in 64-bit mode, the size 
                of the stack pointer is always 64 bits.

                If the source operand is an immediate of size less than the 
                operand size, a sign-extended value is pushed on the stack
            */

            i = 0;

            switch(x86_dctx->osize)
            {
                case X86_OSIZE_16BIT:

                    /* byte */
                    if(op[0].size < X86_OPSIZE_WORD)
                    {
                        op[0].size_orig = op[0].size;
                        op[0].size = tos = X86_OPSIZE_WORD;
                    }
                    break;

                case X86_OSIZE_32BIT:

                    /* byte, word */
                    if(op[0].size < X86_OPSIZE_DWORD)
                    {
                        op[0].size_orig = op[0].size;
                        op[0].size = tos = X86_OPSIZE_DWORD;
                    }
                    break;

                case X86_OSIZE_64BIT:

                    /* byte, word, dword */
                    if(op[0].size < X86_OPSIZE_QWORD)
                    {
                        op[0].size_orig = op[0].size;
                        op[0].size = tos = X86_OPSIZE_QWORD;
                    }
                    break;

                default:
                    break;
            }
        }
        else
        {
            /* loop through the operands and sign-extend imms */
            for(i = 1; i < 4; i++)
            {
                if( op[i].type == X86_OPTYPE_IMM && 
                    op[i].size != op[i-1].size )
                {
                    /* update the op-size */
                    op[i].size_orig = op[i].size;
                    op[i].size = op[i-1].size;

                    tos = op[i-1].size;
                    break;
                }
            }
        }

        /* time to fix */
        if(tos != X86_OPSIZE_NONE)
        {
            switch(tos)
            {
                case X86_OPSIZE_QWORD:

                    switch(op[i].size_orig)
                    {
                        case X86_OPSIZE_DWORD:
                            op[i].data.s64 = op[i].data.s32;
                            break;

                        case X86_OPSIZE_WORD:
                            op[i].data.s64 = op[i].data.s16;
                            break;

                        case X86_OPSIZE_BYTE:
                            op[i].data.s64 = op[i].data.s8;
                            break;

                        default:
                            break;
                    }

                    break;

                case X86_OPSIZE_DWORD:

                    switch(op[i].size_orig)
                    {
                        case X86_OPSIZE_WORD:
                            op[i].data.s32 = op[i].data.s16;
                            break;

                        case X86_OPSIZE_BYTE:
                            op[i].data.s32 = op[i].data.s8;
                            break;

                        default:
                            break;
                    }

                    break;

                case X86_OPSIZE_WORD:

                    switch(op[i].size_orig)
                    {
                        case X86_OPSIZE_BYTE:
                            op[i].data.s16 = op[i].data.s8;
                            break;

                        default:
                            break;
                    }

                    break;

                default:
                    break;
            }
        }
    }

    /* get the EFLAGS */
    x86_dctx->eflags = x86_eflags_get(x86_dctx->mnem);

    return 0;
}

static 
int
decode_operand(x86_dasm_context_t* x86_dctx, const x86_table_entry_t* te, int i)
{
    /* it's necessary to decode the operand size first... */
    decode_opsize(x86_dctx, te, i);

    /* ...and then the operand type */
    return decode_optype(x86_dctx, te, i);
}

static 
void
decode_clear(x86_dasm_context_t* x86_dctx)
{
    x86_operand_t* op = &x86_dctx->operands[0];

    x86_dctx->pos = 0;
    x86_dctx->mnem = X86_MN_INVALID;

    x86_dctx->pos_modrm = 0;
    x86_dctx->pos_sib = 0;
    x86_dctx->pos_rex = 0;
    x86_dctx->pos_opcode = 0;

    x86_dctx->pfx_rex = 0;
    x86_dctx->pfx_last = 0;
    x86_dctx->pfx_seg = 0;
    x86_dctx->pfx_mandatory = 0;

    x86_dctx->pfx_c_osize = 0;
    x86_dctx->pfx_p_rex = false;
    x86_dctx->pfx_p_seg = false;
    x86_dctx->pfx_p_osize = false;
    x86_dctx->pfx_p_asize = false;
    x86_dctx->pfx_p_lock = false;
    x86_dctx->pfx_p_rep = false;
    x86_dctx->pfx_p_repne = false;
    x86_dctx->pfx_p_vex2b = false;
    x86_dctx->pfx_p_vex3b = false;
    x86_dctx->pfx_p_xop = false;
    x86_dctx->pfx_p_evex = false;
    x86_dctx->p_vsib = false;
    x86_dctx->vsib_base = X86_REG_NONE;

    x86_dctx->p_modrm = false;
    x86_dctx->p_sib = false;

    x86_dctx->dest_rip.u64 = 0;

    /* set all the flags to X86_FLAG_NA */
    memset(&x86_dctx->eflags, 0, sizeof(x86_dctx->eflags));

#if !defined(X86_DASM_NOFMT) || defined(X86_DASM_DEBUG)

    memset(x86_dctx->inst_str, 0, sizeof(x86_dctx->inst_str));
    memset(x86_dctx->mnem_str, 0, sizeof(x86_dctx->mnem_str));

#endif

    /* clear the operands */
    for(int i = 0; i < 4; i++)
    {
        memset(&op[i], 0, sizeof(op[i]));

        op[i].type = X86_OPTYPE_NONE;
        op[i].size = X86_OPSIZE_NONE;
        op[i].size_orig = X86_OPSIZE_NONE;
        op[i].asize = X86_ASIZE_NONE;
    }
}

/****************************************************************************

    exported utility functions

****************************************************************************/

void
x86_set_dmode(x86_dasm_context_t* x86_dctx, int dmode)
{
    x86_dctx->dmode = dmode;
}

void
x86_set_ip(x86_dasm_context_t* x86_dctx, uint64_t ip)
{
    x86_dctx->rip.u64 = ip;
}

void
x86_set_buffer(x86_dasm_context_t* x86_dctx, void* code)
{
    /* 16 is max instruction len */
    memcpy(x86_dctx->buffer, code, 16);
}

bool_t
x86_is_jmp(x86_dasm_context_t* x86_dctx)
{
    return (x86_dctx->mnem == X86_MN_JMP || x86_dctx->mnem == X86_MN_JMPF);
}

bool_t
x86_is_jmpcc(x86_dasm_context_t* x86_dctx)
{
    switch(x86_dctx->mnem)
    {
        case X86_MN_JA:
        case X86_MN_JC:
        case X86_MN_JNG:
        case X86_MN_JZ:
        case X86_MN_JG:
        case X86_MN_JNL:
        case X86_MN_JL:
        case X86_MN_JNA:
        case X86_MN_JNC:
        case X86_MN_JNZ:
        case X86_MN_JNO:
        case X86_MN_JNP:
        case X86_MN_JNS:
        case X86_MN_JO:
        case X86_MN_JP:
        case X86_MN_JS:
        case X86_MN_JCXZ:
        case X86_MN_JECXZ:
        case X86_MN_JRCXZ:
            return true;

        default:
            return false;
    }
}

bool_t
x86_is_ret(x86_dasm_context_t* x86_dctx)
{
    switch(x86_dctx->mnem)
    {
        case X86_MN_IRET:
        case X86_MN_IRETW:
        case X86_MN_IRETD:
        case X86_MN_IRETQ:
        case X86_MN_RETN:
        case X86_MN_RETF:
            return true;

        default:
            return false;
    }
}

bool_t
x86_resolve_rip(x86_dasm_context_t* x86_dctx, ulong_t i, uint64_t* out)
{
    x86_operand_t* op = &x86_dctx->operands[i];

    /* check if it's a memory expression */
    if(op->type != X86_OPTYPE_MEXPR)
    {
        return false;
    }

    if(op->mexpr.base != X86_REG_RIP && op->mexpr.base != X86_REG_EIP)
    {
        return false;
    }

    /* compute the address */
    *out = x86_dctx->rip.u64 + x86_dctx->len + op->mexpr.disp.s32;

    if(op->mexpr.base == X86_REG_EIP)
    {
        /* truncate and zero extend */
        *out &= BIT_MASK(uint64_t, 32);
    }

    return true;
}

static uint64_t
x86_resolve_reg(uint8_t reg, x86_regs_t* regs)
{
    int i = 0;

    uint64_t result;
    uint64_t mask;

    /* build mask and get index */
    if(reg >= X86_REG_RAX && reg <= X86_REG_RIP)
    {
        mask = BIT_MASK(uint64_t, 64);

        /* compute register index */
        i = reg - X86_REG_RAX;
    }
    else
    if(reg >= X86_REG_EAX && reg <= X86_REG_EIP)
    {
        mask = BIT_MASK(uint64_t, 32);

        /* compute register index */
        i = reg - X86_REG_EAX;
    }
    else
    if(reg >= X86_REG_AX && reg <= X86_REG_R15W)
    {
        mask = BIT_MASK(uint64_t, 16);

        /* compute register index */
        i = reg - X86_REG_AX;
    }
    else
    if(reg >= X86_REG_AL && reg <= X86_REG_R15L)
    {
        mask = BIT_MASK(uint64_t, 8);

        /* compute register index */
        i = reg - X86_REG_AL;

        if(reg > X86_REG_BH)
        {
            i -= 4;
        }
    }
    else
    {
        mask = 0;
    }

    /* get the reg value */
    result = (*((x86_reg *)regs + i)).u64;
    result &= mask;

    return result;
}

uint64_t
x86_resolve_op(x86_dasm_context_t* x86_dctx, x86_regs_t* regs, ulong_t i)
{
    uint64_t result;

    x86_operand_t* op = &x86_dctx->operands[i];

    /* init */
    result = 0;

    /* memory expression */
    if(op->type == X86_OPTYPE_MEXPR)
    {
        result = *((uint64_t *)&regs->es + (op->seg - X86_REG_ES));

        /* check if there is a base register */
        if(op->mexpr.base != X86_REG_NONE)
        {
            result += x86_resolve_reg(op->mexpr.base, regs);
        }

        /* check if there is an index register */
        if(op->mexpr.index != X86_REG_NONE)
        {
            result += (x86_resolve_reg(op->mexpr.index, regs) * op->mexpr.scale);
        }

        /* check if there is a displacement */
        if(op->mexpr.disp_size != X86_DISP_NONE)
        {
            switch(op->mexpr.disp_size)
            {
                case X86_DISP_8: 
                    result += op->mexpr.disp.s8;
                    break;
                case X86_DISP_16:
                    result += op->mexpr.disp.s16;
                    break;
                case X86_DISP_32:
                    result += op->mexpr.disp.s32;
                    break;
                default: 
                    break;
            }
        }
    }
    else
    /* memory offset */
    if(op->type == X86_OPTYPE_MOFFS)
    {
        result = *((uint64_t *)&regs->es + (op->seg - X86_REG_ES));

        switch(op->asize)
        {
            case X86_ASIZE_WORD:
                result += op->data.u16; 
                break;
            case X86_ASIZE_DWORD:
                result += op->data.u32; 
                break;
            case X86_ASIZE_QWORD:
                result += op->data.u64; 
                break;
            default: 
                dbg_msg("invalid moffs asize!");
                break;
        }
    }
    else
    /* register */
    if(op->type == X86_OPTYPE_REG)
    {
        result += x86_resolve_reg(op->reg, regs);
    }
    else
    /* relative address */
    if(op->type == X86_OPTYPE_REL)
    {
        result = x86_dctx->dest_rip.u64;
    }
    else
    /* immediate */
    if(op->type == X86_OPTYPE_IMM)
    {
        switch(op->size)
        {
            case X86_OPSIZE_BYTE: 
                result = op->data.u8; 
                break;
            case X86_OPSIZE_WORD:
                result = op->data.u16; 
                break;
            case X86_OPSIZE_DWORD:
                result = op->data.u32; 
                break;
            case X86_OPSIZE_QWORD:
                result = op->data.u64; 
                break;
            default: 
                break;
        }
    }

    return result;
}

/****************************************************************************

    formatting functions

****************************************************************************/

#if !defined(X86_DASM_NOFMT) || defined(X86_DASM_DEBUG)

static 
void
format_operand(x86_dasm_context_t* x86_dctx, int i)
{
    char tmp[32], *fmt;

    int64_t j = 0;
    uint64_t u = 0;
    
    x86_operand_t* op = &x86_dctx->operands[i];

    dbg_msg("type=%i", op->type);

    /* clear the tmp buffer */
    memset(tmp, 0, sizeof(tmp));

    switch(op->type)
    {
        case X86_OPTYPE_REG:
            /* copy the register name */
            strcat(op->str, regs_str[op->reg]);
            break;

        case X86_OPTYPE_IMM:

            switch(op->size)
            {
                case X86_OPSIZE_BYTE: 
                    fmt = "%" PRIX8; u = op->data.u8; 
                    break;
                case X86_OPSIZE_WORD:
                    fmt = "%" PRIX16; u = op->data.u16; 
                    break;
                case X86_OPSIZE_DWORD:
                    fmt = "%" PRIX32; u = op->data.u32; 
                    break;
                case X86_OPSIZE_QWORD:
                    fmt = "%" PRIX64; u = op->data.u64; 
                    break;
                default: 
                    fmt = "%" PRIX32;
                    break;
            }

            /* format the string and append it */
            sprintf(tmp, fmt, u);

            if( (tmp[0] >= 'A' && tmp[0] <= 'F') ||
                (tmp[0] >= 'a' && tmp[0] <= 'f') )
            {
                strcat(op->str, "0");
            }

            strcat(op->str, tmp);
            break;

        case X86_OPTYPE_MEXPR:

            /* far jmp and call */
            if( x86_dctx->mnem == X86_MN_JMPF ||
                x86_dctx->mnem == X86_MN_CALLF )
            {
                strcat(op->str, "far ");
            }

            /* format segment prefix */
            if(op->size != X86_OPSIZE_NONE)
            {
                sprintf(tmp, "%s ptr %s:", 
                    get_opsize_str(op->size), regs_str[op->seg]);
            }
            else
            {
                sprintf(tmp, "%s:", regs_str[op->seg]);
            }
            
            strcat(op->str, tmp);
            strcat(op->str, "[");

            /* check if there is a base register */
            if(op->mexpr.base != X86_REG_NONE)
            {
                strcat(op->str, regs_str[op->mexpr.base]);
            }

            /* check if there is an index register */
            if(op->mexpr.index != X86_REG_NONE)
            {
                if(op->mexpr.base != X86_REG_NONE)
                {
                    strcat(op->str, "+");
                }

                strcat(op->str, regs_str[op->mexpr.index]);

                if(op->mexpr.scale > 1)
                {
                    strcat(op->str, "*");

                    if(op->mexpr.scale == 2) strcat(op->str, "2");
                    if(op->mexpr.scale == 4) strcat(op->str, "4");
                    if(op->mexpr.scale == 8) strcat(op->str, "8");
                }
            }

            /* check if there is a displacement */
            if(op->mexpr.disp_size != X86_DISP_NONE)
            {
                switch(op->mexpr.disp_size)
                {
                    case X86_DISP_8: 
                        j = op->mexpr.disp.s8;
                        fmt = (j < 0 ? "-%" PRIX8 : "+%" PRIX8); 
                        break;
                    case X86_DISP_16:
                        j = op->mexpr.disp.s16;
                        fmt = (j < 0 ? "-%" PRIX16 : "+%" PRIX16);
                        break;
                    case X86_DISP_32:
                        j = op->mexpr.disp.s32;
                        fmt = (j < 0 ? "-%" PRIX32 : "+%" PRIX32);
                        break;
                    default: 
                        break;
                }

                /* check if there is only disp */
                if( op->mexpr.base == X86_REG_NONE && 
                    op->mexpr.index == X86_REG_NONE )
                {
                    /* skip the sign */
                    fmt++;
                    
                    if(j == 0) 
                    {
                        /* example: mov dword ptr fs:[0], eax */
                        strcpy(tmp, "0");
                    }
                    else 
                    {
                        sprintf(tmp, fmt, j);
                    }

                    if( (tmp[0] >= 'A' && tmp[0] <= 'F') ||
                        (tmp[0] >= 'a' && tmp[0] <= 'f') )
                    {
                        strcat(op->str, "0");
                    }

                    strcat(op->str, tmp);
                }
                else
                {
                    if(j != 0)
                    {
                        sprintf(tmp, fmt, (j < 0 ? -j : j));

                        if( (tmp[1] >= 'A' && tmp[1] <= 'F') ||
                            (tmp[1] >= 'a' && tmp[1] <= 'f') )
                        {
                            strcat(op->str, (j < 0 ? "-0" : "+0"));
                            strcat(op->str, &tmp[1]);
                        }
                        else
                        {
                            strcat(op->str, tmp);
                        }
                    }   
                }
            }

            strcat(op->str, "]");
            break;

        case X86_OPTYPE_ABS:

            /* copy the code segment */
            sprintf(op->str, "far %" PRIX16 ":", op->abs.seg);

            switch(op->size)
            {
                case X86_OPSIZE_DWORD: 
                    u = op->abs.offs.u16; fmt = "%" PRIX16; 
                    break;
                case X86_OPSIZE_FWORD:
                    u = op->abs.offs.u32; fmt = "%" PRIX32;
                    break;
                case X86_OPSIZE_TWORD:
                    u = op->abs.offs.u64; fmt = "%" PRIX64;
                    break;
                default: 
                    fmt = "%" PRIX32;
                    break;
            }

            /* format the string and append it */
            sprintf(tmp, fmt, u);

            if( (tmp[0] >= 'A' && tmp[0] <= 'F') ||
                (tmp[0] >= 'a' && tmp[0] <= 'f') )
            {
                strcat(op->str, "0");
            }

            strcat(op->str, tmp);
            break;

        case X86_OPTYPE_MOFFS:

            /* format segment prefix */
            if(op->size != X86_OPSIZE_NONE)
            {
                sprintf(tmp, "%s ptr %s:", 
                    get_opsize_str(op->size), regs_str[op->seg]);
            }
            else
            {
                sprintf(tmp, "%s:", regs_str[op->seg]);
            }
            
            strcat(op->str, tmp);
            strcat(op->str, "[");

            switch(op->asize)
            {
                case X86_ASIZE_WORD:
                    fmt = "%" PRIX16; u = op->data.u16; 
                    break;
                case X86_ASIZE_DWORD:
                    fmt = "%" PRIX32; u = op->data.u32; 
                    break;
                case X86_ASIZE_QWORD:
                    fmt = "%" PRIX64; u = op->data.u64; 
                    break;
                default: 
                    fmt = "%" PRIX32;
                    break;
            }

            /* format the string and append it */
            sprintf(tmp, fmt, u);

            if( (tmp[0] >= 'A' && tmp[0] <= 'F') ||
                (tmp[0] >= 'a' && tmp[0] <= 'f') )
            {
                strcat(op->str, "0");
            }

            strcat(op->str, tmp);
            strcat(op->str, "]");
            break;

        case X86_OPTYPE_REL:

            sprintf(tmp, "%s %" PRIX64, 
                (op->size == X86_OPSIZE_BYTE ? "short" : "near"), 
                x86_dctx->dest_rip.u64);

            strcat(op->str, tmp);
            break;

        default:
            break;
    }
}

static 
int
format_instruction(x86_dasm_context_t* x86_dctx, const x86_table_entry_t* te)
{
    char tmp[128];

    /* clear the tmp buffer */
    memset(tmp, 0, sizeof(tmp));

    /* check if the instruction is lock-able */
    if(x86_dctx->pfx_p_lock && te->properties & PF_LOCK)
    {
        strcat(tmp, "lock ");
    }
    else
    if(x86_dctx->pfx_p_rep && te->properties & PF_REP)
    {
        strcat(tmp, "rep ");
    }
    else
    if(x86_dctx->pfx_p_rep && te->properties & PF_REPE_REPNE)
    {
        strcat(tmp, "repe ");
    }
    else
    if(x86_dctx->pfx_p_repne && te->properties & PF_REPE_REPNE)
    {
        strcat(tmp, "repne ");
    }

    /* if there is a VEX prefix then add 'v' before the mnemonic */
    if(x86_dctx->pfx_p_vex2b || x86_dctx->pfx_p_vex3b)
    {
        if( x86_dctx->iset != X86_ISET_BMI2 && 
            x86_dctx->iset != X86_ISET_BMI1 )
        {
            strcat(tmp, "v");
        }
    }

    /* fix condition codes */
    if(te->properties & PF_CC || te->properties & PF_XOPCC)
    {
        char* p;
        char* s = NULL;
        uint8_t j = 0;
        size_t t = 0;

        char* cc_str[] = 
        {
            /* SSE and AVX */
            "eq", "lt", "le", "unord", "neq", "nlt", "nle", "ord",

            /* AVX */
            "eq_uq", "nge", "ngt", "false", "neq_oq", "ge", "gt",
            "true", "eq_os", "lt_oq", "le_oq", "unord_s", "neq_us",
            "nlt_uq", "ord_s", "eq_us", "nge_uq", "ngt_uq", "false_os",
            "neq_os", "ge_oq", "gt_oq", "true_us"
        };

        char* xopcc_str[] = 
        {
            "lt", "le", "gt", "ge", "eq", "neq", "false", "true"
        };

        /* get the last operand (immediate) */
        for(int i = 0; i < 4; i++)
        {
            if(x86_dctx->operands[i].type == X86_OPTYPE_IMM)
            {
                j = x86_dctx->operands[i].data.u8;
            }
        }

        /* get the wildcard position */
        p = strchr(x86_dctx->mnem_str, '*');
        t = p - x86_dctx->mnem_str;

        if(te->properties & PF_CC)
        {
            if(!(x86_dctx->pfx_p_vex2b || x86_dctx->pfx_p_vex3b))
            {
                j &= 7;
            }

            if(j < countof(cc_str))
            {
                s = cc_str[j];
            }
            else
            {
                dbg_err("Invalid condition code (SSE/AVX)");
                return -1;
            }
        }
        else
        if(te->properties & PF_XOPCC)
        {
            if(j < countof(xopcc_str))
            {
                s = xopcc_str[j];
            }
            else
            {
                dbg_err("Invalid condition code (XOP)");
                return -1;
            }
        }

        strncat(tmp, x86_dctx->mnem_str, t);
        strcat(tmp, s);
        strcat(tmp, p + 1);
    }
    else
    {
        /* copy the instruction mnemonic */
        strcat(tmp, x86_dctx->mnem_str);
    }

    /* append space only if there are operands */
    if(x86_dctx->operands[0].type != X86_OPTYPE_NONE)
    {
        strcat(tmp, " ");
    }

    /* append the formatted operands */
    for(int i = 0; i < 4; i++)
    {
        if(x86_dctx->operands[i].type != X86_OPTYPE_NONE)
        {
            if( x86_dctx->operands[i].type == X86_OPTYPE_IMM && 
                (te->properties & PF_CC || te->properties & PF_XOPCC) )
            {
                /* ignore the condition code immediate */
            }
            else
            {
                if(i > 0)
                {
                    strcat(tmp, ", ");
                }

                strcat(tmp, x86_dctx->operands[i].str);
            }
        }
    }

    strcpy(x86_dctx->inst_str, tmp);

    return 0;
}

#endif

/****************************************************************************

    entrypoint

****************************************************************************/

/*

    x86-64 legacy instruction format:

         #       0,1   1,2,3      0,1    0,1   0,1,2,4    0,1,2,4
    +----------+-----+--------+--------+-----+----------+---------+
    | prefixes | REX | OPCODE | MODR/M | SIB |   DISP   |   IMM   |
    +----------+-----+--------+--------+-----+----------+---------+

    note: in 64-bit mode, in some cases disp or imm can be 8 bytes
    (for example check mov: A0-A3 and B8)


    VEX/XOP instruction format:
        
         #          2,3          1        1      0,1   0,1,2,4    0,1
    +----------+-------------+--------+--------+-----+----------+-----+
    | SEG, 67h | VEX2B/VEX3B | OPCODE | MODR/M | SIB |   DISP   | IMM |
    +----------+-------------+--------+--------+-----+----------+-----+

    
    
    3DNow! instruction format:

         2            1          1      0,1,2,4           1
    +----------+-------------+--------+----------+----------------+
    | 0Fh 0Fh  |   MODR/M    |  SIB   |   DISP   | 3DNow!_suffix  |
    +----------+-------------+--------+----------+----------------+

*/

int
x86_dasm(x86_dasm_context_t* x86_dctx)
{
    const x86_table_entry_t* te;

    /* clear the dasm context */
    decode_clear(x86_dctx);

    /* function parses the prefixes (until opcode, excluded) */
    if(decode_prefixes(x86_dctx) < 0)
    {
        dbg_err("failed to decode the prefixes!");
        return -1;
    }

    /* determine the addressing size */
    get_asize(x86_dctx);

    /* determine the operand size */
    get_osize(x86_dctx, 0);

    /* function parses the opcode(s) */
    if(decode_opcode(x86_dctx, &te) < 0)
    {
        dbg_err("failed to decode the opcode!");
        return -1;
    }

    /* validate the instruction */

    if(te->mnem_id == X86_MN_INVALID)
    {
        dbg_err("invalid opcode!");
        return -1;
    }

    if(x86_dctx->dmode == X86_DMODE_64BIT && te->properties & PF_I64)
    {
        dbg_err("opcode is invalid in 64-bit mode!");
        return -1;
    }

    if(x86_dctx->dmode != X86_DMODE_64BIT && te->properties & PF_O64)
    {
        dbg_err("opcode is only valid in 64-bit mode!");
        return -1;
    }

    if( !x86_dctx->pfx_p_vex2b && 
        !x86_dctx->pfx_p_vex3b && te->properties & PF_OVEX )
    {
        dbg_err("opcode requires a VEX prefix!");
        return -1;
    }

    if(x86_dctx->pfx_p_vex2b || x86_dctx->pfx_p_vex3b) 
    {
        uint8_t j = 0;

        j = (x86_dctx->pfx_p_vex2b ? VEX_2B_L : j);
        j = (x86_dctx->pfx_p_vex3b ? VEX_3B_L : j);

        if( (te->properties & PF_VEX128 && j) ||
            (te->properties & PF_VEX256 && !j) )
        {
            dbg_err("invalid vector length!");
            return -1;
        }
    }

    /* calculate the operand size */
    get_osize(x86_dctx, te->properties);

    /* decode the operands (currently max 4) */
    for(int i = 0; i < 4; i++)
    {
        if(decode_operand(x86_dctx, te, i) < 0)
        {
            return -1;
        }
    }

    /* apply some fixups if needed */
    decode_fix(x86_dctx);

    /* check for max instruction length */
    if(x86_dctx->pos > MAX_INST_LENGTH)
    {
        dbg_err("instruction's size exceeded the limit!");
        return -1;
    }

#if !defined(X86_DASM_NOFMT) || defined(X86_DASM_DEBUG)

    /* format the operands (currently max 4) */
    for(int i = 0; i < 4; i++)
    {
        format_operand(x86_dctx, i);
    }

    if(format_instruction(x86_dctx, te) < 0)
        return -1;

#endif
    
    return 0;
}

/*

    This file is part of x86_dasm.

    x86_dasm is a compact x86-64 disassembling library
    
    Copyright 2014 / the`janitor / < email: base64dec(dGhlLmphbml0b3JAcHJvdG9ubWFpbC5jb20=) >

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

        http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.

*/

/****************************************************************************

    One-byte tables

****************************************************************************/

TABLE_BEGIN(1B)

    /* 00h */
    TABLE_ENTRY("add",  X86_MN_ADD,     PF_LOCK,    X86_ISET_GP,    OP(Eb, Gb)),
    TABLE_ENTRY("add",  X86_MN_ADD,     PF_LOCK,    X86_ISET_GP,    OP(Ev, Gv)),
    TABLE_ENTRY("add",  X86_MN_ADD,     PF_NONE,    X86_ISET_GP,    OP(Gb, Eb)),
    TABLE_ENTRY("add",  X86_MN_ADD,     PF_NONE,    X86_ISET_GP,    OP(Gv, Ev)),
    TABLE_ENTRY("add",  X86_MN_ADD,     PF_NONE,    X86_ISET_GP,    OP(AL, Ib)),
    TABLE_ENTRY("add",  X86_MN_ADD,     PF_NONE,    X86_ISET_GP,    OP(rAX, Iz)),
    TABLE_ENTRY("push", X86_MN_PUSH,    PF_I64,     X86_ISET_GP,    OP(ES)),
    TABLE_ENTRY("pop",  X86_MN_POP,     PF_I64,     X86_ISET_GP,    OP(ES)),
    
    TABLE_ENTRY("or",   X86_MN_OR,      PF_LOCK,    X86_ISET_GP,    OP(Eb, Gb)),
    TABLE_ENTRY("or",   X86_MN_OR,      PF_LOCK,    X86_ISET_GP,    OP(Ev, Gv)),
    TABLE_ENTRY("or",   X86_MN_OR,      PF_NONE,    X86_ISET_GP,    OP(Gb, Eb)),
    TABLE_ENTRY("or",   X86_MN_OR,      PF_NONE,    X86_ISET_GP,    OP(Gv, Ev)),
    TABLE_ENTRY("or",   X86_MN_OR,      PF_NONE,    X86_ISET_GP,    OP(AL, Ib)),
    TABLE_ENTRY("or",   X86_MN_OR,      PF_NONE,    X86_ISET_GP,    OP(rAX, Iz)),
    TABLE_ENTRY("push", X86_MN_PUSH,    PF_I64,     X86_ISET_GP,    OP(CS)),

    /* 0Fh: two-byte table escape */
    TABLE_ENTRY_INVALID(),

    /* 10h */
    TABLE_ENTRY("adc",  X86_MN_ADC,     PF_LOCK,    X86_ISET_GP,    OP(Eb, Gb)),
    TABLE_ENTRY("adc",  X86_MN_ADC,     PF_LOCK,    X86_ISET_GP,    OP(Ev, Gv)),
    TABLE_ENTRY("adc",  X86_MN_ADC,     PF_NONE,    X86_ISET_GP,    OP(Gb, Eb)),
    TABLE_ENTRY("adc",  X86_MN_ADC,     PF_NONE,    X86_ISET_GP,    OP(Gv, Ev)),
    TABLE_ENTRY("adc",  X86_MN_ADC,     PF_NONE,    X86_ISET_GP,    OP(AL, Ib)),
    TABLE_ENTRY("adc",  X86_MN_ADC,     PF_NONE,    X86_ISET_GP,    OP(rAX, Iz)),
    TABLE_ENTRY("push", X86_MN_PUSH,    PF_I64,     X86_ISET_GP,    OP(SS)),
    TABLE_ENTRY("pop",  X86_MN_POP,     PF_I64,     X86_ISET_GP,    OP(SS)),
    
    TABLE_ENTRY("sbb",  X86_MN_SBB,     PF_LOCK,    X86_ISET_GP,    OP(Eb, Gb)),
    TABLE_ENTRY("sbb",  X86_MN_SBB,     PF_LOCK,    X86_ISET_GP,    OP(Ev, Gv)),
    TABLE_ENTRY("sbb",  X86_MN_SBB,     PF_NONE,    X86_ISET_GP,    OP(Gb, Eb)),
    TABLE_ENTRY("sbb",  X86_MN_SBB,     PF_NONE,    X86_ISET_GP,    OP(Gv, Ev)),
    TABLE_ENTRY("sbb",  X86_MN_SBB,     PF_NONE,    X86_ISET_GP,    OP(AL, Ib)),
    TABLE_ENTRY("sbb",  X86_MN_SBB,     PF_NONE,    X86_ISET_GP,    OP(rAX, Iz)),
    TABLE_ENTRY("push", X86_MN_PUSH,    PF_I64,     X86_ISET_GP,    OP(DS)),
    TABLE_ENTRY("pop",  X86_MN_POP,     PF_I64,     X86_ISET_GP,    OP(DS)),

    /* 20h */
    TABLE_ENTRY("and",  X86_MN_AND,     PF_LOCK,    X86_ISET_GP,    OP(Eb, Gb)),
    TABLE_ENTRY("and",  X86_MN_AND,     PF_LOCK,    X86_ISET_GP,    OP(Ev, Gv)),
    TABLE_ENTRY("and",  X86_MN_AND,     PF_NONE,    X86_ISET_GP,    OP(Gb, Eb)),
    TABLE_ENTRY("and",  X86_MN_AND,     PF_NONE,    X86_ISET_GP,    OP(Gv, Ev)),
    TABLE_ENTRY("and",  X86_MN_AND,     PF_NONE,    X86_ISET_GP,    OP(AL, Ib)),
    TABLE_ENTRY("and",  X86_MN_AND,     PF_NONE,    X86_ISET_GP,    OP(rAX, Iz)),
    
    /* 26h: prefix (prefixes should have already been parsed) */
    TABLE_ENTRY_INVALID(),

    TABLE_ENTRY("daa",  X86_MN_DAA,     PF_I64,     X86_ISET_GP,    OP(NONE)),
    TABLE_ENTRY("sub",  X86_MN_SUB,     PF_LOCK,    X86_ISET_GP,    OP(Eb, Gb)),
    TABLE_ENTRY("sub",  X86_MN_SUB,     PF_LOCK,    X86_ISET_GP,    OP(Ev, Gv)),
    TABLE_ENTRY("sub",  X86_MN_SUB,     PF_NONE,    X86_ISET_GP,    OP(Gb, Eb)),
    TABLE_ENTRY("sub",  X86_MN_SUB,     PF_NONE,    X86_ISET_GP,    OP(Gv, Ev)),
    TABLE_ENTRY("sub",  X86_MN_SUB,     PF_NONE,    X86_ISET_GP,    OP(AL, Ib)),
    TABLE_ENTRY("sub",  X86_MN_SUB,     PF_NONE,    X86_ISET_GP,    OP(rAX, Iz)),

    /* 2Eh: prefix (prefixes should have already been parsed) */
    TABLE_ENTRY_INVALID(),

    TABLE_ENTRY("das",  X86_MN_DAS,     PF_I64,     X86_ISET_GP,    OP(NONE)),

    /* 30h */
    TABLE_ENTRY("xor",  X86_MN_XOR,     PF_LOCK,    X86_ISET_GP,    OP(Eb, Gb)),
    TABLE_ENTRY("xor",  X86_MN_XOR,     PF_LOCK,    X86_ISET_GP,    OP(Ev, Gv)),
    TABLE_ENTRY("xor",  X86_MN_XOR,     PF_NONE,    X86_ISET_GP,    OP(Gb, Eb)),
    TABLE_ENTRY("xor",  X86_MN_XOR,     PF_NONE,    X86_ISET_GP,    OP(Gv, Ev)),
    TABLE_ENTRY("xor",  X86_MN_XOR,     PF_NONE,    X86_ISET_GP,    OP(AL, Ib)),
    TABLE_ENTRY("xor",  X86_MN_XOR,     PF_NONE,    X86_ISET_GP,    OP(rAX, Iz)),

    /* 36h: prefix (prefixes should have already been parsed) */
    TABLE_ENTRY_INVALID(),

    TABLE_ENTRY("aaa",  X86_MN_AAA,     PF_I64,     X86_ISET_GP,    OP(NONE)),
    TABLE_ENTRY("cmp",  X86_MN_CMP,     PF_LOCK,    X86_ISET_GP,    OP(Eb, Gb)),
    TABLE_ENTRY("cmp",  X86_MN_CMP,     PF_LOCK,    X86_ISET_GP,    OP(Ev, Gv)),
    TABLE_ENTRY("cmp",  X86_MN_CMP,     PF_NONE,    X86_ISET_GP,    OP(Gb, Eb)),
    TABLE_ENTRY("cmp",  X86_MN_CMP,     PF_NONE,    X86_ISET_GP,    OP(Gv, Ev)),
    TABLE_ENTRY("cmp",  X86_MN_CMP,     PF_NONE,    X86_ISET_GP,    OP(AL, Ib)),
    TABLE_ENTRY("cmp",  X86_MN_CMP,     PF_NONE,    X86_ISET_GP,    OP(rAX, Iz)),

    /* 3Eh: prefix (prefixes should have already been parsed) */
    TABLE_ENTRY_INVALID(),

    TABLE_ENTRY("aas",  X86_MN_AAS,     PF_I64,     X86_ISET_GP,    OP(NONE)),

    /* 40h: rex in 64-bit mode should have already been taken care of */
    TABLE_ENTRY("inc",  X86_MN_INC,     PF_I64,     X86_ISET_GP,    OP(eAX)),
    TABLE_ENTRY("inc",  X86_MN_INC,     PF_I64,     X86_ISET_GP,    OP(eCX)),
    TABLE_ENTRY("inc",  X86_MN_INC,     PF_I64,     X86_ISET_GP,    OP(eDX)),
    TABLE_ENTRY("inc",  X86_MN_INC,     PF_I64,     X86_ISET_GP,    OP(eBX)),
    TABLE_ENTRY("inc",  X86_MN_INC,     PF_I64,     X86_ISET_GP,    OP(eSP)),
    TABLE_ENTRY("inc",  X86_MN_INC,     PF_I64,     X86_ISET_GP,    OP(eBP)),
    TABLE_ENTRY("inc",  X86_MN_INC,     PF_I64,     X86_ISET_GP,    OP(eSI)),
    TABLE_ENTRY("inc",  X86_MN_INC,     PF_I64,     X86_ISET_GP,    OP(eDI)),
    TABLE_ENTRY("dec",  X86_MN_DEC,     PF_I64,     X86_ISET_GP,    OP(eAX)),
    TABLE_ENTRY("dec",  X86_MN_DEC,     PF_I64,     X86_ISET_GP,    OP(eCX)),
    TABLE_ENTRY("dec",  X86_MN_DEC,     PF_I64,     X86_ISET_GP,    OP(eDX)),
    TABLE_ENTRY("dec",  X86_MN_DEC,     PF_I64,     X86_ISET_GP,    OP(eBX)),
    TABLE_ENTRY("dec",  X86_MN_DEC,     PF_I64,     X86_ISET_GP,    OP(eSP)),
    TABLE_ENTRY("dec",  X86_MN_DEC,     PF_I64,     X86_ISET_GP,    OP(eBP)),
    TABLE_ENTRY("dec",  X86_MN_DEC,     PF_I64,     X86_ISET_GP,    OP(eSI)),
    TABLE_ENTRY("dec",  X86_MN_DEC,     PF_I64,     X86_ISET_GP,    OP(eDI)),

    /* 50h */
    TABLE_ENTRY("push", X86_MN_PUSH,    PF_D64,     X86_ISET_GP,    OP(rAX_r8)),
    TABLE_ENTRY("push", X86_MN_PUSH,    PF_D64,     X86_ISET_GP,    OP(rCX_r9)),
    TABLE_ENTRY("push", X86_MN_PUSH,    PF_D64,     X86_ISET_GP,    OP(rDX_r10)),
    TABLE_ENTRY("push", X86_MN_PUSH,    PF_D64,     X86_ISET_GP,    OP(rBX_r11)),
    TABLE_ENTRY("push", X86_MN_PUSH,    PF_D64,     X86_ISET_GP,    OP(rSP_r12)),
    TABLE_ENTRY("push", X86_MN_PUSH,    PF_D64,     X86_ISET_GP,    OP(rBP_r13)),
    TABLE_ENTRY("push", X86_MN_PUSH,    PF_D64,     X86_ISET_GP,    OP(rSI_r14)),
    TABLE_ENTRY("push", X86_MN_PUSH,    PF_D64,     X86_ISET_GP,    OP(rDI_r15)),
    TABLE_ENTRY("pop",  X86_MN_POP,     PF_D64,     X86_ISET_GP,    OP(rAX_r8)),
    TABLE_ENTRY("pop",  X86_MN_POP,     PF_D64,     X86_ISET_GP,    OP(rCX_r9)),
    TABLE_ENTRY("pop",  X86_MN_POP,     PF_D64,     X86_ISET_GP,    OP(rDX_r10)),
    TABLE_ENTRY("pop",  X86_MN_POP,     PF_D64,     X86_ISET_GP,    OP(rBX_r11)),
    TABLE_ENTRY("pop",  X86_MN_POP,     PF_D64,     X86_ISET_GP,    OP(rSP_r12)),
    TABLE_ENTRY("pop",  X86_MN_POP,     PF_D64,     X86_ISET_GP,    OP(rBP_r13)),
    TABLE_ENTRY("pop",  X86_MN_POP,     PF_D64,     X86_ISET_GP,    OP(rSI_r14)),
    TABLE_ENTRY("pop",  X86_MN_POP,     PF_D64,     X86_ISET_GP,    OP(rDI_r15)),

    /* 60h - 61h: pusha/pushad and popa/popad depending on operand size */
    TABLE_ENTRY_ESCAPE(60),
    TABLE_ENTRY_ESCAPE(61),

    TABLE_ENTRY("bound", X86_MN_BOUND,  PF_I64,     X86_ISET_GP,    OP(Gv, Ma)),

    /* 63h: ARPL I64 and MOVSXD O64 */
    TABLE_ENTRY_ESCAPE(63),

    /* 64h - 67h: prefix (prefixes should have already been parsed) */
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    /* 68h */
    TABLE_ENTRY("push", X86_MN_PUSH,    PF_D64,     X86_ISET_GP,    OP(Iz)),
    TABLE_ENTRY("imul", X86_MN_IMUL,    PF_NONE,    X86_ISET_GP,    OP(Gv, Ev, Iz)),
    TABLE_ENTRY("push", X86_MN_PUSH,    PF_D64,     X86_ISET_GP,    OP(Ib)),
    TABLE_ENTRY("imul", X86_MN_IMUL,    PF_NONE,    X86_ISET_GP,    OP(Gv, Ev, Ib)),
    TABLE_ENTRY("insb", X86_MN_INSB,    PF_REP,     X86_ISET_GP,    OP(Yb, DX)),

    /* 6Dh: insw/insd depending on operand size */
    TABLE_ENTRY_ESCAPE(6D),

    /* 6Eh */
    TABLE_ENTRY("outsb",    X86_MN_OUTSB,   PF_REP, X86_ISET_GP,    OP(DX, Xb)),

    /* 6Fh: outsw/outsd depending on operand size */
    TABLE_ENTRY_ESCAPE(6F),

    /* 70h */
    TABLE_ENTRY("jo",   X86_MN_JO,      PF_NONE,    X86_ISET_GP,    OP(Jb)),
    TABLE_ENTRY("jno",  X86_MN_JNO,     PF_NONE,    X86_ISET_GP,    OP(Jb)),
    TABLE_ENTRY("jc",   X86_MN_JC,      PF_NONE,    X86_ISET_GP,    OP(Jb)),
    TABLE_ENTRY("jnc",  X86_MN_JNC,     PF_NONE,    X86_ISET_GP,    OP(Jb)),
    TABLE_ENTRY("jz",   X86_MN_JZ,      PF_NONE,    X86_ISET_GP,    OP(Jb)),
    TABLE_ENTRY("jnz",  X86_MN_JNZ,     PF_NONE,    X86_ISET_GP,    OP(Jb)),
    TABLE_ENTRY("jna",  X86_MN_JNA,     PF_NONE,    X86_ISET_GP,    OP(Jb)),
    TABLE_ENTRY("ja",   X86_MN_JA,      PF_NONE,    X86_ISET_GP,    OP(Jb)),
    TABLE_ENTRY("js",   X86_MN_JS,      PF_NONE,    X86_ISET_GP,    OP(Jb)),
    TABLE_ENTRY("jns",  X86_MN_JNS,     PF_NONE,    X86_ISET_GP,    OP(Jb)),
    TABLE_ENTRY("jp",   X86_MN_JP,      PF_NONE,    X86_ISET_GP,    OP(Jb)),
    TABLE_ENTRY("jnp",  X86_MN_JNP,     PF_NONE,    X86_ISET_GP,    OP(Jb)),
    TABLE_ENTRY("jl",   X86_MN_JL,      PF_NONE,    X86_ISET_GP,    OP(Jb)),
    TABLE_ENTRY("jnl",  X86_MN_JNL,     PF_NONE,    X86_ISET_GP,    OP(Jb)),
    TABLE_ENTRY("jng",  X86_MN_JNG,     PF_NONE,    X86_ISET_GP,    OP(Jb)),
    TABLE_ENTRY("jg",   X86_MN_JG,      PF_NONE,    X86_ISET_GP,    OP(Jb)),

    /* 80h: immediate group 1 */
    TABLE_ENTRY_ESCAPE(80),
    TABLE_ENTRY_ESCAPE(81),
    TABLE_ENTRY_ESCAPE(82),
    TABLE_ENTRY_ESCAPE(83),

    /* 84h */
    TABLE_ENTRY("test", X86_MN_TEST,    PF_NONE,    X86_ISET_GP,    OP(Eb, Gb)),
    TABLE_ENTRY("test", X86_MN_TEST,    PF_NONE,    X86_ISET_GP,    OP(Ev, Gv)),
    TABLE_ENTRY("xchg", X86_MN_XCHG,    PF_LOCK,    X86_ISET_GP,    OP(Eb, Gb)),
    TABLE_ENTRY("xchg", X86_MN_XCHG,    PF_LOCK,    X86_ISET_GP,    OP(Ev, Gv)),
    TABLE_ENTRY("mov",  X86_MN_MOV,     PF_NONE,    X86_ISET_GP,    OP(Eb, Gb)),
    TABLE_ENTRY("mov",  X86_MN_MOV,     PF_NONE,    X86_ISET_GP,    OP(Ev, Gv)),
    TABLE_ENTRY("mov",  X86_MN_MOV,     PF_NONE,    X86_ISET_GP,    OP(Gb, Eb)),
    TABLE_ENTRY("mov",  X86_MN_MOV,     PF_NONE,    X86_ISET_GP,    OP(Gv, Ev)),
    TABLE_ENTRY("mov",  X86_MN_MOV,     PF_NONE,    X86_ISET_GP,    OP(Ev, Sw)),
    TABLE_ENTRY("lea",  X86_MN_LEA,     PF_NONE,    X86_ISET_GP,    OP(Gv, M)),
    TABLE_ENTRY("mov",  X86_MN_MOV,     PF_NONE,    X86_ISET_GP,    OP(Sw, Ew)),

    /* 8Fh: group 1A */
    TABLE_ENTRY_ESCAPE(8F),

    /* 90h */
    TABLE_ENTRY("xchg", X86_MN_XCHG,    PF_NONE,    X86_ISET_GP,    OP(rAX, rAX_r8)), /* alias nop, +f3 prefix=pause */
    TABLE_ENTRY("xchg", X86_MN_XCHG,    PF_NONE,    X86_ISET_GP,    OP(rAX, rCX_r9)),
    TABLE_ENTRY("xchg", X86_MN_XCHG,    PF_NONE,    X86_ISET_GP,    OP(rAX, rDX_r10)),
    TABLE_ENTRY("xchg", X86_MN_XCHG,    PF_NONE,    X86_ISET_GP,    OP(rAX, rBX_r11)),
    TABLE_ENTRY("xchg", X86_MN_XCHG,    PF_NONE,    X86_ISET_GP,    OP(rAX, rSP_r12)),
    TABLE_ENTRY("xchg", X86_MN_XCHG,    PF_NONE,    X86_ISET_GP,    OP(rAX, rBP_r13)),
    TABLE_ENTRY("xchg", X86_MN_XCHG,    PF_NONE,    X86_ISET_GP,    OP(rAX, rSI_r14)),
    TABLE_ENTRY("xchg", X86_MN_XCHG,    PF_NONE,    X86_ISET_GP,    OP(rAX, rDI_r15)),

    /* 98h - 99h: cbw/cwde/cdqe and cwd/cdq/cqo depending on operand size */
    TABLE_ENTRY_ESCAPE(98),
    TABLE_ENTRY_ESCAPE(99),

    /* 9Ah */
    TABLE_ENTRY("call",     X86_MN_CALLF,   PF_I64,     X86_ISET_GP,    OP(Ap)),
    TABLE_ENTRY("wait",     X86_MN_WAIT,    PF_NONE,    X86_ISET_FPU,   OP(NONE)), /* alias fwait */

    /* 9Ch - 9Dh: pushf/pushfd/pushfq and popf/popfd/popfq depending on operand size */
    TABLE_ENTRY_ESCAPE(9C),
    TABLE_ENTRY_ESCAPE(9D),

    /* 9Eh */
    TABLE_ENTRY("sahf",     X86_MN_SAHF,    PF_NONE,    X86_ISET_GP,    OP(NONE)),
    TABLE_ENTRY("lahf",     X86_MN_LAHF,    PF_NONE,    X86_ISET_GP,    OP(NONE)),
    
    /* A0h */
    TABLE_ENTRY("mov",      X86_MN_MOV,     PF_NONE,    X86_ISET_GP,    OP(AL, Ob)),
    TABLE_ENTRY("mov",      X86_MN_MOV,     PF_NONE,    X86_ISET_GP,    OP(rAX, Ov)),
    TABLE_ENTRY("mov",      X86_MN_MOV,     PF_NONE,    X86_ISET_GP,    OP(Ob, AL)),
    TABLE_ENTRY("mov",      X86_MN_MOV,     PF_NONE,    X86_ISET_GP,    OP(Ov, rAX)),
    TABLE_ENTRY("movsb",    X86_MN_MOVSB,   PF_REP,     X86_ISET_GP,    OP(Yb, Xb)),

    /* A5h: movsw/movsd/movsq depending on operand size */
    TABLE_ENTRY_ESCAPE(A5),

    /* A6h */
    TABLE_ENTRY("cmpsb",    X86_MN_CMPSB,   PF_REPE_REPNE,  X86_ISET_GP,    OP(Xb, Yb)),

    /* A7h: cmpsw/cmpsd depending on operand size */
    TABLE_ENTRY_ESCAPE(A7),

    /* A8h */
    TABLE_ENTRY("test",     X86_MN_TEST,    PF_NONE,    X86_ISET_GP,    OP(AL, Ib)),
    TABLE_ENTRY("test",     X86_MN_TEST,    PF_NONE,    X86_ISET_GP,    OP(rAX, Iz)),
    TABLE_ENTRY("stosb",    X86_MN_STOSB,   PF_REP,     X86_ISET_GP,    OP(Yb, AL)),

    /* ABh: stosw/stosd/stosq depending on operand size */
    TABLE_ENTRY_ESCAPE(AB),

    /* ACh */
    TABLE_ENTRY("lodsb",    X86_MN_LODSB,   PF_REP,     X86_ISET_GP,    OP(AL, Xb)),

    /* ADh: lodsw/lodsd/lodsq depending on operand size */
    TABLE_ENTRY_ESCAPE(AD),

    /* AEh */
    TABLE_ENTRY("scasb",    X86_MN_SCASB,   PF_REPE_REPNE,  X86_ISET_GP,    OP(AL, Yb)),

    /* AFh: scasw/scasd/scasq depending on operand size */
    TABLE_ENTRY_ESCAPE(AF),

    /* B0h */
    TABLE_ENTRY("mov",  X86_MN_MOV, PF_NONE,    X86_ISET_GP,    OP(AL_r8L, Ib)),
    TABLE_ENTRY("mov",  X86_MN_MOV, PF_NONE,    X86_ISET_GP,    OP(CL_r9L, Ib)),
    TABLE_ENTRY("mov",  X86_MN_MOV, PF_NONE,    X86_ISET_GP,    OP(DL_r10L, Ib)),
    TABLE_ENTRY("mov",  X86_MN_MOV, PF_NONE,    X86_ISET_GP,    OP(BL_r11L, Ib)),
    TABLE_ENTRY("mov",  X86_MN_MOV, PF_NONE,    X86_ISET_GP,    OP(AH_r12L, Ib)),
    TABLE_ENTRY("mov",  X86_MN_MOV, PF_NONE,    X86_ISET_GP,    OP(CH_r13L, Ib)),
    TABLE_ENTRY("mov",  X86_MN_MOV, PF_NONE,    X86_ISET_GP,    OP(DH_r14L, Ib)),
    TABLE_ENTRY("mov",  X86_MN_MOV, PF_NONE,    X86_ISET_GP,    OP(BH_r15L, Ib)),
    
    TABLE_ENTRY("mov",  X86_MN_MOV, PF_NONE,    X86_ISET_GP,    OP(rAX_r8, Iv)),
    TABLE_ENTRY("mov",  X86_MN_MOV, PF_NONE,    X86_ISET_GP,    OP(rCX_r9, Iv)),
    TABLE_ENTRY("mov",  X86_MN_MOV, PF_NONE,    X86_ISET_GP,    OP(rDX_r10, Iv)),
    TABLE_ENTRY("mov",  X86_MN_MOV, PF_NONE,    X86_ISET_GP,    OP(rBX_r11, Iv)),
    TABLE_ENTRY("mov",  X86_MN_MOV, PF_NONE,    X86_ISET_GP,    OP(rSP_r12, Iv)),
    TABLE_ENTRY("mov",  X86_MN_MOV, PF_NONE,    X86_ISET_GP,    OP(rBP_r13, Iv)),
    TABLE_ENTRY("mov",  X86_MN_MOV, PF_NONE,    X86_ISET_GP,    OP(rSI_r14, Iv)),
    TABLE_ENTRY("mov",  X86_MN_MOV, PF_NONE,    X86_ISET_GP,    OP(rDI_r15, Iv)),

    /* C0h - C1h: shift group 2 */
    TABLE_ENTRY_ESCAPE(C0),
    TABLE_ENTRY_ESCAPE(C1),

    /* C2h */
    TABLE_ENTRY("retn", X86_MN_RETN,    PF_NONE,    X86_ISET_GP,    OP(Iw)),
    TABLE_ENTRY("retn", X86_MN_RETN,    PF_NONE,    X86_ISET_GP,    OP(NONE)),
    TABLE_ENTRY("les",  X86_MN_LES,     PF_I64,     X86_ISET_GP,    OP(Gz, Mp)), /* vex +2byte */
    TABLE_ENTRY("lds",  X86_MN_LDS,     PF_I64,     X86_ISET_GP,    OP(Gz, Mp)), /* vex +1byte */

    /* C6h - C7h: group 11 */
    TABLE_ENTRY_ESCAPE(C6),
    TABLE_ENTRY_ESCAPE(C7),

    /* C8h */
    TABLE_ENTRY("enter",    X86_MN_ENTER,   PF_NONE,    X86_ISET_GP,    OP(Iw, Ib)),
    TABLE_ENTRY("leave",    X86_MN_LEAVE,   PF_D64,     X86_ISET_GP,    OP(NONE)),
    TABLE_ENTRY("retf",     X86_MN_RETF,    PF_NONE,    X86_ISET_GP,    OP(Iw)),
    TABLE_ENTRY("retf",     X86_MN_RETF,    PF_NONE,    X86_ISET_GP,    OP(NONE)),
    TABLE_ENTRY("int",      X86_MN_INT3,    PF_NONE,    X86_ISET_GP,    OP(I3)),
    TABLE_ENTRY("int",      X86_MN_INT,     PF_NONE,    X86_ISET_GP,    OP(Ib)),
    TABLE_ENTRY("into",     X86_MN_INTO,    PF_I64,     X86_ISET_GP,    OP(NONE)),

    /* CFh: iretw/iretd/iretq depending on operand size */
    TABLE_ENTRY_ESCAPE(CF),

    /* D0h: shift group 2 */
    TABLE_ENTRY_ESCAPE(D0),
    TABLE_ENTRY_ESCAPE(D1),
    TABLE_ENTRY_ESCAPE(D2),
    TABLE_ENTRY_ESCAPE(D3),

    /* D4h */
    TABLE_ENTRY("aam",      X86_MN_AAM,     PF_I64,     X86_ISET_GP,    OP(Ib)),
    TABLE_ENTRY("aad",      X86_MN_AAD,     PF_I64,     X86_ISET_GP,    OP(Ib)),
    TABLE_ENTRY("salc",     X86_MN_SALC,    PF_NONE,    X86_ISET_GP,    OP(NONE)),
    TABLE_ENTRY("xlatb",    X86_MN_XLAT,    PF_NONE,    X86_ISET_GP,    OP(NONE)),

    /* D8h: x87 FPU co-processor escape */
    TABLE_ENTRY_ESCAPE(D8),
    TABLE_ENTRY_ESCAPE(D9),
    TABLE_ENTRY_ESCAPE(DA),
    TABLE_ENTRY_ESCAPE(DB),
    TABLE_ENTRY_ESCAPE(DC),
    TABLE_ENTRY_ESCAPE(DD),
    TABLE_ENTRY_ESCAPE(DE),
    TABLE_ENTRY_ESCAPE(DF),

    /* E0h */
    TABLE_ENTRY("loopnz",   X86_MN_LOOPNZ,  PF_F64,     X86_ISET_GP,    OP(Jb)),
    TABLE_ENTRY("loopz",    X86_MN_LOOPZ,   PF_F64,     X86_ISET_GP,    OP(Jb)),
    TABLE_ENTRY("loop",     X86_MN_LOOP,    PF_F64,     X86_ISET_GP,    OP(Jb)),

    /* E3h: jcxz/jecxz/jrcxz depending on addressing size */
    TABLE_ENTRY_ESCAPE(E3),

    /* E4h */
    TABLE_ENTRY("in",       X86_MN_IN,      PF_NONE,    X86_ISET_GP,    OP(AL, Ib)),
    TABLE_ENTRY("in",       X86_MN_IN,      PF_NONE,    X86_ISET_GP,    OP(eAX, Ib)),
    TABLE_ENTRY("out",      X86_MN_OUT,     PF_NONE,    X86_ISET_GP,    OP(Ib, AL)),
    TABLE_ENTRY("out",      X86_MN_OUT,     PF_NONE,    X86_ISET_GP,    OP(Ib, eAX)),
    
    TABLE_ENTRY("call",     X86_MN_CALL,    PF_F64,     X86_ISET_GP,    OP(Jz)),
    TABLE_ENTRY("jmp",      X86_MN_JMP,     PF_F64,     X86_ISET_GP,    OP(Jz)),
    TABLE_ENTRY("jmp",      X86_MN_JMPF,    PF_I64,     X86_ISET_GP,    OP(Ap)),
    TABLE_ENTRY("jmp",      X86_MN_JMP,     PF_F64,     X86_ISET_GP,    OP(Jb)),
    TABLE_ENTRY("in",       X86_MN_IN,      PF_NONE,    X86_ISET_GP,    OP(AL, DX)),
    TABLE_ENTRY("in",       X86_MN_IN,      PF_NONE,    X86_ISET_GP,    OP(eAX, DX)),
    TABLE_ENTRY("out",      X86_MN_OUT,     PF_NONE,    X86_ISET_GP,    OP(DX, AL)),
    TABLE_ENTRY("out",      X86_MN_OUT,     PF_NONE,    X86_ISET_GP,    OP(DX, eAX)),

    /* F0h: prefix (prefixes should have already been parsed) */
    TABLE_ENTRY_INVALID(), /* lock */
    TABLE_ENTRY("icebp",    X86_MN_ICEBP,   PF_NONE,    X86_ISET_GP,    OP(NONE)), /* single-step exception (int 01) */
    TABLE_ENTRY_INVALID(), /* repne/xacquire */
    TABLE_ENTRY_INVALID(), /* rep/xrelease */

    /* F4h */
    TABLE_ENTRY("hlt",  X86_MN_HLT,     PF_NONE,    X86_ISET_GP,    OP(NONE)),
    TABLE_ENTRY("cmc",  X86_MN_CMC,     PF_NONE,    X86_ISET_GP,    OP(NONE)),

    /* F6h - F7h: unary group 3 */
    TABLE_ENTRY_ESCAPE(F6),
    TABLE_ENTRY_ESCAPE(F7),

    /* F8h */
    TABLE_ENTRY("clc",  X86_MN_CLC,     PF_NONE,    X86_ISET_GP,    OP(NONE)),
    TABLE_ENTRY("stc",  X86_MN_STC,     PF_NONE,    X86_ISET_GP,    OP(NONE)),
    TABLE_ENTRY("cli",  X86_MN_CLI,     PF_NONE,    X86_ISET_GP,    OP(NONE)),
    TABLE_ENTRY("sti",  X86_MN_STI,     PF_NONE,    X86_ISET_GP,    OP(NONE)),
    TABLE_ENTRY("cld",  X86_MN_CLD,     PF_NONE,    X86_ISET_GP,    OP(NONE)),
    TABLE_ENTRY("std",  X86_MN_STD,     PF_NONE,    X86_ISET_GP,    OP(NONE)),

    /* FEh - FFh: inc/dec group 4,5 */
    TABLE_ENTRY_ESCAPE(FE),
    TABLE_ENTRY_ESCAPE(FF)

TABLE_END(1B)

TABLE_BEGIN(60)

    /* operand size:  0 = 16-bits, 1 = 32-bits, 2 = 64-bits */
    TABLE_ENTRY("pusha",    X86_MN_PUSHA,   PF_I64, X86_ISET_GP,    OP(NONE)),
    TABLE_ENTRY("pushad",   X86_MN_PUSHAD,  PF_I64, X86_ISET_GP,    OP(NONE)),
    TABLE_ENTRY_INVALID()

TABLE_END(60)

TABLE_BEGIN(61)

    /* operand size:  0 = 16-bits, 1 = 32-bits, 2 = 64-bits */
    TABLE_ENTRY("popa",     X86_MN_POPA,    PF_I64, X86_ISET_GP,    OP(NONE)),
    TABLE_ENTRY("popad",    X86_MN_POPAD,   PF_I64, X86_ISET_GP,    OP(NONE)),
    TABLE_ENTRY_INVALID()

TABLE_END(61)

TABLE_BEGIN(63)

    /* dasm mode:  0 = 16-bits, 1 = 32-bits, 2 = 64-bits */
    TABLE_ENTRY("arpl",     X86_MN_ARPL,        PF_I64,     X86_ISET_GP,    OP(Ew, Gw)),
    TABLE_ENTRY("arpl",     X86_MN_ARPL,        PF_I64,     X86_ISET_GP,    OP(Ew, Gw)),
    TABLE_ENTRY("movsxd",   X86_MN_MOVSXD,      PF_NONE,    X86_ISET_GP,    OP(Gv, Ev))

TABLE_END(63)

TABLE_BEGIN(6D)

    /* operand size:  0 = 16-bits, 1 = 32-bits, 2 = 64-bits */
    TABLE_ENTRY("insw",     X86_MN_INSW,    PF_REP, X86_ISET_GP,    OP(Yz, DX)),
    TABLE_ENTRY("insd",     X86_MN_INSD,    PF_REP, X86_ISET_GP,    OP(Yz, DX)),
    TABLE_ENTRY("insd",     X86_MN_INSD,    PF_REP, X86_ISET_GP,    OP(Yz, DX))

TABLE_END(6D)

TABLE_BEGIN(6F)

    /* operand size:  0 = 16-bits, 1 = 32-bits, 2 = 64-bits */
    TABLE_ENTRY("outsw",    X86_MN_OUTSW,   PF_REP, X86_ISET_GP,    OP(DX, Xz)),
    TABLE_ENTRY("outsd",    X86_MN_OUTSD,   PF_REP, X86_ISET_GP,    OP(DX, Xz)),
    TABLE_ENTRY("outsd",    X86_MN_OUTSD,   PF_REP, X86_ISET_GP,    OP(DX, Xz))

TABLE_END(6F)

TABLE_BEGIN(80)

    /* modr/m byte reg/nnn field */
    TABLE_ENTRY("add",  X86_MN_ADD,     PF_LOCK,    X86_ISET_GP,    OP(Eb, Ib)),
    TABLE_ENTRY("or",   X86_MN_OR,      PF_LOCK,    X86_ISET_GP,    OP(Eb, Ib)),
    TABLE_ENTRY("adc",  X86_MN_ADC,     PF_LOCK,    X86_ISET_GP,    OP(Eb, Ib)),
    TABLE_ENTRY("sbb",  X86_MN_SBB,     PF_LOCK,    X86_ISET_GP,    OP(Eb, Ib)),
    TABLE_ENTRY("and",  X86_MN_AND,     PF_LOCK,    X86_ISET_GP,    OP(Eb, Ib)),
    TABLE_ENTRY("sub",  X86_MN_SUB,     PF_LOCK,    X86_ISET_GP,    OP(Eb, Ib)),
    TABLE_ENTRY("xor",  X86_MN_XOR,     PF_LOCK,    X86_ISET_GP,    OP(Eb, Ib)),
    TABLE_ENTRY("cmp",  X86_MN_CMP,     PF_NONE,    X86_ISET_GP,    OP(Eb, Ib))

TABLE_END(80)

TABLE_BEGIN(81)

    /* modr/m byte reg/nnn field */
    TABLE_ENTRY("add",  X86_MN_ADD,     PF_LOCK,    X86_ISET_GP,    OP(Ev, Iz)),
    TABLE_ENTRY("or",   X86_MN_OR,      PF_LOCK,    X86_ISET_GP,    OP(Ev, Iz)),
    TABLE_ENTRY("adc",  X86_MN_ADC,     PF_LOCK,    X86_ISET_GP,    OP(Ev, Iz)),
    TABLE_ENTRY("sbb",  X86_MN_SBB,     PF_LOCK,    X86_ISET_GP,    OP(Ev, Iz)),
    TABLE_ENTRY("and",  X86_MN_AND,     PF_LOCK,    X86_ISET_GP,    OP(Ev, Iz)),
    TABLE_ENTRY("sub",  X86_MN_SUB,     PF_LOCK,    X86_ISET_GP,    OP(Ev, Iz)),
    TABLE_ENTRY("xor",  X86_MN_XOR,     PF_LOCK,    X86_ISET_GP,    OP(Ev, Iz)),
    TABLE_ENTRY("cmp",  X86_MN_CMP,     PF_NONE,    X86_ISET_GP,    OP(Ev, Iz))

TABLE_END(81)

TABLE_BEGIN(82)

    /* modr/m byte reg/nnn field */
    TABLE_ENTRY("add",  X86_MN_ADD,     PF_I64 | PF_LOCK,       X86_ISET_GP,    OP(Eb, Ib)),
    TABLE_ENTRY("or",   X86_MN_OR,      PF_I64 | PF_LOCK,       X86_ISET_GP,    OP(Eb, Ib)),
    TABLE_ENTRY("adc",  X86_MN_ADC,     PF_I64 | PF_LOCK,       X86_ISET_GP,    OP(Eb, Ib)),
    TABLE_ENTRY("sbb",  X86_MN_SBB,     PF_I64 | PF_LOCK,       X86_ISET_GP,    OP(Eb, Ib)),
    TABLE_ENTRY("and",  X86_MN_AND,     PF_I64 | PF_LOCK,       X86_ISET_GP,    OP(Eb, Ib)),
    TABLE_ENTRY("sub",  X86_MN_SUB,     PF_I64 | PF_LOCK,       X86_ISET_GP,    OP(Eb, Ib)),
    TABLE_ENTRY("xor",  X86_MN_XOR,     PF_I64 | PF_LOCK,       X86_ISET_GP,    OP(Eb, Ib)),
    TABLE_ENTRY("cmp",  X86_MN_CMP,     PF_I64,                 X86_ISET_GP,    OP(Eb, Ib))

TABLE_END(82)

TABLE_BEGIN(83)

    /* modr/m byte reg/nnn field */
    TABLE_ENTRY("add",  X86_MN_ADD,     PF_LOCK,    X86_ISET_GP,    OP(Ev, Ib)),
    TABLE_ENTRY("or",   X86_MN_OR,      PF_LOCK,    X86_ISET_GP,    OP(Ev, Ib)),
    TABLE_ENTRY("adc",  X86_MN_ADC,     PF_LOCK,    X86_ISET_GP,    OP(Ev, Ib)),
    TABLE_ENTRY("sbb",  X86_MN_SBB,     PF_LOCK,    X86_ISET_GP,    OP(Ev, Ib)),
    TABLE_ENTRY("and",  X86_MN_AND,     PF_LOCK,    X86_ISET_GP,    OP(Ev, Ib)),
    TABLE_ENTRY("sub",  X86_MN_SUB,     PF_LOCK,    X86_ISET_GP,    OP(Ev, Ib)),
    TABLE_ENTRY("xor",  X86_MN_XOR,     PF_LOCK,    X86_ISET_GP,    OP(Ev, Ib)),
    TABLE_ENTRY("cmp",  X86_MN_CMP,     PF_NONE,    X86_ISET_GP,    OP(Ev, Ib))

TABLE_END(83)

TABLE_BEGIN(8F)

    /* modr/m byte reg/nnn field */
    TABLE_ENTRY("pop",  X86_MN_POP,     PF_D64,     X86_ISET_GP,    OP(Ev)),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID()

TABLE_END(8F)

TABLE_BEGIN(98)

    /* operand size: 0 = 16-bits, 1 = 32-bits, 2 = 64-bits */
    TABLE_ENTRY("cbw",  X86_MN_CBW,     PF_NONE,    X86_ISET_GP,    OP(NONE)),
    TABLE_ENTRY("cwde", X86_MN_CWDE,    PF_NONE,    X86_ISET_GP,    OP(NONE)),
    TABLE_ENTRY("cdqe", X86_MN_CDQE,    PF_NONE,    X86_ISET_GP,    OP(NONE))

TABLE_END(98)

TABLE_BEGIN(99)

    /* operand size: 0 = 16-bits, 1 = 32-bits, 2 = 64-bits */
    TABLE_ENTRY("cwd",  X86_MN_CWD, PF_NONE,    X86_ISET_GP,    OP(NONE)),
    TABLE_ENTRY("cdq",  X86_MN_CDQ, PF_NONE,    X86_ISET_GP,    OP(NONE)),
    TABLE_ENTRY("cqo",  X86_MN_CQO, PF_NONE,    X86_ISET_GP,    OP(NONE))

TABLE_END(99)

TABLE_BEGIN(9C)

    /* operand size: 0 = 16-bits, 1 = 32-bits, 2 = 64-bits */
    TABLE_ENTRY("pushf",    X86_MN_PUSHF,   PF_D64, X86_ISET_GP,    OP(NONE)),
    TABLE_ENTRY("pushfd",   X86_MN_PUSHFD,  PF_D64, X86_ISET_GP,    OP(NONE)),
    TABLE_ENTRY("pushfq",   X86_MN_PUSHFQ,  PF_D64, X86_ISET_GP,    OP(NONE))

TABLE_END(9C)

TABLE_BEGIN(9D)

    /* operand size: 0 = 16-bits, 1 = 32-bits, 2 = 64-bits */
    TABLE_ENTRY("popf",     X86_MN_POPF,    PF_D64, X86_ISET_GP,    OP(NONE)),
    TABLE_ENTRY("popfd",    X86_MN_POPFD,   PF_D64, X86_ISET_GP,    OP(NONE)),
    TABLE_ENTRY("popfq",    X86_MN_POPFQ,   PF_D64, X86_ISET_GP,    OP(NONE))

TABLE_END(9D)

TABLE_BEGIN(A5)

    /* operand size: 0 = 16-bits, 1 = 32-bits, 2 = 64-bits */
    TABLE_ENTRY("movsw",    X86_MN_MOVSW,   PF_REP, X86_ISET_GP,    OP(Yv, Xv)),
    TABLE_ENTRY("movsd",    X86_MN_MOVSD,   PF_REP, X86_ISET_GP,    OP(Yv, Xv)),
    TABLE_ENTRY("movsq",    X86_MN_MOVSQ,   PF_REP, X86_ISET_GP,    OP(Yv, Xv))

TABLE_END(A5)

TABLE_BEGIN(A7)

    /* operand size: 0 = 16-bits, 1 = 32-bits, 2 = 64-bits */
    TABLE_ENTRY("cmpsw",    X86_MN_CMPSW,   PF_REPE_REPNE,  X86_ISET_GP,    OP(Xv, Yv)),
    TABLE_ENTRY("cmpsd",    X86_MN_CMPSD,   PF_REPE_REPNE,  X86_ISET_GP,    OP(Xv, Yv)),
    TABLE_ENTRY("cmpsq",    X86_MN_CMPSQ,   PF_REPE_REPNE,  X86_ISET_GP,    OP(Xv, Yv)),

TABLE_END(A7)

TABLE_BEGIN(AB)

    /* operand size: 0 = 16-bits, 1 = 32-bits, 2 = 64-bits */
    TABLE_ENTRY("stosw",    X86_MN_STOSW,   PF_REP, X86_ISET_GP,    OP(Yv, rAX)),
    TABLE_ENTRY("stosd",    X86_MN_STOSD,   PF_REP, X86_ISET_GP,    OP(Yv, rAX)),
    TABLE_ENTRY("stosq",    X86_MN_STOSQ,   PF_REP, X86_ISET_GP,    OP(Yv, rAX))

TABLE_END(AB)

TABLE_BEGIN(AD)

    /* operand size: 0 = 16-bits, 1 = 32-bits, 2 = 64-bits */
    TABLE_ENTRY("lodsw",    X86_MN_LODSW,   PF_REP, X86_ISET_GP,    OP(rAX, Xv)),
    TABLE_ENTRY("lodsd",    X86_MN_LODSD,   PF_REP, X86_ISET_GP,    OP(rAX, Xv)),
    TABLE_ENTRY("lodsq",    X86_MN_LODSQ,   PF_REP, X86_ISET_GP,    OP(rAX, Xv))

TABLE_END(AD)

TABLE_BEGIN(AF)

    /* operand size: 0 = 16-bits, 1 = 32-bits, 2 = 64-bits */
    TABLE_ENTRY("scasw",    X86_MN_SCASW,   PF_REPE_REPNE,  X86_ISET_GP,    OP(rAX, Xv)),
    TABLE_ENTRY("scasd",    X86_MN_SCASD,   PF_REPE_REPNE,  X86_ISET_GP,    OP(rAX, Xv)),
    TABLE_ENTRY("scasq",    X86_MN_SCASQ,   PF_REPE_REPNE,  X86_ISET_GP,    OP(rAX, Xv))

TABLE_END(AF)

TABLE_BEGIN(C0)

    /* modr/m byte reg/nnn field */
    TABLE_ENTRY("rol",      X86_MN_ROL, PF_NONE,    X86_ISET_GP,    OP(Eb, Ib)),
    TABLE_ENTRY("ror",      X86_MN_ROR, PF_NONE,    X86_ISET_GP,    OP(Eb, Ib)),
    TABLE_ENTRY("rcl",      X86_MN_RCL, PF_NONE,    X86_ISET_GP,    OP(Eb, Ib)),
    TABLE_ENTRY("rcr",      X86_MN_RCR, PF_NONE,    X86_ISET_GP,    OP(Eb, Ib)),
    TABLE_ENTRY("shl",      X86_MN_SHL, PF_NONE,    X86_ISET_GP,    OP(Eb, Ib)),
    TABLE_ENTRY("shr",      X86_MN_SHR, PF_NONE,    X86_ISET_GP,    OP(Eb, Ib)),
    TABLE_ENTRY("shl",      X86_MN_SHL, PF_NONE,    X86_ISET_GP,    OP(Eb, Ib)),
    TABLE_ENTRY("sar",      X86_MN_SAR, PF_NONE,    X86_ISET_GP,    OP(Eb, Ib))

TABLE_END(C0)

TABLE_BEGIN(C1)

    /* modr/m byte reg/nnn field */
    TABLE_ENTRY("rol",      X86_MN_ROL, PF_NONE,    X86_ISET_GP,    OP(Ev, Ib)),
    TABLE_ENTRY("ror",      X86_MN_ROR, PF_NONE,    X86_ISET_GP,    OP(Ev, Ib)),
    TABLE_ENTRY("rcl",      X86_MN_RCL, PF_NONE,    X86_ISET_GP,    OP(Ev, Ib)),
    TABLE_ENTRY("rcr",      X86_MN_RCR, PF_NONE,    X86_ISET_GP,    OP(Ev, Ib)),
    TABLE_ENTRY("shl",      X86_MN_SHL, PF_NONE,    X86_ISET_GP,    OP(Ev, Ib)),
    TABLE_ENTRY("shr",      X86_MN_SHR, PF_NONE,    X86_ISET_GP,    OP(Ev, Ib)),
    TABLE_ENTRY("shl",      X86_MN_SHL, PF_NONE,    X86_ISET_GP,    OP(Ev, Ib)),
    TABLE_ENTRY("sar",      X86_MN_SAR, PF_NONE,    X86_ISET_GP,    OP(Ev, Ib))

TABLE_END(C1)

TABLE_BEGIN(C6)

    /* modr/m byte reg/nnn field */
    TABLE_ENTRY("mov",      X86_MN_MOV, PF_NONE,    X86_ISET_GP,    OP(Eb, Ib)),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY("xabort",   X86_MN_XABORT,  PF_NONE,    X86_ISET_TSX,   OP(Ib))

TABLE_END(C6)

TABLE_BEGIN(C7)

    /* modr/m byte reg/nnn field */
    TABLE_ENTRY("mov",      X86_MN_MOV, PF_NONE,    X86_ISET_GP,    OP(Ev, Iz)),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY("xbegin",   X86_MN_XBEGIN,  PF_NONE,    X86_ISET_TSX,   OP(Jz))

TABLE_END(C7)

TABLE_BEGIN(CF)

    /* operand size: 0 = 16-bits, 1 = 32-bits, 2 = 64-bits */
    TABLE_ENTRY("iretw",    X86_MN_IRETW,   PF_NONE,    X86_ISET_GP,    OP(NONE)),
    TABLE_ENTRY("iretd",    X86_MN_IRETD,   PF_NONE,    X86_ISET_GP,    OP(NONE)),
    TABLE_ENTRY("iretq",    X86_MN_IRETQ,   PF_NONE,    X86_ISET_GP,    OP(NONE))

TABLE_END(CF)

TABLE_BEGIN(D0)

    /* modr/m byte reg/nnn field */
    TABLE_ENTRY("rol",      X86_MN_ROL, PF_NONE,    X86_ISET_GP,    OP(Eb, I1)),
    TABLE_ENTRY("ror",      X86_MN_ROR, PF_NONE,    X86_ISET_GP,    OP(Eb, I1)),
    TABLE_ENTRY("rcl",      X86_MN_RCL, PF_NONE,    X86_ISET_GP,    OP(Eb, I1)),
    TABLE_ENTRY("rcr",      X86_MN_RCR, PF_NONE,    X86_ISET_GP,    OP(Eb, I1)),
    TABLE_ENTRY("shl",      X86_MN_SHL, PF_NONE,    X86_ISET_GP,    OP(Eb, I1)),
    TABLE_ENTRY("shr",      X86_MN_SHR, PF_NONE,    X86_ISET_GP,    OP(Eb, I1)),
    TABLE_ENTRY("shl",      X86_MN_SHL, PF_NONE,    X86_ISET_GP,    OP(Eb, I1)),
    TABLE_ENTRY("sar",      X86_MN_SAR, PF_NONE,    X86_ISET_GP,    OP(Eb, I1))

TABLE_END(D0)

TABLE_BEGIN(D1)

    /* modr/m byte reg/nnn field */
    TABLE_ENTRY("rol",      X86_MN_ROL, PF_NONE,    X86_ISET_GP,    OP(Ev, I1)),
    TABLE_ENTRY("ror",      X86_MN_ROR, PF_NONE,    X86_ISET_GP,    OP(Ev, I1)),
    TABLE_ENTRY("rcl",      X86_MN_RCL, PF_NONE,    X86_ISET_GP,    OP(Ev, I1)),
    TABLE_ENTRY("rcr",      X86_MN_RCR, PF_NONE,    X86_ISET_GP,    OP(Ev, I1)),
    TABLE_ENTRY("shl",      X86_MN_SHL, PF_NONE,    X86_ISET_GP,    OP(Ev, I1)),
    TABLE_ENTRY("shr",      X86_MN_SHR, PF_NONE,    X86_ISET_GP,    OP(Ev, I1)),
    TABLE_ENTRY("shl",      X86_MN_SHL, PF_NONE,    X86_ISET_GP,    OP(Ev, I1)),
    TABLE_ENTRY("sar",      X86_MN_SAR, PF_NONE,    X86_ISET_GP,    OP(Ev, I1))

TABLE_END(D1)

TABLE_BEGIN(D2)

    /* modr/m byte reg/nnn field */
    TABLE_ENTRY("rol",      X86_MN_ROL, PF_NONE,    X86_ISET_GP,    OP(Eb, CL)),
    TABLE_ENTRY("ror",      X86_MN_ROR, PF_NONE,    X86_ISET_GP,    OP(Eb, CL)),
    TABLE_ENTRY("rcl",      X86_MN_RCL, PF_NONE,    X86_ISET_GP,    OP(Eb, CL)),
    TABLE_ENTRY("rcr",      X86_MN_RCR, PF_NONE,    X86_ISET_GP,    OP(Eb, CL)),
    TABLE_ENTRY("shl",      X86_MN_SHL, PF_NONE,    X86_ISET_GP,    OP(Eb, CL)),
    TABLE_ENTRY("shr",      X86_MN_SHR, PF_NONE,    X86_ISET_GP,    OP(Eb, CL)),
    TABLE_ENTRY("shl",      X86_MN_SHL, PF_NONE,    X86_ISET_GP,    OP(Eb, CL)),
    TABLE_ENTRY("sar",      X86_MN_SAR, PF_NONE,    X86_ISET_GP,    OP(Eb, CL))

TABLE_END(D2)

TABLE_BEGIN(D3)

    /* modr/m byte reg/nnn field */
    TABLE_ENTRY("rol",      X86_MN_ROL, PF_NONE,    X86_ISET_GP,    OP(Ev, CL)),
    TABLE_ENTRY("ror",      X86_MN_ROR, PF_NONE,    X86_ISET_GP,    OP(Ev, CL)),
    TABLE_ENTRY("rcl",      X86_MN_RCL, PF_NONE,    X86_ISET_GP,    OP(Ev, CL)),
    TABLE_ENTRY("rcr",      X86_MN_RCR, PF_NONE,    X86_ISET_GP,    OP(Ev, CL)),
    TABLE_ENTRY("shl",      X86_MN_SHL, PF_NONE,    X86_ISET_GP,    OP(Ev, CL)),
    TABLE_ENTRY("shr",      X86_MN_SHR, PF_NONE,    X86_ISET_GP,    OP(Ev, CL)),
    TABLE_ENTRY("shl",      X86_MN_SHL, PF_NONE,    X86_ISET_GP,    OP(Ev, CL)),
    TABLE_ENTRY("sar",      X86_MN_SAR, PF_NONE,    X86_ISET_GP,    OP(Ev, CL))

TABLE_END(D3)

TABLE_BEGIN(E3)

    /* addressing size: 0 = 16-bits, 1 = 32-bits, 2 = 64-bits */
    TABLE_ENTRY("jcxz",     X86_MN_JCXZ,    PF_F64,     X86_ISET_GP,    OP(Jb)),
    TABLE_ENTRY("jecxz",    X86_MN_JECXZ,   PF_F64,     X86_ISET_GP,    OP(Jb)),
    TABLE_ENTRY("jrcxz",    X86_MN_JRCXZ,   PF_F64,     X86_ISET_GP,    OP(Jb))

TABLE_END(E3)

TABLE_BEGIN(F6)

    /* modr/m byte reg/nnn field */
    TABLE_ENTRY("test",     X86_MN_TEST,    PF_NONE,    X86_ISET_GP,    OP(Eb, Ib)),
    TABLE_ENTRY("test",     X86_MN_TEST,    PF_NONE,    X86_ISET_GP,    OP(Eb, Ib)),
    TABLE_ENTRY("not",      X86_MN_NOT,     PF_LOCK,    X86_ISET_GP,    OP(Eb)),
    TABLE_ENTRY("neg",      X86_MN_NEG,     PF_LOCK,    X86_ISET_GP,    OP(Eb)),
    TABLE_ENTRY("mul",      X86_MN_MUL,     PF_NONE,    X86_ISET_GP,    OP(Eb)), // OP(AL, Eb)
    TABLE_ENTRY("imul",     X86_MN_IMUL,    PF_NONE,    X86_ISET_GP,    OP(Eb)),
    TABLE_ENTRY("div",      X86_MN_DIV,     PF_NONE,    X86_ISET_GP,    OP(Eb)),
    TABLE_ENTRY("idiv",     X86_MN_IDIV,    PF_NONE,    X86_ISET_GP,    OP(Eb))

TABLE_END(F6)

TABLE_BEGIN(F7)

    /* modr/m byte reg/nnn field */
    TABLE_ENTRY("test",     X86_MN_TEST,    PF_NONE,    X86_ISET_GP,    OP(Ev, Iz)),
    TABLE_ENTRY("test",     X86_MN_TEST,    PF_NONE,    X86_ISET_GP,    OP(Ev, Iz)),
    TABLE_ENTRY("not",      X86_MN_NOT,     PF_LOCK,    X86_ISET_GP,    OP(Ev)),
    TABLE_ENTRY("neg",      X86_MN_NEG,     PF_LOCK,    X86_ISET_GP,    OP(Ev)),
    TABLE_ENTRY("mul",      X86_MN_MUL,     PF_NONE,    X86_ISET_GP,    OP(Ev)), // OP(rAX, Ev)
    TABLE_ENTRY("imul",     X86_MN_IMUL,    PF_NONE,    X86_ISET_GP,    OP(Ev)),
    TABLE_ENTRY("div",      X86_MN_DIV,     PF_NONE,    X86_ISET_GP,    OP(Ev)),
    TABLE_ENTRY("idiv",     X86_MN_IDIV,    PF_NONE,    X86_ISET_GP,    OP(Ev))

TABLE_END(F7)

TABLE_BEGIN(FE)

    /* modr/m byte reg/nnn field */
    TABLE_ENTRY("inc",      X86_MN_INC, PF_LOCK,    X86_ISET_GP,    OP(Eb)),
    TABLE_ENTRY("dec",      X86_MN_DEC, PF_LOCK,    X86_ISET_GP,    OP(Eb)),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID()

TABLE_END(FE)

TABLE_BEGIN(FF)

    /* modr/m byte reg/nnn field */
    TABLE_ENTRY("inc",      X86_MN_INC,     PF_LOCK,    X86_ISET_GP,    OP(Ev)),
    TABLE_ENTRY("dec",      X86_MN_DEC,     PF_LOCK,    X86_ISET_GP,    OP(Ev)),
    TABLE_ENTRY("call",     X86_MN_CALL,    PF_F64,     X86_ISET_GP,    OP(Ev)),
    TABLE_ENTRY("call",     X86_MN_CALLF,   PF_NONE,    X86_ISET_GP,    OP(Ep)),
    TABLE_ENTRY("jmp",      X86_MN_JMP,     PF_F64,     X86_ISET_GP,    OP(Ev)),
    TABLE_ENTRY("jmp",      X86_MN_JMPF,    PF_NONE,    X86_ISET_GP,    OP(Mp)),
    TABLE_ENTRY("push",     X86_MN_PUSH,    PF_D64,     X86_ISET_GP,    OP(Ev)),
    TABLE_ENTRY_INVALID()

TABLE_END(FF)

/****************************************************************************

    x87 FPU co-processor tables

****************************************************************************/

TABLE_BEGIN(D8)

    /* modr/m byte mod field:  0 = !11b, 1 = 11b */
    TABLE_ENTRY_ESCAPE(D8_MOD_N11),
    TABLE_ENTRY_ESCAPE(D8_MOD_11)

TABLE_END(D8)

TABLE_BEGIN(D9)

    /* modr/m byte mod field:  0 = !11b, 1 = 11b */
    TABLE_ENTRY_ESCAPE(D9_MOD_N11),
    TABLE_ENTRY_ESCAPE(D9_MOD_11)

TABLE_END(D9)

TABLE_BEGIN(DA)

    /* modr/m byte mod field:  0 = !11b, 1 = 11b */
    TABLE_ENTRY_ESCAPE(DA_MOD_N11),
    TABLE_ENTRY_ESCAPE(DA_MOD_11)

TABLE_END(DA)

TABLE_BEGIN(DB)

    /* modr/m byte mod field:  0 = !11b, 1 = 11b */
    TABLE_ENTRY_ESCAPE(DB_MOD_N11),
    TABLE_ENTRY_ESCAPE(DB_MOD_11)

TABLE_END(DB)

TABLE_BEGIN(DC)

    /* modr/m byte mod field:  0 = !11b, 1 = 11b */
    TABLE_ENTRY_ESCAPE(DC_MOD_N11),
    TABLE_ENTRY_ESCAPE(DC_MOD_11)

TABLE_END(DC)

TABLE_BEGIN(DD)

    /* modr/m byte mod field:  0 = !11b, 1 = 11b */
    TABLE_ENTRY_ESCAPE(DD_MOD_N11),
    TABLE_ENTRY_ESCAPE(DD_MOD_11)

TABLE_END(DD)

TABLE_BEGIN(DE)

    /* modr/m byte mod field:  0 = !11b, 1 = 11b */
    TABLE_ENTRY_ESCAPE(DE_MOD_N11),
    TABLE_ENTRY_ESCAPE(DE_MOD_11)

TABLE_END(DE)

TABLE_BEGIN(DF)

    /* modr/m byte mod field:  0 = !11b, 1 = 11b */
    TABLE_ENTRY_ESCAPE(DF_MOD_N11),
    TABLE_ENTRY_ESCAPE(DF_MOD_11)

TABLE_END(DF)

TABLE_BEGIN(D8_MOD_N11)

    /* modr/m byte reg/nnn field */
    TABLE_ENTRY("fadd",     X86_MN_FADD,        PF_NONE,    X86_ISET_FPU,   OP(Md)), /* 32-bit real (single-real) */
    TABLE_ENTRY("fmul",     X86_MN_FMUL,        PF_NONE,    X86_ISET_FPU,   OP(Md)), /* 32-bit real (single-real) */
    TABLE_ENTRY("fcom",     X86_MN_FCOM,        PF_NONE,    X86_ISET_FPU,   OP(Md)), /* 32-bit real (single-real) */
    TABLE_ENTRY("fcomp",    X86_MN_FCOMP,       PF_NONE,    X86_ISET_FPU,   OP(Md)), /* 32-bit real (single-real) */
    TABLE_ENTRY("fsub",     X86_MN_FSUB,        PF_NONE,    X86_ISET_FPU,   OP(Md)), /* 32-bit real (single-real) */
    TABLE_ENTRY("fsubr",    X86_MN_FSUBR,       PF_NONE,    X86_ISET_FPU,   OP(Md)), /* 32-bit real (single-real) */
    TABLE_ENTRY("fdiv",     X86_MN_FDIV,        PF_NONE,    X86_ISET_FPU,   OP(Md)), /* 32-bit real (single-real) */
    TABLE_ENTRY("fdivr",    X86_MN_FDIVR,       PF_NONE,    X86_ISET_FPU,   OP(Md))  /* 32-bit real (single-real) */

TABLE_END(D8_MOD_N11)

TABLE_BEGIN(D8_MOD_11)

    /* modr/m byte:  index = modr/m byte - C0h */
    TABLE_ENTRY("fadd",     X86_MN_FADD,        PF_NONE,    X86_ISET_FPU,   OP(ST0, ST0)),
    TABLE_ENTRY("fadd",     X86_MN_FADD,        PF_NONE,    X86_ISET_FPU,   OP(ST0, ST1)),
    TABLE_ENTRY("fadd",     X86_MN_FADD,        PF_NONE,    X86_ISET_FPU,   OP(ST0, ST2)),
    TABLE_ENTRY("fadd",     X86_MN_FADD,        PF_NONE,    X86_ISET_FPU,   OP(ST0, ST3)),
    TABLE_ENTRY("fadd",     X86_MN_FADD,        PF_NONE,    X86_ISET_FPU,   OP(ST0, ST4)),
    TABLE_ENTRY("fadd",     X86_MN_FADD,        PF_NONE,    X86_ISET_FPU,   OP(ST0, ST5)),
    TABLE_ENTRY("fadd",     X86_MN_FADD,        PF_NONE,    X86_ISET_FPU,   OP(ST0, ST6)),
    TABLE_ENTRY("fadd",     X86_MN_FADD,        PF_NONE,    X86_ISET_FPU,   OP(ST0, ST7)),

    /* 08h */
    TABLE_ENTRY("fmul",     X86_MN_FMUL,        PF_NONE,    X86_ISET_FPU,   OP(ST0, ST0)),
    TABLE_ENTRY("fmul",     X86_MN_FMUL,        PF_NONE,    X86_ISET_FPU,   OP(ST0, ST1)),
    TABLE_ENTRY("fmul",     X86_MN_FMUL,        PF_NONE,    X86_ISET_FPU,   OP(ST0, ST2)),
    TABLE_ENTRY("fmul",     X86_MN_FMUL,        PF_NONE,    X86_ISET_FPU,   OP(ST0, ST3)),
    TABLE_ENTRY("fmul",     X86_MN_FMUL,        PF_NONE,    X86_ISET_FPU,   OP(ST0, ST4)),
    TABLE_ENTRY("fmul",     X86_MN_FMUL,        PF_NONE,    X86_ISET_FPU,   OP(ST0, ST5)),
    TABLE_ENTRY("fmul",     X86_MN_FMUL,        PF_NONE,    X86_ISET_FPU,   OP(ST0, ST6)),
    TABLE_ENTRY("fmul",     X86_MN_FMUL,        PF_NONE,    X86_ISET_FPU,   OP(ST0, ST7)),

    /* 10h */
    TABLE_ENTRY("fcom",     X86_MN_FCOM,        PF_NONE,    X86_ISET_FPU,   OP(ST0, ST0)),
    TABLE_ENTRY("fcom",     X86_MN_FCOM,        PF_NONE,    X86_ISET_FPU,   OP(ST0, ST1)),
    TABLE_ENTRY("fcom",     X86_MN_FCOM,        PF_NONE,    X86_ISET_FPU,   OP(ST0, ST2)),
    TABLE_ENTRY("fcom",     X86_MN_FCOM,        PF_NONE,    X86_ISET_FPU,   OP(ST0, ST3)),
    TABLE_ENTRY("fcom",     X86_MN_FCOM,        PF_NONE,    X86_ISET_FPU,   OP(ST0, ST4)),
    TABLE_ENTRY("fcom",     X86_MN_FCOM,        PF_NONE,    X86_ISET_FPU,   OP(ST0, ST5)),
    TABLE_ENTRY("fcom",     X86_MN_FCOM,        PF_NONE,    X86_ISET_FPU,   OP(ST0, ST6)),
    TABLE_ENTRY("fcom",     X86_MN_FCOM,        PF_NONE,    X86_ISET_FPU,   OP(ST0, ST7)),

    /* 18h */
    TABLE_ENTRY("fcomp",    X86_MN_FCOMP,       PF_NONE,    X86_ISET_FPU,   OP(ST0, ST0)),
    TABLE_ENTRY("fcomp",    X86_MN_FCOMP,       PF_NONE,    X86_ISET_FPU,   OP(ST0, ST1)),
    TABLE_ENTRY("fcomp",    X86_MN_FCOMP,       PF_NONE,    X86_ISET_FPU,   OP(ST0, ST2)),
    TABLE_ENTRY("fcomp",    X86_MN_FCOMP,       PF_NONE,    X86_ISET_FPU,   OP(ST0, ST3)),
    TABLE_ENTRY("fcomp",    X86_MN_FCOMP,       PF_NONE,    X86_ISET_FPU,   OP(ST0, ST4)),
    TABLE_ENTRY("fcomp",    X86_MN_FCOMP,       PF_NONE,    X86_ISET_FPU,   OP(ST0, ST5)),
    TABLE_ENTRY("fcomp",    X86_MN_FCOMP,       PF_NONE,    X86_ISET_FPU,   OP(ST0, ST6)),
    TABLE_ENTRY("fcomp",    X86_MN_FCOMP,       PF_NONE,    X86_ISET_FPU,   OP(ST0, ST7)),

    /* 20h */
    TABLE_ENTRY("fsub",     X86_MN_FSUB,        PF_NONE,    X86_ISET_FPU,   OP(ST0, ST0)),
    TABLE_ENTRY("fsub",     X86_MN_FSUB,        PF_NONE,    X86_ISET_FPU,   OP(ST0, ST1)),
    TABLE_ENTRY("fsub",     X86_MN_FSUB,        PF_NONE,    X86_ISET_FPU,   OP(ST0, ST2)),
    TABLE_ENTRY("fsub",     X86_MN_FSUB,        PF_NONE,    X86_ISET_FPU,   OP(ST0, ST3)),
    TABLE_ENTRY("fsub",     X86_MN_FSUB,        PF_NONE,    X86_ISET_FPU,   OP(ST0, ST4)),
    TABLE_ENTRY("fsub",     X86_MN_FSUB,        PF_NONE,    X86_ISET_FPU,   OP(ST0, ST5)),
    TABLE_ENTRY("fsub",     X86_MN_FSUB,        PF_NONE,    X86_ISET_FPU,   OP(ST0, ST6)),
    TABLE_ENTRY("fsub",     X86_MN_FSUB,        PF_NONE,    X86_ISET_FPU,   OP(ST0, ST7)),

    /* 28h */
    TABLE_ENTRY("fsubr",    X86_MN_FSUBR,       PF_NONE,    X86_ISET_FPU,   OP(ST0, ST0)),
    TABLE_ENTRY("fsubr",    X86_MN_FSUBR,       PF_NONE,    X86_ISET_FPU,   OP(ST0, ST1)),
    TABLE_ENTRY("fsubr",    X86_MN_FSUBR,       PF_NONE,    X86_ISET_FPU,   OP(ST0, ST2)),
    TABLE_ENTRY("fsubr",    X86_MN_FSUBR,       PF_NONE,    X86_ISET_FPU,   OP(ST0, ST3)),
    TABLE_ENTRY("fsubr",    X86_MN_FSUBR,       PF_NONE,    X86_ISET_FPU,   OP(ST0, ST4)),
    TABLE_ENTRY("fsubr",    X86_MN_FSUBR,       PF_NONE,    X86_ISET_FPU,   OP(ST0, ST5)),
    TABLE_ENTRY("fsubr",    X86_MN_FSUBR,       PF_NONE,    X86_ISET_FPU,   OP(ST0, ST6)),
    TABLE_ENTRY("fsubr",    X86_MN_FSUBR,       PF_NONE,    X86_ISET_FPU,   OP(ST0, ST7)),

    /* 30h */
    TABLE_ENTRY("fdiv",     X86_MN_FDIV,        PF_NONE,    X86_ISET_FPU,   OP(ST0, ST0)),
    TABLE_ENTRY("fdiv",     X86_MN_FDIV,        PF_NONE,    X86_ISET_FPU,   OP(ST0, ST1)),
    TABLE_ENTRY("fdiv",     X86_MN_FDIV,        PF_NONE,    X86_ISET_FPU,   OP(ST0, ST2)),
    TABLE_ENTRY("fdiv",     X86_MN_FDIV,        PF_NONE,    X86_ISET_FPU,   OP(ST0, ST3)),
    TABLE_ENTRY("fdiv",     X86_MN_FDIV,        PF_NONE,    X86_ISET_FPU,   OP(ST0, ST4)),
    TABLE_ENTRY("fdiv",     X86_MN_FDIV,        PF_NONE,    X86_ISET_FPU,   OP(ST0, ST5)),
    TABLE_ENTRY("fdiv",     X86_MN_FDIV,        PF_NONE,    X86_ISET_FPU,   OP(ST0, ST6)),
    TABLE_ENTRY("fdiv",     X86_MN_FDIV,        PF_NONE,    X86_ISET_FPU,   OP(ST0, ST7)),

    /* 38h */
    TABLE_ENTRY("fdivr",    X86_MN_FDIVR,       PF_NONE,    X86_ISET_FPU,   OP(ST0, ST0)),
    TABLE_ENTRY("fdivr",    X86_MN_FDIVR,       PF_NONE,    X86_ISET_FPU,   OP(ST0, ST1)),
    TABLE_ENTRY("fdivr",    X86_MN_FDIVR,       PF_NONE,    X86_ISET_FPU,   OP(ST0, ST2)),
    TABLE_ENTRY("fdivr",    X86_MN_FDIVR,       PF_NONE,    X86_ISET_FPU,   OP(ST0, ST3)),
    TABLE_ENTRY("fdivr",    X86_MN_FDIVR,       PF_NONE,    X86_ISET_FPU,   OP(ST0, ST4)),
    TABLE_ENTRY("fdivr",    X86_MN_FDIVR,       PF_NONE,    X86_ISET_FPU,   OP(ST0, ST5)),
    TABLE_ENTRY("fdivr",    X86_MN_FDIVR,       PF_NONE,    X86_ISET_FPU,   OP(ST0, ST6)),
    TABLE_ENTRY("fdivr",    X86_MN_FDIVR,       PF_NONE,    X86_ISET_FPU,   OP(ST0, ST7))

TABLE_END(D8_MOD_11)

TABLE_BEGIN(D9_MOD_N11)

    /* modr/m byte reg/nnn field */
    TABLE_ENTRY("fld",      X86_MN_FLD,         PF_NONE,    X86_ISET_FPU,   OP(Md)), /* 32-bit real (single-real) */
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY("fst",      X86_MN_FST,         PF_NONE,    X86_ISET_FPU,   OP(Md)), /* 32-bit real (single-real) */
    TABLE_ENTRY("fstp",     X86_MN_FSTP,        PF_NONE,    X86_ISET_FPU,   OP(Md)), /* 32-bit real (single-real) */
    TABLE_ENTRY("fldenv",   X86_MN_FLDENV,      PF_NONE,    X86_ISET_FPU,   OP(M)),  /* 14/28 bytes */
    TABLE_ENTRY("fldcw",    X86_MN_FLDCW,       PF_NONE,    X86_ISET_FPU,   OP(Mw)), /* 2 bytes -> word */
    TABLE_ENTRY("fnstenv",  X86_MN_FNSTENV,     PF_NONE,    X86_ISET_FPU,   OP(M)),  /* 14/28 bytes */
    TABLE_ENTRY("fnstcw",   X86_MN_FNSTCW,      PF_NONE,    X86_ISET_FPU,   OP(Mw))  /* 2 bytes -> word */

    /*
        The FSTENV instruction checks for and handles any pending unmasked 
        floating-point exceptions before storing the FPU environment; 
        the FNSTENV instruction does not.

        The assembler issues two instructions for the FSTENV instruction (an 
        FWAIT instruction followed by an FNSTENV instruction), and the 
        processor executes each of these instructions separately. If an 
        exception is generated for either of these instructions, the save EIP 
        points to the instruction that caused the exception.
    */

TABLE_END(D9_MOD_N11)

TABLE_BEGIN(D9_MOD_11)

    /* modr/m byte:  index = modr/m byte - C0h */
    TABLE_ENTRY("fld",      X86_MN_FLD,     PF_NONE,    X86_ISET_FPU,   OP(ST0, ST0)),
    TABLE_ENTRY("fld",      X86_MN_FLD,     PF_NONE,    X86_ISET_FPU,   OP(ST0, ST1)),
    TABLE_ENTRY("fld",      X86_MN_FLD,     PF_NONE,    X86_ISET_FPU,   OP(ST0, ST2)),
    TABLE_ENTRY("fld",      X86_MN_FLD,     PF_NONE,    X86_ISET_FPU,   OP(ST0, ST3)),
    TABLE_ENTRY("fld",      X86_MN_FLD,     PF_NONE,    X86_ISET_FPU,   OP(ST0, ST4)),
    TABLE_ENTRY("fld",      X86_MN_FLD,     PF_NONE,    X86_ISET_FPU,   OP(ST0, ST5)),
    TABLE_ENTRY("fld",      X86_MN_FLD,     PF_NONE,    X86_ISET_FPU,   OP(ST0, ST6)),
    TABLE_ENTRY("fld",      X86_MN_FLD,     PF_NONE,    X86_ISET_FPU,   OP(ST0, ST7)),

    /* 08h */
    TABLE_ENTRY("fxch",     X86_MN_FXCH,        PF_NONE,    X86_ISET_FPU,   OP(ST0, ST0)),
    TABLE_ENTRY("fxch",     X86_MN_FXCH,        PF_NONE,    X86_ISET_FPU,   OP(ST0, ST1)),
    TABLE_ENTRY("fxch",     X86_MN_FXCH,        PF_NONE,    X86_ISET_FPU,   OP(ST0, ST2)),
    TABLE_ENTRY("fxch",     X86_MN_FXCH,        PF_NONE,    X86_ISET_FPU,   OP(ST0, ST3)),
    TABLE_ENTRY("fxch",     X86_MN_FXCH,        PF_NONE,    X86_ISET_FPU,   OP(ST0, ST4)),
    TABLE_ENTRY("fxch",     X86_MN_FXCH,        PF_NONE,    X86_ISET_FPU,   OP(ST0, ST5)),
    TABLE_ENTRY("fxch",     X86_MN_FXCH,        PF_NONE,    X86_ISET_FPU,   OP(ST0, ST6)),
    TABLE_ENTRY("fxch",     X86_MN_FXCH,        PF_NONE,    X86_ISET_FPU,   OP(ST0, ST7)),

    /* 10h */
    TABLE_ENTRY("fnop",     X86_MN_FNOP,        PF_NONE,    X86_ISET_FPU,   OP(NONE)),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    /* 18h */
    TABLE_ENTRY("fstp1",    X86_MN_FSTP1,       PF_NONE,    X86_ISET_FPU,   OP(ST0)),
    TABLE_ENTRY("fstp1",    X86_MN_FSTP1,       PF_NONE,    X86_ISET_FPU,   OP(ST1)),
    TABLE_ENTRY("fstp1",    X86_MN_FSTP1,       PF_NONE,    X86_ISET_FPU,   OP(ST2)),
    TABLE_ENTRY("fstp1",    X86_MN_FSTP1,       PF_NONE,    X86_ISET_FPU,   OP(ST3)),
    TABLE_ENTRY("fstp1",    X86_MN_FSTP1,       PF_NONE,    X86_ISET_FPU,   OP(ST4)),
    TABLE_ENTRY("fstp1",    X86_MN_FSTP1,       PF_NONE,    X86_ISET_FPU,   OP(ST5)),
    TABLE_ENTRY("fstp1",    X86_MN_FSTP1,       PF_NONE,    X86_ISET_FPU,   OP(ST6)),
    TABLE_ENTRY("fstp1",    X86_MN_FSTP1,       PF_NONE,    X86_ISET_FPU,   OP(ST7)),

    /* 20h */
    TABLE_ENTRY("fchs",     X86_MN_FCHS,        PF_NONE,    X86_ISET_FPU,   OP(NONE)),
    TABLE_ENTRY("fabs",     X86_MN_FABS,        PF_NONE,    X86_ISET_FPU,   OP(NONE)),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY("ftst",     X86_MN_FTST,        PF_NONE,    X86_ISET_FPU,   OP(NONE)),
    TABLE_ENTRY("fxam",     X86_MN_FXAM,        PF_NONE,    X86_ISET_FPU,   OP(NONE)),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    /* 28h */
    TABLE_ENTRY("fld1",     X86_MN_FLD1,        PF_NONE,    X86_ISET_FPU,   OP(NONE)),
    TABLE_ENTRY("fldl2t",   X86_MN_FLDL2T,      PF_NONE,    X86_ISET_FPU,   OP(NONE)),
    TABLE_ENTRY("fldl2e",   X86_MN_FLDL2E,      PF_NONE,    X86_ISET_FPU,   OP(NONE)),
    TABLE_ENTRY("fldpi",    X86_MN_FLDPI,       PF_NONE,    X86_ISET_FPU,   OP(NONE)),
    TABLE_ENTRY("fldlg2",   X86_MN_FLDLG2,      PF_NONE,    X86_ISET_FPU,   OP(NONE)),
    TABLE_ENTRY("fldln2",   X86_MN_FLDLN2,      PF_NONE,    X86_ISET_FPU,   OP(NONE)),
    TABLE_ENTRY("fldz",     X86_MN_FLDZ,        PF_NONE,    X86_ISET_FPU,   OP(NONE)),
    TABLE_ENTRY_INVALID(),

    /* 30h */
    TABLE_ENTRY("f2xm1",    X86_MN_F2XM1,       PF_NONE,    X86_ISET_FPU,   OP(NONE)),
    TABLE_ENTRY("fyl2x",    X86_MN_FYL2X,       PF_NONE,    X86_ISET_FPU,   OP(NONE)),
    TABLE_ENTRY("fptan",    X86_MN_FPTAN,       PF_NONE,    X86_ISET_FPU,   OP(NONE)),
    TABLE_ENTRY("fpatan",   X86_MN_FPATAN,      PF_NONE,    X86_ISET_FPU,   OP(NONE)),
    TABLE_ENTRY("fxtract",  X86_MN_FXTRACT,     PF_NONE,    X86_ISET_FPU,   OP(NONE)),
    TABLE_ENTRY("fprem1",   X86_MN_FPREM1,      PF_NONE,    X86_ISET_FPU,   OP(NONE)),
    TABLE_ENTRY("fdecstp",  X86_MN_FDECSTP,     PF_NONE,    X86_ISET_FPU,   OP(NONE)),
    TABLE_ENTRY("fincstp",  X86_MN_FINCSTP,     PF_NONE,    X86_ISET_FPU,   OP(NONE)),

    /* 38h */
    TABLE_ENTRY("fprem",    X86_MN_FPREM,       PF_NONE,    X86_ISET_FPU,   OP(NONE)),
    TABLE_ENTRY("fyl2xp1",  X86_MN_FYL2XP1,     PF_NONE,    X86_ISET_FPU,   OP(NONE)),
    TABLE_ENTRY("fsqrt",    X86_MN_FSQRT,       PF_NONE,    X86_ISET_FPU,   OP(NONE)),
    TABLE_ENTRY("fsincos",  X86_MN_FSINCOS,     PF_NONE,    X86_ISET_FPU,   OP(NONE)),
    TABLE_ENTRY("frndint",  X86_MN_FRNDINT,     PF_NONE,    X86_ISET_FPU,   OP(NONE)),
    TABLE_ENTRY("fscale",   X86_MN_FSCALE,      PF_NONE,    X86_ISET_FPU,   OP(NONE)),
    TABLE_ENTRY("fsin",     X86_MN_FSIN,        PF_NONE,    X86_ISET_FPU,   OP(NONE)),
    TABLE_ENTRY("fcos",     X86_MN_FCOS,        PF_NONE,    X86_ISET_FPU,   OP(NONE))

TABLE_END(D9_MOD_11)

TABLE_BEGIN(DA_MOD_N11)

    /* modr/m byte reg/nnn field */
    TABLE_ENTRY("fiadd",    X86_MN_FIADD,       PF_NONE,    X86_ISET_FPU,   OP(Md)), /* dword-integer */
    TABLE_ENTRY("fimul",    X86_MN_FIMUL,       PF_NONE,    X86_ISET_FPU,   OP(Md)), /* dword-integer */
    TABLE_ENTRY("ficom",    X86_MN_FICOM,       PF_NONE,    X86_ISET_FPU,   OP(Md)), /* dword-integer */
    TABLE_ENTRY("ficomp",   X86_MN_FICOMP,      PF_NONE,    X86_ISET_FPU,   OP(Md)), /* dword-integer */
    TABLE_ENTRY("fisub",    X86_MN_FISUB,       PF_NONE,    X86_ISET_FPU,   OP(Md)), /* dword-integer */
    TABLE_ENTRY("fisubr",   X86_MN_FISUBR,      PF_NONE,    X86_ISET_FPU,   OP(Md)), /* dword-integer */
    TABLE_ENTRY("fidiv",    X86_MN_FIDIV,       PF_NONE,    X86_ISET_FPU,   OP(Md)), /* dword-integer */
    TABLE_ENTRY("fidivr",   X86_MN_FIDIVR,      PF_NONE,    X86_ISET_FPU,   OP(Md))  /* dword-integer */

TABLE_END(DA_MOD_N11)

TABLE_BEGIN(DA_MOD_11)

    /* modr/m byte:  index = modr/m byte - C0h */
    TABLE_ENTRY("fcmovb",   X86_MN_FCMOVB,      PF_NONE,    X86_ISET_FPU,   OP(ST0, ST0)),
    TABLE_ENTRY("fcmovb",   X86_MN_FCMOVB,      PF_NONE,    X86_ISET_FPU,   OP(ST0, ST1)),
    TABLE_ENTRY("fcmovb",   X86_MN_FCMOVB,      PF_NONE,    X86_ISET_FPU,   OP(ST0, ST2)),
    TABLE_ENTRY("fcmovb",   X86_MN_FCMOVB,      PF_NONE,    X86_ISET_FPU,   OP(ST0, ST3)),
    TABLE_ENTRY("fcmovb",   X86_MN_FCMOVB,      PF_NONE,    X86_ISET_FPU,   OP(ST0, ST4)),
    TABLE_ENTRY("fcmovb",   X86_MN_FCMOVB,      PF_NONE,    X86_ISET_FPU,   OP(ST0, ST5)),
    TABLE_ENTRY("fcmovb",   X86_MN_FCMOVB,      PF_NONE,    X86_ISET_FPU,   OP(ST0, ST6)),
    TABLE_ENTRY("fcmovb",   X86_MN_FCMOVB,      PF_NONE,    X86_ISET_FPU,   OP(ST0, ST7)),

    /* 08h */
    TABLE_ENTRY("fcmove",   X86_MN_FCMOVE,      PF_NONE,    X86_ISET_FPU,   OP(ST0, ST0)),
    TABLE_ENTRY("fcmove",   X86_MN_FCMOVE,      PF_NONE,    X86_ISET_FPU,   OP(ST0, ST1)),
    TABLE_ENTRY("fcmove",   X86_MN_FCMOVE,      PF_NONE,    X86_ISET_FPU,   OP(ST0, ST2)),
    TABLE_ENTRY("fcmove",   X86_MN_FCMOVE,      PF_NONE,    X86_ISET_FPU,   OP(ST0, ST3)),
    TABLE_ENTRY("fcmove",   X86_MN_FCMOVE,      PF_NONE,    X86_ISET_FPU,   OP(ST0, ST4)),
    TABLE_ENTRY("fcmove",   X86_MN_FCMOVE,      PF_NONE,    X86_ISET_FPU,   OP(ST0, ST5)),
    TABLE_ENTRY("fcmove",   X86_MN_FCMOVE,      PF_NONE,    X86_ISET_FPU,   OP(ST0, ST6)),
    TABLE_ENTRY("fcmove",   X86_MN_FCMOVE,      PF_NONE,    X86_ISET_FPU,   OP(ST0, ST7)),

    /* 10h */
    TABLE_ENTRY("fcmovbe",  X86_MN_FCMOVBE,     PF_NONE,    X86_ISET_FPU,   OP(ST0, ST0)),
    TABLE_ENTRY("fcmovbe",  X86_MN_FCMOVBE,     PF_NONE,    X86_ISET_FPU,   OP(ST0, ST1)),
    TABLE_ENTRY("fcmovbe",  X86_MN_FCMOVBE,     PF_NONE,    X86_ISET_FPU,   OP(ST0, ST2)),
    TABLE_ENTRY("fcmovbe",  X86_MN_FCMOVBE,     PF_NONE,    X86_ISET_FPU,   OP(ST0, ST3)),
    TABLE_ENTRY("fcmovbe",  X86_MN_FCMOVBE,     PF_NONE,    X86_ISET_FPU,   OP(ST0, ST4)),
    TABLE_ENTRY("fcmovbe",  X86_MN_FCMOVBE,     PF_NONE,    X86_ISET_FPU,   OP(ST0, ST5)),
    TABLE_ENTRY("fcmovbe",  X86_MN_FCMOVBE,     PF_NONE,    X86_ISET_FPU,   OP(ST0, ST6)),
    TABLE_ENTRY("fcmovbe",  X86_MN_FCMOVBE,     PF_NONE,    X86_ISET_FPU,   OP(ST0, ST7)),

    /* 18h */
    TABLE_ENTRY("fcmovu",   X86_MN_FCMOVU,      PF_NONE,    X86_ISET_FPU,   OP(ST0, ST0)),
    TABLE_ENTRY("fcmovu",   X86_MN_FCMOVU,      PF_NONE,    X86_ISET_FPU,   OP(ST0, ST1)),
    TABLE_ENTRY("fcmovu",   X86_MN_FCMOVU,      PF_NONE,    X86_ISET_FPU,   OP(ST0, ST2)),
    TABLE_ENTRY("fcmovu",   X86_MN_FCMOVU,      PF_NONE,    X86_ISET_FPU,   OP(ST0, ST3)),
    TABLE_ENTRY("fcmovu",   X86_MN_FCMOVU,      PF_NONE,    X86_ISET_FPU,   OP(ST0, ST4)),
    TABLE_ENTRY("fcmovu",   X86_MN_FCMOVU,      PF_NONE,    X86_ISET_FPU,   OP(ST0, ST5)),
    TABLE_ENTRY("fcmovu",   X86_MN_FCMOVU,      PF_NONE,    X86_ISET_FPU,   OP(ST0, ST6)),
    TABLE_ENTRY("fcmovu",   X86_MN_FCMOVU,      PF_NONE,    X86_ISET_FPU,   OP(ST0, ST7)),

    /* 20h */
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    /* 28h */
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY("fucompp",  X86_MN_FUCOMPP, PF_NONE,    X86_ISET_FPU,   OP(NONE)),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    /* 30h */
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    /* 38h */
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID()

TABLE_END(DA_MOD_11)

TABLE_BEGIN(DB_MOD_N11)

    /* modr/m byte reg/nnn field */
    TABLE_ENTRY("fild",     X86_MN_FILD,        PF_NONE,    X86_ISET_FPU,   OP(Md)), /* dword-integer */
    TABLE_ENTRY("fisttp",   X86_MN_FISTTP,      PF_NONE,    X86_ISET_FPU,   OP(Md)), /* dword-integer */
    TABLE_ENTRY("fist",     X86_MN_FIST,        PF_NONE,    X86_ISET_FPU,   OP(Md)), /* dword-integer */
    TABLE_ENTRY("fistp",    X86_MN_FISTP,       PF_NONE,    X86_ISET_FPU,   OP(Md)), /* dword-integer */
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY("fld",      X86_MN_FLD,         PF_NONE,    X86_ISET_FPU,   OP(Mt)), /* 80-bit real (extended-real) */
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY("fstp",     X86_MN_FSTP,        PF_NONE,    X86_ISET_FPU,   OP(Mt))  /* 80-bit real (extended-real) */

TABLE_END(DB_MOD_N11)

TABLE_BEGIN(DB_MOD_11)

    /* modr/m byte:  index = modr/m byte - C0h */
    TABLE_ENTRY("fcmovnb",  X86_MN_FCMOVNB, PF_NONE,    X86_ISET_FPU,   OP(ST0, ST0)),
    TABLE_ENTRY("fcmovnb",  X86_MN_FCMOVNB, PF_NONE,    X86_ISET_FPU,   OP(ST0, ST1)),
    TABLE_ENTRY("fcmovnb",  X86_MN_FCMOVNB, PF_NONE,    X86_ISET_FPU,   OP(ST0, ST2)),
    TABLE_ENTRY("fcmovnb",  X86_MN_FCMOVNB, PF_NONE,    X86_ISET_FPU,   OP(ST0, ST3)),
    TABLE_ENTRY("fcmovnb",  X86_MN_FCMOVNB, PF_NONE,    X86_ISET_FPU,   OP(ST0, ST4)),
    TABLE_ENTRY("fcmovnb",  X86_MN_FCMOVNB, PF_NONE,    X86_ISET_FPU,   OP(ST0, ST5)),
    TABLE_ENTRY("fcmovnb",  X86_MN_FCMOVNB, PF_NONE,    X86_ISET_FPU,   OP(ST0, ST6)),
    TABLE_ENTRY("fcmovnb",  X86_MN_FCMOVNB, PF_NONE,    X86_ISET_FPU,   OP(ST0, ST7)),

    /* 08h */
    TABLE_ENTRY("fcmovne",  X86_MN_FCMOVNE, PF_NONE,    X86_ISET_FPU,   OP(ST0, ST0)),
    TABLE_ENTRY("fcmovne",  X86_MN_FCMOVNE, PF_NONE,    X86_ISET_FPU,   OP(ST0, ST1)),
    TABLE_ENTRY("fcmovne",  X86_MN_FCMOVNE, PF_NONE,    X86_ISET_FPU,   OP(ST0, ST2)),
    TABLE_ENTRY("fcmovne",  X86_MN_FCMOVNE, PF_NONE,    X86_ISET_FPU,   OP(ST0, ST3)),
    TABLE_ENTRY("fcmovne",  X86_MN_FCMOVNE, PF_NONE,    X86_ISET_FPU,   OP(ST0, ST4)),
    TABLE_ENTRY("fcmovne",  X86_MN_FCMOVNE, PF_NONE,    X86_ISET_FPU,   OP(ST0, ST5)),
    TABLE_ENTRY("fcmovne",  X86_MN_FCMOVNE, PF_NONE,    X86_ISET_FPU,   OP(ST0, ST6)),
    TABLE_ENTRY("fcmovne",  X86_MN_FCMOVNE, PF_NONE,    X86_ISET_FPU,   OP(ST0, ST7)),

    /* 10h */
    TABLE_ENTRY("fcmovnbe", X86_MN_FCMOVNBE,    PF_NONE,    X86_ISET_FPU,   OP(ST0, ST0)),
    TABLE_ENTRY("fcmovnbe", X86_MN_FCMOVNBE,    PF_NONE,    X86_ISET_FPU,   OP(ST0, ST1)),
    TABLE_ENTRY("fcmovnbe", X86_MN_FCMOVNBE,    PF_NONE,    X86_ISET_FPU,   OP(ST0, ST2)),
    TABLE_ENTRY("fcmovnbe", X86_MN_FCMOVNBE,    PF_NONE,    X86_ISET_FPU,   OP(ST0, ST3)),
    TABLE_ENTRY("fcmovnbe", X86_MN_FCMOVNBE,    PF_NONE,    X86_ISET_FPU,   OP(ST0, ST4)),
    TABLE_ENTRY("fcmovnbe", X86_MN_FCMOVNBE,    PF_NONE,    X86_ISET_FPU,   OP(ST0, ST5)),
    TABLE_ENTRY("fcmovnbe", X86_MN_FCMOVNBE,    PF_NONE,    X86_ISET_FPU,   OP(ST0, ST6)),
    TABLE_ENTRY("fcmovnbe", X86_MN_FCMOVNBE,    PF_NONE,    X86_ISET_FPU,   OP(ST0, ST7)),

    /* 18h */
    TABLE_ENTRY("fcmovnu",  X86_MN_FCMOVNU, PF_NONE,    X86_ISET_FPU,   OP(ST0, ST0)),
    TABLE_ENTRY("fcmovnu",  X86_MN_FCMOVNU, PF_NONE,    X86_ISET_FPU,   OP(ST0, ST1)),
    TABLE_ENTRY("fcmovnu",  X86_MN_FCMOVNU, PF_NONE,    X86_ISET_FPU,   OP(ST0, ST2)),
    TABLE_ENTRY("fcmovnu",  X86_MN_FCMOVNU, PF_NONE,    X86_ISET_FPU,   OP(ST0, ST3)),
    TABLE_ENTRY("fcmovnu",  X86_MN_FCMOVNU, PF_NONE,    X86_ISET_FPU,   OP(ST0, ST4)),
    TABLE_ENTRY("fcmovnu",  X86_MN_FCMOVNU, PF_NONE,    X86_ISET_FPU,   OP(ST0, ST5)),
    TABLE_ENTRY("fcmovnu",  X86_MN_FCMOVNU, PF_NONE,    X86_ISET_FPU,   OP(ST0, ST6)),
    TABLE_ENTRY("fcmovnu",  X86_MN_FCMOVNU, PF_NONE,    X86_ISET_FPU,   OP(ST0, ST7)),

    /* 20h */
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY("fnclex",   X86_MN_FNCLEX,      PF_NONE,    X86_ISET_FPU,   OP(NONE)),
    TABLE_ENTRY("fninit",   X86_MN_FNINIT,      PF_NONE,    X86_ISET_FPU,   OP(NONE)),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    /*
        The assembler issues two instructions for the FCLEX instruction (an 
        FWAIT instruction followed by an FNCLEX instruction), and the 
        processor executes each of these instructions separately. If an 
        exception is generated for either of these instructions, the save EIP 
        points to the instruction that caused the exception.

        same shit for FINIT/FNINIT
    */

    /* 28h */
    TABLE_ENTRY("fucomi",   X86_MN_FUCOMI,      PF_NONE,    X86_ISET_FPU,   OP(ST0, ST0)),
    TABLE_ENTRY("fucomi",   X86_MN_FUCOMI,      PF_NONE,    X86_ISET_FPU,   OP(ST0, ST1)),
    TABLE_ENTRY("fucomi",   X86_MN_FUCOMI,      PF_NONE,    X86_ISET_FPU,   OP(ST0, ST2)),
    TABLE_ENTRY("fucomi",   X86_MN_FUCOMI,      PF_NONE,    X86_ISET_FPU,   OP(ST0, ST3)),
    TABLE_ENTRY("fucomi",   X86_MN_FUCOMI,      PF_NONE,    X86_ISET_FPU,   OP(ST0, ST4)),
    TABLE_ENTRY("fucomi",   X86_MN_FUCOMI,      PF_NONE,    X86_ISET_FPU,   OP(ST0, ST5)),
    TABLE_ENTRY("fucomi",   X86_MN_FUCOMI,      PF_NONE,    X86_ISET_FPU,   OP(ST0, ST6)),
    TABLE_ENTRY("fucomi",   X86_MN_FUCOMI,      PF_NONE,    X86_ISET_FPU,   OP(ST0, ST7)),

    /* 30h */
    TABLE_ENTRY("fcomi",    X86_MN_FCOMI,       PF_NONE,    X86_ISET_FPU,   OP(ST0, ST0)),
    TABLE_ENTRY("fcomi",    X86_MN_FCOMI,       PF_NONE,    X86_ISET_FPU,   OP(ST0, ST1)),
    TABLE_ENTRY("fcomi",    X86_MN_FCOMI,       PF_NONE,    X86_ISET_FPU,   OP(ST0, ST2)),
    TABLE_ENTRY("fcomi",    X86_MN_FCOMI,       PF_NONE,    X86_ISET_FPU,   OP(ST0, ST3)),
    TABLE_ENTRY("fcomi",    X86_MN_FCOMI,       PF_NONE,    X86_ISET_FPU,   OP(ST0, ST4)),
    TABLE_ENTRY("fcomi",    X86_MN_FCOMI,       PF_NONE,    X86_ISET_FPU,   OP(ST0, ST5)),
    TABLE_ENTRY("fcomi",    X86_MN_FCOMI,       PF_NONE,    X86_ISET_FPU,   OP(ST0, ST6)),
    TABLE_ENTRY("fcomi",    X86_MN_FCOMI,       PF_NONE,    X86_ISET_FPU,   OP(ST0, ST7)),

    /* 38h */
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID()

TABLE_END(DB_MOD_11)

TABLE_BEGIN(DC_MOD_N11)

    /* modr/m byte reg/nnn field */
    TABLE_ENTRY("fadd",     X86_MN_FADD,        PF_NONE,    X86_ISET_FPU,   OP(Mq)), /* 64-bit real (double-real) */
    TABLE_ENTRY("fmul",     X86_MN_FMUL,        PF_NONE,    X86_ISET_FPU,   OP(Mq)), /* 64-bit real (double-real) */
    TABLE_ENTRY("fcom",     X86_MN_FCOM,        PF_NONE,    X86_ISET_FPU,   OP(Mq)), /* 64-bit real (double-real) */
    TABLE_ENTRY("fcomp",    X86_MN_FCOMP,       PF_NONE,    X86_ISET_FPU,   OP(Mq)), /* 64-bit real (double-real) */
    TABLE_ENTRY("fsub",     X86_MN_FSUB,        PF_NONE,    X86_ISET_FPU,   OP(Mq)), /* 64-bit real (double-real) */
    TABLE_ENTRY("fsubr",    X86_MN_FSUBR,       PF_NONE,    X86_ISET_FPU,   OP(Mq)), /* 64-bit real (double-real) */
    TABLE_ENTRY("fdiv",     X86_MN_FDIV,        PF_NONE,    X86_ISET_FPU,   OP(Mq)), /* 64-bit real (double-real) */
    TABLE_ENTRY("fdivr",    X86_MN_FDIVR,       PF_NONE,    X86_ISET_FPU,   OP(Mq))  /* 64-bit real (double-real) */

TABLE_END(DC_MOD_N11)

TABLE_BEGIN(DC_MOD_11)

    /* modr/m byte:  index = modr/m byte - C0h */
    TABLE_ENTRY("fadd",     X86_MN_FADD,        PF_NONE,    X86_ISET_FPU,   OP(ST0, ST0)),
    TABLE_ENTRY("fadd",     X86_MN_FADD,        PF_NONE,    X86_ISET_FPU,   OP(ST1, ST0)),
    TABLE_ENTRY("fadd",     X86_MN_FADD,        PF_NONE,    X86_ISET_FPU,   OP(ST2, ST0)),
    TABLE_ENTRY("fadd",     X86_MN_FADD,        PF_NONE,    X86_ISET_FPU,   OP(ST3, ST0)),
    TABLE_ENTRY("fadd",     X86_MN_FADD,        PF_NONE,    X86_ISET_FPU,   OP(ST4, ST0)),
    TABLE_ENTRY("fadd",     X86_MN_FADD,        PF_NONE,    X86_ISET_FPU,   OP(ST5, ST0)),
    TABLE_ENTRY("fadd",     X86_MN_FADD,        PF_NONE,    X86_ISET_FPU,   OP(ST6, ST0)),
    TABLE_ENTRY("fadd",     X86_MN_FADD,        PF_NONE,    X86_ISET_FPU,   OP(ST7, ST0)),

    /* 08h */
    TABLE_ENTRY("fmul",     X86_MN_FMUL,        PF_NONE,    X86_ISET_FPU,   OP(ST0, ST0)),
    TABLE_ENTRY("fmul",     X86_MN_FMUL,        PF_NONE,    X86_ISET_FPU,   OP(ST1, ST0)),
    TABLE_ENTRY("fmul",     X86_MN_FMUL,        PF_NONE,    X86_ISET_FPU,   OP(ST2, ST0)),
    TABLE_ENTRY("fmul",     X86_MN_FMUL,        PF_NONE,    X86_ISET_FPU,   OP(ST3, ST0)),
    TABLE_ENTRY("fmul",     X86_MN_FMUL,        PF_NONE,    X86_ISET_FPU,   OP(ST4, ST0)),
    TABLE_ENTRY("fmul",     X86_MN_FMUL,        PF_NONE,    X86_ISET_FPU,   OP(ST5, ST0)),
    TABLE_ENTRY("fmul",     X86_MN_FMUL,        PF_NONE,    X86_ISET_FPU,   OP(ST6, ST0)),
    TABLE_ENTRY("fmul",     X86_MN_FMUL,        PF_NONE,    X86_ISET_FPU,   OP(ST7, ST0)),

    /* 10h */
    TABLE_ENTRY("fcom2",    X86_MN_FCOM2,       PF_NONE,    X86_ISET_FPU,   OP(ST0)),
    TABLE_ENTRY("fcom2",    X86_MN_FCOM2,       PF_NONE,    X86_ISET_FPU,   OP(ST1)),
    TABLE_ENTRY("fcom2",    X86_MN_FCOM2,       PF_NONE,    X86_ISET_FPU,   OP(ST2)),
    TABLE_ENTRY("fcom2",    X86_MN_FCOM2,       PF_NONE,    X86_ISET_FPU,   OP(ST3)),
    TABLE_ENTRY("fcom2",    X86_MN_FCOM2,       PF_NONE,    X86_ISET_FPU,   OP(ST4)),
    TABLE_ENTRY("fcom2",    X86_MN_FCOM2,       PF_NONE,    X86_ISET_FPU,   OP(ST5)),
    TABLE_ENTRY("fcom2",    X86_MN_FCOM2,       PF_NONE,    X86_ISET_FPU,   OP(ST6)),
    TABLE_ENTRY("fcom2",    X86_MN_FCOM2,       PF_NONE,    X86_ISET_FPU,   OP(ST7)),

    /* 18h */
    TABLE_ENTRY("fcomp3",   X86_MN_FCOMP3,      PF_NONE,    X86_ISET_FPU,   OP(ST0)),
    TABLE_ENTRY("fcomp3",   X86_MN_FCOMP3,      PF_NONE,    X86_ISET_FPU,   OP(ST1)),
    TABLE_ENTRY("fcomp3",   X86_MN_FCOMP3,      PF_NONE,    X86_ISET_FPU,   OP(ST2)),
    TABLE_ENTRY("fcomp3",   X86_MN_FCOMP3,      PF_NONE,    X86_ISET_FPU,   OP(ST3)),
    TABLE_ENTRY("fcomp3",   X86_MN_FCOMP3,      PF_NONE,    X86_ISET_FPU,   OP(ST4)),
    TABLE_ENTRY("fcomp3",   X86_MN_FCOMP3,      PF_NONE,    X86_ISET_FPU,   OP(ST5)),
    TABLE_ENTRY("fcomp3",   X86_MN_FCOMP3,      PF_NONE,    X86_ISET_FPU,   OP(ST6)),
    TABLE_ENTRY("fcomp3",   X86_MN_FCOMP3,      PF_NONE,    X86_ISET_FPU,   OP(ST7)),

    /* 20h */
    TABLE_ENTRY("fsubr",    X86_MN_FSUBR,       PF_NONE,    X86_ISET_FPU,   OP(ST0, ST0)),
    TABLE_ENTRY("fsubr",    X86_MN_FSUBR,       PF_NONE,    X86_ISET_FPU,   OP(ST1, ST0)),
    TABLE_ENTRY("fsubr",    X86_MN_FSUBR,       PF_NONE,    X86_ISET_FPU,   OP(ST2, ST0)),
    TABLE_ENTRY("fsubr",    X86_MN_FSUBR,       PF_NONE,    X86_ISET_FPU,   OP(ST3, ST0)),
    TABLE_ENTRY("fsubr",    X86_MN_FSUBR,       PF_NONE,    X86_ISET_FPU,   OP(ST4, ST0)),
    TABLE_ENTRY("fsubr",    X86_MN_FSUBR,       PF_NONE,    X86_ISET_FPU,   OP(ST5, ST0)),
    TABLE_ENTRY("fsubr",    X86_MN_FSUBR,       PF_NONE,    X86_ISET_FPU,   OP(ST6, ST0)),
    TABLE_ENTRY("fsubr",    X86_MN_FSUBR,       PF_NONE,    X86_ISET_FPU,   OP(ST7, ST0)),

    /* 28h */
    TABLE_ENTRY("fsub",     X86_MN_FSUB,        PF_NONE,    X86_ISET_FPU,   OP(ST0, ST0)),
    TABLE_ENTRY("fsub",     X86_MN_FSUB,        PF_NONE,    X86_ISET_FPU,   OP(ST1, ST0)),
    TABLE_ENTRY("fsub",     X86_MN_FSUB,        PF_NONE,    X86_ISET_FPU,   OP(ST2, ST0)),
    TABLE_ENTRY("fsub",     X86_MN_FSUB,        PF_NONE,    X86_ISET_FPU,   OP(ST3, ST0)),
    TABLE_ENTRY("fsub",     X86_MN_FSUB,        PF_NONE,    X86_ISET_FPU,   OP(ST4, ST0)),
    TABLE_ENTRY("fsub",     X86_MN_FSUB,        PF_NONE,    X86_ISET_FPU,   OP(ST5, ST0)),
    TABLE_ENTRY("fsub",     X86_MN_FSUB,        PF_NONE,    X86_ISET_FPU,   OP(ST6, ST0)),
    TABLE_ENTRY("fsub",     X86_MN_FSUB,        PF_NONE,    X86_ISET_FPU,   OP(ST7, ST0)),

    /* 30h */
    TABLE_ENTRY("fdivr",    X86_MN_FDIVR,       PF_NONE,    X86_ISET_FPU,   OP(ST0, ST0)),
    TABLE_ENTRY("fdivr",    X86_MN_FDIVR,       PF_NONE,    X86_ISET_FPU,   OP(ST1, ST0)),
    TABLE_ENTRY("fdivr",    X86_MN_FDIVR,       PF_NONE,    X86_ISET_FPU,   OP(ST2, ST0)),
    TABLE_ENTRY("fdivr",    X86_MN_FDIVR,       PF_NONE,    X86_ISET_FPU,   OP(ST3, ST0)),
    TABLE_ENTRY("fdivr",    X86_MN_FDIVR,       PF_NONE,    X86_ISET_FPU,   OP(ST4, ST0)),
    TABLE_ENTRY("fdivr",    X86_MN_FDIVR,       PF_NONE,    X86_ISET_FPU,   OP(ST5, ST0)),
    TABLE_ENTRY("fdivr",    X86_MN_FDIVR,       PF_NONE,    X86_ISET_FPU,   OP(ST6, ST0)),
    TABLE_ENTRY("fdivr",    X86_MN_FDIVR,       PF_NONE,    X86_ISET_FPU,   OP(ST7, ST0)),

    /* 38h */
    TABLE_ENTRY("fdiv",     X86_MN_FDIV,        PF_NONE,    X86_ISET_FPU,   OP(ST0, ST0)),
    TABLE_ENTRY("fdiv",     X86_MN_FDIV,        PF_NONE,    X86_ISET_FPU,   OP(ST1, ST0)),
    TABLE_ENTRY("fdiv",     X86_MN_FDIV,        PF_NONE,    X86_ISET_FPU,   OP(ST2, ST0)),
    TABLE_ENTRY("fdiv",     X86_MN_FDIV,        PF_NONE,    X86_ISET_FPU,   OP(ST3, ST0)),
    TABLE_ENTRY("fdiv",     X86_MN_FDIV,        PF_NONE,    X86_ISET_FPU,   OP(ST4, ST0)),
    TABLE_ENTRY("fdiv",     X86_MN_FDIV,        PF_NONE,    X86_ISET_FPU,   OP(ST5, ST0)),
    TABLE_ENTRY("fdiv",     X86_MN_FDIV,        PF_NONE,    X86_ISET_FPU,   OP(ST6, ST0)),
    TABLE_ENTRY("fdiv",     X86_MN_FDIV,        PF_NONE,    X86_ISET_FPU,   OP(ST7, ST0))

TABLE_END(DC_MOD_11)

TABLE_BEGIN(DD_MOD_N11)

    /* modr/m byte reg/nnn field */
    TABLE_ENTRY("fld",      X86_MN_FLD,         PF_NONE,    X86_ISET_FPU,   OP(Mq)), /* 64-bit real (double-real) */ 
    TABLE_ENTRY("fisttp",   X86_MN_FISTTP,      PF_NONE,    X86_ISET_FPU,   OP(Mq)), /* integer64 */
    TABLE_ENTRY("fst",      X86_MN_FST,         PF_NONE,    X86_ISET_FPU,   OP(Mq)), /* 64-bit real (double-real) */
    TABLE_ENTRY("fstp",     X86_MN_FSTP,        PF_NONE,    X86_ISET_FPU,   OP(Mq)), /* 64-bit real (double-real) */
    TABLE_ENTRY("frstor",   X86_MN_FRSTOR,      PF_NONE,    X86_ISET_FPU,   OP(M)),  /* 98/108 bytes */
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY("fnsave",   X86_MN_FNSAVE,      PF_NONE,    X86_ISET_FPU,   OP(M)),  /* 98/108 bytes */
    TABLE_ENTRY("fnstsw",   X86_MN_FNSTSW,      PF_NONE,    X86_ISET_FPU,   OP(Mw))  /* 2 bytes -> word */

    /* 
        FNSAVE/FSAVE and FNSTSW/FSTSW with WAIT/FWAIT prefix same shit like 
        before. 
    */

TABLE_END(DD_MOD_N11)

TABLE_BEGIN(DD_MOD_11)

    /* modr/m byte:  index = modr/m byte - C0h */
    TABLE_ENTRY("ffree",    X86_MN_FFREE,       PF_NONE,    X86_ISET_FPU,   OP(ST0)),
    TABLE_ENTRY("ffree",    X86_MN_FFREE,       PF_NONE,    X86_ISET_FPU,   OP(ST1)),
    TABLE_ENTRY("ffree",    X86_MN_FFREE,       PF_NONE,    X86_ISET_FPU,   OP(ST2)),
    TABLE_ENTRY("ffree",    X86_MN_FFREE,       PF_NONE,    X86_ISET_FPU,   OP(ST3)),
    TABLE_ENTRY("ffree",    X86_MN_FFREE,       PF_NONE,    X86_ISET_FPU,   OP(ST4)),
    TABLE_ENTRY("ffree",    X86_MN_FFREE,       PF_NONE,    X86_ISET_FPU,   OP(ST5)),
    TABLE_ENTRY("ffree",    X86_MN_FFREE,       PF_NONE,    X86_ISET_FPU,   OP(ST6)),
    TABLE_ENTRY("ffree",    X86_MN_FFREE,       PF_NONE,    X86_ISET_FPU,   OP(ST7)),

    /* 08h */
    TABLE_ENTRY("fxch4",    X86_MN_FXCH4,       PF_NONE,    X86_ISET_FPU,   OP(ST0)),
    TABLE_ENTRY("fxch4",    X86_MN_FXCH4,       PF_NONE,    X86_ISET_FPU,   OP(ST1)),
    TABLE_ENTRY("fxch4",    X86_MN_FXCH4,       PF_NONE,    X86_ISET_FPU,   OP(ST2)),
    TABLE_ENTRY("fxch4",    X86_MN_FXCH4,       PF_NONE,    X86_ISET_FPU,   OP(ST3)),
    TABLE_ENTRY("fxch4",    X86_MN_FXCH4,       PF_NONE,    X86_ISET_FPU,   OP(ST4)),
    TABLE_ENTRY("fxch4",    X86_MN_FXCH4,       PF_NONE,    X86_ISET_FPU,   OP(ST5)),
    TABLE_ENTRY("fxch4",    X86_MN_FXCH4,       PF_NONE,    X86_ISET_FPU,   OP(ST6)),
    TABLE_ENTRY("fxch4",    X86_MN_FXCH4,       PF_NONE,    X86_ISET_FPU,   OP(ST7)),

    /* 10h */
    TABLE_ENTRY("fst",      X86_MN_FST,     PF_NONE,    X86_ISET_FPU,   OP(ST0)),
    TABLE_ENTRY("fst",      X86_MN_FST,     PF_NONE,    X86_ISET_FPU,   OP(ST1)),
    TABLE_ENTRY("fst",      X86_MN_FST,     PF_NONE,    X86_ISET_FPU,   OP(ST2)),
    TABLE_ENTRY("fst",      X86_MN_FST,     PF_NONE,    X86_ISET_FPU,   OP(ST3)),
    TABLE_ENTRY("fst",      X86_MN_FST,     PF_NONE,    X86_ISET_FPU,   OP(ST4)),
    TABLE_ENTRY("fst",      X86_MN_FST,     PF_NONE,    X86_ISET_FPU,   OP(ST5)),
    TABLE_ENTRY("fst",      X86_MN_FST,     PF_NONE,    X86_ISET_FPU,   OP(ST6)),
    TABLE_ENTRY("fst",      X86_MN_FST,     PF_NONE,    X86_ISET_FPU,   OP(ST7)),

    /* 18h */
    TABLE_ENTRY("fstp",     X86_MN_FSTP,        PF_NONE,    X86_ISET_FPU,   OP(ST0)),
    TABLE_ENTRY("fstp",     X86_MN_FSTP,        PF_NONE,    X86_ISET_FPU,   OP(ST1)),
    TABLE_ENTRY("fstp",     X86_MN_FSTP,        PF_NONE,    X86_ISET_FPU,   OP(ST2)),
    TABLE_ENTRY("fstp",     X86_MN_FSTP,        PF_NONE,    X86_ISET_FPU,   OP(ST3)),
    TABLE_ENTRY("fstp",     X86_MN_FSTP,        PF_NONE,    X86_ISET_FPU,   OP(ST4)),
    TABLE_ENTRY("fstp",     X86_MN_FSTP,        PF_NONE,    X86_ISET_FPU,   OP(ST5)),
    TABLE_ENTRY("fstp",     X86_MN_FSTP,        PF_NONE,    X86_ISET_FPU,   OP(ST6)),
    TABLE_ENTRY("fstp",     X86_MN_FSTP,        PF_NONE,    X86_ISET_FPU,   OP(ST7)),

    /* 20h */
    TABLE_ENTRY("fucom",    X86_MN_FUCOM,       PF_NONE,    X86_ISET_FPU,   OP(ST0, ST0)),
    TABLE_ENTRY("fucom",    X86_MN_FUCOM,       PF_NONE,    X86_ISET_FPU,   OP(ST1, ST0)),
    TABLE_ENTRY("fucom",    X86_MN_FUCOM,       PF_NONE,    X86_ISET_FPU,   OP(ST2, ST0)),
    TABLE_ENTRY("fucom",    X86_MN_FUCOM,       PF_NONE,    X86_ISET_FPU,   OP(ST3, ST0)),
    TABLE_ENTRY("fucom",    X86_MN_FUCOM,       PF_NONE,    X86_ISET_FPU,   OP(ST4, ST0)),
    TABLE_ENTRY("fucom",    X86_MN_FUCOM,       PF_NONE,    X86_ISET_FPU,   OP(ST5, ST0)),
    TABLE_ENTRY("fucom",    X86_MN_FUCOM,       PF_NONE,    X86_ISET_FPU,   OP(ST6, ST0)),
    TABLE_ENTRY("fucom",    X86_MN_FUCOM,       PF_NONE,    X86_ISET_FPU,   OP(ST7, ST0)),

    /* 28h */
    TABLE_ENTRY("fucomp",   X86_MN_FUCOMP,      PF_NONE,    X86_ISET_FPU,   OP(ST0, ST0)),
    TABLE_ENTRY("fucomp",   X86_MN_FUCOMP,      PF_NONE,    X86_ISET_FPU,   OP(ST1, ST0)),
    TABLE_ENTRY("fucomp",   X86_MN_FUCOMP,      PF_NONE,    X86_ISET_FPU,   OP(ST2, ST0)),
    TABLE_ENTRY("fucomp",   X86_MN_FUCOMP,      PF_NONE,    X86_ISET_FPU,   OP(ST3, ST0)),
    TABLE_ENTRY("fucomp",   X86_MN_FUCOMP,      PF_NONE,    X86_ISET_FPU,   OP(ST4, ST0)),
    TABLE_ENTRY("fucomp",   X86_MN_FUCOMP,      PF_NONE,    X86_ISET_FPU,   OP(ST5, ST0)),
    TABLE_ENTRY("fucomp",   X86_MN_FUCOMP,      PF_NONE,    X86_ISET_FPU,   OP(ST6, ST0)),
    TABLE_ENTRY("fucomp",   X86_MN_FUCOMP,      PF_NONE,    X86_ISET_FPU,   OP(ST7, ST0)),

    /* 30h */
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    /* 38h */
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID()

TABLE_END(DD_MOD_11)

TABLE_BEGIN(DE_MOD_N11)

    /* modr/m byte reg/nnn field */
    TABLE_ENTRY("fiadd",    X86_MN_FIADD,       PF_NONE,    X86_ISET_FPU,   OP(Mw)), /* word integer */
    TABLE_ENTRY("fimul",    X86_MN_FIMUL,       PF_NONE,    X86_ISET_FPU,   OP(Mw)), /* word integer */
    TABLE_ENTRY("ficom",    X86_MN_FICOM,       PF_NONE,    X86_ISET_FPU,   OP(Mw)), /* word integer */
    TABLE_ENTRY("ficomp",   X86_MN_FICOMP,      PF_NONE,    X86_ISET_FPU,   OP(Mw)), /* word integer */
    TABLE_ENTRY("fisub",    X86_MN_FISUB,       PF_NONE,    X86_ISET_FPU,   OP(Mw)), /* word integer */
    TABLE_ENTRY("fisubr",   X86_MN_FISUBR,      PF_NONE,    X86_ISET_FPU,   OP(Mw)), /* word integer */
    TABLE_ENTRY("fidiv",    X86_MN_FIDIV,       PF_NONE,    X86_ISET_FPU,   OP(Mw)), /* word integer */
    TABLE_ENTRY("fidivr",   X86_MN_FIDIVR,      PF_NONE,    X86_ISET_FPU,   OP(Mw))  /* word integer */

TABLE_END(DE_MOD_N11)

TABLE_BEGIN(DE_MOD_11)

    /* modr/m byte:  index = modr/m byte - C0h */
    TABLE_ENTRY("faddp",    X86_MN_FADDP,       PF_NONE,    X86_ISET_FPU,   OP(ST0, ST0)),
    TABLE_ENTRY("faddp",    X86_MN_FADDP,       PF_NONE,    X86_ISET_FPU,   OP(ST1, ST0)),
    TABLE_ENTRY("faddp",    X86_MN_FADDP,       PF_NONE,    X86_ISET_FPU,   OP(ST2, ST0)),
    TABLE_ENTRY("faddp",    X86_MN_FADDP,       PF_NONE,    X86_ISET_FPU,   OP(ST3, ST0)),
    TABLE_ENTRY("faddp",    X86_MN_FADDP,       PF_NONE,    X86_ISET_FPU,   OP(ST4, ST0)),
    TABLE_ENTRY("faddp",    X86_MN_FADDP,       PF_NONE,    X86_ISET_FPU,   OP(ST5, ST0)),
    TABLE_ENTRY("faddp",    X86_MN_FADDP,       PF_NONE,    X86_ISET_FPU,   OP(ST6, ST0)),
    TABLE_ENTRY("faddp",    X86_MN_FADDP,       PF_NONE,    X86_ISET_FPU,   OP(ST7, ST0)),

    /* 08h */
    TABLE_ENTRY("fmulp",    X86_MN_FMULP,       PF_NONE,    X86_ISET_FPU,   OP(ST0, ST0)),
    TABLE_ENTRY("fmulp",    X86_MN_FMULP,       PF_NONE,    X86_ISET_FPU,   OP(ST1, ST0)),
    TABLE_ENTRY("fmulp",    X86_MN_FMULP,       PF_NONE,    X86_ISET_FPU,   OP(ST2, ST0)),
    TABLE_ENTRY("fmulp",    X86_MN_FMULP,       PF_NONE,    X86_ISET_FPU,   OP(ST3, ST0)),
    TABLE_ENTRY("fmulp",    X86_MN_FMULP,       PF_NONE,    X86_ISET_FPU,   OP(ST4, ST0)),
    TABLE_ENTRY("fmulp",    X86_MN_FMULP,       PF_NONE,    X86_ISET_FPU,   OP(ST5, ST0)),
    TABLE_ENTRY("fmulp",    X86_MN_FMULP,       PF_NONE,    X86_ISET_FPU,   OP(ST6, ST0)),
    TABLE_ENTRY("fmulp",    X86_MN_FMULP,       PF_NONE,    X86_ISET_FPU,   OP(ST7, ST0)),

    /* 10h */
    TABLE_ENTRY("fcomp5",   X86_MN_FCOMP5,      PF_NONE,    X86_ISET_FPU,   OP(ST0)),
    TABLE_ENTRY("fcomp5",   X86_MN_FCOMP5,      PF_NONE,    X86_ISET_FPU,   OP(ST1)),
    TABLE_ENTRY("fcomp5",   X86_MN_FCOMP5,      PF_NONE,    X86_ISET_FPU,   OP(ST2)),
    TABLE_ENTRY("fcomp5",   X86_MN_FCOMP5,      PF_NONE,    X86_ISET_FPU,   OP(ST3)),
    TABLE_ENTRY("fcomp5",   X86_MN_FCOMP5,      PF_NONE,    X86_ISET_FPU,   OP(ST4)),
    TABLE_ENTRY("fcomp5",   X86_MN_FCOMP5,      PF_NONE,    X86_ISET_FPU,   OP(ST5)),
    TABLE_ENTRY("fcomp5",   X86_MN_FCOMP5,      PF_NONE,    X86_ISET_FPU,   OP(ST6)),
    TABLE_ENTRY("fcomp5",   X86_MN_FCOMP5,      PF_NONE,    X86_ISET_FPU,   OP(ST7)),

    /* 18h */
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY("fcompp",   X86_MN_FCOMPP,      PF_NONE,    X86_ISET_FPU,   OP(NONE)),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    /* 20h */
    TABLE_ENTRY("fsubrp",   X86_MN_FSUBRP,      PF_NONE,    X86_ISET_FPU,   OP(ST0, ST0)),
    TABLE_ENTRY("fsubrp",   X86_MN_FSUBRP,      PF_NONE,    X86_ISET_FPU,   OP(ST1, ST0)),
    TABLE_ENTRY("fsubrp",   X86_MN_FSUBRP,      PF_NONE,    X86_ISET_FPU,   OP(ST2, ST0)),
    TABLE_ENTRY("fsubrp",   X86_MN_FSUBRP,      PF_NONE,    X86_ISET_FPU,   OP(ST3, ST0)),
    TABLE_ENTRY("fsubrp",   X86_MN_FSUBRP,      PF_NONE,    X86_ISET_FPU,   OP(ST4, ST0)),
    TABLE_ENTRY("fsubrp",   X86_MN_FSUBRP,      PF_NONE,    X86_ISET_FPU,   OP(ST5, ST0)),
    TABLE_ENTRY("fsubrp",   X86_MN_FSUBRP,      PF_NONE,    X86_ISET_FPU,   OP(ST6, ST0)),
    TABLE_ENTRY("fsubrp",   X86_MN_FSUBRP,      PF_NONE,    X86_ISET_FPU,   OP(ST7, ST0)),

    /* 28h */
    TABLE_ENTRY("fsubp",    X86_MN_FSUBP,       PF_NONE,    X86_ISET_FPU,   OP(ST0, ST0)),
    TABLE_ENTRY("fsubp",    X86_MN_FSUBP,       PF_NONE,    X86_ISET_FPU,   OP(ST1, ST0)),
    TABLE_ENTRY("fsubp",    X86_MN_FSUBP,       PF_NONE,    X86_ISET_FPU,   OP(ST2, ST0)),
    TABLE_ENTRY("fsubp",    X86_MN_FSUBP,       PF_NONE,    X86_ISET_FPU,   OP(ST3, ST0)),
    TABLE_ENTRY("fsubp",    X86_MN_FSUBP,       PF_NONE,    X86_ISET_FPU,   OP(ST4, ST0)),
    TABLE_ENTRY("fsubp",    X86_MN_FSUBP,       PF_NONE,    X86_ISET_FPU,   OP(ST5, ST0)),
    TABLE_ENTRY("fsubp",    X86_MN_FSUBP,       PF_NONE,    X86_ISET_FPU,   OP(ST6, ST0)),
    TABLE_ENTRY("fsubp",    X86_MN_FSUBP,       PF_NONE,    X86_ISET_FPU,   OP(ST7, ST0)),

    /* 30h */
    TABLE_ENTRY("fdivrp",   X86_MN_FDIVRP,      PF_NONE,    X86_ISET_FPU,   OP(ST0, ST0)),
    TABLE_ENTRY("fdivrp",   X86_MN_FDIVRP,      PF_NONE,    X86_ISET_FPU,   OP(ST1, ST0)),
    TABLE_ENTRY("fdivrp",   X86_MN_FDIVRP,      PF_NONE,    X86_ISET_FPU,   OP(ST2, ST0)),
    TABLE_ENTRY("fdivrp",   X86_MN_FDIVRP,      PF_NONE,    X86_ISET_FPU,   OP(ST3, ST0)),
    TABLE_ENTRY("fdivrp",   X86_MN_FDIVRP,      PF_NONE,    X86_ISET_FPU,   OP(ST4, ST0)),
    TABLE_ENTRY("fdivrp",   X86_MN_FDIVRP,      PF_NONE,    X86_ISET_FPU,   OP(ST5, ST0)),
    TABLE_ENTRY("fdivrp",   X86_MN_FDIVRP,      PF_NONE,    X86_ISET_FPU,   OP(ST6, ST0)),
    TABLE_ENTRY("fdivrp",   X86_MN_FDIVRP,      PF_NONE,    X86_ISET_FPU,   OP(ST7, ST0)),

    /* 38h */
    TABLE_ENTRY("fdivp",    X86_MN_FDIVP,       PF_NONE,    X86_ISET_FPU,   OP(ST0, ST0)),
    TABLE_ENTRY("fdivp",    X86_MN_FDIVP,       PF_NONE,    X86_ISET_FPU,   OP(ST1, ST0)),
    TABLE_ENTRY("fdivp",    X86_MN_FDIVP,       PF_NONE,    X86_ISET_FPU,   OP(ST2, ST0)),
    TABLE_ENTRY("fdivp",    X86_MN_FDIVP,       PF_NONE,    X86_ISET_FPU,   OP(ST3, ST0)),
    TABLE_ENTRY("fdivp",    X86_MN_FDIVP,       PF_NONE,    X86_ISET_FPU,   OP(ST4, ST0)),
    TABLE_ENTRY("fdivp",    X86_MN_FDIVP,       PF_NONE,    X86_ISET_FPU,   OP(ST5, ST0)),
    TABLE_ENTRY("fdivp",    X86_MN_FDIVP,       PF_NONE,    X86_ISET_FPU,   OP(ST6, ST0)),
    TABLE_ENTRY("fdivp",    X86_MN_FDIVP,       PF_NONE,    X86_ISET_FPU,   OP(ST7, ST0))

TABLE_END(DE_MOD_11)

TABLE_BEGIN(DF_MOD_N11)

    /* modr/m byte reg/nnn field */
    TABLE_ENTRY("fild",     X86_MN_FILD,        PF_NONE,    X86_ISET_FPU,   OP(Mw)), /* word integer */
    TABLE_ENTRY("fisttp",   X86_MN_FISTTP,      PF_NONE,    X86_ISET_FPU,   OP(Mw)), /* word integer */
    TABLE_ENTRY("fist",     X86_MN_FIST,        PF_NONE,    X86_ISET_FPU,   OP(Mw)), /* word integer */
    TABLE_ENTRY("fistp",    X86_MN_FISTP,       PF_NONE,    X86_ISET_FPU,   OP(Mw)), /* word integer */
    TABLE_ENTRY("fbld",     X86_MN_FBLD,        PF_NONE,    X86_ISET_FPU,   OP(Mt)), /* 80-bit (packed-BCD) */
    TABLE_ENTRY("fild",     X86_MN_FILD,        PF_NONE,    X86_ISET_FPU,   OP(Mq)), /* qword integer */
    TABLE_ENTRY("fbstp",    X86_MN_FBSTP,       PF_NONE,    X86_ISET_FPU,   OP(Mt)), /* 80-bit (packed-BCD) */
    TABLE_ENTRY("fistp",    X86_MN_FISTP,       PF_NONE,    X86_ISET_FPU,   OP(Mq))  /* qword integer */

TABLE_END(DF_MOD_N11)

TABLE_BEGIN(DF_MOD_11)

    /* modr/m byte:  index = modr/m byte - C0h */
    TABLE_ENTRY("ffreep",   X86_MN_FFREEP,      PF_NONE,    X86_ISET_FPU,   OP(ST0)),
    TABLE_ENTRY("ffreep",   X86_MN_FFREEP,      PF_NONE,    X86_ISET_FPU,   OP(ST1)),
    TABLE_ENTRY("ffreep",   X86_MN_FFREEP,      PF_NONE,    X86_ISET_FPU,   OP(ST2)),
    TABLE_ENTRY("ffreep",   X86_MN_FFREEP,      PF_NONE,    X86_ISET_FPU,   OP(ST3)),
    TABLE_ENTRY("ffreep",   X86_MN_FFREEP,      PF_NONE,    X86_ISET_FPU,   OP(ST4)),
    TABLE_ENTRY("ffreep",   X86_MN_FFREEP,      PF_NONE,    X86_ISET_FPU,   OP(ST5)),
    TABLE_ENTRY("ffreep",   X86_MN_FFREEP,      PF_NONE,    X86_ISET_FPU,   OP(ST6)),
    TABLE_ENTRY("ffreep",   X86_MN_FFREEP,      PF_NONE,    X86_ISET_FPU,   OP(ST7)),

    /* 08h */
    TABLE_ENTRY("fxch7",    X86_MN_FXCH7,       PF_NONE,    X86_ISET_FPU,   OP(ST0)),
    TABLE_ENTRY("fxch7",    X86_MN_FXCH7,       PF_NONE,    X86_ISET_FPU,   OP(ST1)),
    TABLE_ENTRY("fxch7",    X86_MN_FXCH7,       PF_NONE,    X86_ISET_FPU,   OP(ST2)),
    TABLE_ENTRY("fxch7",    X86_MN_FXCH7,       PF_NONE,    X86_ISET_FPU,   OP(ST3)),
    TABLE_ENTRY("fxch7",    X86_MN_FXCH7,       PF_NONE,    X86_ISET_FPU,   OP(ST4)),
    TABLE_ENTRY("fxch7",    X86_MN_FXCH7,       PF_NONE,    X86_ISET_FPU,   OP(ST5)),
    TABLE_ENTRY("fxch7",    X86_MN_FXCH7,       PF_NONE,    X86_ISET_FPU,   OP(ST6)),
    TABLE_ENTRY("fxch7",    X86_MN_FXCH7,       PF_NONE,    X86_ISET_FPU,   OP(ST7)),

    /* 10h */
    TABLE_ENTRY("fstp8",    X86_MN_FSTP8,       PF_NONE,    X86_ISET_FPU,   OP(ST0)),
    TABLE_ENTRY("fstp8",    X86_MN_FSTP8,       PF_NONE,    X86_ISET_FPU,   OP(ST1)),
    TABLE_ENTRY("fstp8",    X86_MN_FSTP8,       PF_NONE,    X86_ISET_FPU,   OP(ST2)),
    TABLE_ENTRY("fstp8",    X86_MN_FSTP8,       PF_NONE,    X86_ISET_FPU,   OP(ST3)),
    TABLE_ENTRY("fstp8",    X86_MN_FSTP8,       PF_NONE,    X86_ISET_FPU,   OP(ST4)),
    TABLE_ENTRY("fstp8",    X86_MN_FSTP8,       PF_NONE,    X86_ISET_FPU,   OP(ST5)),
    TABLE_ENTRY("fstp8",    X86_MN_FSTP8,       PF_NONE,    X86_ISET_FPU,   OP(ST6)),
    TABLE_ENTRY("fstp8",    X86_MN_FSTP8,       PF_NONE,    X86_ISET_FPU,   OP(ST7)),

    /* 18h */
    TABLE_ENTRY("fstp9",    X86_MN_FSTP9,       PF_NONE,    X86_ISET_FPU,   OP(ST0)),
    TABLE_ENTRY("fstp9",    X86_MN_FSTP9,       PF_NONE,    X86_ISET_FPU,   OP(ST1)),
    TABLE_ENTRY("fstp9",    X86_MN_FSTP9,       PF_NONE,    X86_ISET_FPU,   OP(ST2)),
    TABLE_ENTRY("fstp9",    X86_MN_FSTP9,       PF_NONE,    X86_ISET_FPU,   OP(ST3)),
    TABLE_ENTRY("fstp9",    X86_MN_FSTP9,       PF_NONE,    X86_ISET_FPU,   OP(ST4)),
    TABLE_ENTRY("fstp9",    X86_MN_FSTP9,       PF_NONE,    X86_ISET_FPU,   OP(ST5)),
    TABLE_ENTRY("fstp9",    X86_MN_FSTP9,       PF_NONE,    X86_ISET_FPU,   OP(ST6)),
    TABLE_ENTRY("fstp9",    X86_MN_FSTP9,       PF_NONE,    X86_ISET_FPU,   OP(ST7)),

    /* 20h */
    TABLE_ENTRY("fnstsw",   X86_MN_FNSTSW,      PF_NONE,    X86_ISET_FPU,   OP(AX)),
    TABLE_ENTRY("fstdw",    X86_MN_FSTDW,       PF_NONE,    X86_ISET_FPU,   OP(AX)),
    TABLE_ENTRY("fstsg",    X86_MN_FSTSG,       PF_NONE,    X86_ISET_FPU,   OP(AX)),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    /* 28h */
    TABLE_ENTRY("fucomip",  X86_MN_FUCOMIP, PF_NONE,    X86_ISET_FPU,   OP(ST0, ST0)),
    TABLE_ENTRY("fucomip",  X86_MN_FUCOMIP, PF_NONE,    X86_ISET_FPU,   OP(ST0, ST1)),
    TABLE_ENTRY("fucomip",  X86_MN_FUCOMIP, PF_NONE,    X86_ISET_FPU,   OP(ST0, ST2)),
    TABLE_ENTRY("fucomip",  X86_MN_FUCOMIP, PF_NONE,    X86_ISET_FPU,   OP(ST0, ST3)),
    TABLE_ENTRY("fucomip",  X86_MN_FUCOMIP, PF_NONE,    X86_ISET_FPU,   OP(ST0, ST4)),
    TABLE_ENTRY("fucomip",  X86_MN_FUCOMIP, PF_NONE,    X86_ISET_FPU,   OP(ST0, ST5)),
    TABLE_ENTRY("fucomip",  X86_MN_FUCOMIP, PF_NONE,    X86_ISET_FPU,   OP(ST0, ST6)),
    TABLE_ENTRY("fucomip",  X86_MN_FUCOMIP, PF_NONE,    X86_ISET_FPU,   OP(ST0, ST7)),

    /* 30h */
    TABLE_ENTRY("fcomip",   X86_MN_FCOMIP,      PF_NONE,    X86_ISET_FPU,   OP(ST0, ST0)),
    TABLE_ENTRY("fcomip",   X86_MN_FCOMIP,      PF_NONE,    X86_ISET_FPU,   OP(ST0, ST1)),
    TABLE_ENTRY("fcomip",   X86_MN_FCOMIP,      PF_NONE,    X86_ISET_FPU,   OP(ST0, ST2)),
    TABLE_ENTRY("fcomip",   X86_MN_FCOMIP,      PF_NONE,    X86_ISET_FPU,   OP(ST0, ST3)),
    TABLE_ENTRY("fcomip",   X86_MN_FCOMIP,      PF_NONE,    X86_ISET_FPU,   OP(ST0, ST4)),
    TABLE_ENTRY("fcomip",   X86_MN_FCOMIP,      PF_NONE,    X86_ISET_FPU,   OP(ST0, ST5)),
    TABLE_ENTRY("fcomip",   X86_MN_FCOMIP,      PF_NONE,    X86_ISET_FPU,   OP(ST0, ST6)),
    TABLE_ENTRY("fcomip",   X86_MN_FCOMIP,      PF_NONE,    X86_ISET_FPU,   OP(ST0, ST7)),

    /* 38h */
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID()

TABLE_END(DF_MOD_11)

/****************************************************************************

    Two-byte tables - no mandatory prefix

****************************************************************************/

TABLE_BEGIN(0F)

    /* 00h: group 6, 7 */
    TABLE_ENTRY_ESCAPE(0F_00),
    TABLE_ENTRY_ESCAPE(0F_01),

    /* 02h */
    TABLE_ENTRY("lar",          X86_MN_LAR,     PF_PFXNA,   X86_ISET_GP,        OP(Gv, Ew)),
    TABLE_ENTRY("lsl",          X86_MN_LSL,     PF_PFXNA,   X86_ISET_GP,        OP(Gv, Ew)),

    /* 04h */
    TABLE_ENTRY_INVALID(),

    /* 05h */
    TABLE_ENTRY("syscall",      X86_MN_SYSCALL,     PF_O64 | PF_PFXNA,      X86_ISET_GP,        OP(NONE)),
    TABLE_ENTRY("clts",         X86_MN_CLTS,        PF_NONE | PF_PFXNA,     X86_ISET_GP,        OP(NONE)),
    TABLE_ENTRY("sysret",       X86_MN_SYSRET,      PF_O64 | PF_PFXNA,      X86_ISET_GP,        OP(NONE)),
    TABLE_ENTRY("invd",         X86_MN_INVD,        PF_NONE | PF_PFXNA,     X86_ISET_GP,        OP(NONE)),
    TABLE_ENTRY("wbinvd",       X86_MN_WBINVD,      PF_NONE | PF_PFXNA,     X86_ISET_GP,        OP(NONE)),

    /* 0Ah */
    TABLE_ENTRY_INVALID(),

    /* 0Bh */
    TABLE_ENTRY("ud2",          X86_MN_UD2,     PF_NONE | PF_PFXNA,     X86_ISET_GP,        OP(NONE)),

    /* 0Ch */
    TABLE_ENTRY_INVALID(),

    /* 0Dh: group p */
    TABLE_ENTRY_ESCAPE(0F_0D),

    /* 0Eh: AMD only */
    TABLE_ENTRY("femms",        X86_MN_FEMMS,   PF_NONE | PF_PFXNA,     X86_ISET_GP,        OP(NONE)),

    /* 0Fh: AMD only, 3DNow! escape */
    TABLE_ENTRY_ESCAPE(0F_0F),
    
    /* 10h */
    TABLE_ENTRY("movups",       X86_MN_MOVUPS,      PF_VEX,     X86_ISET_SSE,       OP(Vps, Wps)),
    TABLE_ENTRY("movups",       X86_MN_MOVUPS,      PF_VEX,     X86_ISET_SSE,       OP(Wps, Vps)),

    /* 12h: movlps or movhlps */
    TABLE_ENTRY_ESCAPE(0F_12),

    /* 13h */
    TABLE_ENTRY("movlps",       X86_MN_MOVLPS,      PF_VEX128,  X86_ISET_SSE,       OP(Mq, Vq)),
    TABLE_ENTRY("unpcklps",     X86_MN_UNPCKLPS,    PF_VEX,     X86_ISET_SSE,       OP(Vx, Hx, Wx)),
    TABLE_ENTRY("unpckhps",     X86_MN_UNPCKHPS,    PF_VEX,     X86_ISET_SSE,       OP(Vx, Hx, Wx)),

    /* 16h: movhps or movlhps */
    TABLE_ENTRY_ESCAPE(0F_16),

    /* 17h */
    TABLE_ENTRY("movhps",       X86_MN_MOVHPS,      PF_VEX128,  X86_ISET_SSE,       OP(Mq, Vq)),

    /* 18h: group 16 */
    TABLE_ENTRY_ESCAPE(0F_18),

    /* 19h */
    TABLE_ENTRY_ESCAPE(F2_0F_19),

    TABLE_ENTRY("nop",          X86_MN_NOP,     PF_NONE | PF_PFXNA, X86_ISET_GP,        OP(Ev)),
    TABLE_ENTRY("nop",          X86_MN_NOP,     PF_NONE | PF_PFXNA, X86_ISET_GP,        OP(Ev)),
    
    TABLE_ENTRY_ESCAPE(0F_1C),
    TABLE_ENTRY_ESCAPE(0F_1D),
    TABLE_ENTRY_ESCAPE(0F_1E),
    TABLE_ENTRY_ESCAPE(0F_1F),

    /* 20h */
    TABLE_ENTRY("mov",          X86_MN_MOV,     PF_F64 | PF_PFXNA,      X86_ISET_GP,        OP(Ry, Cy)),
    TABLE_ENTRY("mov",          X86_MN_MOV,     PF_F64 | PF_PFXNA,      X86_ISET_GP,        OP(Ry, Dy)),
    TABLE_ENTRY("mov",          X86_MN_MOV,     PF_F64 | PF_PFXNA,      X86_ISET_GP,        OP(Cy, Ry)),
    TABLE_ENTRY("mov",          X86_MN_MOV,     PF_F64 | PF_PFXNA,      X86_ISET_GP,        OP(Dy, Ry)),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY("movaps",       X86_MN_MOVAPS,      PF_VEX,     X86_ISET_SSE,       OP(Vps, Wps)),
    TABLE_ENTRY("movaps",       X86_MN_MOVAPS,      PF_VEX,     X86_ISET_SSE,       OP(Wps, Vps)),
    TABLE_ENTRY("cvtpi2ps",     X86_MN_CVTPI2PS,    PF_NONE,    X86_ISET_SSE,       OP(Vps, Qpi)),
    TABLE_ENTRY("movntps",      X86_MN_MOVNTPS,     PF_VEX,     X86_ISET_SSE,       OP(Mps, Vps)),
    TABLE_ENTRY("cvttps2pi",    X86_MN_CVTTPS2PI,   PF_NONE,    X86_ISET_SSE,       OP(Ppi, Wps)),
    TABLE_ENTRY("cvtps2pi",     X86_MN_CVTPS2PI,    PF_NONE,    X86_ISET_SSE,       OP(Ppi, Wps)),
    TABLE_ENTRY("ucomiss",      X86_MN_UCOMISS,     PF_VEX128,  X86_ISET_SSE,       OP(Vss, Wss)),
    TABLE_ENTRY("comiss",       X86_MN_COMISS,      PF_VEX128,  X86_ISET_SSE,       OP(Vss, Wss)),

    /* 30h */
    TABLE_ENTRY("wrmsr",        X86_MN_WRMSR,       PF_NONE | PF_PFXNA,     X86_ISET_GP,        OP(NONE)),
    TABLE_ENTRY("rdtsc",        X86_MN_RDTSC,       PF_NONE | PF_PFXNA,     X86_ISET_GP,        OP(NONE)),
    TABLE_ENTRY("rdmsr",        X86_MN_RDMSR,       PF_NONE | PF_PFXNA,     X86_ISET_GP,        OP(NONE)),
    TABLE_ENTRY("rdpmc",        X86_MN_RDPMC,       PF_NONE | PF_PFXNA,     X86_ISET_GP,        OP(NONE)),
    TABLE_ENTRY("sysenter",     X86_MN_SYSENTER,    PF_I64 | PF_PFXNA,      X86_ISET_GP,        OP(NONE)),
    TABLE_ENTRY("sysexit",      X86_MN_SYSEXIT,     PF_I64 | PF_PFXNA,      X86_ISET_GP,        OP(NONE)),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY("getsec",       X86_MN_GETSEC,      PF_NONE | PF_PFXNA,     X86_ISET_GP,        OP(NONE)),

    /* 38h: three-byte table escape */
    TABLE_ENTRY_ESCAPE(0F_38),
    TABLE_ENTRY_INVALID(),

    /* 3Ah: three-byte table escape */
    TABLE_ENTRY_ESCAPE(0F_3A),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    /* 40h */
    TABLE_ENTRY("cmovo",        X86_MN_CMOVO,       PF_NONE | PF_PFXNA, X86_ISET_GP,        OP(Gv, Ev)),
    TABLE_ENTRY("cmovno",       X86_MN_CMOVNO,      PF_NONE | PF_PFXNA, X86_ISET_GP,        OP(Gv, Ev)),
    TABLE_ENTRY("cmovc",        X86_MN_CMOVC,       PF_NONE | PF_PFXNA, X86_ISET_GP,        OP(Gv, Ev)),
    TABLE_ENTRY("cmovnc",       X86_MN_CMOVNC,      PF_NONE | PF_PFXNA, X86_ISET_GP,        OP(Gv, Ev)),
    TABLE_ENTRY("cmovz",        X86_MN_CMOVZ,       PF_NONE | PF_PFXNA, X86_ISET_GP,        OP(Gv, Ev)),
    TABLE_ENTRY("cmovnz",       X86_MN_CMOVNZ,      PF_NONE | PF_PFXNA, X86_ISET_GP,        OP(Gv, Ev)),
    TABLE_ENTRY("cmovna",       X86_MN_CMOVNA,      PF_NONE | PF_PFXNA, X86_ISET_GP,        OP(Gv, Ev)),
    TABLE_ENTRY("cmova",        X86_MN_CMOVA,       PF_NONE | PF_PFXNA, X86_ISET_GP,        OP(Gv, Ev)),
    TABLE_ENTRY("cmovs",        X86_MN_CMOVS,       PF_NONE | PF_PFXNA, X86_ISET_GP,        OP(Gv, Ev)),
    TABLE_ENTRY("cmovns",       X86_MN_CMOVNS,      PF_NONE | PF_PFXNA, X86_ISET_GP,        OP(Gv, Ev)),
    TABLE_ENTRY("cmovp",        X86_MN_CMOVP,       PF_NONE | PF_PFXNA, X86_ISET_GP,        OP(Gv, Ev)),
    TABLE_ENTRY("cmovnp",       X86_MN_CMOVNP,      PF_NONE | PF_PFXNA, X86_ISET_GP,        OP(Gv, Ev)),
    TABLE_ENTRY("cmovl",        X86_MN_CMOVL,       PF_NONE | PF_PFXNA, X86_ISET_GP,        OP(Gv, Ev)),
    TABLE_ENTRY("cmovnl",       X86_MN_CMOVNL,      PF_NONE | PF_PFXNA, X86_ISET_GP,        OP(Gv, Ev)),
    TABLE_ENTRY("cmovng",       X86_MN_CMOVNG,      PF_NONE | PF_PFXNA, X86_ISET_GP,        OP(Gv, Ev)),
    TABLE_ENTRY("cmovg",        X86_MN_CMOVG,       PF_NONE | PF_PFXNA, X86_ISET_GP,        OP(Gv, Ev)),

    /* 50h */
    TABLE_ENTRY("movmskps",     X86_MN_MOVMSKPS,    PF_VEX,     X86_ISET_SSE,       OP(Gy, Ups)),
    TABLE_ENTRY("sqrtps",       X86_MN_SQRTPS,      PF_VEX,     X86_ISET_SSE,       OP(Vps, Wps)),
    TABLE_ENTRY("rsqrtps",      X86_MN_RSQRTPS,     PF_VEX,     X86_ISET_SSE,       OP(Vps, Wps)),
    TABLE_ENTRY("rcpps",        X86_MN_RCPPS,       PF_VEX,     X86_ISET_SSE,       OP(Vps, Wps)),
    TABLE_ENTRY("andps",        X86_MN_ANDPS,       PF_VEX,     X86_ISET_SSE,       OP(Vps, Hps, Wps)),
    TABLE_ENTRY("andnps",       X86_MN_ANDNPS,      PF_VEX,     X86_ISET_SSE,       OP(Vps, Hps, Wps)),
    TABLE_ENTRY("orps",         X86_MN_ORPS,        PF_VEX,     X86_ISET_SSE,       OP(Vps, Hps, Wps)),
    TABLE_ENTRY("xorps",        X86_MN_XORPS,       PF_VEX,     X86_ISET_SSE,       OP(Vps, Hps, Wps)),
    TABLE_ENTRY("addps",        X86_MN_ADDPS,       PF_VEX,     X86_ISET_SSE,       OP(Vps, Hps, Wps)),
    TABLE_ENTRY("mulps",        X86_MN_MULPS,       PF_VEX,     X86_ISET_SSE,       OP(Vps, Hps, Wps)),
    TABLE_ENTRY("cvtps2pd",     X86_MN_CVTPS2PD,    PF_VEX,     X86_ISET_SSE2,      OP(Vpd, Wps)),
    TABLE_ENTRY("cvtdq2ps",     X86_MN_CVTDQ2PS,    PF_VEX,     X86_ISET_SSE2,      OP(Vps, Wdq)),
    TABLE_ENTRY("subps",        X86_MN_SUBPS,       PF_VEX,     X86_ISET_SSE,       OP(Vps, Hps, Wps)),
    TABLE_ENTRY("minps",        X86_MN_MINPS,       PF_VEX,     X86_ISET_SSE,       OP(Vps, Hps, Wps)),
    TABLE_ENTRY("divps",        X86_MN_DIVPS,       PF_VEX,     X86_ISET_SSE,       OP(Vps, Hps, Wps)),
    TABLE_ENTRY("maxps",        X86_MN_MAXPS,       PF_VEX,     X86_ISET_SSE,       OP(Vps, Hps, Wps)),

    /* 60h */
    TABLE_ENTRY("punpcklbw",    X86_MN_PUNPCKLBW,   PF_NONE,    X86_ISET_MMX,       OP(Pq, Qd)),
    TABLE_ENTRY("punpcklwd",    X86_MN_PUNPCKLWD,   PF_NONE,    X86_ISET_MMX,       OP(Pq, Qd)),
    TABLE_ENTRY("punpckldq",    X86_MN_PUNPCKLDQ,   PF_NONE,    X86_ISET_MMX,       OP(Pq, Qd)),
    TABLE_ENTRY("packsswb",     X86_MN_PACKSSWB,    PF_NONE,    X86_ISET_MMX,       OP(Pq, Qq)),
    TABLE_ENTRY("pcmpgtb",      X86_MN_PCMPGTB,     PF_NONE,    X86_ISET_MMX,       OP(Pq, Qq)),
    TABLE_ENTRY("pcmpgtw",      X86_MN_PCMPGTW,     PF_NONE,    X86_ISET_MMX,       OP(Pq, Qq)),
    TABLE_ENTRY("pcmpgtd",      X86_MN_PCMPGTD,     PF_NONE,    X86_ISET_MMX,       OP(Pq, Qq)),
    TABLE_ENTRY("packuswb",     X86_MN_PACKUSWB,    PF_NONE,    X86_ISET_MMX,       OP(Pq, Qq)),
    TABLE_ENTRY("punpckhbw",    X86_MN_PUNPCKHBW,   PF_NONE,    X86_ISET_MMX,       OP(Pq, Qd)),
    TABLE_ENTRY("punpckhwd",    X86_MN_PUNPCKHWD,   PF_NONE,    X86_ISET_MMX,       OP(Pq, Qd)),
    TABLE_ENTRY("punpckhdq",    X86_MN_PUNPCKHDQ,   PF_NONE,    X86_ISET_MMX,       OP(Pq, Qd)),
    TABLE_ENTRY("packssdw",     X86_MN_PACKSSDW,    PF_NONE,    X86_ISET_MMX,       OP(Pq, Qq)),

    /* 6Ch */
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    /* 6Eh */
    TABLE_ENTRY_ESCAPE(0F_6E),
    TABLE_ENTRY("movq",         X86_MN_MOVQ,        PF_NONE,    X86_ISET_MMX,       OP(Pq, Qq)),

    /* 70h */
    TABLE_ENTRY("pshufw",       X86_MN_PSHUFW,      PF_NONE,    X86_ISET_SSE,       OP(Pq, Qq, Ib)),

    /* 71h: group 12, 13, 14 */
    TABLE_ENTRY_ESCAPE(0F_71),
    TABLE_ENTRY_ESCAPE(0F_72),
    TABLE_ENTRY_ESCAPE(0F_73),

    /* 74h */
    TABLE_ENTRY("pcmpeqb",      X86_MN_PCMPEQB,     PF_NONE,    X86_ISET_MMX,       OP(Pq, Qq)),
    TABLE_ENTRY("pcmpeqw",      X86_MN_PCMPEQW,     PF_NONE,    X86_ISET_MMX,       OP(Pq, Qq)),
    TABLE_ENTRY("pcmpeqd",      X86_MN_PCMPEQD,     PF_NONE,    X86_ISET_MMX,       OP(Pq, Qq)),
    TABLE_ENTRY("emms",         X86_MN_EMMS,        PF_NONE,    X86_ISET_MMX,       OP(NONE)),
    TABLE_ENTRY("vmread",       X86_MN_VMREAD,      PF_NONE,    X86_ISET_VMX,       OP(Ey, Gy)),
    TABLE_ENTRY("vmwrite",      X86_MN_VMWRITE,     PF_NONE,    X86_ISET_VMX,       OP(Gy, Ey)),

    /* 7Ah */
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    /* 7Eh */
    TABLE_ENTRY_ESCAPE(0F_7E),
    TABLE_ENTRY("movq",         X86_MN_MOVQ,        PF_NONE,    X86_ISET_MMX,       OP(Qq, Pq)),

    /* 80h */
    TABLE_ENTRY("jo",           X86_MN_JO,          PF_F64 | PF_PFXNA,      X86_ISET_GP,        OP(Jz)),
    TABLE_ENTRY("jno",          X86_MN_JNO,         PF_F64 | PF_PFXNA,      X86_ISET_GP,        OP(Jz)),
    TABLE_ENTRY("jc",           X86_MN_JC,          PF_F64 | PF_PFXNA,      X86_ISET_GP,        OP(Jz)),
    TABLE_ENTRY("jnc",          X86_MN_JNC,         PF_F64 | PF_PFXNA,      X86_ISET_GP,        OP(Jz)),
    TABLE_ENTRY("jz",           X86_MN_JZ,          PF_F64 | PF_PFXNA,      X86_ISET_GP,        OP(Jz)),
    TABLE_ENTRY("jnz",          X86_MN_JNZ,         PF_F64 | PF_PFXNA,      X86_ISET_GP,        OP(Jz)),
    TABLE_ENTRY("jna",          X86_MN_JNA,         PF_F64 | PF_PFXNA,      X86_ISET_GP,        OP(Jz)),
    TABLE_ENTRY("ja",           X86_MN_JA,          PF_F64 | PF_PFXNA,      X86_ISET_GP,        OP(Jz)),
    TABLE_ENTRY("js",           X86_MN_JS,          PF_F64 | PF_PFXNA,      X86_ISET_GP,        OP(Jz)),
    TABLE_ENTRY("jns",          X86_MN_JNS,         PF_F64 | PF_PFXNA,      X86_ISET_GP,        OP(Jz)),
    TABLE_ENTRY("jp",           X86_MN_JP,          PF_F64 | PF_PFXNA,      X86_ISET_GP,        OP(Jz)),
    TABLE_ENTRY("jnp",          X86_MN_JNP,         PF_F64 | PF_PFXNA,      X86_ISET_GP,        OP(Jz)),
    TABLE_ENTRY("jl",           X86_MN_JL,          PF_F64 | PF_PFXNA,      X86_ISET_GP,        OP(Jz)),
    TABLE_ENTRY("jnl",          X86_MN_JNL,         PF_F64 | PF_PFXNA,      X86_ISET_GP,        OP(Jz)),
    TABLE_ENTRY("jng",          X86_MN_JNG,         PF_F64 | PF_PFXNA,      X86_ISET_GP,        OP(Jz)),
    TABLE_ENTRY("jg",           X86_MN_JG,          PF_F64 | PF_PFXNA,      X86_ISET_GP,        OP(Jz)),

    /* 90h */
    TABLE_ENTRY("seto",         X86_MN_SETO,        PF_NONE | PF_PFXNA,     X86_ISET_GP,    OP(Eb)),
    TABLE_ENTRY("setno",        X86_MN_SETNO,       PF_NONE | PF_PFXNA,     X86_ISET_GP,    OP(Eb)),
    TABLE_ENTRY("setc",         X86_MN_SETC,        PF_NONE | PF_PFXNA,     X86_ISET_GP,    OP(Eb)),
    TABLE_ENTRY("setnc",        X86_MN_SETNC,       PF_NONE | PF_PFXNA,     X86_ISET_GP,    OP(Eb)),
    TABLE_ENTRY("setz",         X86_MN_SETZ,        PF_NONE | PF_PFXNA,     X86_ISET_GP,    OP(Eb)),
    TABLE_ENTRY("setnz",        X86_MN_SETNZ,       PF_NONE | PF_PFXNA,     X86_ISET_GP,    OP(Eb)),
    TABLE_ENTRY("setna",        X86_MN_SETNA,       PF_NONE | PF_PFXNA,     X86_ISET_GP,    OP(Eb)),
    TABLE_ENTRY("seta",         X86_MN_SETA,        PF_NONE | PF_PFXNA,     X86_ISET_GP,    OP(Eb)),
    TABLE_ENTRY("sets",         X86_MN_SETS,        PF_NONE | PF_PFXNA,     X86_ISET_GP,    OP(Eb)),
    TABLE_ENTRY("setns",        X86_MN_SETNS,       PF_NONE | PF_PFXNA,     X86_ISET_GP,    OP(Eb)),
    TABLE_ENTRY("setp",         X86_MN_SETP,        PF_NONE | PF_PFXNA,     X86_ISET_GP,    OP(Eb)),
    TABLE_ENTRY("setnp",        X86_MN_SETNP,       PF_NONE | PF_PFXNA,     X86_ISET_GP,    OP(Eb)),
    TABLE_ENTRY("setl",         X86_MN_SETL,        PF_NONE | PF_PFXNA,     X86_ISET_GP,    OP(Eb)),
    TABLE_ENTRY("setnl",        X86_MN_SETNL,       PF_NONE | PF_PFXNA,     X86_ISET_GP,    OP(Eb)),
    TABLE_ENTRY("setng",        X86_MN_SETNG,       PF_NONE | PF_PFXNA,     X86_ISET_GP,    OP(Eb)),
    TABLE_ENTRY("setg",         X86_MN_SETG,        PF_NONE | PF_PFXNA,     X86_ISET_GP,    OP(Eb)),

    /* A0h */
    TABLE_ENTRY("push",         X86_MN_PUSH,        PF_D64 | PF_PFXNA,      X86_ISET_GP,        OP(FS)),
    TABLE_ENTRY("pop",          X86_MN_POP,         PF_D64 | PF_PFXNA,      X86_ISET_GP,        OP(FS)),
    TABLE_ENTRY("cpuid",        X86_MN_CPUID,       PF_NONE | PF_PFXNA,     X86_ISET_GP,        OP(NONE)),
    TABLE_ENTRY("bt",           X86_MN_BT,          PF_NONE | PF_PFXNA,     X86_ISET_GP,        OP(Ev, Gv)),
    TABLE_ENTRY("shld",         X86_MN_SHLD,        PF_NONE | PF_PFXNA,     X86_ISET_GP,        OP(Ev, Gv, Ib)),
    TABLE_ENTRY("shld",         X86_MN_SHLD,        PF_NONE | PF_PFXNA,     X86_ISET_GP,        OP(Ev, Gv, CL)), 

    /* A6h */
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    /* A8h */
    TABLE_ENTRY("push",         X86_MN_PUSH,        PF_D64 | PF_PFXNA,      X86_ISET_GP,        OP(GS)),
    TABLE_ENTRY("pop",          X86_MN_POP,         PF_D64 | PF_PFXNA,      X86_ISET_GP,        OP(GS)),
    TABLE_ENTRY("rsm",          X86_MN_RSM,         PF_NONE | PF_PFXNA,     X86_ISET_GP,        OP(NONE)),
    TABLE_ENTRY("bts",          X86_MN_BTS,         PF_LOCK | PF_PFXNA,     X86_ISET_GP,        OP(Ev, Gv)),
    TABLE_ENTRY("shrd",         X86_MN_SHRD,        PF_NONE | PF_PFXNA,     X86_ISET_GP,        OP(Ev, Gv, Ib)),
    TABLE_ENTRY("shrd",         X86_MN_SHRD,        PF_NONE | PF_PFXNA,     X86_ISET_GP,        OP(Ev, Gv, CL)),

    /* AEh: group 15 */
    TABLE_ENTRY_ESCAPE(0F_AE),

    /* AFh */
    TABLE_ENTRY("imul",         X86_MN_IMUL,        PF_NONE | PF_PFXNA,     X86_ISET_GP,        OP(Gv, Ev)),

    /* B0h */
    TABLE_ENTRY("cmpxchg",      X86_MN_CMPXCHG,     PF_LOCK | PF_PFXNA,     X86_ISET_GP,        OP(Eb, Gb)),
    TABLE_ENTRY("cmpxchg",      X86_MN_CMPXCHG,     PF_LOCK | PF_PFXNA,     X86_ISET_GP,        OP(Ev, Gv)),
    TABLE_ENTRY("lss",          X86_MN_LSS,         PF_NONE | PF_PFXNA,     X86_ISET_GP,        OP(Gv, Mp)),
    TABLE_ENTRY("btr",          X86_MN_BTR,         PF_LOCK | PF_PFXNA,     X86_ISET_GP,        OP(Ev, Gv)),
    TABLE_ENTRY("lfs",          X86_MN_LFS,         PF_NONE | PF_PFXNA,     X86_ISET_GP,        OP(Gv, Mp)),
    TABLE_ENTRY("lgs",          X86_MN_LGS,         PF_NONE | PF_PFXNA,     X86_ISET_GP,        OP(Gv, Mp)),
    TABLE_ENTRY("movzx",        X86_MN_MOVZX,       PF_NONE | PF_PFXNA,     X86_ISET_GP,        OP(Gv, Eb)),
    TABLE_ENTRY("movzx",        X86_MN_MOVZX,       PF_NONE | PF_PFXNA,     X86_ISET_GP,        OP(Gv, Ew)),

    /* B8h */
    TABLE_ENTRY_INVALID(),

    /* B9h: group 10, 8 */
    TABLE_ENTRY_ESCAPE(0F_B9),
    TABLE_ENTRY_ESCAPE(0F_BA),

    /* BBh */
    TABLE_ENTRY("btc",          X86_MN_BTC,         PF_LOCK | PF_PFXNA,     X86_ISET_GP,        OP(Ev, Gv)),
    TABLE_ENTRY("bsf",          X86_MN_BSF,         PF_NONE | PF_PFXNA,     X86_ISET_GP,        OP(Gv, Ev)),
    TABLE_ENTRY("bsr",          X86_MN_BSR,         PF_NONE | PF_PFXNA,     X86_ISET_GP,        OP(Gv, Ev)),
    TABLE_ENTRY("movsx",        X86_MN_MOVSX,       PF_NONE | PF_PFXNA,     X86_ISET_GP,        OP(Gv, Eb)),
    TABLE_ENTRY("movsx",        X86_MN_MOVSX,       PF_NONE | PF_PFXNA,     X86_ISET_GP,        OP(Gv, Ew)),

    /* C0h */
    TABLE_ENTRY("xadd",         X86_MN_XADD,        PF_LOCK | PF_PFXNA,     X86_ISET_GP,        OP(Eb, Gb)),
    TABLE_ENTRY("xadd",         X86_MN_XADD,        PF_LOCK | PF_PFXNA,     X86_ISET_GP,        OP(Ev, Gv)),
    TABLE_ENTRY("cmp*ps",       X86_MN_CMPPS,       PF_VEX | PF_CC,         X86_ISET_SSE,       OP(Vps, Hps, Wps, Ib)),
    TABLE_ENTRY("movnti",       X86_MN_MOVNTI,      PF_NONE,                X86_ISET_SSE2,      OP(My, Gy)),
    
    /* C4h */
    TABLE_ENTRY_ESCAPE(0F_C4),

    TABLE_ENTRY("pextrw",       X86_MN_PEXTRW,      PF_NONE,    X86_ISET_SSE,       OP(Gw, Nq, Ib)),
    TABLE_ENTRY("shufps",       X86_MN_SHUFPS,      PF_VEX,     X86_ISET_SSE,       OP(Vps, Hps, Wps, Ib)),

    /* C7h: group 9 */
    TABLE_ENTRY_ESCAPE(0F_C7),

    /* C8h */
    TABLE_ENTRY("bswap",        X86_MN_BSWAP,       PF_NONE | PF_PFXNA,     X86_ISET_GP,        OP(rAX_r8)),
    TABLE_ENTRY("bswap",        X86_MN_BSWAP,       PF_NONE | PF_PFXNA,     X86_ISET_GP,        OP(rCX_r9)),
    TABLE_ENTRY("bswap",        X86_MN_BSWAP,       PF_NONE | PF_PFXNA,     X86_ISET_GP,        OP(rDX_r10)),
    TABLE_ENTRY("bswap",        X86_MN_BSWAP,       PF_NONE | PF_PFXNA,     X86_ISET_GP,        OP(rBX_r11)),
    TABLE_ENTRY("bswap",        X86_MN_BSWAP,       PF_NONE | PF_PFXNA,     X86_ISET_GP,        OP(rSP_r12)),
    TABLE_ENTRY("bswap",        X86_MN_BSWAP,       PF_NONE | PF_PFXNA,     X86_ISET_GP,        OP(rBP_r13)),
    TABLE_ENTRY("bswap",        X86_MN_BSWAP,       PF_NONE | PF_PFXNA,     X86_ISET_GP,        OP(rSI_r14)),
    TABLE_ENTRY("bswap",        X86_MN_BSWAP,       PF_NONE | PF_PFXNA,     X86_ISET_GP,        OP(rDI_r15)),

    /* D0h */
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY("psrlw",        X86_MN_PSRLW,       PF_NONE,    X86_ISET_MMX,       OP(Pq, Qq)),
    TABLE_ENTRY("psrld",        X86_MN_PSRLD,       PF_NONE,    X86_ISET_MMX,       OP(Pq, Qq)),
    TABLE_ENTRY("psrlq",        X86_MN_PSRLQ,       PF_NONE,    X86_ISET_MMX,       OP(Pq, Qq)),
    TABLE_ENTRY("paddq",        X86_MN_PADDQ,       PF_NONE,    X86_ISET_SSE2,      OP(Pq, Qq)),
    TABLE_ENTRY("pmullw",       X86_MN_PMULLW,      PF_NONE,    X86_ISET_MMX,       OP(Pq, Qq)),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY("pmovmskb",     X86_MN_PMOVMSKB,    PF_NONE,    X86_ISET_SSE,       OP(Gd, Nq)),
    TABLE_ENTRY("psubusb",      X86_MN_PSUBUSB,     PF_NONE,    X86_ISET_MMX,       OP(Pq, Qq)),
    TABLE_ENTRY("psubusw",      X86_MN_PSUBUSW,     PF_NONE,    X86_ISET_MMX,       OP(Pq, Qq)),
    TABLE_ENTRY("pminub",       X86_MN_PMINUB,      PF_NONE,    X86_ISET_SSE,       OP(Pq, Qq)),
    TABLE_ENTRY("pand",         X86_MN_PAND,        PF_NONE,    X86_ISET_MMX,       OP(Pq, Qq)),
    TABLE_ENTRY("paddusb",      X86_MN_PADDUSB,     PF_NONE,    X86_ISET_MMX,       OP(Pq, Qq)),
    TABLE_ENTRY("paddusw",      X86_MN_PADDUSW,     PF_NONE,    X86_ISET_MMX,       OP(Pq, Qq)),
    TABLE_ENTRY("pmaxub",       X86_MN_PMAXUB,      PF_NONE,    X86_ISET_SSE,       OP(Pq, Qq)),
    TABLE_ENTRY("pandn",        X86_MN_PANDN,       PF_NONE,    X86_ISET_MMX,       OP(Pq, Qq)),

    /* E0h */
    TABLE_ENTRY("pavgb",        X86_MN_PAVGB,       PF_NONE,    X86_ISET_MMX,       OP(Pq, Qq)),
    TABLE_ENTRY("psraw",        X86_MN_PSRAW,       PF_NONE,    X86_ISET_MMX,       OP(Pq, Qq)),
    TABLE_ENTRY("psrad",        X86_MN_PSRAD,       PF_NONE,    X86_ISET_MMX,       OP(Pq, Qq)),
    TABLE_ENTRY("pavgw",        X86_MN_PAVGW,       PF_NONE,    X86_ISET_MMX,       OP(Pq, Qq)),
    TABLE_ENTRY("pmulhuw",      X86_MN_PMULHUW,     PF_NONE,    X86_ISET_SSE,       OP(Pq, Qq)),
    TABLE_ENTRY("pmulhw",       X86_MN_PMULHW,      PF_NONE,    X86_ISET_MMX,       OP(Pq, Qq)),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY("movntq",       X86_MN_MOVNTQ,      PF_NONE,    X86_ISET_SSE,       OP(Mq, Pq)),
    TABLE_ENTRY("psubsb",       X86_MN_PSUBSB,      PF_NONE,    X86_ISET_MMX,       OP(Pq, Qq)),
    TABLE_ENTRY("psubsw",       X86_MN_PSUBSW,      PF_NONE,    X86_ISET_MMX,       OP(Pq, Qq)),
    TABLE_ENTRY("pminsw",       X86_MN_PMINSW,      PF_NONE,    X86_ISET_SSE,       OP(Pq, Qq)),
    TABLE_ENTRY("por",          X86_MN_POR,         PF_NONE,    X86_ISET_MMX,       OP(Pq, Qq)),
    TABLE_ENTRY("paddsb",       X86_MN_PADDSB,      PF_NONE,    X86_ISET_MMX,       OP(Pq, Qq)),
    TABLE_ENTRY("paddsw",       X86_MN_PADDSW,      PF_NONE,    X86_ISET_MMX,       OP(Pq, Qq)),
    TABLE_ENTRY("pmaxsw",       X86_MN_PMAXSW,      PF_NONE,    X86_ISET_SSE,       OP(Pq, Qq)),
    TABLE_ENTRY("pxor",         X86_MN_PXOR,        PF_NONE,    X86_ISET_MMX,       OP(Pq, Qq)),

    /* F0h */
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY("psllw",        X86_MN_PSLLW,       PF_NONE,    X86_ISET_MMX,       OP(Pq, Qq)),
    TABLE_ENTRY("pslld",        X86_MN_PSLLD,       PF_NONE,    X86_ISET_MMX,       OP(Pq, Qq)),
    TABLE_ENTRY("psllq",        X86_MN_PSLLQ,       PF_NONE,    X86_ISET_MMX,       OP(Pq, Qq)),
    TABLE_ENTRY("pmuludq",      X86_MN_PMULUDQ,     PF_NONE,    X86_ISET_SSE2,      OP(Pq, Qq)),
    TABLE_ENTRY("pmaddwd",      X86_MN_PMADDWD,     PF_NONE,    X86_ISET_MMX,       OP(Pq, Qq)),
    TABLE_ENTRY("psadbw",       X86_MN_PSADBW,      PF_NONE,    X86_ISET_SSE,       OP(Pq, Qq)),
    TABLE_ENTRY("maskmovq",     X86_MN_MASKMOVQ,    PF_NONE,    X86_ISET_SSE,       OP(Pq, Qq)),
    TABLE_ENTRY("psubb",        X86_MN_PSUBB,       PF_NONE,    X86_ISET_MMX,       OP(Pq, Qq)),
    TABLE_ENTRY("psubw",        X86_MN_PSUBW,       PF_NONE,    X86_ISET_MMX,       OP(Pq, Qq)),
    TABLE_ENTRY("psubd",        X86_MN_PSUBD,       PF_NONE,    X86_ISET_MMX,       OP(Pq, Qq)),
    TABLE_ENTRY("psubq",        X86_MN_PSUBQ,       PF_NONE,    X86_ISET_SSE2,      OP(Pq, Qq)),
    TABLE_ENTRY("paddb",        X86_MN_PADDB,       PF_NONE,    X86_ISET_MMX,       OP(Pq, Qq)),
    TABLE_ENTRY("paddw",        X86_MN_PADDW,       PF_NONE,    X86_ISET_MMX,       OP(Pq, Qq)),
    TABLE_ENTRY("paddd",        X86_MN_PADDD,       PF_NONE,    X86_ISET_MMX,       OP(Pq, Qq)),
    TABLE_ENTRY("ud0",          X86_MN_UD,          PF_NONE,    X86_ISET_MMX,       OP(Pq, Qq))

TABLE_END(0F)

TABLE_BEGIN(0F_00)

    /* modr/m byte reg/nnn field */

    /* 00h: sldt Mw/Rv */
    TABLE_ENTRY_ESCAPE(0F_00_REG0),

    /* 01h: str Mw/Rv */
    TABLE_ENTRY_ESCAPE(0F_00_REG1),

    /* 02h */
    TABLE_ENTRY("lldt",         X86_MN_LLDT,        PF_NONE | PF_PFXNA, X86_ISET_GP,        OP(Ew)),
    TABLE_ENTRY("ltr",          X86_MN_LTR,         PF_NONE | PF_PFXNA, X86_ISET_GP,        OP(Ew)),
    TABLE_ENTRY("verr",         X86_MN_VERR,        PF_NONE | PF_PFXNA, X86_ISET_GP,        OP(Ew)),
    TABLE_ENTRY("verw",         X86_MN_VERW,        PF_NONE | PF_PFXNA, X86_ISET_GP,        OP(Ew)),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID()

TABLE_END(0F_00)

TABLE_BEGIN(0F_00_REG0)

    /* modr/m byte mod field:  0 = !11b, 1 = 11b */
    TABLE_ENTRY("sldt",         X86_MN_SLDT,        PF_NONE | PF_PFXNA, X86_ISET_GP,        OP(Mw)),
    TABLE_ENTRY("sldt",         X86_MN_SLDT,        PF_NONE | PF_PFXNA, X86_ISET_GP,        OP(Rv))

TABLE_END(0F_00_REG0)

TABLE_BEGIN(0F_00_REG1)

    /* modr/m byte mod field:  0 = !11b, 1 = 11b */
    TABLE_ENTRY("str",          X86_MN_STR,     PF_NONE | PF_PFXNA, X86_ISET_GP,        OP(Mw)),
    TABLE_ENTRY("str",          X86_MN_STR,     PF_NONE | PF_PFXNA, X86_ISET_GP,        OP(Rv))

TABLE_END(0F_00_REG1)

TABLE_BEGIN(0F_01)

    /* modr/m byte reg/nnn field */

    /* 00h: sgdt or r/m extension */
    TABLE_ENTRY_ESCAPE(0F_01_REG0),

    /* 01h: sidt or r/m extension */
    TABLE_ENTRY_ESCAPE(0F_01_REG1),

    /* 02h: lgdt or r/m extension */
    TABLE_ENTRY_ESCAPE(0F_01_REG2),

    /* 03h: lidt or r/m extension */
    TABLE_ENTRY_ESCAPE(0F_01_REG3),

    /* 04h: smsw Mw/Rv */
    TABLE_ENTRY_ESCAPE(0F_01_REG4),

    /* 05h */
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY("lmsw",         X86_MN_LMSW,        PF_NONE | PF_PFXNA, X86_ISET_GP,        OP(Ew)),

    /* 07h: invlpg or r/m extension */
    TABLE_ENTRY_ESCAPE(0F_01_REG7)

TABLE_END(0F_01)

TABLE_BEGIN(0F_01_REG0)

    /* modr/m byte mod field:  0 = !11b, 1 = 11b */
    TABLE_ENTRY("sgdt",         X86_MN_SGDT,        PF_NONE | PF_PFXNA, X86_ISET_GP,        OP(Ms)),

    /* 01h: r/m extension */
    TABLE_ENTRY_ESCAPE(0F_01_REG0_RM)

TABLE_END(0F_01_REG0)

TABLE_BEGIN(0F_01_REG1)

    /* modr/m byte mod field:  0 = !11b, 1 = 11b */
    TABLE_ENTRY("sidt",         X86_MN_SIDT,        PF_NONE | PF_PFXNA, X86_ISET_GP,        OP(Ms)),

    /* 01h: r/m extension */
    TABLE_ENTRY_ESCAPE(0F_01_REG1_RM)

TABLE_END(0F_01_REG1)

TABLE_BEGIN(0F_01_REG2)

    /* modr/m byte mod field:  0 = !11b, 1 = 11b */
    TABLE_ENTRY("lgdt",         X86_MN_LGDT,        PF_NONE | PF_PFXNA, X86_ISET_GP,        OP(Ms)),

    /* 01h: r/m extension */
    TABLE_ENTRY_ESCAPE(0F_01_REG2_RM)

TABLE_END(0F_01_REG2)

TABLE_BEGIN(0F_01_REG3)

    /* modr/m byte mod field:  0 = !11b, 1 = 11b */
    TABLE_ENTRY("lidt",         X86_MN_LIDT,        PF_NONE | PF_PFXNA, X86_ISET_GP,        OP(Ms)),

    /* 01h: r/m extension */
    TABLE_ENTRY_ESCAPE(0F_01_REG3_RM)

TABLE_END(0F_01_REG3)

TABLE_BEGIN(0F_01_REG4)

    /* modr/m byte mod field:  0 = !11b, 1 = 11b */
    TABLE_ENTRY("smsw",         X86_MN_SMSW,        PF_NONE | PF_PFXNA, X86_ISET_GP,        OP(Mw)),
    TABLE_ENTRY("smsw",         X86_MN_SMSW,        PF_NONE | PF_PFXNA, X86_ISET_GP,        OP(Rv))

TABLE_END(0F_01_REG4)

TABLE_BEGIN(0F_01_REG7)

    /* modr/m byte mod field:  0 = !11b, 1 = 11b */
    TABLE_ENTRY("invlpg",       X86_MN_INVLPG,      PF_NONE | PF_PFXNA, X86_ISET_GP,        OP(Mb)),
    
    /* 01h: r/m extension */
    TABLE_ENTRY_ESCAPE(0F_01_REG7_RM)

TABLE_END(0F_01_REG7)

TABLE_BEGIN(0F_01_REG0_RM)

    /* modr/m byte r/m field */
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY("vmcall",       X86_MN_VMCALL,      PF_NONE | PF_PFXNA, X86_ISET_VMX,       OP(NONE)),
    TABLE_ENTRY("vmlaunch",     X86_MN_VMLAUNCH,    PF_NONE | PF_PFXNA, X86_ISET_VMX,       OP(NONE)),
    TABLE_ENTRY("vmresume",     X86_MN_VMRESUME,    PF_NONE | PF_PFXNA, X86_ISET_VMX,       OP(NONE)),
    TABLE_ENTRY("vmxoff",       X86_MN_VMXOFF,      PF_NONE | PF_PFXNA, X86_ISET_VMX,       OP(NONE)),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID()

TABLE_END(0F_01_REG0_RM)

TABLE_BEGIN(0F_01_REG1_RM)

    /* modr/m byte r/m field */
    TABLE_ENTRY("monitor",      X86_MN_MONITOR,     PF_NONE | PF_PFXNA, X86_ISET_GP,        OP(NONE)),
    TABLE_ENTRY("mwait",        X86_MN_MWAIT,       PF_NONE | PF_PFXNA, X86_ISET_GP,        OP(NONE)),
    TABLE_ENTRY("clac",         X86_MN_CLAC,        PF_NONE | PF_PFXNA, X86_ISET_GP,        OP(NONE)),
    TABLE_ENTRY("stac",         X86_MN_STAC,        PF_NONE | PF_PFXNA, X86_ISET_GP,        OP(NONE)),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID()

TABLE_END(0F_01_REG1_RM)

TABLE_BEGIN(0F_01_REG2_RM)

    /* modr/m byte r/m field */
    TABLE_ENTRY("xgetbv",       X86_MN_XGETBV,      PF_NONE | PF_PFXNA, X86_ISET_GP,        OP(NONE)),
    TABLE_ENTRY("xsetbv",       X86_MN_XSETBV,      PF_NONE | PF_PFXNA, X86_ISET_GP,        OP(NONE)),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY("vmfunc",       X86_MN_VMFUNC,      PF_NONE | PF_PFXNA, X86_ISET_VMX,       OP(NONE)),
    TABLE_ENTRY("xend",         X86_MN_XEND,        PF_NONE | PF_PFXNA, X86_ISET_GP,        OP(NONE)),
    TABLE_ENTRY("xtest",        X86_MN_XTEST,       PF_NONE | PF_PFXNA, X86_ISET_GP,        OP(NONE)),
    TABLE_ENTRY_INVALID()

TABLE_END(0F_01_REG2_RM)

TABLE_BEGIN(0F_01_REG3_RM)

    /* modr/m byte r/m field */
    TABLE_ENTRY("vmrun",        X86_MN_VMRUN,       PF_NONE | PF_PFXNA, X86_ISET_SVM,       OP(NONE)),
    TABLE_ENTRY("vmcall",       X86_MN_VMCALL,      PF_NONE | PF_PFXNA, X86_ISET_SVM,       OP(NONE)),
    TABLE_ENTRY("vmload",       X86_MN_VMLOAD,      PF_NONE | PF_PFXNA, X86_ISET_SVM,       OP(NONE)),
    TABLE_ENTRY("vmsave",       X86_MN_VMSAVE,      PF_NONE | PF_PFXNA, X86_ISET_SVM,       OP(NONE)),
    TABLE_ENTRY("stgi",         X86_MN_STGI,        PF_NONE | PF_PFXNA, X86_ISET_SVM,       OP(NONE)),
    TABLE_ENTRY("clgi",         X86_MN_CLGI,        PF_NONE | PF_PFXNA, X86_ISET_SVM,       OP(NONE)),
    TABLE_ENTRY("skinit",       X86_MN_SKINIT,      PF_NONE | PF_PFXNA, X86_ISET_SVM,       OP(NONE)),
    TABLE_ENTRY("invlpga",      X86_MN_INVLPGA,     PF_NONE | PF_PFXNA, X86_ISET_SVM,       OP(NONE))

TABLE_END(0F_01_REG3_RM)

TABLE_BEGIN(0F_01_REG7_RM)

    /* modr/m byte r/m field */
    TABLE_ENTRY("swapgs",       X86_MN_SWAPGS,      PF_O64 | PF_PFXNA,      X86_ISET_GP,        OP(NONE)),
    TABLE_ENTRY("rdtscp",       X86_MN_RDTSCP,      PF_NONE | PF_PFXNA,     X86_ISET_GP,        OP(NONE)),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID()

TABLE_END(0F_01_REG7_RM)

TABLE_BEGIN(0F_0D)
    
    /* modr/m byte reg/nnn field */

    /* p group on amd docs, prefetchw Ev on intel docs; should check better */
    TABLE_ENTRY("prefetch",     X86_MN_PREFETCH,    PF_NONE | PF_PFXNA, X86_ISET_GP,        OP(M)),
    TABLE_ENTRY("prefetchw",    X86_MN_PREFETCHW,   PF_NONE | PF_PFXNA, X86_ISET_GP,        OP(Ev)),
    TABLE_ENTRY("prefetchwt1",  X86_MN_PREFETCHWT1, PF_NONE | PF_PFXNA, X86_ISET_GP,        OP(M)),
    TABLE_ENTRY("prefetch",     X86_MN_PREFETCH,    PF_NONE | PF_PFXNA, X86_ISET_GP,        OP(M)),
    TABLE_ENTRY("prefetch",     X86_MN_PREFETCH,    PF_NONE | PF_PFXNA, X86_ISET_GP,        OP(M)),
    TABLE_ENTRY("prefetch",     X86_MN_PREFETCH,    PF_NONE | PF_PFXNA, X86_ISET_GP,        OP(M)),
    TABLE_ENTRY("prefetch",     X86_MN_PREFETCH,    PF_NONE | PF_PFXNA, X86_ISET_GP,        OP(M)),
    TABLE_ENTRY("prefetch",     X86_MN_PREFETCH,    PF_NONE | PF_PFXNA, X86_ISET_GP,        OP(M))

TABLE_END(0F_0D)

TABLE_BEGIN(0F_12)

    /* modr/m byte mod field:  0 = !11b, 1 = 11b */
    TABLE_ENTRY("movlps",       X86_MN_MOVLPS,      PF_VEX128,  X86_ISET_SSE,       OP(Vq, Hq, Mq)),
    TABLE_ENTRY("movhlps",      X86_MN_MOVHLPS,     PF_VEX128,  X86_ISET_SSE,       OP(Vq, Hq, Uq))

TABLE_END(0F_12)

TABLE_BEGIN(0F_16)

    /* modr/m byte mod field:  0 = !11b, 1 = 11b */
    TABLE_ENTRY("movhps",       X86_MN_MOVHPS,      PF_VEX128,  X86_ISET_SSE,       OP(Vdq, Hq, Mq)),
    TABLE_ENTRY("movlhps",      X86_MN_MOVLHPS,     PF_VEX128,  X86_ISET_SSE,       OP(Vdq, Hq, Uq))

TABLE_END(0F_16)

TABLE_BEGIN(0F_18)

    /* modr/m byte reg/nnn field */

    /* 00h: prefetch NTA or invalid*/
    TABLE_ENTRY_ESCAPE(0F_18_REG0),

    /* 01h: prefetch T0 or invalid*/
    TABLE_ENTRY_ESCAPE(0F_18_REG1),

    /* 02h: prefetch T1 or invalid*/
    TABLE_ENTRY_ESCAPE(0F_18_REG2),

    /* 03h: prefetch T2 or invalid*/
    TABLE_ENTRY_ESCAPE(0F_18_REG3),

    TABLE_ENTRY("nop",      X86_MN_NOP,     PF_NONE | PF_PFXNA, X86_ISET_GP,        OP(Eb)),
    TABLE_ENTRY("nop",      X86_MN_NOP,     PF_NONE | PF_PFXNA, X86_ISET_GP,        OP(Eb)),
    TABLE_ENTRY("nop",      X86_MN_NOP,     PF_NONE | PF_PFXNA, X86_ISET_GP,        OP(Eb)),
    TABLE_ENTRY("nop",      X86_MN_NOP,     PF_NONE | PF_PFXNA, X86_ISET_GP,        OP(Eb))

TABLE_END(0F_18)

TABLE_BEGIN(0F_18_REG0)

    /* modr/m byte mod field:  0 = !11b, 1 = 11b */
    TABLE_ENTRY("prefetchnta",  X86_MN_PREFETCHNTA,     PF_NONE | PF_PFXNA, X86_ISET_SSE,   OP(Mb)),
    TABLE_ENTRY_INVALID()

TABLE_END(0F_18_REG0)

TABLE_BEGIN(0F_18_REG1)

    /* modr/m byte mod field:  0 = !11b, 1 = 11b */
    TABLE_ENTRY("prefetcht0",   X86_MN_PREFETCHT0,      PF_NONE | PF_PFXNA, X86_ISET_SSE,   OP(Mb)),
    TABLE_ENTRY_INVALID()

TABLE_END(0F_18_REG1)

TABLE_BEGIN(0F_18_REG2)

    /* modr/m byte mod field:  0 = !11b, 1 = 11b */
    TABLE_ENTRY("prefetcht1",   X86_MN_PREFETCHT1,      PF_NONE | PF_PFXNA, X86_ISET_SSE,   OP(Mb)),
    TABLE_ENTRY_INVALID()

TABLE_END(0F_18_REG2)

TABLE_BEGIN(0F_18_REG3)

    /* modr/m byte mod field:  0 = !11b, 1 = 11b */
    TABLE_ENTRY("prefetcht2",   X86_MN_PREFETCHT2,      PF_NONE | PF_PFXNA, X86_ISET_SSE,   OP(Mb)),
    TABLE_ENTRY_INVALID()

TABLE_END(0F_18_REG3)

TABLE_BEGIN(0F_19)

    /* modr/m byte reg/nnn field */
    TABLE_ENTRY("nop",  X86_MN_NOP,     PF_NONE | PF_PFXNA,         X86_ISET_GP, OP(Ev)),
    TABLE_ENTRY("nop",  X86_MN_NOP,     PF_NONE | PF_PFXNA,         X86_ISET_GP, OP(Ev)),
    TABLE_ENTRY("nop",  X86_MN_NOP,     PF_NONE | PF_PFXNA,         X86_ISET_GP, OP(Ev)),
    TABLE_ENTRY("nop",  X86_MN_NOP,     PF_NONE | PF_PFXNA,         X86_ISET_GP, OP(Ev)),
    TABLE_ENTRY("nop",  X86_MN_NOP,     PF_NONE | PF_PFXNA,         X86_ISET_GP, OP(Ev)),
    TABLE_ENTRY("nop",  X86_MN_NOP,     PF_NONE | PF_PFXNA,         X86_ISET_GP, OP(Ev)),
    TABLE_ENTRY("nop",  X86_MN_NOP,     PF_NONE | PF_PFXNA,         X86_ISET_GP, OP(Ev)),
    TABLE_ENTRY("nop",  X86_MN_NOP,     PF_NONE | PF_PFXNA,         X86_ISET_GP, OP(Ev))

TABLE_END(0F_19)

TABLE_BEGIN(0F_1C)

    /* modr/m byte reg/nnn field */
    TABLE_ENTRY("nop",  X86_MN_NOP,     PF_NONE | PF_PFXNA,         X86_ISET_GP, OP(Ev)),
    TABLE_ENTRY("nop",  X86_MN_NOP,     PF_NONE | PF_PFXNA,         X86_ISET_GP, OP(Ev)),
    TABLE_ENTRY("nop",  X86_MN_NOP,     PF_NONE | PF_PFXNA,         X86_ISET_GP, OP(Ev)),
    TABLE_ENTRY("nop",  X86_MN_NOP,     PF_NONE | PF_PFXNA,         X86_ISET_GP, OP(Ev)),
    TABLE_ENTRY("nop",  X86_MN_NOP,     PF_NONE | PF_PFXNA,         X86_ISET_GP, OP(Ev)),
    TABLE_ENTRY("nop",  X86_MN_NOP,     PF_NONE | PF_PFXNA,         X86_ISET_GP, OP(Ev)),
    TABLE_ENTRY("nop",  X86_MN_NOP,     PF_NONE | PF_PFXNA,         X86_ISET_GP, OP(Ev)),
    TABLE_ENTRY("nop",  X86_MN_NOP,     PF_NONE | PF_PFXNA,         X86_ISET_GP, OP(Ev))

TABLE_END(0F_1C)

TABLE_BEGIN(0F_1D)

    /* modr/m byte reg/nnn field */
    TABLE_ENTRY("nop",  X86_MN_NOP,     PF_NONE | PF_PFXNA,         X86_ISET_GP, OP(Ev)),
    TABLE_ENTRY("nop",  X86_MN_NOP,     PF_NONE | PF_PFXNA,         X86_ISET_GP, OP(Ev)),
    TABLE_ENTRY("nop",  X86_MN_NOP,     PF_NONE | PF_PFXNA,         X86_ISET_GP, OP(Ev)),
    TABLE_ENTRY("nop",  X86_MN_NOP,     PF_NONE | PF_PFXNA,         X86_ISET_GP, OP(Ev)),
    TABLE_ENTRY("nop",  X86_MN_NOP,     PF_NONE | PF_PFXNA,         X86_ISET_GP, OP(Ev)),
    TABLE_ENTRY("nop",  X86_MN_NOP,     PF_NONE | PF_PFXNA,         X86_ISET_GP, OP(Ev)),
    TABLE_ENTRY("nop",  X86_MN_NOP,     PF_NONE | PF_PFXNA,         X86_ISET_GP, OP(Ev)),
    TABLE_ENTRY("nop",  X86_MN_NOP,     PF_NONE | PF_PFXNA,         X86_ISET_GP, OP(Ev))

TABLE_END(0F_1D)

TABLE_BEGIN(0F_1E)

    /* modr/m byte reg/nnn field */
    TABLE_ENTRY("nop",  X86_MN_NOP,     PF_NONE | PF_PFXNA,         X86_ISET_GP, OP(Ev)),
    TABLE_ENTRY("nop",  X86_MN_NOP,     PF_NONE | PF_PFXNA,         X86_ISET_GP, OP(Ev)),
    TABLE_ENTRY("nop",  X86_MN_NOP,     PF_NONE | PF_PFXNA,         X86_ISET_GP, OP(Ev)),
    TABLE_ENTRY("nop",  X86_MN_NOP,     PF_NONE | PF_PFXNA,         X86_ISET_GP, OP(Ev)),
    TABLE_ENTRY("nop",  X86_MN_NOP,     PF_NONE | PF_PFXNA,         X86_ISET_GP, OP(Ev)),
    TABLE_ENTRY("nop",  X86_MN_NOP,     PF_NONE | PF_PFXNA,         X86_ISET_GP, OP(Ev)),
    TABLE_ENTRY("nop",  X86_MN_NOP,     PF_NONE | PF_PFXNA,         X86_ISET_GP, OP(Ev)),
    TABLE_ENTRY("nop",  X86_MN_NOP,     PF_NONE | PF_PFXNA,         X86_ISET_GP, OP(Ev))

TABLE_END(0F_1E)

TABLE_BEGIN(0F_1F)

    /* modr/m byte reg/nnn field */
    TABLE_ENTRY("nop",  X86_MN_NOP,     PF_NONE | PF_PFXNA,         X86_ISET_GP, OP(Ev)),
    TABLE_ENTRY("nop",  X86_MN_NOP,     PF_NONE | PF_PFXNA,         X86_ISET_GP, OP(Ev)),
    TABLE_ENTRY("nop",  X86_MN_NOP,     PF_NONE | PF_PFXNA,         X86_ISET_GP, OP(Ev)),
    TABLE_ENTRY("nop",  X86_MN_NOP,     PF_NONE | PF_PFXNA,         X86_ISET_GP, OP(Ev)),
    TABLE_ENTRY("nop",  X86_MN_NOP,     PF_NONE | PF_PFXNA,         X86_ISET_GP, OP(Ev)),
    TABLE_ENTRY("nop",  X86_MN_NOP,     PF_NONE | PF_PFXNA,         X86_ISET_GP, OP(Ev)),
    TABLE_ENTRY("nop",  X86_MN_NOP,     PF_NONE | PF_PFXNA,         X86_ISET_GP, OP(Ev)),
    TABLE_ENTRY("nop",  X86_MN_NOP,     PF_NONE | PF_PFXNA,         X86_ISET_GP, OP(Ev))

TABLE_END(0F_1F)

TABLE_BEGIN(0F_6E)

    /* operand size: 0 = 16-bits, 1 = 32-bits, 2 = 64-bits */
    TABLE_ENTRY("movd",         X86_MN_MOVD,            PF_NONE,    X86_ISET_MMX,   OP(Py, Ey)),
    TABLE_ENTRY("movd",         X86_MN_MOVD,            PF_NONE,    X86_ISET_MMX,   OP(Py, Ey)),
    TABLE_ENTRY("movq",         X86_MN_MOVQ,            PF_NONE,    X86_ISET_MMX,   OP(Py, Ey))

TABLE_END(0F_6E)

TABLE_BEGIN(0F_71)

    /* modr/m byte reg/nnn field */

    /* 00h */
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    /* 02h: psrlw or invalid*/
    TABLE_ENTRY_ESCAPE(0F_71_REG2),
    
    TABLE_ENTRY_INVALID(),
    
    /* 04h: psraw or invalid*/
    TABLE_ENTRY_ESCAPE(0F_71_REG4),

    TABLE_ENTRY_INVALID(),
    
    /* 06h: psllw or invalid*/
    TABLE_ENTRY_ESCAPE(0F_71_REG6),

    TABLE_ENTRY_INVALID()

TABLE_END(0F_71)

TABLE_BEGIN(0F_71_REG2)

    /* modr/m byte mod field:  0 = !11b, 1 = 11b */
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY("psrlw",        X86_MN_PSRLW,       PF_NONE,        X86_ISET_MMX,   OP(Nq, Ib))

TABLE_END(0F_71_REG2)

TABLE_BEGIN(0F_71_REG4)

    /* modr/m byte mod field:  0 = !11b, 1 = 11b */
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY("psraw",        X86_MN_PSRAW,       PF_NONE,        X86_ISET_MMX,   OP(Nq, Ib))

TABLE_END(0F_71_REG4)

TABLE_BEGIN(0F_71_REG6)

    /* modr/m byte mod field:  0 = !11b, 1 = 11b */
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY("psllw",        X86_MN_PSLLW,       PF_NONE,        X86_ISET_MMX,   OP(Nq, Ib))

TABLE_END(0F_71_REG6)

TABLE_BEGIN(0F_72)

    /* modr/m byte reg/nnn field */

    /* 00h */
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    /* 02h: psrld or invalid*/
    TABLE_ENTRY_ESCAPE(0F_72_REG2),

    TABLE_ENTRY_INVALID(),

    /* 04h: psrad or invalid*/
    TABLE_ENTRY_ESCAPE(0F_72_REG4),
    
    TABLE_ENTRY_INVALID(),

    /* 06h: pslld or invalid*/
    TABLE_ENTRY_ESCAPE(0F_72_REG6),
    
    TABLE_ENTRY_INVALID()

TABLE_END(0F_72)

TABLE_BEGIN(0F_72_REG2)

    /* modr/m byte mod field:  0 = !11b, 1 = 11b */
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY("psrld",        X86_MN_PSRLD,       PF_NONE,        X86_ISET_MMX,   OP(Nq, Ib))

TABLE_END(0F_72_REG2)

TABLE_BEGIN(0F_72_REG4)

    /* modr/m byte mod field:  0 = !11b, 1 = 11b */
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY("psrad",        X86_MN_PSRAD,       PF_NONE,        X86_ISET_MMX,   OP(Nq, Ib))

TABLE_END(0F_72_REG4)

TABLE_BEGIN(0F_72_REG6)

    /* modr/m byte mod field:  0 = !11b, 1 = 11b */
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY("pslld",        X86_MN_PSLLD,       PF_NONE,        X86_ISET_MMX,   OP(Nq, Ib))

TABLE_END(0F_72_REG6)

TABLE_BEGIN(0F_73)

    /* modr/m byte reg/nnn field */

    /* 00h */
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    /* 02h: psrlq or invalid*/
    TABLE_ENTRY_ESCAPE(0F_73_REG2),

    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    /* 06h: psllq or invalid*/
    TABLE_ENTRY_ESCAPE(0F_73_REG6),
    
    TABLE_ENTRY_INVALID()

TABLE_END(0F_73)

TABLE_BEGIN(0F_73_REG2)

    /* modr/m byte mod field:  0 = !11b, 1 = 11b */
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY("psrlq",        X86_MN_PSRLQ,       PF_NONE,        X86_ISET_MMX,   OP(Nq, Ib))

TABLE_END(0F_73_REG2)

TABLE_BEGIN(0F_73_REG6)

    /* modr/m byte mod field:  0 = !11b, 1 = 11b */
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY("psllq",        X86_MN_PSLLQ,       PF_NONE,        X86_ISET_MMX,   OP(Nq, Ib))

TABLE_END(0F_73_REG6)

TABLE_BEGIN(0F_7E)

    /* operand size: 0 = 16-bits, 1 = 32-bits, 2 = 64-bits */
    TABLE_ENTRY("movd",         X86_MN_MOVD,        PF_NONE,        X86_ISET_MMX,   OP(Ey, Py)),
    TABLE_ENTRY("movd",         X86_MN_MOVD,        PF_NONE,        X86_ISET_MMX,   OP(Ey, Py)),
    TABLE_ENTRY("movq",         X86_MN_MOVQ,        PF_NONE,        X86_ISET_MMX,   OP(Ey, Py))

TABLE_END(0F_7E)

TABLE_BEGIN(0F_AE)

    /* modr/m byte reg/nnn field */

    /* 00h: fxsave or invalid*/
    TABLE_ENTRY_ESCAPE(0F_AE_REG0),

    /* 01h: fxrstor or invalid*/
    TABLE_ENTRY_ESCAPE(0F_AE_REG1),

    /* 02h: ldmxcsr or invalid*/
    TABLE_ENTRY_ESCAPE(0F_AE_REG2),

    /* 03h: stmxcsr or invalid*/
    TABLE_ENTRY_ESCAPE(0F_AE_REG3),

    /* 04h: xsave or invalid*/
    TABLE_ENTRY_ESCAPE(0F_AE_REG4),

    /* 05h: xrstor or lfence*/
    TABLE_ENTRY_ESCAPE(0F_AE_REG5),

    /* 06h: xsaveopt or mfence*/
    TABLE_ENTRY_ESCAPE(0F_AE_REG6),

    /* 07h: clflush or sfence*/
    TABLE_ENTRY_ESCAPE(0F_AE_REG7)

TABLE_END(0F_AE)

TABLE_BEGIN(0F_AE_REG0)

    /* modr/m byte mod field:  0 = !11b, 1 = 11b */
    TABLE_ENTRY("fxsave",       X86_MN_FXSAVE,      PF_NONE,        X86_ISET_FPU,   OP(M)), /* 512-byte (+fxsave64 with rex.w) */
    TABLE_ENTRY_INVALID()

TABLE_END(0F_AE_REG0)

TABLE_BEGIN(0F_AE_REG1)

    /* modr/m byte mod field:  0 = !11b, 1 = 11b */
    TABLE_ENTRY("fxrstor",      X86_MN_FXRSTOR,     PF_NONE,        X86_ISET_FPU,   OP(M)), /* 512-byte (+fxrstor64 with rex.w) */
    TABLE_ENTRY_INVALID()

TABLE_END(0F_AE_REG1)

TABLE_BEGIN(0F_AE_REG2)

    /* modr/m byte mod field:  0 = !11b, 1 = 11b */
    TABLE_ENTRY("ldmxcsr",      X86_MN_LDMXCSR,     PF_VEX,         X86_ISET_SSE,   OP(Md)),
    TABLE_ENTRY_INVALID()

TABLE_END(0F_AE_REG2)

TABLE_BEGIN(0F_AE_REG3)

    /* modr/m byte mod field:  0 = !11b, 1 = 11b */
    TABLE_ENTRY("stmxcsr",      X86_MN_STMXCSR,     PF_VEX,         X86_ISET_SSE,   OP(Md)),
    TABLE_ENTRY_INVALID()

TABLE_END(0F_AE_REG3)

TABLE_BEGIN(0F_AE_REG4)

    /* modr/m byte mod field:  0 = !11b, 1 = 11b */
    TABLE_ENTRY("xsave",        X86_MN_XSAVE,       PF_NONE,        X86_ISET_SSE,   OP(M)), /* mem (+xsave64 with rex.w) */
    TABLE_ENTRY_INVALID()

TABLE_END(0F_AE_REG4)

TABLE_BEGIN(0F_AE_REG5)

    /* modr/m byte mod field:  0 = !11b, 1 = 11b */
    TABLE_ENTRY("xrstor",       X86_MN_XRSTOR,      PF_NONE,        X86_ISET_SSE,   OP(M)), /* mem (+xrstor64 with rex.w) */
    TABLE_ENTRY("lfence",       X86_MN_LFENCE,      PF_NONE,        X86_ISET_GP,    OP(NONE))

TABLE_END(0F_AE_REG5)

TABLE_BEGIN(0F_AE_REG6)

    /* modr/m byte mod field:  0 = !11b, 1 = 11b */
    TABLE_ENTRY("xsaveopt",     X86_MN_XSAVEOPT,    PF_NONE,        X86_ISET_SSE,   OP(M)), /* mem (+xsaveopt64 with rex.w) */
    TABLE_ENTRY("mfence",       X86_MN_MFENCE,      PF_NONE,        X86_ISET_GP,    OP(NONE))

TABLE_END(0F_AE_REG6)

TABLE_BEGIN(0F_AE_REG7)

    /* modr/m byte mod field:  0 = !11b, 1 = 11b */
    TABLE_ENTRY("clflush",      X86_MN_CLFLUSH,     PF_NONE,        X86_ISET_SSE2,  OP(Mb)), /* 8-bit */
    TABLE_ENTRY("sfence",       X86_MN_SFENCE,      PF_NONE,        X86_ISET_SSE,   OP(NONE))

TABLE_END(0F_AE_REG7)

TABLE_BEGIN(0F_B9)

    /* modr/m byte reg/nnn field */

    /* 00h */
    TABLE_ENTRY("ud1",          X86_MN_UD1,     PF_NONE | PF_PFXNA,     X86_ISET_GP,    OP(NONE)),
    TABLE_ENTRY("ud1",          X86_MN_UD1,     PF_NONE | PF_PFXNA,     X86_ISET_GP,    OP(NONE)),
    TABLE_ENTRY("ud1",          X86_MN_UD1,     PF_NONE | PF_PFXNA,     X86_ISET_GP,    OP(NONE)),
    TABLE_ENTRY("ud1",          X86_MN_UD1,     PF_NONE | PF_PFXNA,     X86_ISET_GP,    OP(NONE)),
    TABLE_ENTRY("ud1",          X86_MN_UD1,     PF_NONE | PF_PFXNA,     X86_ISET_GP,    OP(NONE)),
    TABLE_ENTRY("ud1",          X86_MN_UD1,     PF_NONE | PF_PFXNA,     X86_ISET_GP,    OP(NONE)),
    TABLE_ENTRY("ud1",          X86_MN_UD1,     PF_NONE | PF_PFXNA,     X86_ISET_GP,    OP(NONE)),
    TABLE_ENTRY("ud1",          X86_MN_UD1,     PF_NONE | PF_PFXNA,     X86_ISET_GP,    OP(NONE))

TABLE_END(0F_B9)

TABLE_BEGIN(0F_BA)

    /* modr/m byte reg/nnn field */

    /* 00h */
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY("bt",           X86_MN_BT,          PF_NONE | PF_PFXNA,     X86_ISET_GP,    OP(Ev, Ib)),
    TABLE_ENTRY("bts",          X86_MN_BTS,         PF_LOCK | PF_PFXNA,     X86_ISET_GP,    OP(Ev, Ib)),
    TABLE_ENTRY("btr",          X86_MN_BTR,         PF_LOCK | PF_PFXNA,     X86_ISET_GP,    OP(Ev, Ib)),
    TABLE_ENTRY("btc",          X86_MN_BTC,         PF_LOCK | PF_PFXNA,     X86_ISET_GP,    OP(Ev, Ib))

TABLE_END(0F_BA)

TABLE_BEGIN(0F_C4)

    /* modr/m byte mod field:  0 = !11b, 1 = 11b */
    TABLE_ENTRY("pinsrw",       X86_MN_PINSRW,      PF_NONE,        X86_ISET_SSE,   OP(Pq, Mw, Ib)),
    TABLE_ENTRY("pinsrw",       X86_MN_PINSRW,      PF_NONE,        X86_ISET_SSE,   OP(Pq, Ry, Ib))

TABLE_END(0F_C4)

TABLE_BEGIN(0F_C7)

    /* modr/m byte reg/nnn field */

    /* 00h */
    TABLE_ENTRY_INVALID(),

    /* cmpxchg8b or 16b depending on operand size */
    TABLE_ENTRY_ESCAPE(0F_C7_REG1),

    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_ESCAPE(0F_C7_REG3),
    TABLE_ENTRY_ESCAPE(0F_C7_REG4),
    TABLE_ENTRY_ESCAPE(0F_C7_REG5),
    
    /* vmptrld or rdrand */
    TABLE_ENTRY_ESCAPE(0F_C7_REG6),

    /* vmptrst or rdseed */
    TABLE_ENTRY_ESCAPE(0F_C7_REG7)

TABLE_END(0F_C7)

TABLE_BEGIN(0F_C7_REG1)

    /* operand size: 0 = 16-bits, 1 = 32-bits, 2 = 64-bits */
    TABLE_ENTRY("cmpxchg8b",    X86_MN_CMPXCHG8B,       PF_LOCK,    X86_ISET_GP,    OP(Mq)),
    TABLE_ENTRY("cmpxchg8b",    X86_MN_CMPXCHG8B,       PF_LOCK,    X86_ISET_GP,    OP(Mq)),
    TABLE_ENTRY("cmpxchg16b",   X86_MN_CMPXCHG16B,      PF_LOCK,    X86_ISET_GP,    OP(Mdq))

TABLE_END(0F_C7_REG1)

TABLE_BEGIN(0F_C7_REG3)

    /* operand size: 0 = 16-bits, 1 = 32-bits, 2 = 64-bits */
    TABLE_ENTRY("xrstors",      X86_MN_XRSTORS,     PF_NONE,    X86_ISET_UNKNOWN,   OP(M)), 
    TABLE_ENTRY("xrstors",      X86_MN_XRSTORS,     PF_NONE,    X86_ISET_UNKNOWN,   OP(M)), 
    TABLE_ENTRY("xrstors64",    X86_MN_XRSTORS64,   PF_NONE,    X86_ISET_UNKNOWN,   OP(M))

TABLE_END(0F_C7_REG3)

TABLE_BEGIN(0F_C7_REG4)

    /* operand size: 0 = 16-bits, 1 = 32-bits, 2 = 64-bits */
    TABLE_ENTRY("xsavec",       X86_MN_XSAVEC,      PF_NONE,    X86_ISET_UNKNOWN,   OP(M)), 
    TABLE_ENTRY("xsavec",       X86_MN_XSAVEC,      PF_NONE,    X86_ISET_UNKNOWN,   OP(M)), 
    TABLE_ENTRY("xsavec64",     X86_MN_XSAVEC64,    PF_NONE,    X86_ISET_UNKNOWN,   OP(M))

TABLE_END(0F_C7_REG4)

TABLE_BEGIN(0F_C7_REG5)

    /* operand size: 0 = 16-bits, 1 = 32-bits, 2 = 64-bits */
    TABLE_ENTRY("xsaves",       X86_MN_XSAVES,      PF_NONE,    X86_ISET_UNKNOWN,   OP(M)), 
    TABLE_ENTRY("xsaves",       X86_MN_XSAVES,      PF_NONE,    X86_ISET_UNKNOWN,   OP(M)), 
    TABLE_ENTRY("xsaves64",     X86_MN_XSAVES64,    PF_NONE,    X86_ISET_UNKNOWN,   OP(M))

TABLE_END(0F_C7_REG5)

TABLE_BEGIN(0F_C7_REG6)

    /* modr/m byte mod field:  0 = !11b, 1 = 11b */
    TABLE_ENTRY("vmptrld",      X86_MN_VMPTRLD,         PF_NONE | PF_PFXNA, X86_ISET_VMX,   OP(Mq)),
    TABLE_ENTRY("rdrand",       X86_MN_RDRAND,          PF_NONE | PF_PFXNA, X86_ISET_GP,    OP(Rv))

TABLE_END(0F_C7_REG6)

TABLE_BEGIN(0F_C7_REG7)

    /* modr/m byte mod field:  0 = !11b, 1 = 11b */
    TABLE_ENTRY("vmptrst",      X86_MN_VMPTRST,         PF_NONE | PF_PFXNA, X86_ISET_VMX,   OP(Mq)),
    TABLE_ENTRY("rdseed",       X86_MN_RDSEED,          PF_NONE | PF_PFXNA, X86_ISET_GP,    OP(Rv))

TABLE_END(0F_C7_REG7)

/****************************************************************************

    Two-byte tables - 66h mandatory prefix

****************************************************************************/

TABLE_BEGIN(66_0F)

    /* 00h: group 6, 7; both groups have prefix n/a */
    TABLE_ENTRY_ESCAPE(0F_00),
    TABLE_ENTRY_ESCAPE(0F_01),

    /* 02h */
    TABLE_ENTRY("lar",          X86_MN_LAR,     PF_NONE | PF_PFXNA,     X86_ISET_GP,    OP(Gv, Ew)),
    TABLE_ENTRY("lsl",          X86_MN_LSL,     PF_NONE | PF_PFXNA,     X86_ISET_GP,    OP(Gv, Ew)),

    /* 04h */
    TABLE_ENTRY_INVALID(),

    /* 05h */
    TABLE_ENTRY("syscall",      X86_MN_SYSCALL,     PF_O64 | PF_PFXNA,      X86_ISET_GP,    OP(NONE)),
    TABLE_ENTRY("clts",         X86_MN_CLTS,        PF_NONE | PF_PFXNA,     X86_ISET_GP,    OP(NONE)),
    TABLE_ENTRY("sysret",       X86_MN_SYSRET,      PF_O64 | PF_PFXNA,      X86_ISET_GP,    OP(NONE)),
    TABLE_ENTRY("invd",         X86_MN_INVD,        PF_NONE | PF_PFXNA,     X86_ISET_GP,    OP(NONE)),
    TABLE_ENTRY("wbinvd",       X86_MN_WBINVD,      PF_NONE | PF_PFXNA,     X86_ISET_GP,    OP(NONE)),

    /* 0Ah */
    TABLE_ENTRY_INVALID(),

    /* 0Bh */
    TABLE_ENTRY("ud2",          X86_MN_UD2,         PF_NONE | PF_PFXNA,     X86_ISET_GP,    OP(NONE)),

    /* 0Ch */
    TABLE_ENTRY_INVALID(),

    /* 0Dh: group p; group has prefix n/a */
    TABLE_ENTRY_ESCAPE(0F_0D),

    /* 0Eh */
    TABLE_ENTRY("femms",        X86_MN_FEMMS,       PF_NONE | PF_PFXNA,     X86_ISET_GP,    OP(NONE)),

    /* 0Fh: AMD only, 3DNow! escape */
    TABLE_ENTRY_INVALID(),
    
    /* 10h */
    TABLE_ENTRY("movupd",       X86_MN_MOVUPD,      PF_VEX,         X86_ISET_SSE2,  OP(Vpd, Wpd)),
    TABLE_ENTRY("movupd",       X86_MN_MOVUPD,      PF_VEX,         X86_ISET_SSE2,  OP(Wpd, Vpd)),

    /* 12h */
    TABLE_ENTRY("movlpd",       X86_MN_MOVLPD,      PF_VEX128,      X86_ISET_SSE2,  OP(Vq, Hq, Mq)),
    TABLE_ENTRY("movlpd",       X86_MN_MOVLPD,      PF_VEX128,      X86_ISET_SSE2,  OP(Mq, Vq)),
    TABLE_ENTRY("unpcklpd",     X86_MN_UNPCKLPD,    PF_VEX,         X86_ISET_SSE2,  OP(Vx, Hx, Wx)),
    TABLE_ENTRY("unpckhpd",     X86_MN_UNPCKHPD,    PF_VEX,         X86_ISET_SSE2,  OP(Vx, Hx, Wx)),
    TABLE_ENTRY("movhpd",       X86_MN_MOVHPD,      PF_VEX128,      X86_ISET_SSE2,  OP(Vdq, Hq, Mq)),
    TABLE_ENTRY("movhpd",       X86_MN_MOVHPD,      PF_VEX128,      X86_ISET_SSE2,  OP(Mq, Vq)),

    /* 18h: group 16; group has prefix n/a */
    TABLE_ENTRY_ESCAPE(0F_18),

    /* 19h */
    TABLE_ENTRY_ESCAPE(66_0F_19),

    TABLE_ENTRY("nop",          X86_MN_NOP,     PF_NONE | PF_PFXNA,     X86_ISET_GP,    OP(Ev)),
    TABLE_ENTRY("nop",          X86_MN_NOP,     PF_NONE | PF_PFXNA,     X86_ISET_GP,    OP(Ev)),

    TABLE_ENTRY_ESCAPE(66_0F_1C),
    TABLE_ENTRY_ESCAPE(66_0F_1D),
    TABLE_ENTRY_ESCAPE(66_0F_1E),
    TABLE_ENTRY_ESCAPE(66_0F_1F),

    /* 20h */
    TABLE_ENTRY("mov",          X86_MN_MOV,     PF_F64 | PF_PFXNA,          X86_ISET_GP,    OP(Ry, Cy)),
    TABLE_ENTRY("mov",          X86_MN_MOV,     PF_F64 | PF_PFXNA,          X86_ISET_GP,    OP(Ry, Dy)),
    TABLE_ENTRY("mov",          X86_MN_MOV,     PF_F64 | PF_PFXNA,          X86_ISET_GP,    OP(Cy, Ry)),
    TABLE_ENTRY("mov",          X86_MN_MOV,     PF_F64 | PF_PFXNA,          X86_ISET_GP,    OP(Dy, Ry)),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    
    /* 28h */
    TABLE_ENTRY("movapd",       X86_MN_MOVAPD,      PF_VEX,         X86_ISET_SSE2,  OP(Vpd, Wpd)),
    TABLE_ENTRY("movapd",       X86_MN_MOVAPD,      PF_VEX,         X86_ISET_SSE2,  OP(Wpd, Vpd)),
    TABLE_ENTRY("cvtpi2pd",     X86_MN_CVTPI2PD,    PF_NONE,        X86_ISET_SSE2,  OP(Vpd, Qpi)),
    TABLE_ENTRY("movntpd",      X86_MN_MOVNTPD,     PF_VEX,         X86_ISET_SSE,   OP(Mpd, Vpd)),
    TABLE_ENTRY("cvttpd2pi",    X86_MN_CVTTPD2PI,   PF_NONE,        X86_ISET_SSE2,  OP(Ppi, Wpd)),
    TABLE_ENTRY("cvtpd2pi",     X86_MN_CVTPD2PI,    PF_NONE,        X86_ISET_SSE2,  OP(Ppi, Wpd)),
    TABLE_ENTRY("ucomisd",      X86_MN_UCOMISD,     PF_VEX128,      X86_ISET_SSE2,  OP(Vsd, Wsd)),
    TABLE_ENTRY("comisd",       X86_MN_COMISD,      PF_VEX128,      X86_ISET_SSE2,  OP(Vsd, Wsd)),

    /* 30h */
    TABLE_ENTRY("wrmsr",        X86_MN_WRMSR,       PF_NONE | PF_PFXNA,     X86_ISET_GP,    OP(NONE)),
    TABLE_ENTRY("rdtsc",        X86_MN_RDTSC,       PF_NONE | PF_PFXNA,     X86_ISET_GP,    OP(NONE)),
    TABLE_ENTRY("rdmsr",        X86_MN_RDMSR,       PF_NONE | PF_PFXNA,     X86_ISET_GP,    OP(NONE)),
    TABLE_ENTRY("rdpmc",        X86_MN_RDPMC,       PF_NONE | PF_PFXNA,     X86_ISET_GP,    OP(NONE)),
    TABLE_ENTRY("sysenter",     X86_MN_SYSENTER,    PF_NONE | PF_PFXNA,     X86_ISET_GP,    OP(NONE)),
    TABLE_ENTRY("sysexit",      X86_MN_SYSEXIT,     PF_NONE | PF_PFXNA,     X86_ISET_GP,    OP(NONE)),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    /* 38h: three-byte table escape */
    TABLE_ENTRY_ESCAPE(66_0F_38),
    TABLE_ENTRY_INVALID(),

    /* 3Ah: three-byte table escape */
    TABLE_ENTRY_ESCAPE(66_0F_3A),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    /* 40h */
    TABLE_ENTRY("cmovo",        X86_MN_CMOVO,       PF_NONE | PF_PFXNA, X86_ISET_GP,        OP(Gv, Ev)),
    TABLE_ENTRY("cmovno",       X86_MN_CMOVNO,      PF_NONE | PF_PFXNA, X86_ISET_GP,        OP(Gv, Ev)),
    TABLE_ENTRY("cmovc",        X86_MN_CMOVC,       PF_NONE | PF_PFXNA, X86_ISET_GP,        OP(Gv, Ev)),
    TABLE_ENTRY("cmovnc",       X86_MN_CMOVNC,      PF_NONE | PF_PFXNA, X86_ISET_GP,        OP(Gv, Ev)),
    TABLE_ENTRY("cmovz",        X86_MN_CMOVZ,       PF_NONE | PF_PFXNA, X86_ISET_GP,        OP(Gv, Ev)),
    TABLE_ENTRY("cmovnz",       X86_MN_CMOVNZ,      PF_NONE | PF_PFXNA, X86_ISET_GP,        OP(Gv, Ev)),
    TABLE_ENTRY("cmovna",       X86_MN_CMOVNA,      PF_NONE | PF_PFXNA, X86_ISET_GP,        OP(Gv, Ev)),
    TABLE_ENTRY("cmova",        X86_MN_CMOVA,       PF_NONE | PF_PFXNA, X86_ISET_GP,        OP(Gv, Ev)),
    TABLE_ENTRY("cmovs",        X86_MN_CMOVS,       PF_NONE | PF_PFXNA, X86_ISET_GP,        OP(Gv, Ev)),
    TABLE_ENTRY("cmovns",       X86_MN_CMOVNS,      PF_NONE | PF_PFXNA, X86_ISET_GP,        OP(Gv, Ev)),
    TABLE_ENTRY("cmovp",        X86_MN_CMOVP,       PF_NONE | PF_PFXNA, X86_ISET_GP,        OP(Gv, Ev)),
    TABLE_ENTRY("cmovnp",       X86_MN_CMOVPO,      PF_NONE | PF_PFXNA, X86_ISET_GP,        OP(Gv, Ev)),
    TABLE_ENTRY("cmovl",        X86_MN_CMOVL,       PF_NONE | PF_PFXNA, X86_ISET_GP,        OP(Gv, Ev)),
    TABLE_ENTRY("cmovnl",       X86_MN_CMOVNL,      PF_NONE | PF_PFXNA, X86_ISET_GP,        OP(Gv, Ev)),
    TABLE_ENTRY("cmovng",       X86_MN_CMOVNG,      PF_NONE | PF_PFXNA, X86_ISET_GP,        OP(Gv, Ev)),
    TABLE_ENTRY("cmovg",        X86_MN_CMOVG,       PF_NONE | PF_PFXNA, X86_ISET_GP,        OP(Gv, Ev)),

    /* 50h */
    TABLE_ENTRY("movmskpd",     X86_MN_MOVMSKPD,    PF_VEX,         X86_ISET_SSE2,  OP(Gd, Upd)),
    TABLE_ENTRY("sqrtpd",       X86_MN_SQRTPD,      PF_VEX,         X86_ISET_SSE2,  OP(Vpd, Wpd)),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY("andpd",        X86_MN_ANDPD,       PF_VEX,         X86_ISET_SSE,   OP(Vpd, Hpd, Wpd)),
    TABLE_ENTRY("andnpd",       X86_MN_ANDNPD,      PF_VEX,         X86_ISET_SSE2,  OP(Vpd, Hpd, Wpd)),
    TABLE_ENTRY("orpd",         X86_MN_ORPD,        PF_VEX,         X86_ISET_SSE2,  OP(Vpd, Hpd, Wpd)),
    TABLE_ENTRY("xorpd",        X86_MN_XORPD,       PF_VEX,         X86_ISET_SSE2,  OP(Vpd, Hpd, Wpd)),
    TABLE_ENTRY("addpd",        X86_MN_ADDPD,       PF_VEX,         X86_ISET_SSE2,  OP(Vpd, Hpd, Wpd)),
    TABLE_ENTRY("mulpd",        X86_MN_MULPD,       PF_VEX,         X86_ISET_SSE2,  OP(Vpd, Hpd, Wpd)),
    TABLE_ENTRY("cvtpd2ps",     X86_MN_CVTPD2PS,    PF_VEX,         X86_ISET_SSE2,  OP(Vps, Wpd)),
    TABLE_ENTRY("cvtps2dq",     X86_MN_CVTPS2DQ,    PF_VEX,         X86_ISET_SSE2,  OP(Vdq, Wps)),
    TABLE_ENTRY("subpd",        X86_MN_SUBPD,       PF_VEX,         X86_ISET_SSE2,  OP(Vpd, Hpd, Wpd)),
    TABLE_ENTRY("minpd",        X86_MN_MINPD,       PF_VEX,         X86_ISET_SSE2,  OP(Vpd, Hpd, Wpd)),
    TABLE_ENTRY("divpd",        X86_MN_DIVPD,       PF_VEX,         X86_ISET_SSE2,  OP(Vpd, Hpd, Wpd)),
    TABLE_ENTRY("maxpd",        X86_MN_MAXPD,       PF_VEX,         X86_ISET_SSE2,  OP(Vpd, Hpd, Wpd)),

    /* 60h */
    TABLE_ENTRY("punpcklbw",    X86_MN_PUNPCKLBW,   PF_VEX,         X86_ISET_MMX,   OP(Vx, Hx, Wx)),
    TABLE_ENTRY("punpcklwd",    X86_MN_PUNPCKLWD,   PF_VEX,         X86_ISET_MMX,   OP(Vx, Hx, Wx)),
    TABLE_ENTRY("punpckldq",    X86_MN_PUNPCKLDQ,   PF_VEX,         X86_ISET_MMX,   OP(Vx, Hx, Wx)),
    TABLE_ENTRY("packsswb",     X86_MN_PACKSSWB,    PF_VEX,         X86_ISET_MMX,   OP(Vx, Hx, Wx)),
    TABLE_ENTRY("pcmpgtb",      X86_MN_PCMPGTB,     PF_VEX,         X86_ISET_MMX,   OP(Vx, Hx, Wx)),
    TABLE_ENTRY("pcmpgtw",      X86_MN_PCMPGTW,     PF_VEX,         X86_ISET_MMX,   OP(Vx, Hx, Wx)),
    TABLE_ENTRY("pcmpgtd",      X86_MN_PCMPGTD,     PF_VEX,         X86_ISET_MMX,   OP(Vx, Hx, Wx)),
    TABLE_ENTRY("packuswb",     X86_MN_PACKUSWB,    PF_VEX,         X86_ISET_MMX,   OP(Vx, Hx, Wx)),
    TABLE_ENTRY("punpckhbw",    X86_MN_PUNPCKHBW,   PF_VEX,         X86_ISET_MMX,   OP(Vx, Hx, Wx)),
    TABLE_ENTRY("punpckhwd",    X86_MN_PUNPCKHWD,   PF_VEX,         X86_ISET_MMX,   OP(Vx, Hx, Wx)),
    TABLE_ENTRY("punpckhdq",    X86_MN_PUNPCKHDQ,   PF_VEX,         X86_ISET_MMX,   OP(Vx, Hx, Wx)),
    TABLE_ENTRY("packssdw",     X86_MN_PACKSSDW,    PF_VEX,         X86_ISET_MMX,   OP(Vx, Hx, Wx)),
    TABLE_ENTRY("punpcklqdq",   X86_MN_PUNPCKLQDQ,  PF_VEX,         X86_ISET_SSE2,  OP(Vx, Hx, Wx)),
    TABLE_ENTRY("punpckhqdq",   X86_MN_PUNPCKHQDQ,  PF_VEX,         X86_ISET_SSE2,  OP(Vx, Hx, Wx)),
    
    TABLE_ENTRY_ESCAPE(66_0F_6E),
    
    TABLE_ENTRY("movdqa",       X86_MN_MOVDQA,      PF_VEX,         X86_ISET_SSE2,  OP(Vx, Wx)),

    /* 70h */
    TABLE_ENTRY("pshufd",       X86_MN_PSHUFD,      PF_VEX,         X86_ISET_SSE2,  OP(Vx, Wx, Ib)),

    /* 71h: group 12, 13, 14 */
    TABLE_ENTRY_ESCAPE(66_0F_71),
    TABLE_ENTRY_ESCAPE(66_0F_72),
    TABLE_ENTRY_ESCAPE(66_0F_73),

    /* 74h */
    TABLE_ENTRY("pcmpeqb",      X86_MN_PCMPEQB,     PF_VEX,         X86_ISET_MMX,   OP(Vx, Hx, Wx)),
    TABLE_ENTRY("pcmpeqw",      X86_MN_PCMPEQW,     PF_VEX,         X86_ISET_MMX,   OP(Vx, Hx, Wx)),
    TABLE_ENTRY("pcmpeqd",      X86_MN_PCMPEQD,     PF_VEX,         X86_ISET_MMX,   OP(Vx, Hx, Wx)),
    TABLE_ENTRY_INVALID(),

    /* 78h: group 17 */
    TABLE_ENTRY_ESCAPE(66_0F_78),

    /* 79h */
    TABLE_ENTRY("extrq",        X86_MN_EXTRQ,       PF_NONE,        X86_ISET_SSE4A, OP(Vss, Uss)),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY("haddpd",       X86_MN_HADDPD,      PF_VEX,         X86_ISET_SSE3,  OP(Vpd, Hpd, Wpd)),
    TABLE_ENTRY("hsubpd",       X86_MN_HSUBPD,      PF_VEX,         X86_ISET_SSE3,  OP(Vpd, Hpd, Wpd)),
    
    TABLE_ENTRY_ESCAPE(66_0F_7E),

    TABLE_ENTRY("movdqa",       X86_MN_MOVDQA,      PF_VEX,         X86_ISET_MMX,   OP(Wx, Vx)),

    /* 80h */
    TABLE_ENTRY("jo",           X86_MN_JO,          PF_F64 | PF_PFXNA,      X86_ISET_GP,        OP(Jz)),
    TABLE_ENTRY("jno",          X86_MN_JNO,         PF_F64 | PF_PFXNA,      X86_ISET_GP,        OP(Jz)),
    TABLE_ENTRY("jc",           X86_MN_JC,          PF_F64 | PF_PFXNA,      X86_ISET_GP,        OP(Jz)),
    TABLE_ENTRY("jnc",          X86_MN_JNC,         PF_F64 | PF_PFXNA,      X86_ISET_GP,        OP(Jz)),
    TABLE_ENTRY("jz",           X86_MN_JZ,          PF_F64 | PF_PFXNA,      X86_ISET_GP,        OP(Jz)),
    TABLE_ENTRY("jnz",          X86_MN_JNZ,         PF_F64 | PF_PFXNA,      X86_ISET_GP,        OP(Jz)),
    TABLE_ENTRY("jna",          X86_MN_JNA,         PF_F64 | PF_PFXNA,      X86_ISET_GP,        OP(Jz)),
    TABLE_ENTRY("ja",           X86_MN_JA,          PF_F64 | PF_PFXNA,      X86_ISET_GP,        OP(Jz)),
    TABLE_ENTRY("js",           X86_MN_JS,          PF_F64 | PF_PFXNA,      X86_ISET_GP,        OP(Jz)),
    TABLE_ENTRY("jns",          X86_MN_JNS,         PF_F64 | PF_PFXNA,      X86_ISET_GP,        OP(Jz)),
    TABLE_ENTRY("jp",           X86_MN_JP,          PF_F64 | PF_PFXNA,      X86_ISET_GP,        OP(Jz)),
    TABLE_ENTRY("jnp",          X86_MN_JNP,         PF_F64 | PF_PFXNA,      X86_ISET_GP,        OP(Jz)),
    TABLE_ENTRY("jl",           X86_MN_JL,          PF_F64 | PF_PFXNA,      X86_ISET_GP,        OP(Jz)),
    TABLE_ENTRY("jnl",          X86_MN_JNL,         PF_F64 | PF_PFXNA,      X86_ISET_GP,        OP(Jz)),
    TABLE_ENTRY("jng",          X86_MN_JNG,         PF_F64 | PF_PFXNA,      X86_ISET_GP,        OP(Jz)),
    TABLE_ENTRY("jg",           X86_MN_JG,          PF_F64 | PF_PFXNA,      X86_ISET_GP,        OP(Jz)),

    /* 90h */
    TABLE_ENTRY("seto",         X86_MN_SETO,        PF_NONE | PF_PFXNA,     X86_ISET_GP,    OP(Eb)),
    TABLE_ENTRY("setno",        X86_MN_SETNO,       PF_NONE | PF_PFXNA,     X86_ISET_GP,    OP(Eb)),
    TABLE_ENTRY("setc",         X86_MN_SETC,        PF_NONE | PF_PFXNA,     X86_ISET_GP,    OP(Eb)),
    TABLE_ENTRY("setnc",        X86_MN_SETNC,       PF_NONE | PF_PFXNA,     X86_ISET_GP,    OP(Eb)),
    TABLE_ENTRY("setz",         X86_MN_SETZ,        PF_NONE | PF_PFXNA,     X86_ISET_GP,    OP(Eb)),
    TABLE_ENTRY("setnz",        X86_MN_SETNZ,       PF_NONE | PF_PFXNA,     X86_ISET_GP,    OP(Eb)),
    TABLE_ENTRY("setna",        X86_MN_SETNA,       PF_NONE | PF_PFXNA,     X86_ISET_GP,    OP(Eb)),
    TABLE_ENTRY("seta",         X86_MN_SETA,        PF_NONE | PF_PFXNA,     X86_ISET_GP,    OP(Eb)),
    TABLE_ENTRY("sets",         X86_MN_SETS,        PF_NONE | PF_PFXNA,     X86_ISET_GP,    OP(Eb)),
    TABLE_ENTRY("setns",        X86_MN_SETNS,       PF_NONE | PF_PFXNA,     X86_ISET_GP,    OP(Eb)),
    TABLE_ENTRY("setp",         X86_MN_SETP,        PF_NONE | PF_PFXNA,     X86_ISET_GP,    OP(Eb)),
    TABLE_ENTRY("setnp",        X86_MN_SETNP,       PF_NONE | PF_PFXNA,     X86_ISET_GP,    OP(Eb)),
    TABLE_ENTRY("setl",         X86_MN_SETL,        PF_NONE | PF_PFXNA,     X86_ISET_GP,    OP(Eb)),
    TABLE_ENTRY("setnl",        X86_MN_SETNL,       PF_NONE | PF_PFXNA,     X86_ISET_GP,    OP(Eb)),
    TABLE_ENTRY("setng",        X86_MN_SETNG,       PF_NONE | PF_PFXNA,     X86_ISET_GP,    OP(Eb)),
    TABLE_ENTRY("setg",         X86_MN_SETG,        PF_NONE | PF_PFXNA,     X86_ISET_GP,    OP(Eb)),

    /* A0h */
    TABLE_ENTRY("push",         X86_MN_PUSH,        PF_D64 | PF_PFXNA,          X86_ISET_GP,    OP(FS)),
    TABLE_ENTRY("pop",          X86_MN_POP,         PF_D64 | PF_PFXNA,          X86_ISET_GP,    OP(FS)),
    TABLE_ENTRY("cpuid",        X86_MN_CPUID,       PF_NONE | PF_PFXNA,         X86_ISET_GP,    OP(NONE)),
    TABLE_ENTRY("bt",           X86_MN_BT,          PF_NONE | PF_PFXNA,         X86_ISET_GP,    OP(Ev, Gv)),
    TABLE_ENTRY("shld",         X86_MN_SHLD,        PF_NONE | PF_PFXNA,         X86_ISET_GP,    OP(Ev, Gv, Ib)),
    TABLE_ENTRY("shld",         X86_MN_SHLD,        PF_NONE | PF_PFXNA,         X86_ISET_GP,    OP(Ev, Gv, CL)),

    /* A6h */
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    /* A8h */
    TABLE_ENTRY("push",         X86_MN_PUSH,        PF_D64 | PF_PFXNA,          X86_ISET_GP,    OP(GS)),
    TABLE_ENTRY("pop",          X86_MN_POP,         PF_D64 | PF_PFXNA,          X86_ISET_GP,    OP(GS)),
    TABLE_ENTRY("rsm",          X86_MN_RSM,         PF_NONE | PF_PFXNA,         X86_ISET_GP,    OP(NONE)),
    TABLE_ENTRY("bts",          X86_MN_BTS,         PF_LOCK | PF_PFXNA,         X86_ISET_GP,    OP(Ev, Gv)),
    TABLE_ENTRY("shrd",         X86_MN_SHRD,        PF_NONE | PF_PFXNA,         X86_ISET_GP,    OP(Ev, Gv, Ib)),
    TABLE_ENTRY("shrd",         X86_MN_SHRD,        PF_NONE | PF_PFXNA,         X86_ISET_GP,    OP(Ev, Gv, CL)),

    /* AEh: group 15 */
    TABLE_ENTRY_ESCAPE(66_0F_AE),

    /* AFh */
    TABLE_ENTRY("imul",         X86_MN_IMUL,        PF_NONE | PF_PFXNA,         X86_ISET_GP,    OP(Gv, Ev)),

    /* B0h */
    TABLE_ENTRY("cmpxchg",      X86_MN_CMPXCHG,     PF_LOCK | PF_PFXNA,     X86_ISET_GP,    OP(Eb, Gb)),
    TABLE_ENTRY("cmpxchg",      X86_MN_CMPXCHG,     PF_LOCK | PF_PFXNA,     X86_ISET_GP,    OP(Ev, Gv)),
    TABLE_ENTRY("lss",          X86_MN_LSS,         PF_NONE | PF_PFXNA,     X86_ISET_GP,    OP(Gv, Mp)),
    TABLE_ENTRY("btr",          X86_MN_BTR,         PF_LOCK | PF_PFXNA,     X86_ISET_GP,    OP(Ev, Gv)),
    TABLE_ENTRY("lfs",          X86_MN_LFS,         PF_NONE | PF_PFXNA,     X86_ISET_GP,    OP(Gv, Mp)),
    TABLE_ENTRY("lgs",          X86_MN_LGS,         PF_NONE | PF_PFXNA,     X86_ISET_GP,    OP(Gv, Mp)),
    TABLE_ENTRY("movzx",        X86_MN_MOVZX,       PF_NONE | PF_PFXNA,     X86_ISET_GP,    OP(Gv, Eb)),
    TABLE_ENTRY("movzx",        X86_MN_MOVZX,       PF_NONE | PF_PFXNA,     X86_ISET_GP,    OP(Gv, Ew)),

    /* B8h */
    TABLE_ENTRY_INVALID(),

    /* B9h: group 10, 8; both groups have prefix n/a */
    TABLE_ENTRY_ESCAPE(0F_B9),
    TABLE_ENTRY_ESCAPE(0F_BA),

    /* BBh */
    TABLE_ENTRY("btc",          X86_MN_BTC,         PF_LOCK | PF_PFXNA,     X86_ISET_GP,    OP(Ev, Gv)),
    TABLE_ENTRY("bsf",          X86_MN_BSF,         PF_NONE | PF_PFXNA,     X86_ISET_GP,    OP(Gv, Ev)),
    TABLE_ENTRY("bsr",          X86_MN_BSR,         PF_NONE | PF_PFXNA,     X86_ISET_GP,    OP(Gv, Ev)),
    TABLE_ENTRY("movsx",        X86_MN_MOVSX,       PF_NONE | PF_PFXNA,     X86_ISET_GP,    OP(Gv, Eb)),
    TABLE_ENTRY("movsx",        X86_MN_MOVSX,       PF_NONE | PF_PFXNA,     X86_ISET_GP,    OP(Gv, Ew)),

    /* C0h */
    TABLE_ENTRY("xadd",         X86_MN_XADD,        PF_LOCK | PF_PFXNA,     X86_ISET_GP,        OP(Eb, Gb)),
    TABLE_ENTRY("xadd",         X86_MN_XADD,        PF_LOCK | PF_PFXNA,     X86_ISET_GP,        OP(Ev, Gv)),
    TABLE_ENTRY("cmp*pd",       X86_MN_CMPPD,       PF_VEX | PF_CC,         X86_ISET_SSE2,      OP(Vpd, Hpd, Wpd, Ib)),
    TABLE_ENTRY_INVALID(),

    TABLE_ENTRY_ESCAPE(66_0F_C4),

    TABLE_ENTRY("pextrw",       X86_MN_PEXTRW,      PF_VEX128,      X86_ISET_SSE,   OP(Gw, Udq, Ib)),
    TABLE_ENTRY("shufpd",       X86_MN_SHUFPD,      PF_VEX,         X86_ISET_SSE2,  OP(Vpd, Hpd, Wpd, Ib)),

    /* C7h: group 9; group has prefix n/a -> not for intel */
    TABLE_ENTRY_ESCAPE(66_0F_C7),

    /* C8h */
    TABLE_ENTRY("bswap",        X86_MN_BSWAP,       PF_NONE | PF_PFXNA,     X86_ISET_GP,    OP(rAX_r8)),
    TABLE_ENTRY("bswap",        X86_MN_BSWAP,       PF_NONE | PF_PFXNA,     X86_ISET_GP,    OP(rCX_r9)),
    TABLE_ENTRY("bswap",        X86_MN_BSWAP,       PF_NONE | PF_PFXNA,     X86_ISET_GP,    OP(rDX_r10)),
    TABLE_ENTRY("bswap",        X86_MN_BSWAP,       PF_NONE | PF_PFXNA,     X86_ISET_GP,    OP(rBX_r11)),
    TABLE_ENTRY("bswap",        X86_MN_BSWAP,       PF_NONE | PF_PFXNA,     X86_ISET_GP,    OP(rSP_r12)),
    TABLE_ENTRY("bswap",        X86_MN_BSWAP,       PF_NONE | PF_PFXNA,     X86_ISET_GP,    OP(rBP_r13)),
    TABLE_ENTRY("bswap",        X86_MN_BSWAP,       PF_NONE | PF_PFXNA,     X86_ISET_GP,    OP(rSI_r14)),
    TABLE_ENTRY("bswap",        X86_MN_BSWAP,       PF_NONE | PF_PFXNA,     X86_ISET_GP,    OP(rDI_r15)),

    /* D0h */
    TABLE_ENTRY("addsubpd",     X86_MN_ADDSUBPD,    PF_VEX,         X86_ISET_SSE3,  OP(Vpd, Hpd, Wpd)),
    TABLE_ENTRY("psrlw",        X86_MN_PSRLW,       PF_VEX,         X86_ISET_MMX,   OP(Vx, Hx, Wss)),
    TABLE_ENTRY("psrld",        X86_MN_PSRLD,       PF_VEX,         X86_ISET_MMX,   OP(Vx, Hx, Wss)),
    TABLE_ENTRY("psrlq",        X86_MN_PSRLQ,       PF_VEX,         X86_ISET_MMX,   OP(Vx, Hx, Wss)),
    TABLE_ENTRY("paddq",        X86_MN_PADDQ,       PF_VEX,         X86_ISET_SSE,   OP(Vx, Hx, Wx)),
    TABLE_ENTRY("pmullw",       X86_MN_PMULLW,      PF_VEX,         X86_ISET_MMX,   OP(Vx, Hx, Wx)),
    TABLE_ENTRY("movq",         X86_MN_MOVQ,        PF_VEX,         X86_ISET_MMX,   OP(Wq, Vq)),
    TABLE_ENTRY("pmovmskb",     X86_MN_PMOVMSKB,    PF_VEX,         X86_ISET_SSE,   OP(Gd, Ux)),

    TABLE_ENTRY("psubusb",      X86_MN_PSUBUSB,     PF_VEX,         X86_ISET_MMX,   OP(Vx, Hx, Wx)),
    TABLE_ENTRY("psubusw",      X86_MN_PSUBUSW,     PF_VEX,         X86_ISET_MMX,   OP(Vx, Hx, Wx)),
    TABLE_ENTRY("pminub",       X86_MN_PMINUB,      PF_VEX,         X86_ISET_SSE,   OP(Vx, Hx, Wx)),
    TABLE_ENTRY("pand",         X86_MN_PAND,        PF_VEX,         X86_ISET_MMX,   OP(Vx, Hx, Wx)),
    TABLE_ENTRY("paddusb",      X86_MN_PADDUSB,     PF_VEX,         X86_ISET_MMX,   OP(Vx, Hx, Wx)),
    TABLE_ENTRY("paddusw",      X86_MN_PADDUSW,     PF_VEX,         X86_ISET_MMX,   OP(Vx, Hx, Wx)),
    TABLE_ENTRY("pmaxub",       X86_MN_PMAXUB,      PF_VEX,         X86_ISET_SSE,   OP(Vx, Hx, Wx)),
    TABLE_ENTRY("pandn",        X86_MN_PANDN,       PF_VEX,         X86_ISET_MMX,   OP(Vx, Hx, Wx)),

    /* E0h */
    TABLE_ENTRY("pavgb",        X86_MN_PAVGB,       PF_VEX,         X86_ISET_SSE,   OP(Vx, Hx, Wx)),
    TABLE_ENTRY("psraw",        X86_MN_PSRAW,       PF_VEX,         X86_ISET_MMX,   OP(Vx, Hx, Wss)),
    TABLE_ENTRY("psrad",        X86_MN_PSRAD,       PF_VEX,         X86_ISET_MMX,   OP(Vx, Hx, Wss)),
    TABLE_ENTRY("pavgw",        X86_MN_PAVGW,       PF_VEX,         X86_ISET_SSE,   OP(Vx, Hx, Wx)),
    TABLE_ENTRY("pmulhuw",      X86_MN_PMULHUW,     PF_VEX,         X86_ISET_SSE,   OP(Vx, Hx, Wx)),
    TABLE_ENTRY("pmulhw",       X86_MN_PMULHW,      PF_VEX,         X86_ISET_MMX,   OP(Vx, Hx, Wx)),
    TABLE_ENTRY("cvttpd2dq",    X86_MN_CVTTPD2DQ,   PF_VEX,         X86_ISET_SSE2,  OP(Vx, Wpd)),
    TABLE_ENTRY("movntdq",      X86_MN_MOVNTDQ,     PF_VEX,         X86_ISET_MMX,   OP(Mx, Vx)),

    TABLE_ENTRY("psubsb",       X86_MN_PSUBSB,      PF_VEX,         X86_ISET_MMX,   OP(Vx, Hx, Wx)),
    TABLE_ENTRY("psubsw",       X86_MN_PSUBSW,      PF_VEX,         X86_ISET_MMX,   OP(Vx, Hx, Wx)),
    TABLE_ENTRY("pminsw",       X86_MN_PMINSW,      PF_VEX,         X86_ISET_SSE,   OP(Vx, Hx, Wx)),
    TABLE_ENTRY("por",          X86_MN_POR,         PF_VEX,         X86_ISET_MMX,   OP(Vx, Hx, Wx)),
    TABLE_ENTRY("paddsb",       X86_MN_PADDSB,      PF_VEX,         X86_ISET_MMX,   OP(Vx, Hx, Wx)),
    TABLE_ENTRY("paddsw",       X86_MN_PADDSW,      PF_VEX,         X86_ISET_MMX,   OP(Vx, Hx, Wx)),
    TABLE_ENTRY("pmaxsw",       X86_MN_PMAXSW,      PF_VEX,         X86_ISET_SSE,   OP(Vx, Hx, Wx)),
    TABLE_ENTRY("pxor",         X86_MN_PXOR,        PF_VEX,         X86_ISET_MMX,   OP(Vx, Hx, Wx)),

    /* F0h */
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY("psllw",        X86_MN_PSLLW,       PF_VEX,         X86_ISET_MMX,   OP(Vx, Hx, Wss)),
    TABLE_ENTRY("pslld",        X86_MN_PSLLD,       PF_VEX,         X86_ISET_MMX,   OP(Vx, Hx, Wss)),
    TABLE_ENTRY("psllq",        X86_MN_PSLLQ,       PF_VEX,         X86_ISET_MMX,   OP(Vx, Hx, Wss)),
    TABLE_ENTRY("pmuludq",      X86_MN_PMULUDQ,     PF_VEX,         X86_ISET_SSE2,  OP(Vx, Hx, Wx)),
    TABLE_ENTRY("pmaddwd",      X86_MN_PMADDWD,     PF_VEX,         X86_ISET_MMX,   OP(Vx, Hx, Wx)),
    TABLE_ENTRY("psadbw",       X86_MN_PSADBW,      PF_VEX,         X86_ISET_MMX,   OP(Vx, Hx, Wx)),
    TABLE_ENTRY("maskmovdqu",   X86_MN_MASKMOVDQU,  PF_VEX,         X86_ISET_SSE,   OP(Vdq, Udq)),
    
    TABLE_ENTRY("psubb",        X86_MN_PSUBB,       PF_VEX,         X86_ISET_MMX,   OP(Vx, Hx, Wx)),
    TABLE_ENTRY("psubw",        X86_MN_PSUBW,       PF_VEX,         X86_ISET_MMX,   OP(Vx, Hx, Wx)),
    TABLE_ENTRY("psubd",        X86_MN_PSUBD,       PF_VEX,         X86_ISET_MMX,   OP(Vx, Hx, Wx)),
    TABLE_ENTRY("psubq",        X86_MN_PSUBQ,       PF_VEX,         X86_ISET_SSE2,  OP(Vx, Hx, Wx)),
    TABLE_ENTRY("paddb",        X86_MN_PADDB,       PF_VEX,         X86_ISET_MMX,   OP(Vx, Hx, Wx)),
    TABLE_ENTRY("paddw",        X86_MN_PADDW,       PF_VEX,         X86_ISET_MMX,   OP(Vx, Hx, Wx)),
    TABLE_ENTRY("paddd",        X86_MN_PADDD,       PF_VEX,         X86_ISET_MMX,   OP(Vx, Hx, Wx)),
    TABLE_ENTRY("ud0",          X86_MN_UD,          PF_VEX,         X86_ISET_MMX,   OP(Pq, Qq))

TABLE_END(66_0F)

TABLE_BEGIN(66_0F_19)

    /* modr/m byte reg/nnn field */
    TABLE_ENTRY("nop",  X86_MN_NOP,     PF_NONE | PF_PFXNA,         X86_ISET_GP, OP(Ev)),
    TABLE_ENTRY("nop",  X86_MN_NOP,     PF_NONE | PF_PFXNA,         X86_ISET_GP, OP(Ev)),
    TABLE_ENTRY("nop",  X86_MN_NOP,     PF_NONE | PF_PFXNA,         X86_ISET_GP, OP(Ev)),
    TABLE_ENTRY("nop",  X86_MN_NOP,     PF_NONE | PF_PFXNA,         X86_ISET_GP, OP(Ev)),
    TABLE_ENTRY("nop",  X86_MN_NOP,     PF_NONE | PF_PFXNA,         X86_ISET_GP, OP(Ev)),
    TABLE_ENTRY("nop",  X86_MN_NOP,     PF_NONE | PF_PFXNA,         X86_ISET_GP, OP(Ev)),
    TABLE_ENTRY("nop",  X86_MN_NOP,     PF_NONE | PF_PFXNA,         X86_ISET_GP, OP(Ev)),
    TABLE_ENTRY("nop",  X86_MN_NOP,     PF_NONE | PF_PFXNA,         X86_ISET_GP, OP(Ev))

TABLE_END(66_0F_19)

TABLE_BEGIN(66_0F_1C)

    /* modr/m byte reg/nnn field */
    TABLE_ENTRY("nop",  X86_MN_NOP,     PF_NONE | PF_PFXNA,         X86_ISET_GP, OP(Ev)),
    TABLE_ENTRY("nop",  X86_MN_NOP,     PF_NONE | PF_PFXNA,         X86_ISET_GP, OP(Ev)),
    TABLE_ENTRY("nop",  X86_MN_NOP,     PF_NONE | PF_PFXNA,         X86_ISET_GP, OP(Ev)),
    TABLE_ENTRY("nop",  X86_MN_NOP,     PF_NONE | PF_PFXNA,         X86_ISET_GP, OP(Ev)),
    TABLE_ENTRY("nop",  X86_MN_NOP,     PF_NONE | PF_PFXNA,         X86_ISET_GP, OP(Ev)),
    TABLE_ENTRY("nop",  X86_MN_NOP,     PF_NONE | PF_PFXNA,         X86_ISET_GP, OP(Ev)),
    TABLE_ENTRY("nop",  X86_MN_NOP,     PF_NONE | PF_PFXNA,         X86_ISET_GP, OP(Ev)),
    TABLE_ENTRY("nop",  X86_MN_NOP,     PF_NONE | PF_PFXNA,         X86_ISET_GP, OP(Ev))

TABLE_END(66_0F_1C)

TABLE_BEGIN(66_0F_1D)

    /* modr/m byte reg/nnn field */
    TABLE_ENTRY("nop",  X86_MN_NOP,     PF_NONE | PF_PFXNA,         X86_ISET_GP, OP(Ev)),
    TABLE_ENTRY("nop",  X86_MN_NOP,     PF_NONE | PF_PFXNA,         X86_ISET_GP, OP(Ev)),
    TABLE_ENTRY("nop",  X86_MN_NOP,     PF_NONE | PF_PFXNA,         X86_ISET_GP, OP(Ev)),
    TABLE_ENTRY("nop",  X86_MN_NOP,     PF_NONE | PF_PFXNA,         X86_ISET_GP, OP(Ev)),
    TABLE_ENTRY("nop",  X86_MN_NOP,     PF_NONE | PF_PFXNA,         X86_ISET_GP, OP(Ev)),
    TABLE_ENTRY("nop",  X86_MN_NOP,     PF_NONE | PF_PFXNA,         X86_ISET_GP, OP(Ev)),
    TABLE_ENTRY("nop",  X86_MN_NOP,     PF_NONE | PF_PFXNA,         X86_ISET_GP, OP(Ev)),
    TABLE_ENTRY("nop",  X86_MN_NOP,     PF_NONE | PF_PFXNA,         X86_ISET_GP, OP(Ev))

TABLE_END(66_0F_1D)

TABLE_BEGIN(66_0F_1E)

    /* modr/m byte reg/nnn field */
    TABLE_ENTRY("nop",  X86_MN_NOP,     PF_NONE | PF_PFXNA,         X86_ISET_GP, OP(Ev)),
    TABLE_ENTRY("nop",  X86_MN_NOP,     PF_NONE | PF_PFXNA,         X86_ISET_GP, OP(Ev)),
    TABLE_ENTRY("nop",  X86_MN_NOP,     PF_NONE | PF_PFXNA,         X86_ISET_GP, OP(Ev)),
    TABLE_ENTRY("nop",  X86_MN_NOP,     PF_NONE | PF_PFXNA,         X86_ISET_GP, OP(Ev)),
    TABLE_ENTRY("nop",  X86_MN_NOP,     PF_NONE | PF_PFXNA,         X86_ISET_GP, OP(Ev)),
    TABLE_ENTRY("nop",  X86_MN_NOP,     PF_NONE | PF_PFXNA,         X86_ISET_GP, OP(Ev)),
    TABLE_ENTRY("nop",  X86_MN_NOP,     PF_NONE | PF_PFXNA,         X86_ISET_GP, OP(Ev)),
    TABLE_ENTRY("nop",  X86_MN_NOP,     PF_NONE | PF_PFXNA,         X86_ISET_GP, OP(Ev))

TABLE_END(66_0F_1E)

TABLE_BEGIN(66_0F_1F)

    /* modr/m byte reg/nnn field */
    TABLE_ENTRY("nop",  X86_MN_NOP,     PF_NONE | PF_PFXNA,         X86_ISET_GP, OP(Ev)),
    TABLE_ENTRY("nop",  X86_MN_NOP,     PF_NONE | PF_PFXNA,         X86_ISET_GP, OP(Ev)),
    TABLE_ENTRY("nop",  X86_MN_NOP,     PF_NONE | PF_PFXNA,         X86_ISET_GP, OP(Ev)),
    TABLE_ENTRY("nop",  X86_MN_NOP,     PF_NONE | PF_PFXNA,         X86_ISET_GP, OP(Ev)),
    TABLE_ENTRY("nop",  X86_MN_NOP,     PF_NONE | PF_PFXNA,         X86_ISET_GP, OP(Ev)),
    TABLE_ENTRY("nop",  X86_MN_NOP,     PF_NONE | PF_PFXNA,         X86_ISET_GP, OP(Ev)),
    TABLE_ENTRY("nop",  X86_MN_NOP,     PF_NONE | PF_PFXNA,         X86_ISET_GP, OP(Ev)),
    TABLE_ENTRY("nop",  X86_MN_NOP,     PF_NONE | PF_PFXNA,         X86_ISET_GP, OP(Ev))

TABLE_END(66_0F_1F)

TABLE_BEGIN(66_0F_6E)

    /* operand size: 0 = 16-bits, 1 = 32-bits, 2 = 64-bits */
    TABLE_ENTRY("movd",         X86_MN_MOVD,        PF_VEX128,      X86_ISET_MMX,   OP(Vy, Ey)),
    TABLE_ENTRY("movd",         X86_MN_MOVD,        PF_VEX128,      X86_ISET_MMX,   OP(Vy, Ey)),
    TABLE_ENTRY("movq",         X86_MN_MOVQ,        PF_VEX128,      X86_ISET_MMX,   OP(Vy, Ey))

TABLE_END(66_0F_6E)

TABLE_BEGIN(66_0F_71)

    /* modr/m byte reg/nnn field */

    /* 00h */
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    /* 02h: psrlw or invalid*/
    TABLE_ENTRY_ESCAPE(66_0F_71_REG2),

    TABLE_ENTRY_INVALID(),

    /* 04h: psraw or invalid*/
    TABLE_ENTRY_ESCAPE(66_0F_71_REG4),
    
    TABLE_ENTRY_INVALID(),

    /* 06h: psllw or invalid*/
    TABLE_ENTRY_ESCAPE(66_0F_71_REG6),
    
    TABLE_ENTRY_INVALID()

TABLE_END(66_0F_71)

TABLE_BEGIN(66_0F_71_REG2)

    /* modr/m byte mod field:  0 = !11b, 1 = 11b */
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY("psrlw",        X86_MN_PSRLW,       PF_VEX,         X86_ISET_MMX,   OP(Hx, Ux, Ib))

TABLE_END(66_0F_71_REG2)

TABLE_BEGIN(66_0F_71_REG4)

    /* modr/m byte mod field:  0 = !11b, 1 = 11b */
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY("psraw",        X86_MN_PSRAW,       PF_VEX,         X86_ISET_MMX,   OP(Hx, Ux, Ib))

TABLE_END(66_0F_71_REG4)

TABLE_BEGIN(66_0F_71_REG6)

    /* modr/m byte mod field:  0 = !11b, 1 = 11b */
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY("psllw",        X86_MN_PSLLW,       PF_VEX,         X86_ISET_MMX,   OP(Hx, Ux, Ib))

TABLE_END(66_0F_71_REG6)

TABLE_BEGIN(66_0F_72)

    /* modr/m byte reg/nnn field */

    /* 00h */
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    /* 02h: psrld or invalid*/
    TABLE_ENTRY_ESCAPE(66_0F_72_REG2),

    TABLE_ENTRY_INVALID(),

    /* 04h: psrad or invalid*/
    TABLE_ENTRY_ESCAPE(66_0F_72_REG4),
    
    TABLE_ENTRY_INVALID(),

    /* 06h: pslld or invalid*/
    TABLE_ENTRY_ESCAPE(66_0F_72_REG6),
    
    TABLE_ENTRY_INVALID()

TABLE_END(66_0F_72)

TABLE_BEGIN(66_0F_72_REG2)

    /* modr/m byte mod field:  0 = !11b, 1 = 11b */
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY("psrld",        X86_MN_PSRLD,       PF_VEX,         X86_ISET_MMX,   OP(Hx, Ux, Ib))

TABLE_END(66_0F_72_REG2)

TABLE_BEGIN(66_0F_72_REG4)

    /* modr/m byte mod field:  0 = !11b, 1 = 11b */
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY("psrad",        X86_MN_PSRAD,       PF_VEX,         X86_ISET_MMX,   OP(Hx, Ux, Ib))

TABLE_END(66_0F_72_REG4)

TABLE_BEGIN(66_0F_72_REG6)

    /* modr/m byte mod field:  0 = !11b, 1 = 11b */
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY("pslld",        X86_MN_PSLLD,       PF_VEX,         X86_ISET_MMX,   OP(Hx, Ux, Ib))

TABLE_END(66_0F_72_REG6)

TABLE_BEGIN(66_0F_73)

    /* modr/m byte reg/nnn field */

    /* 00h */
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    /* 02h: psrlq or invalid*/
    TABLE_ENTRY_ESCAPE(66_0F_73_REG2),

    /* 03h: psrldq or invalid*/
    TABLE_ENTRY_ESCAPE(66_0F_73_REG3),

    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    /* 06h: psllq or invalid*/
    TABLE_ENTRY_ESCAPE(66_0F_73_REG6),
    
    /* 07h: pslldq or invalid*/
    TABLE_ENTRY_ESCAPE(66_0F_73_REG7)

TABLE_END(66_0F_73)

TABLE_BEGIN(66_0F_73_REG2)

    /* modr/m byte mod field:  0 = !11b, 1 = 11b */
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY("psrlq",        X86_MN_PSRLQ,       PF_VEX,         X86_ISET_MMX,   OP(Hx, Ux, Ib))

TABLE_END(66_0F_73_REG2)

TABLE_BEGIN(66_0F_73_REG3)

    /* modr/m byte mod field:  0 = !11b, 1 = 11b */
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY("psrldq",       X86_MN_PSRLDQ,      PF_VEX,         X86_ISET_SSE2,  OP(Hx, Ux, Ib))

TABLE_END(66_0F_73_REG3)

TABLE_BEGIN(66_0F_73_REG6)

    /* modr/m byte mod field:  0 = !11b, 1 = 11b */
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY("psllq",        X86_MN_PSLLQ,       PF_VEX,         X86_ISET_MMX,   OP(Hx, Ux, Ib))

TABLE_END(66_0F_73_REG6)

TABLE_BEGIN(66_0F_73_REG7)

    /* modr/m byte mod field:  0 = !11b, 1 = 11b */
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY("pslldq",       X86_MN_PSLLDQ,      PF_VEX,         X86_ISET_SSE2,  OP(Hx, Ux, Ib))

TABLE_END(66_0F_73_REG7)

TABLE_BEGIN(66_0F_7E)

    /* operand size: 0 = 16-bits, 1 = 32-bits, 2 = 64-bits */
    TABLE_ENTRY("movd",         X86_MN_MOVD,        PF_VEX128,      X86_ISET_MMX,   OP(Ey, Vy)),
    TABLE_ENTRY("movd",         X86_MN_MOVD,        PF_VEX128,      X86_ISET_MMX,   OP(Ey, Vy)),
    TABLE_ENTRY("movq",         X86_MN_MOVQ,        PF_VEX128,      X86_ISET_MMX,   OP(Ey, Vy))

TABLE_END(66_0F_7E)

TABLE_BEGIN(66_0F_AE)

    /* modr/m byte reg/nnn field */

    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID()

TABLE_END(66_0F_AE)

TABLE_BEGIN(66_0F_78)

    /* modr/m byte reg/nnn field */

    TABLE_ENTRY("extrq",        X86_MN_EXTRQ,       PF_NONE,        X86_ISET_SSE4A, OP(Vss, Ib, Ib)),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID()

TABLE_END(66_0F_78)

TABLE_BEGIN(66_0F_C4)

    /* modr/m byte mod field:  0 = !11b, 1 = 11b */
    TABLE_ENTRY("pinsrw",       X86_MN_PINSRW,      PF_VEX128,      X86_ISET_SSE,   OP(Vdq, Hdq, Mw, Ib)),
    TABLE_ENTRY("pinsrw",       X86_MN_PINSRW,      PF_VEX128,      X86_ISET_SSE,   OP(Vdq, Hdq, Ry, Ib))

TABLE_END(66_0F_C4)

TABLE_BEGIN(66_0F_C7)

    /* modr/m byte reg/nnn field */

    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    
    /* vmclear */
    TABLE_ENTRY_ESCAPE(66_0F_C7_REG6),
    TABLE_ENTRY_INVALID()

TABLE_END(66_0F_C7)

TABLE_BEGIN(66_0F_C7_REG6)

    /* modr/m byte mod field:  0 = !11b, 1 = 11b */
    TABLE_ENTRY("vmclear",      X86_MN_VMCLEAR,     PF_NONE | PF_PFXNA,     X86_ISET_VMX,   OP(Mq)),
    TABLE_ENTRY("rdrand",       X86_MN_RDRAND,      PF_NONE | PF_PFXNA,     X86_ISET_GP,    OP(Rv))

TABLE_END(66_0F_C7_REG6)

/****************************************************************************

    Two-byte tables - F3h mandatory prefix

****************************************************************************/

TABLE_BEGIN(F3_0F)

    /* 00h: group 6, 7; both groups have prefix n/a */
    TABLE_ENTRY_ESCAPE(0F_00),
    TABLE_ENTRY_ESCAPE(F3_0F_01),

    /* 02h */
    TABLE_ENTRY("lar",          X86_MN_LAR,         PF_NONE | PF_PFXNA,     X86_ISET_GP,    OP(Gv, Ew)),
    TABLE_ENTRY("lsl",          X86_MN_LSL,         PF_NONE | PF_PFXNA,     X86_ISET_GP,    OP(Gv, Ew)),

    /* 04h */
    TABLE_ENTRY_INVALID(),

    /* 05h */
    TABLE_ENTRY("syscall",      X86_MN_SYSCALL,     PF_O64 | PF_PFXNA,          X86_ISET_GP,    OP(NONE)),
    TABLE_ENTRY("clts",         X86_MN_CLTS,        PF_NONE | PF_PFXNA,         X86_ISET_GP,    OP(NONE)),
    TABLE_ENTRY("sysret",       X86_MN_SYSRET,      PF_O64 | PF_PFXNA,          X86_ISET_GP,    OP(NONE)),
    TABLE_ENTRY("invd",         X86_MN_INVD,        PF_NONE | PF_PFXNA,         X86_ISET_GP,    OP(NONE)),
    TABLE_ENTRY("wbinvd",       X86_MN_WBINVD,      PF_NONE | PF_PFXNA,         X86_ISET_GP,    OP(NONE)),

    /* 0Ah */
    TABLE_ENTRY_INVALID(),

    /* 0Bh */
    TABLE_ENTRY("ud2",          X86_MN_UD2,         PF_NONE | PF_PFXNA,     X86_ISET_GP,    OP(NONE)),

    /* 0Ch */
    TABLE_ENTRY_INVALID(),

    /* 0Dh: group p; group has prefix n/a */
    TABLE_ENTRY_ESCAPE(0F_0D),

    /* 0Eh */
    TABLE_ENTRY("femms",        X86_MN_FEMMS,       PF_NONE | PF_PFXNA,     X86_ISET_GP,    OP(NONE)),

    /* 0Fh: AMD only, 3DNow! escape */
    TABLE_ENTRY_INVALID(),
    
    /* 10h */
    TABLE_ENTRY("movss",        X86_MN_MOVSS,       PF_VEX128,      X86_ISET_SSE,   OP(Vx, Hx, Wss)),
    TABLE_ENTRY("movss",        X86_MN_MOVSS,       PF_VEX128,      X86_ISET_SSE,   OP(Wss, Hx, Vss)),

    /* 12h */
    TABLE_ENTRY("movsldup",     X86_MN_MOVSLDUP,    PF_VEX,         X86_ISET_SSE3,  OP(Vx, Wx)),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY("movshdup",     X86_MN_MOVSHDUP,    PF_VEX,         X86_ISET_SSE3,  OP(Vx, Wx)),
    TABLE_ENTRY_INVALID(),

    /* 18h: group 16; group has prefix n/a */
    TABLE_ENTRY_ESCAPE(0F_18),

    /* 19h */
    TABLE_ENTRY_ESCAPE(F3_0F_19),

    TABLE_ENTRY("nop",          X86_MN_NOP,     PF_NONE | PF_PFXNA,     X86_ISET_GP,    OP(Ev)),
    TABLE_ENTRY("nop",          X86_MN_NOP,     PF_NONE | PF_PFXNA,     X86_ISET_GP,    OP(Ev)),

    TABLE_ENTRY_ESCAPE(F3_0F_1C),
    TABLE_ENTRY_ESCAPE(F3_0F_1D),
    TABLE_ENTRY_ESCAPE(F3_0F_1E),
    TABLE_ENTRY_ESCAPE(F3_0F_1F),

    /* 20h */
    TABLE_ENTRY("mov",          X86_MN_MOV,     PF_F64 | PF_PFXNA,      X86_ISET_GP,    OP(Ry, Cy)),
    TABLE_ENTRY("mov",          X86_MN_MOV,     PF_F64 | PF_PFXNA,      X86_ISET_GP,    OP(Ry, Dy)),
    TABLE_ENTRY("mov",          X86_MN_MOV,     PF_F64 | PF_PFXNA,      X86_ISET_GP,    OP(Cy, Ry)),
    TABLE_ENTRY("mov",          X86_MN_MOV,     PF_F64 | PF_PFXNA,      X86_ISET_GP,    OP(Dy, Ry)),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    
    /* 28h */
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY("cvtsi2ss",     X86_MN_CVTSI2SS,    PF_VEX128,      X86_ISET_SSE,   OP(Vss, Hss, Ey)),
    TABLE_ENTRY("movntss",      X86_MN_MOVNTSS,     PF_NONE,        X86_ISET_SSE4A, OP(Md, Vss)),
    TABLE_ENTRY("cvttss2si",    X86_MN_CVTTSS2SI,   PF_VEX128,      X86_ISET_SSE,   OP(Gy, Wss)),
    TABLE_ENTRY("cvtss2si",     X86_MN_CVTSS2SI,    PF_VEX128,      X86_ISET_SSE,   OP(Gy, Wss)),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    /* 30h */
    TABLE_ENTRY("wrmsr",        X86_MN_WRMSR,       PF_NONE | PF_PFXNA,     X86_ISET_GP,    OP(NONE)),
    TABLE_ENTRY("rdtsc",        X86_MN_RDTSC,       PF_NONE | PF_PFXNA,     X86_ISET_GP,    OP(NONE)),
    TABLE_ENTRY("rdmsr",        X86_MN_RDMSR,       PF_NONE | PF_PFXNA,     X86_ISET_GP,    OP(NONE)),
    TABLE_ENTRY("rdpmc",        X86_MN_RDPMC,       PF_NONE | PF_PFXNA,     X86_ISET_GP,    OP(NONE)),
    TABLE_ENTRY("sysenter",     X86_MN_SYSENTER,    PF_NONE | PF_PFXNA,     X86_ISET_GP,    OP(NONE)),
    TABLE_ENTRY("sysexit",      X86_MN_SYSEXIT,     PF_NONE | PF_PFXNA,     X86_ISET_GP,    OP(NONE)),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    /* 38h: three-byte table escape */
    TABLE_ENTRY_ESCAPE(F3_0F_38),
    TABLE_ENTRY_INVALID(),

    /* 3Ah: three-byte table escape */
    TABLE_ENTRY_ESCAPE(F3_0F_3A),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    /* 40h */
    TABLE_ENTRY("cmovo",        X86_MN_CMOVO,       PF_NONE | PF_PFXNA, X86_ISET_GP,        OP(Gv, Ev)),
    TABLE_ENTRY("cmovno",       X86_MN_CMOVNO,      PF_NONE | PF_PFXNA, X86_ISET_GP,        OP(Gv, Ev)),
    TABLE_ENTRY("cmovc",        X86_MN_CMOVC,       PF_NONE | PF_PFXNA, X86_ISET_GP,        OP(Gv, Ev)),
    TABLE_ENTRY("cmovnc",       X86_MN_CMOVNC,      PF_NONE | PF_PFXNA, X86_ISET_GP,        OP(Gv, Ev)),
    TABLE_ENTRY("cmovz",        X86_MN_CMOVZ,       PF_NONE | PF_PFXNA, X86_ISET_GP,        OP(Gv, Ev)),
    TABLE_ENTRY("cmovnz",       X86_MN_CMOVNZ,      PF_NONE | PF_PFXNA, X86_ISET_GP,        OP(Gv, Ev)),
    TABLE_ENTRY("cmovna",       X86_MN_CMOVNA,      PF_NONE | PF_PFXNA, X86_ISET_GP,        OP(Gv, Ev)),
    TABLE_ENTRY("cmova",        X86_MN_CMOVA,       PF_NONE | PF_PFXNA, X86_ISET_GP,        OP(Gv, Ev)),
    TABLE_ENTRY("cmovs",        X86_MN_CMOVS,       PF_NONE | PF_PFXNA, X86_ISET_GP,        OP(Gv, Ev)),
    TABLE_ENTRY("cmovns",       X86_MN_CMOVNS,      PF_NONE | PF_PFXNA, X86_ISET_GP,        OP(Gv, Ev)),
    TABLE_ENTRY("cmovp",        X86_MN_CMOVP,       PF_NONE | PF_PFXNA, X86_ISET_GP,        OP(Gv, Ev)),
    TABLE_ENTRY("cmovnp",       X86_MN_CMOVPO,      PF_NONE | PF_PFXNA, X86_ISET_GP,        OP(Gv, Ev)),
    TABLE_ENTRY("cmovl",        X86_MN_CMOVL,       PF_NONE | PF_PFXNA, X86_ISET_GP,        OP(Gv, Ev)),
    TABLE_ENTRY("cmovnl",       X86_MN_CMOVNL,      PF_NONE | PF_PFXNA, X86_ISET_GP,        OP(Gv, Ev)),
    TABLE_ENTRY("cmovng",       X86_MN_CMOVNG,      PF_NONE | PF_PFXNA, X86_ISET_GP,        OP(Gv, Ev)),
    TABLE_ENTRY("cmovg",        X86_MN_CMOVG,       PF_NONE | PF_PFXNA, X86_ISET_GP,        OP(Gv, Ev)),

    /* 50h */
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY("sqrtss",       X86_MN_SQRTSS,      PF_VEX128,      X86_ISET_SSE,   OP(Vss, Hss, Wss)),
    TABLE_ENTRY("rsqrtss",      X86_MN_RSQRTSS,     PF_VEX128,      X86_ISET_SSE,   OP(Vss, Hss, Wss)),
    TABLE_ENTRY("rcpss",        X86_MN_RCPSS,       PF_VEX128,      X86_ISET_SSE,   OP(Vss, Hss, Wss)),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY("addss",        X86_MN_ADDSS,       PF_VEX128,      X86_ISET_SSE,   OP(Vss, Hss, Wss)),
    TABLE_ENTRY("mulss",        X86_MN_MULSS,       PF_VEX128,      X86_ISET_SSE,   OP(Vss, Hss, Wss)),
    TABLE_ENTRY("cvtss2sd",     X86_MN_CVTSS2SD,    PF_VEX128,      X86_ISET_SSE2,  OP(Vsd, Hx, Wss)),
    TABLE_ENTRY("cvttps2dq",    X86_MN_CVTTPS2DQ,   PF_VEX,         X86_ISET_SSE2,  OP(Vdq, Wps)),
    TABLE_ENTRY("subss",        X86_MN_SUBSS,       PF_VEX128,      X86_ISET_SSE,   OP(Vss, Hss, Wss)),
    TABLE_ENTRY("minss",        X86_MN_MINSS,       PF_VEX128,      X86_ISET_SSE,   OP(Vss, Hss, Wss)),
    TABLE_ENTRY("divss",        X86_MN_DIVSS,       PF_VEX128,      X86_ISET_SSE,   OP(Vss, Hss, Wss)),
    TABLE_ENTRY("maxss",        X86_MN_MAXSS,       PF_VEX128,      X86_ISET_SSE,   OP(Vss, Hss, Wss)),

    /* 60h */
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY("movdqu",       X86_MN_MOVDQU,      PF_VEX,         X86_ISET_SSE2,  OP(Vx, Wx)),

    /* 70h */
    TABLE_ENTRY("pshufhw",      X86_MN_PSHUFHW,     PF_VEX,         X86_ISET_SSE2,  OP(Vx, Wx, Ib)),

    /* 71h: group 12, 13, 14 */
    TABLE_ENTRY_ESCAPE(F3_0F_71),
    TABLE_ENTRY_ESCAPE(F3_0F_72),
    TABLE_ENTRY_ESCAPE(F3_0F_73),

    /* 74h */
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    /* 78h */
    TABLE_ENTRY_INVALID(),

    /* 79h */
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY("movq",         X86_MN_MOVQ,        PF_VEX128,      X86_ISET_MMX,   OP(Vq, Wq)),
    TABLE_ENTRY("movdqu",       X86_MN_MOVDQU,      PF_VEX,         X86_ISET_MMX,   OP(Wx, Vx)),

    /* 80h */
    TABLE_ENTRY("jo",           X86_MN_JO,          PF_F64 | PF_PFXNA,      X86_ISET_GP,        OP(Jz)),
    TABLE_ENTRY("jno",          X86_MN_JNO,         PF_F64 | PF_PFXNA,      X86_ISET_GP,        OP(Jz)),
    TABLE_ENTRY("jc",           X86_MN_JC,          PF_F64 | PF_PFXNA,      X86_ISET_GP,        OP(Jz)),
    TABLE_ENTRY("jnc",          X86_MN_JNC,         PF_F64 | PF_PFXNA,      X86_ISET_GP,        OP(Jz)),
    TABLE_ENTRY("jz",           X86_MN_JZ,          PF_F64 | PF_PFXNA,      X86_ISET_GP,        OP(Jz)),
    TABLE_ENTRY("jnz",          X86_MN_JNZ,         PF_F64 | PF_PFXNA,      X86_ISET_GP,        OP(Jz)),
    TABLE_ENTRY("jna",          X86_MN_JNA,         PF_F64 | PF_PFXNA,      X86_ISET_GP,        OP(Jz)),
    TABLE_ENTRY("ja",           X86_MN_JA,          PF_F64 | PF_PFXNA,      X86_ISET_GP,        OP(Jz)),
    TABLE_ENTRY("js",           X86_MN_JS,          PF_F64 | PF_PFXNA,      X86_ISET_GP,        OP(Jz)),
    TABLE_ENTRY("jns",          X86_MN_JNS,         PF_F64 | PF_PFXNA,      X86_ISET_GP,        OP(Jz)),
    TABLE_ENTRY("jp",           X86_MN_JP,          PF_F64 | PF_PFXNA,      X86_ISET_GP,        OP(Jz)),
    TABLE_ENTRY("jnp",          X86_MN_JNP,         PF_F64 | PF_PFXNA,      X86_ISET_GP,        OP(Jz)),
    TABLE_ENTRY("jl",           X86_MN_JL,          PF_F64 | PF_PFXNA,      X86_ISET_GP,        OP(Jz)),
    TABLE_ENTRY("jnl",          X86_MN_JNL,         PF_F64 | PF_PFXNA,      X86_ISET_GP,        OP(Jz)),
    TABLE_ENTRY("jng",          X86_MN_JNG,         PF_F64 | PF_PFXNA,      X86_ISET_GP,        OP(Jz)),
    TABLE_ENTRY("jg",           X86_MN_JG,          PF_F64 | PF_PFXNA,      X86_ISET_GP,        OP(Jz)),

    /* 90h */
    TABLE_ENTRY("seto",         X86_MN_SETO,        PF_NONE | PF_PFXNA,     X86_ISET_GP,    OP(Eb)),
    TABLE_ENTRY("setno",        X86_MN_SETNO,       PF_NONE | PF_PFXNA,     X86_ISET_GP,    OP(Eb)),
    TABLE_ENTRY("setc",         X86_MN_SETC,        PF_NONE | PF_PFXNA,     X86_ISET_GP,    OP(Eb)),
    TABLE_ENTRY("setnc",        X86_MN_SETNC,       PF_NONE | PF_PFXNA,     X86_ISET_GP,    OP(Eb)),
    TABLE_ENTRY("setz",         X86_MN_SETZ,        PF_NONE | PF_PFXNA,     X86_ISET_GP,    OP(Eb)),
    TABLE_ENTRY("setnz",        X86_MN_SETNZ,       PF_NONE | PF_PFXNA,     X86_ISET_GP,    OP(Eb)),
    TABLE_ENTRY("setna",        X86_MN_SETNA,       PF_NONE | PF_PFXNA,     X86_ISET_GP,    OP(Eb)),
    TABLE_ENTRY("seta",         X86_MN_SETA,        PF_NONE | PF_PFXNA,     X86_ISET_GP,    OP(Eb)),
    TABLE_ENTRY("sets",         X86_MN_SETS,        PF_NONE | PF_PFXNA,     X86_ISET_GP,    OP(Eb)),
    TABLE_ENTRY("setns",        X86_MN_SETNS,       PF_NONE | PF_PFXNA,     X86_ISET_GP,    OP(Eb)),
    TABLE_ENTRY("setp",         X86_MN_SETP,        PF_NONE | PF_PFXNA,     X86_ISET_GP,    OP(Eb)),
    TABLE_ENTRY("setnp",        X86_MN_SETNP,       PF_NONE | PF_PFXNA,     X86_ISET_GP,    OP(Eb)),
    TABLE_ENTRY("setl",         X86_MN_SETL,        PF_NONE | PF_PFXNA,     X86_ISET_GP,    OP(Eb)),
    TABLE_ENTRY("setnl",        X86_MN_SETNL,       PF_NONE | PF_PFXNA,     X86_ISET_GP,    OP(Eb)),
    TABLE_ENTRY("setng",        X86_MN_SETNG,       PF_NONE | PF_PFXNA,     X86_ISET_GP,    OP(Eb)),
    TABLE_ENTRY("setg",         X86_MN_SETG,        PF_NONE | PF_PFXNA,     X86_ISET_GP,    OP(Eb)),

    /* A0h */
    TABLE_ENTRY("push",         X86_MN_PUSH,        PF_D64 | PF_PFXNA,          X86_ISET_GP,    OP(FS)),
    TABLE_ENTRY("pop",          X86_MN_POP,         PF_D64 | PF_PFXNA,          X86_ISET_GP,    OP(FS)),
    TABLE_ENTRY("cpuid",        X86_MN_CPUID,       PF_NONE | PF_PFXNA,         X86_ISET_GP,    OP(NONE)),
    TABLE_ENTRY("bt",           X86_MN_BT,          PF_NONE | PF_PFXNA,         X86_ISET_GP,    OP(Ev, Gv)),
    TABLE_ENTRY("shld",         X86_MN_SHLD,        PF_NONE | PF_PFXNA,         X86_ISET_GP,    OP(Ev, Gv, Ib)),
    TABLE_ENTRY("shld",         X86_MN_SHLD,        PF_NONE | PF_PFXNA,         X86_ISET_GP,    OP(Ev, Gv, CL)),

    /* A6h */
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    /* A8h */
    TABLE_ENTRY("push",         X86_MN_PUSH,        PF_D64 | PF_PFXNA,          X86_ISET_GP,    OP(GS)),
    TABLE_ENTRY("pop",          X86_MN_POP,         PF_D64 | PF_PFXNA,          X86_ISET_GP,    OP(GS)),
    TABLE_ENTRY("rsm",          X86_MN_RSM,         PF_NONE | PF_PFXNA,         X86_ISET_GP,    OP(NONE)),
    TABLE_ENTRY("bts",          X86_MN_BTS,         PF_LOCK | PF_PFXNA,         X86_ISET_GP,    OP(Ev, Gv)),
    TABLE_ENTRY("shrd",         X86_MN_SHRD,        PF_NONE | PF_PFXNA,         X86_ISET_GP,    OP(Ev, Gv, Ib)),
    TABLE_ENTRY("shrd",         X86_MN_SHRD,        PF_NONE | PF_PFXNA,         X86_ISET_GP,    OP(Ev, Gv, CL)),

    /* AEh: group 15 */
    TABLE_ENTRY_ESCAPE(F3_0F_AE),

    /* AFh */
    TABLE_ENTRY("imul",         X86_MN_IMUL,        PF_NONE | PF_PFXNA,         X86_ISET_GP,    OP(Gv, Ev)),

    /* B0h */
    TABLE_ENTRY("cmpxchg",      X86_MN_CMPXCHG,     PF_LOCK | PF_PFXNA,     X86_ISET_GP,    OP(Eb, Gb)),
    TABLE_ENTRY("cmpxchg",      X86_MN_CMPXCHG,     PF_LOCK | PF_PFXNA,     X86_ISET_GP,    OP(Ev, Gv)),
    TABLE_ENTRY("lss",          X86_MN_LSS,         PF_NONE | PF_PFXNA,     X86_ISET_GP,    OP(Gv, Mp)),
    TABLE_ENTRY("btr",          X86_MN_BTR,         PF_LOCK | PF_PFXNA,     X86_ISET_GP,    OP(Ev, Gv)),
    TABLE_ENTRY("lfs",          X86_MN_LFS,         PF_NONE | PF_PFXNA,     X86_ISET_GP,    OP(Gv, Mp)),
    TABLE_ENTRY("lgs",          X86_MN_LGS,         PF_NONE | PF_PFXNA,     X86_ISET_GP,    OP(Gv, Mp)),
    TABLE_ENTRY("movzx",        X86_MN_MOVZX,       PF_NONE | PF_PFXNA,     X86_ISET_GP,    OP(Gv, Eb)),
    TABLE_ENTRY("movzx",        X86_MN_MOVZX,       PF_NONE | PF_PFXNA,     X86_ISET_GP,    OP(Gv, Ew)),

    /* B8h */
    TABLE_ENTRY("popcnt",       X86_MN_POPCNT,      PF_NONE | PF_PFXNA,     X86_ISET_SSE42, OP(Gv, Ev)),

    /* B9h: group 10, 8; both groups have prefix n/a */
    TABLE_ENTRY_ESCAPE(0F_B9),
    TABLE_ENTRY_ESCAPE(0F_BA),

    /* BBh */
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY("tzcnt",        X86_MN_TZCNT,       PF_NONE | PF_PFXNA,     X86_ISET_BMI1,  OP(Gv, Ev)),
    TABLE_ENTRY("lzcnt",        X86_MN_LZCNT,       PF_NONE | PF_PFXNA,     X86_ISET_BMI1,  OP(Gv, Ev)),
    TABLE_ENTRY("movsx",        X86_MN_MOVSX,       PF_NONE | PF_PFXNA,     X86_ISET_GP,    OP(Gv, Eb)),
    TABLE_ENTRY("movsx",        X86_MN_MOVSX,       PF_NONE | PF_PFXNA,     X86_ISET_GP,    OP(Gv, Ew)),

    /* C0h */
    TABLE_ENTRY("xadd",         X86_MN_XADD,        PF_LOCK | PF_PFXNA,     X86_ISET_GP,        OP(Eb, Gb)),
    TABLE_ENTRY("xadd",         X86_MN_XADD,        PF_LOCK | PF_PFXNA,     X86_ISET_GP,        OP(Ev, Gv)),
    TABLE_ENTRY("cmp*ss",       X86_MN_CMPSS,       PF_VEX | PF_CC,         X86_ISET_SSE,       OP(Vss, Hss, Wss, Ib)),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    /* C7h: group 9; group has prefix n/a */
    TABLE_ENTRY_ESCAPE(F3_0F_C7),

    /* C8h */
    TABLE_ENTRY("bswap",        X86_MN_BSWAP,       PF_NONE | PF_PFXNA,     X86_ISET_GP,    OP(rAX_r8)),
    TABLE_ENTRY("bswap",        X86_MN_BSWAP,       PF_NONE | PF_PFXNA,     X86_ISET_GP,    OP(rCX_r9)),
    TABLE_ENTRY("bswap",        X86_MN_BSWAP,       PF_NONE | PF_PFXNA,     X86_ISET_GP,    OP(rDX_r10)),
    TABLE_ENTRY("bswap",        X86_MN_BSWAP,       PF_NONE | PF_PFXNA,     X86_ISET_GP,    OP(rBX_r11)),
    TABLE_ENTRY("bswap",        X86_MN_BSWAP,       PF_NONE | PF_PFXNA,     X86_ISET_GP,    OP(rSP_r12)),
    TABLE_ENTRY("bswap",        X86_MN_BSWAP,       PF_NONE | PF_PFXNA,     X86_ISET_GP,    OP(rBP_r13)),
    TABLE_ENTRY("bswap",        X86_MN_BSWAP,       PF_NONE | PF_PFXNA,     X86_ISET_GP,    OP(rSI_r14)),
    TABLE_ENTRY("bswap",        X86_MN_BSWAP,       PF_NONE | PF_PFXNA,     X86_ISET_GP,    OP(rDI_r15)),

    /* D0h */
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY("movq2dq",      X86_MN_MOVQ2DQ,     PF_NONE,        X86_ISET_SSE2,  OP(Vdq, Nq)),
    TABLE_ENTRY_INVALID(),

    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    /* E0h */
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY("cvtdq2pd",     X86_MN_CVTDQ2PD,    PF_VEX,         X86_ISET_SSE2,  OP(Vx, Wpd)),
    TABLE_ENTRY_INVALID(),

    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    /* F0h */
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY("ud0",          X86_MN_UD,          PF_VEX128,      X86_ISET_MMX,   OP(Pq, Qq))

TABLE_END(F3_0F)

TABLE_BEGIN(F3_0F_01)

    /* modr/m byte reg/nnn field */

    /* 00h: sgdt or r/m extension */
    TABLE_ENTRY_ESCAPE(0F_01_REG0),

    /* 01h: sidt or r/m extension */
    TABLE_ENTRY_ESCAPE(0F_01_REG1),

    /* 02h: lgdt or r/m extension */
    TABLE_ENTRY_ESCAPE(0F_01_REG2),

    /* 03h: lidt or r/m extension */
    TABLE_ENTRY_ESCAPE(0F_01_REG3),

    /* 04h: smsw Mw/Rv */
    TABLE_ENTRY_ESCAPE(0F_01_REG4),

    /* 05h */
    TABLE_ENTRY_ESCAPE(F3_0F_01_REG5),

    /* 06h */
    TABLE_ENTRY("lmsw", X86_MN_LMSW,    PF_NONE | PF_PFXNA, X86_ISET_GP,        OP(Ew)),

    /* 07h: invlpg or r/m extension */
    TABLE_ENTRY_ESCAPE(0F_01_REG7)

TABLE_END(F3_0F_01)

TABLE_BEGIN(F3_0F_01_REG5)

    /* modr/m byte mod field:  0 = !11b, 1 = 11b */
    TABLE_ENTRY("rstorssp",         X86_MN_RSTORSSP,        PF_NONE,            X86_ISET_CET,       OP(Mq)),

    /* r/m extension */
    TABLE_ENTRY_ESCAPE(F3_0F_01_REG5_RM)

TABLE_END(F3_0F_01_REG5)

TABLE_BEGIN(F3_0F_01_REG5_RM)

    /* modr/m byte r/m field */
    TABLE_ENTRY("setssbsy",         X86_MN_SETSSBSY,        PF_NONE,            X86_ISET_CET,       OP(NONE)),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY("saveprevssp",      X86_MN_SAVEPREVSSP,     PF_NONE,            X86_ISET_CET,       OP(NONE)),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID()

TABLE_END(F3_0F_01_REG5_RM)

TABLE_BEGIN(F3_0F_19)

    /* modr/m byte reg/nnn field */
    TABLE_ENTRY("nop",  X86_MN_NOP,     PF_NONE | PF_PFXNA,         X86_ISET_GP, OP(Ev)),
    TABLE_ENTRY("nop",  X86_MN_NOP,     PF_NONE | PF_PFXNA,         X86_ISET_GP, OP(Ev)),
    TABLE_ENTRY("nop",  X86_MN_NOP,     PF_NONE | PF_PFXNA,         X86_ISET_GP, OP(Ev)),
    TABLE_ENTRY("nop",  X86_MN_NOP,     PF_NONE | PF_PFXNA,         X86_ISET_GP, OP(Ev)),
    TABLE_ENTRY("nop",  X86_MN_NOP,     PF_NONE | PF_PFXNA,         X86_ISET_GP, OP(Ev)),
    TABLE_ENTRY("nop",  X86_MN_NOP,     PF_NONE | PF_PFXNA,         X86_ISET_GP, OP(Ev)),
    TABLE_ENTRY("nop",  X86_MN_NOP,     PF_NONE | PF_PFXNA,         X86_ISET_GP, OP(Ev)),
    TABLE_ENTRY("nop",  X86_MN_NOP,     PF_NONE | PF_PFXNA,         X86_ISET_GP, OP(Ev))

TABLE_END(F3_0F_19)

TABLE_BEGIN(F3_0F_1C)

    /* modr/m byte reg/nnn field */
    TABLE_ENTRY("nop",  X86_MN_NOP,     PF_NONE | PF_PFXNA,         X86_ISET_GP, OP(Ev)),
    TABLE_ENTRY("nop",  X86_MN_NOP,     PF_NONE | PF_PFXNA,         X86_ISET_GP, OP(Ev)),
    TABLE_ENTRY("nop",  X86_MN_NOP,     PF_NONE | PF_PFXNA,         X86_ISET_GP, OP(Ev)),
    TABLE_ENTRY("nop",  X86_MN_NOP,     PF_NONE | PF_PFXNA,         X86_ISET_GP, OP(Ev)),
    TABLE_ENTRY("nop",  X86_MN_NOP,     PF_NONE | PF_PFXNA,         X86_ISET_GP, OP(Ev)),
    TABLE_ENTRY("nop",  X86_MN_NOP,     PF_NONE | PF_PFXNA,         X86_ISET_GP, OP(Ev)),
    TABLE_ENTRY("nop",  X86_MN_NOP,     PF_NONE | PF_PFXNA,         X86_ISET_GP, OP(Ev)),
    TABLE_ENTRY("nop",  X86_MN_NOP,     PF_NONE | PF_PFXNA,         X86_ISET_GP, OP(Ev))

TABLE_END(F3_0F_1C)

TABLE_BEGIN(F3_0F_1D)

    /* modr/m byte reg/nnn field */
    TABLE_ENTRY("nop",  X86_MN_NOP,     PF_NONE | PF_PFXNA,         X86_ISET_GP, OP(Ev)),
    TABLE_ENTRY("nop",  X86_MN_NOP,     PF_NONE | PF_PFXNA,         X86_ISET_GP, OP(Ev)),
    TABLE_ENTRY("nop",  X86_MN_NOP,     PF_NONE | PF_PFXNA,         X86_ISET_GP, OP(Ev)),
    TABLE_ENTRY("nop",  X86_MN_NOP,     PF_NONE | PF_PFXNA,         X86_ISET_GP, OP(Ev)),
    TABLE_ENTRY("nop",  X86_MN_NOP,     PF_NONE | PF_PFXNA,         X86_ISET_GP, OP(Ev)),
    TABLE_ENTRY("nop",  X86_MN_NOP,     PF_NONE | PF_PFXNA,         X86_ISET_GP, OP(Ev)),
    TABLE_ENTRY("nop",  X86_MN_NOP,     PF_NONE | PF_PFXNA,         X86_ISET_GP, OP(Ev)),
    TABLE_ENTRY("nop",  X86_MN_NOP,     PF_NONE | PF_PFXNA,         X86_ISET_GP, OP(Ev))

TABLE_END(F3_0F_1D)

TABLE_BEGIN(F3_0F_1E)

    /* modr/m byte reg/nnn field */
    TABLE_ENTRY("nop",  X86_MN_NOP,     PF_NONE | PF_PFXNA,         X86_ISET_GP, OP(Ev)),

    TABLE_ENTRY_ESCAPE(F3_0F_1E_REG1),

    TABLE_ENTRY("nop",  X86_MN_NOP,     PF_NONE | PF_PFXNA,         X86_ISET_GP, OP(Ev)),
    TABLE_ENTRY("nop",  X86_MN_NOP,     PF_NONE | PF_PFXNA,         X86_ISET_GP, OP(Ev)),
    TABLE_ENTRY("nop",  X86_MN_NOP,     PF_NONE | PF_PFXNA,         X86_ISET_GP, OP(Ev)),
    TABLE_ENTRY("nop",  X86_MN_NOP,     PF_NONE | PF_PFXNA,         X86_ISET_GP, OP(Ev)),
    TABLE_ENTRY("nop",  X86_MN_NOP,     PF_NONE | PF_PFXNA,         X86_ISET_GP, OP(Ev)),
    
    TABLE_ENTRY_ESCAPE(F3_0F_1E_REG7_RM)

TABLE_END(F3_0F_1E)

TABLE_BEGIN(F3_0F_1E_REG1)

    TABLE_ENTRY("rdsspd",       X86_MN_RDSSPD,      PF_NONE,    X86_ISET_CET,       OP(Rd)),
    TABLE_ENTRY("rdsspd",       X86_MN_RDSSPD,      PF_NONE,    X86_ISET_CET,       OP(Rd)),
    TABLE_ENTRY("rdsspq",       X86_MN_RDSSPQ,      PF_NONE,    X86_ISET_CET,       OP(Rq))

TABLE_END(F3_0F_1E_REG1)

TABLE_BEGIN(F3_0F_1E_REG7_RM)

    /* modr/m byte r/m field */
    TABLE_ENTRY("nop",  X86_MN_NOP,     PF_NONE | PF_PFXNA,         X86_ISET_GP, OP(Ev)),
    TABLE_ENTRY("nop",  X86_MN_NOP,     PF_NONE | PF_PFXNA,         X86_ISET_GP, OP(Ev)),
    TABLE_ENTRY("endbr64",      X86_MN_ENDBR64,     PF_NONE,        X86_ISET_GP, OP(NONE)),
    TABLE_ENTRY("endbr32",      X86_MN_ENDBR32,     PF_NONE,        X86_ISET_GP, OP(NONE)),
    TABLE_ENTRY("nop",  X86_MN_NOP,     PF_NONE | PF_PFXNA,         X86_ISET_GP, OP(Ev)),
    TABLE_ENTRY("nop",  X86_MN_NOP,     PF_NONE | PF_PFXNA,         X86_ISET_GP, OP(Ev)),
    TABLE_ENTRY("nop",  X86_MN_NOP,     PF_NONE | PF_PFXNA,         X86_ISET_GP, OP(Ev)),
    TABLE_ENTRY("nop",  X86_MN_NOP,     PF_NONE | PF_PFXNA,         X86_ISET_GP, OP(Ev))

TABLE_END(F3_0F_1E_REG7_RM)

TABLE_BEGIN(F3_0F_1F)

    /* modr/m byte reg/nnn field */
    TABLE_ENTRY("nop",  X86_MN_NOP,     PF_NONE | PF_PFXNA,         X86_ISET_GP, OP(Ev)),
    TABLE_ENTRY("nop",  X86_MN_NOP,     PF_NONE | PF_PFXNA,         X86_ISET_GP, OP(Ev)),
    TABLE_ENTRY("nop",  X86_MN_NOP,     PF_NONE | PF_PFXNA,         X86_ISET_GP, OP(Ev)),
    TABLE_ENTRY("nop",  X86_MN_NOP,     PF_NONE | PF_PFXNA,         X86_ISET_GP, OP(Ev)),
    TABLE_ENTRY("nop",  X86_MN_NOP,     PF_NONE | PF_PFXNA,         X86_ISET_GP, OP(Ev)),
    TABLE_ENTRY("nop",  X86_MN_NOP,     PF_NONE | PF_PFXNA,         X86_ISET_GP, OP(Ev)),
    TABLE_ENTRY("nop",  X86_MN_NOP,     PF_NONE | PF_PFXNA,         X86_ISET_GP, OP(Ev)),
    TABLE_ENTRY("nop",  X86_MN_NOP,     PF_NONE | PF_PFXNA,         X86_ISET_GP, OP(Ev))

TABLE_END(F3_0F_1F)

TABLE_BEGIN(F3_0F_71)

    /* modr/m byte reg/nnn field */

    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID()

TABLE_END(F3_0F_71)

TABLE_BEGIN(F3_0F_72)

    /* modr/m byte reg/nnn field */

    /* 00h */
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID()

TABLE_END(F3_0F_72)

TABLE_BEGIN(F3_0F_73)

    /* modr/m byte reg/nnn field */

    /* 00h */
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID()

TABLE_END(F3_0F_73)

TABLE_BEGIN(F3_0F_AE)

    /* modr/m byte reg/nnn field */

    TABLE_ENTRY("rdfsbase",     X86_MN_RDFSBASE,    PF_NONE,        X86_ISET_GP,    OP(Ry)),
    TABLE_ENTRY("wrfsbase",     X86_MN_WRFSBASE,    PF_NONE,        X86_ISET_GP,    OP(Ry)),
    TABLE_ENTRY("rdgsbase",     X86_MN_RDGSBASE,    PF_NONE,        X86_ISET_GP,    OP(Ry)),
    TABLE_ENTRY("wrgsbase",     X86_MN_WRGSBASE,    PF_NONE,        X86_ISET_GP,    OP(Ry)),
    TABLE_ENTRY("ptwrite",      X86_MN_PTWRITE,     PF_NONE,        X86_ISET_GP,    OP(Ry)),
    TABLE_ENTRY_ESCAPE(F3_0F_AE_REG5),
    TABLE_ENTRY("clrssbsy",     X86_MN_CLRSSBSY,    PF_NONE,        X86_ISET_CET,   OP(Mq)),
    TABLE_ENTRY_INVALID()

TABLE_END(F3_0F_AE)

TABLE_BEGIN(F3_0F_AE_REG5)

    /* operand size: 0 = 16-bits, 1 = 32-bits, 2 = 64-bits */
    TABLE_ENTRY("incsspd",          X86_MN_INCSSPD,         PF_NONE,        X86_ISET_CET,       OP(Rd)),
    TABLE_ENTRY("incsspd",          X86_MN_INCSSPD,         PF_NONE,        X86_ISET_CET,       OP(Rd)),
    TABLE_ENTRY("incsspq",          X86_MN_INCSSPQ,         PF_NONE,        X86_ISET_CET,       OP(Rq))

TABLE_END(F3_0F_AE_REG5)

TABLE_BEGIN(F3_0F_C7)

    /* modr/m byte reg/nnn field */

    /* 00h */
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    
    /* vmxon or rdrand */
    TABLE_ENTRY_ESCAPE(F3_0F_C7_REG6),

    /* vmptrst or rdseed */
    TABLE_ENTRY_ESCAPE(F3_0F_C7_REG7)

TABLE_END(F3_0F_C7)

TABLE_BEGIN(F3_0F_C7_REG6)

    /* modr/m byte mod field:  0 = !11b, 1 = 11b */
    TABLE_ENTRY("vmxon",        X86_MN_VMXON,       PF_NONE | PF_PFXNA,     X86_ISET_VMX,   OP(Mq)),
    TABLE_ENTRY("rdrand",       X86_MN_RDRAND,      PF_NONE | PF_PFXNA,     X86_ISET_GP,    OP(Rv))

TABLE_END(F3_0F_C7_REG6)

TABLE_BEGIN(F3_0F_C7_REG7)

    /* modr/m byte mod field:  0 = !11b, 1 = 11b */
    TABLE_ENTRY("vmptrst",      X86_MN_VMPTRST,     PF_NONE | PF_PFXNA,     X86_ISET_VMX,   OP(Mq)),
    TABLE_ENTRY("rdseed",       X86_MN_RDSEED,      PF_NONE | PF_PFXNA,     X86_ISET_GP,    OP(Rv))

TABLE_END(F3_0F_C7_REG7)

/****************************************************************************

    Two-byte tables - F2h mandatory prefix

****************************************************************************/

TABLE_BEGIN(F2_0F)

    /* 00h: group 6, 7; both groups have prefix n/a */
    TABLE_ENTRY_ESCAPE(0F_00),
    TABLE_ENTRY_ESCAPE(0F_01),

    /* 02h */
    TABLE_ENTRY("lar",          X86_MN_LAR,         PF_NONE | PF_PFXNA,     X86_ISET_GP,    OP(Gv, Ew)),
    TABLE_ENTRY("lsl",          X86_MN_LSL,         PF_NONE | PF_PFXNA,     X86_ISET_GP,    OP(Gv, Ew)),

    /* 04h */
    TABLE_ENTRY_INVALID(),

    /* 05h */
    TABLE_ENTRY("syscall",      X86_MN_SYSCALL,     PF_O64 | PF_PFXNA,          X86_ISET_GP,    OP(NONE)),
    TABLE_ENTRY("clts",         X86_MN_CLTS,        PF_NONE | PF_PFXNA,         X86_ISET_GP,    OP(NONE)),
    TABLE_ENTRY("sysret",       X86_MN_SYSRET,      PF_O64 | PF_PFXNA,          X86_ISET_GP,    OP(NONE)),
    TABLE_ENTRY("invd",         X86_MN_INVD,        PF_NONE | PF_PFXNA,         X86_ISET_GP,    OP(NONE)),
    TABLE_ENTRY("wbinvd",       X86_MN_WBINVD,      PF_NONE | PF_PFXNA,         X86_ISET_GP,    OP(NONE)),

    /* 0Ah */
    TABLE_ENTRY_INVALID(),

    /* 0Bh */
    TABLE_ENTRY("ud2",          X86_MN_UD2,         PF_NONE | PF_PFXNA,         X86_ISET_GP,    OP(NONE)),

    /* 0Ch */
    TABLE_ENTRY_INVALID(),

    /* 0Dh: group p; group has prefix n/a */
    TABLE_ENTRY_ESCAPE(0F_0D),

    /* 0Eh */
    TABLE_ENTRY("femms",        X86_MN_FEMMS,       PF_NONE | PF_PFXNA,         X86_ISET_GP,    OP(NONE)),

    /* 0Fh: AMD only, 3DNow! escape */
    TABLE_ENTRY_INVALID(),
    
    /* 10h */
    TABLE_ENTRY("movsd",        X86_MN_MOVSD,       PF_VEX128,      X86_ISET_SSE2,  OP(Vx, Hx, Wsd)),
    TABLE_ENTRY("movsd",        X86_MN_MOVSD,       PF_VEX128,      X86_ISET_SSE2,  OP(Wq, Hx, Vsd)),

    /* 12h */
    TABLE_ENTRY("movddup",      X86_MN_MOVDDUP,     PF_VEX,         X86_ISET_SSE3,  OP(Vx, Wx)),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    /* 18h: group 16; group has prefix n/a */
    TABLE_ENTRY_ESCAPE(0F_18),

    /* 19h */
    TABLE_ENTRY_ESCAPE(F2_0F_19),

    TABLE_ENTRY("nop",          X86_MN_NOP,     PF_NONE | PF_PFXNA,     X86_ISET_GP,    OP(Ev)),
    TABLE_ENTRY("nop",          X86_MN_NOP,     PF_NONE | PF_PFXNA,     X86_ISET_GP,    OP(Ev)),

    TABLE_ENTRY_ESCAPE(F2_0F_1C),
    TABLE_ENTRY_ESCAPE(F2_0F_1D),
    TABLE_ENTRY_ESCAPE(F2_0F_1E),
    TABLE_ENTRY_ESCAPE(F2_0F_1F),

    /* 20h */
    TABLE_ENTRY("mov",          X86_MN_MOV,     PF_F64 | PF_PFXNA,          X86_ISET_GP,    OP(Ry, Cy)),
    TABLE_ENTRY("mov",          X86_MN_MOV,     PF_F64 | PF_PFXNA,          X86_ISET_GP,    OP(Ry, Dy)),
    TABLE_ENTRY("mov",          X86_MN_MOV,     PF_F64 | PF_PFXNA,          X86_ISET_GP,    OP(Cy, Ry)),
    TABLE_ENTRY("mov",          X86_MN_MOV,     PF_F64 | PF_PFXNA,          X86_ISET_GP,    OP(Dy, Ry)),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    
    /* 28h */
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY("cvtsi2sd",     X86_MN_CVTSI2SD,    PF_VEX128,      X86_ISET_SSE2,  OP(Vsd, Hsd, Ey)),
    TABLE_ENTRY("movntsd",      X86_MN_MOVNTSD,     PF_NONE,        X86_ISET_SSE4A, OP(Mq, Vsd)),
    TABLE_ENTRY("cvttsd2si",    X86_MN_CVTTSD2SI,   PF_VEX128,      X86_ISET_SSE2,  OP(Gy, Wsd)),
    TABLE_ENTRY("cvtsd2si",     X86_MN_CVTSD2SI,    PF_VEX128,      X86_ISET_SSE2,  OP(Gy, Wsd)),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    /* 30h */
    TABLE_ENTRY("wrmsr",        X86_MN_WRMSR,       PF_NONE | PF_PFXNA,     X86_ISET_GP,    OP(NONE)),
    TABLE_ENTRY("rdtsc",        X86_MN_RDTSC,       PF_NONE | PF_PFXNA,     X86_ISET_GP,    OP(NONE)),
    TABLE_ENTRY("rdmsr",        X86_MN_RDMSR,       PF_NONE | PF_PFXNA,     X86_ISET_GP,    OP(NONE)),
    TABLE_ENTRY("rdpmc",        X86_MN_RDPMC,       PF_NONE | PF_PFXNA,     X86_ISET_GP,    OP(NONE)),
    TABLE_ENTRY("sysenter",     X86_MN_SYSENTER,    PF_NONE | PF_PFXNA,     X86_ISET_GP,    OP(NONE)),
    TABLE_ENTRY("sysexit",      X86_MN_SYSEXIT,     PF_NONE | PF_PFXNA,     X86_ISET_GP,    OP(NONE)),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    /* 38h: three-byte table escape */
    TABLE_ENTRY_ESCAPE(F2_0F_38),
    TABLE_ENTRY_INVALID(),

    /* 3Ah: three-byte table escape */
    TABLE_ENTRY_ESCAPE(F2_0F_3A),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    /* 40h */
    TABLE_ENTRY("cmovo",        X86_MN_CMOVO,       PF_NONE | PF_PFXNA, X86_ISET_GP,        OP(Gv, Ev)),
    TABLE_ENTRY("cmovno",       X86_MN_CMOVNO,      PF_NONE | PF_PFXNA, X86_ISET_GP,        OP(Gv, Ev)),
    TABLE_ENTRY("cmovc",        X86_MN_CMOVC,       PF_NONE | PF_PFXNA, X86_ISET_GP,        OP(Gv, Ev)),
    TABLE_ENTRY("cmovnc",       X86_MN_CMOVNC,      PF_NONE | PF_PFXNA, X86_ISET_GP,        OP(Gv, Ev)),
    TABLE_ENTRY("cmovz",        X86_MN_CMOVZ,       PF_NONE | PF_PFXNA, X86_ISET_GP,        OP(Gv, Ev)),
    TABLE_ENTRY("cmovnz",       X86_MN_CMOVNZ,      PF_NONE | PF_PFXNA, X86_ISET_GP,        OP(Gv, Ev)),
    TABLE_ENTRY("cmovna",       X86_MN_CMOVNA,      PF_NONE | PF_PFXNA, X86_ISET_GP,        OP(Gv, Ev)),
    TABLE_ENTRY("cmova",        X86_MN_CMOVA,       PF_NONE | PF_PFXNA, X86_ISET_GP,        OP(Gv, Ev)),
    TABLE_ENTRY("cmovs",        X86_MN_CMOVS,       PF_NONE | PF_PFXNA, X86_ISET_GP,        OP(Gv, Ev)),
    TABLE_ENTRY("cmovns",       X86_MN_CMOVNS,      PF_NONE | PF_PFXNA, X86_ISET_GP,        OP(Gv, Ev)),
    TABLE_ENTRY("cmovp",        X86_MN_CMOVP,       PF_NONE | PF_PFXNA, X86_ISET_GP,        OP(Gv, Ev)),
    TABLE_ENTRY("cmovnp",       X86_MN_CMOVPO,      PF_NONE | PF_PFXNA, X86_ISET_GP,        OP(Gv, Ev)),
    TABLE_ENTRY("cmovl",        X86_MN_CMOVL,       PF_NONE | PF_PFXNA, X86_ISET_GP,        OP(Gv, Ev)),
    TABLE_ENTRY("cmovnl",       X86_MN_CMOVNL,      PF_NONE | PF_PFXNA, X86_ISET_GP,        OP(Gv, Ev)),
    TABLE_ENTRY("cmovng",       X86_MN_CMOVNG,      PF_NONE | PF_PFXNA, X86_ISET_GP,        OP(Gv, Ev)),
    TABLE_ENTRY("cmovg",        X86_MN_CMOVG,       PF_NONE | PF_PFXNA, X86_ISET_GP,        OP(Gv, Ev)),

    /* 50h */
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY("sqrtsd",       X86_MN_SQRTSD,      PF_VEX128,      X86_ISET_SSE2,  OP(Vsd, Hsd, Wsd)),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY("addsd",        X86_MN_ADDSD,       PF_VEX128,      X86_ISET_SSE2,  OP(Vsd, Hsd, Wsd)),
    TABLE_ENTRY("mulsd",        X86_MN_MULSD,       PF_VEX128,      X86_ISET_SSE2,  OP(Vsd, Hsd, Wsd)),
    TABLE_ENTRY("cvtsd2ss",     X86_MN_CVTSD2SS,    PF_VEX128,      X86_ISET_SSE2,  OP(Vss, Hx, Wsd)),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY("subsd",        X86_MN_SUBSD,       PF_VEX128,      X86_ISET_SSE2,  OP(Vsd, Hsd, Wsd)),
    TABLE_ENTRY("minsd",        X86_MN_MINSD,       PF_VEX128,      X86_ISET_SSE2,  OP(Vsd, Hsd, Wsd)),
    TABLE_ENTRY("divsd",        X86_MN_DIVSD,       PF_VEX128,      X86_ISET_SSE2,  OP(Vsd, Hsd, Wsd)),
    TABLE_ENTRY("maxsd",        X86_MN_MAXSD,       PF_VEX128,      X86_ISET_SSE2,  OP(Vsd, Hsd, Wsd)),

    /* 60h */
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    /* 70h */
    TABLE_ENTRY("pshuflw",      X86_MN_PSHUFLW, PF_VEX,         X86_ISET_SSE2,  OP(Vx, Wx, Ib)),

    /* 71h: group 12, 13, 14 */
    TABLE_ENTRY_ESCAPE(F2_0F_71),
    TABLE_ENTRY_ESCAPE(F2_0F_72),
    TABLE_ENTRY_ESCAPE(F2_0F_73),

    /* 74h */
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    /* 78h */
    TABLE_ENTRY("insertq",      X86_MN_INSERTQ,     PF_NONE,        X86_ISET_SSE4A, OP(Vss, Uss, Ib, Ib)),
    TABLE_ENTRY("insertq",      X86_MN_INSERTQ,     PF_NONE,        X86_ISET_SSE4A, OP(Vss, Uss)),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY("haddps",       X86_MN_HADDPS,      PF_VEX,         X86_ISET_SSE3,  OP(Vps, Hps, Wps)),
    TABLE_ENTRY("hsubps",       X86_MN_HSUBPS,      PF_VEX,         X86_ISET_SSE3,  OP(Vps, Hps, Wps)),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    /* 80h */
    TABLE_ENTRY("jo",           X86_MN_JO,          PF_F64 | PF_PFXNA,      X86_ISET_GP,        OP(Jz)),
    TABLE_ENTRY("jno",          X86_MN_JNO,         PF_F64 | PF_PFXNA,      X86_ISET_GP,        OP(Jz)),
    TABLE_ENTRY("jc",           X86_MN_JC,          PF_F64 | PF_PFXNA,      X86_ISET_GP,        OP(Jz)),
    TABLE_ENTRY("jnc",          X86_MN_JNC,         PF_F64 | PF_PFXNA,      X86_ISET_GP,        OP(Jz)),
    TABLE_ENTRY("jz",           X86_MN_JZ,          PF_F64 | PF_PFXNA,      X86_ISET_GP,        OP(Jz)),
    TABLE_ENTRY("jnz",          X86_MN_JNZ,         PF_F64 | PF_PFXNA,      X86_ISET_GP,        OP(Jz)),
    TABLE_ENTRY("jna",          X86_MN_JNA,         PF_F64 | PF_PFXNA,      X86_ISET_GP,        OP(Jz)),
    TABLE_ENTRY("ja",           X86_MN_JA,          PF_F64 | PF_PFXNA,      X86_ISET_GP,        OP(Jz)),
    TABLE_ENTRY("js",           X86_MN_JS,          PF_F64 | PF_PFXNA,      X86_ISET_GP,        OP(Jz)),
    TABLE_ENTRY("jns",          X86_MN_JNS,         PF_F64 | PF_PFXNA,      X86_ISET_GP,        OP(Jz)),
    TABLE_ENTRY("jp",           X86_MN_JP,          PF_F64 | PF_PFXNA,      X86_ISET_GP,        OP(Jz)),
    TABLE_ENTRY("jnp",          X86_MN_JNP,         PF_F64 | PF_PFXNA,      X86_ISET_GP,        OP(Jz)),
    TABLE_ENTRY("jl",           X86_MN_JL,          PF_F64 | PF_PFXNA,      X86_ISET_GP,        OP(Jz)),
    TABLE_ENTRY("jnl",          X86_MN_JNL,         PF_F64 | PF_PFXNA,      X86_ISET_GP,        OP(Jz)),
    TABLE_ENTRY("jng",          X86_MN_JNG,         PF_F64 | PF_PFXNA,      X86_ISET_GP,        OP(Jz)),
    TABLE_ENTRY("jg",           X86_MN_JG,          PF_F64 | PF_PFXNA,      X86_ISET_GP,        OP(Jz)),

    /* 90h */
    TABLE_ENTRY("seto",         X86_MN_SETO,        PF_NONE | PF_PFXNA,     X86_ISET_GP,    OP(Eb)),
    TABLE_ENTRY("setno",        X86_MN_SETNO,       PF_NONE | PF_PFXNA,     X86_ISET_GP,    OP(Eb)),
    TABLE_ENTRY("setc",         X86_MN_SETC,        PF_NONE | PF_PFXNA,     X86_ISET_GP,    OP(Eb)),
    TABLE_ENTRY("setnc",        X86_MN_SETNC,       PF_NONE | PF_PFXNA,     X86_ISET_GP,    OP(Eb)),
    TABLE_ENTRY("setz",         X86_MN_SETZ,        PF_NONE | PF_PFXNA,     X86_ISET_GP,    OP(Eb)),
    TABLE_ENTRY("setnz",        X86_MN_SETNZ,       PF_NONE | PF_PFXNA,     X86_ISET_GP,    OP(Eb)),
    TABLE_ENTRY("setna",        X86_MN_SETNA,       PF_NONE | PF_PFXNA,     X86_ISET_GP,    OP(Eb)),
    TABLE_ENTRY("seta",         X86_MN_SETA,        PF_NONE | PF_PFXNA,     X86_ISET_GP,    OP(Eb)),
    TABLE_ENTRY("sets",         X86_MN_SETS,        PF_NONE | PF_PFXNA,     X86_ISET_GP,    OP(Eb)),
    TABLE_ENTRY("setns",        X86_MN_SETNS,       PF_NONE | PF_PFXNA,     X86_ISET_GP,    OP(Eb)),
    TABLE_ENTRY("setp",         X86_MN_SETP,        PF_NONE | PF_PFXNA,     X86_ISET_GP,    OP(Eb)),
    TABLE_ENTRY("setnp",        X86_MN_SETNP,       PF_NONE | PF_PFXNA,     X86_ISET_GP,    OP(Eb)),
    TABLE_ENTRY("setl",         X86_MN_SETL,        PF_NONE | PF_PFXNA,     X86_ISET_GP,    OP(Eb)),
    TABLE_ENTRY("setnl",        X86_MN_SETNL,       PF_NONE | PF_PFXNA,     X86_ISET_GP,    OP(Eb)),
    TABLE_ENTRY("setng",        X86_MN_SETNG,       PF_NONE | PF_PFXNA,     X86_ISET_GP,    OP(Eb)),
    TABLE_ENTRY("setg",         X86_MN_SETG,        PF_NONE | PF_PFXNA,     X86_ISET_GP,    OP(Eb)),

    /* A0h */
    TABLE_ENTRY("push",         X86_MN_PUSH,        PF_D64 | PF_PFXNA,          X86_ISET_GP,    OP(FS)),
    TABLE_ENTRY("pop",          X86_MN_POP,         PF_D64 | PF_PFXNA,          X86_ISET_GP,    OP(FS)),
    TABLE_ENTRY("cpuid",        X86_MN_CPUID,       PF_NONE | PF_PFXNA,         X86_ISET_GP,    OP(NONE)),
    TABLE_ENTRY("bt",           X86_MN_BT,          PF_NONE | PF_PFXNA,         X86_ISET_GP,    OP(Ev, Gv)),
    TABLE_ENTRY("shld",         X86_MN_SHLD,        PF_NONE | PF_PFXNA,         X86_ISET_GP,    OP(Ev, Gv, Ib)),
    TABLE_ENTRY("shld",         X86_MN_SHLD,        PF_NONE | PF_PFXNA,         X86_ISET_GP,    OP(Ev, Gv, CL)),

    /* A6h */
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    /* A8h */
    TABLE_ENTRY("push",         X86_MN_PUSH,        PF_D64 | PF_PFXNA,          X86_ISET_GP,    OP(GS)),
    TABLE_ENTRY("pop",          X86_MN_POP,         PF_D64 | PF_PFXNA,          X86_ISET_GP,    OP(GS)),
    TABLE_ENTRY("rsm",          X86_MN_RSM,         PF_NONE | PF_PFXNA,         X86_ISET_GP,    OP(NONE)),
    TABLE_ENTRY("bts",          X86_MN_BTS,         PF_LOCK | PF_PFXNA,         X86_ISET_GP,    OP(Ev, Gv)),
    TABLE_ENTRY("shrd",         X86_MN_SHRD,        PF_NONE | PF_PFXNA,         X86_ISET_GP,    OP(Ev, Gv, Ib)),
    TABLE_ENTRY("shrd",         X86_MN_SHRD,        PF_NONE | PF_PFXNA,         X86_ISET_GP,    OP(Ev, Gv, CL)),

    /* AEh: group 15 */
    TABLE_ENTRY_ESCAPE(F2_0F_AE),

    /* AFh */
    TABLE_ENTRY("imul",         X86_MN_IMUL,        PF_NONE | PF_PFXNA,         X86_ISET_GP,    OP(Gv, Ev)),

    /* B0h */
    TABLE_ENTRY("cmpxchg",      X86_MN_CMPXCHG,     PF_LOCK | PF_PFXNA,     X86_ISET_GP,    OP(Eb, Gb)),
    TABLE_ENTRY("cmpxchg",      X86_MN_CMPXCHG,     PF_LOCK | PF_PFXNA,     X86_ISET_GP,    OP(Ev, Gv)),
    TABLE_ENTRY("lss",          X86_MN_LSS,         PF_NONE | PF_PFXNA,     X86_ISET_GP,    OP(Gz, Mp)),
    TABLE_ENTRY("btr",          X86_MN_BTR,         PF_LOCK | PF_PFXNA,     X86_ISET_GP,    OP(Ev, Gv)),
    TABLE_ENTRY("lfs",          X86_MN_LFS,         PF_NONE | PF_PFXNA,     X86_ISET_GP,    OP(Gz, Mp)),
    TABLE_ENTRY("lgs",          X86_MN_LGS,         PF_NONE | PF_PFXNA,     X86_ISET_GP,    OP(Gz, Mp)),
    TABLE_ENTRY("movzx",        X86_MN_MOVZX,       PF_NONE | PF_PFXNA,     X86_ISET_GP,    OP(Gv, Eb)),
    TABLE_ENTRY("movzx",        X86_MN_MOVZX,       PF_NONE | PF_PFXNA,     X86_ISET_GP,    OP(Gv, Ew)),

    /* B8h */
    TABLE_ENTRY_INVALID(),

    /* B9h: group 10, 8; both groups have prefix n/a */
    TABLE_ENTRY_ESCAPE(0F_B9),
    TABLE_ENTRY_ESCAPE(0F_BA),

    /* BBh */
    TABLE_ENTRY("btc",          X86_MN_BTC,         PF_LOCK | PF_PFXNA,     X86_ISET_GP,    OP(Ev, Gv)),
    TABLE_ENTRY("bsf",          X86_MN_BSF,         PF_NONE | PF_PFXNA,     X86_ISET_GP,    OP(Gv, Ev)),
    TABLE_ENTRY("bsr",          X86_MN_BSR,         PF_NONE | PF_PFXNA,     X86_ISET_GP,    OP(Gv, Ev)),
    TABLE_ENTRY("movsx",        X86_MN_MOVSX,       PF_NONE | PF_PFXNA,     X86_ISET_GP,    OP(Gv, Eb)),
    TABLE_ENTRY("movsx",        X86_MN_MOVSX,       PF_NONE | PF_PFXNA,     X86_ISET_GP,    OP(Gv, Ew)),
        
    /* C0h */
    TABLE_ENTRY("xadd",         X86_MN_XADD,        PF_LOCK | PF_PFXNA,     X86_ISET_GP,        OP(Eb, Gb)),
    TABLE_ENTRY("xadd",         X86_MN_XADD,        PF_LOCK | PF_PFXNA,     X86_ISET_GP,        OP(Ev, Gv)),
    TABLE_ENTRY("cmp*sd",       X86_MN_CMPSD,       PF_VEX | PF_CC,         X86_ISET_SSE2,      OP(Vsd, Hsd, Wq, Ib)),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    /* C7h: group 9; group has prefix n/a */
    TABLE_ENTRY_ESCAPE(0F_C7),

    /* C8h */
    TABLE_ENTRY("bswap",        X86_MN_BSWAP,       PF_NONE | PF_PFXNA,     X86_ISET_GP,    OP(rAX_r8)),
    TABLE_ENTRY("bswap",        X86_MN_BSWAP,       PF_NONE | PF_PFXNA,     X86_ISET_GP,    OP(rCX_r9)),
    TABLE_ENTRY("bswap",        X86_MN_BSWAP,       PF_NONE | PF_PFXNA,     X86_ISET_GP,    OP(rDX_r10)),
    TABLE_ENTRY("bswap",        X86_MN_BSWAP,       PF_NONE | PF_PFXNA,     X86_ISET_GP,    OP(rBX_r11)),
    TABLE_ENTRY("bswap",        X86_MN_BSWAP,       PF_NONE | PF_PFXNA,     X86_ISET_GP,    OP(rSP_r12)),
    TABLE_ENTRY("bswap",        X86_MN_BSWAP,       PF_NONE | PF_PFXNA,     X86_ISET_GP,    OP(rBP_r13)),
    TABLE_ENTRY("bswap",        X86_MN_BSWAP,       PF_NONE | PF_PFXNA,     X86_ISET_GP,    OP(rSI_r14)),
    TABLE_ENTRY("bswap",        X86_MN_BSWAP,       PF_NONE | PF_PFXNA,     X86_ISET_GP,    OP(rDI_r15)),

    /* D0h */
    TABLE_ENTRY("addsubps",     X86_MN_ADDSUBPS,    PF_VEX,         X86_ISET_SSE3,  OP(Vps, Hps, Wps)),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY("movdq2q",      X86_MN_MOVDQ2Q,     PF_NONE,        X86_ISET_SSE2,  OP(Pq, Uq)),
    TABLE_ENTRY_INVALID(),

    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    /* E0h */
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY("cvtpd2dq",     X86_MN_CVTPD2DQ,    PF_VEX,         X86_ISET_SSE2,  OP(Vx, Wpd)),
    TABLE_ENTRY_INVALID(),

    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    /* F0h */
    TABLE_ENTRY("lddqu",        X86_MN_LDDQU,       PF_VEX,         X86_ISET_SSE3,  OP(Vx, Wx)),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY("ud0",          X86_MN_UD,          PF_VEX128,      X86_ISET_MMX,   OP(Pq, Qq))

TABLE_END(F2_0F)

TABLE_BEGIN(F2_0F_19)

    /* modr/m byte reg/nnn field */
    TABLE_ENTRY("nop",  X86_MN_NOP,     PF_NONE | PF_PFXNA,         X86_ISET_GP, OP(Ev)),
    TABLE_ENTRY("nop",  X86_MN_NOP,     PF_NONE | PF_PFXNA,         X86_ISET_GP, OP(Ev)),
    TABLE_ENTRY("nop",  X86_MN_NOP,     PF_NONE | PF_PFXNA,         X86_ISET_GP, OP(Ev)),
    TABLE_ENTRY("nop",  X86_MN_NOP,     PF_NONE | PF_PFXNA,         X86_ISET_GP, OP(Ev)),
    TABLE_ENTRY("nop",  X86_MN_NOP,     PF_NONE | PF_PFXNA,         X86_ISET_GP, OP(Ev)),
    TABLE_ENTRY("nop",  X86_MN_NOP,     PF_NONE | PF_PFXNA,         X86_ISET_GP, OP(Ev)),
    TABLE_ENTRY("nop",  X86_MN_NOP,     PF_NONE | PF_PFXNA,         X86_ISET_GP, OP(Ev)),
    TABLE_ENTRY("nop",  X86_MN_NOP,     PF_NONE | PF_PFXNA,         X86_ISET_GP, OP(Ev))

TABLE_END(F2_0F_19)

TABLE_BEGIN(F2_0F_1C)

    /* modr/m byte reg/nnn field */
    TABLE_ENTRY("nop",  X86_MN_NOP,     PF_NONE | PF_PFXNA,         X86_ISET_GP, OP(Ev)),
    TABLE_ENTRY("nop",  X86_MN_NOP,     PF_NONE | PF_PFXNA,         X86_ISET_GP, OP(Ev)),
    TABLE_ENTRY("nop",  X86_MN_NOP,     PF_NONE | PF_PFXNA,         X86_ISET_GP, OP(Ev)),
    TABLE_ENTRY("nop",  X86_MN_NOP,     PF_NONE | PF_PFXNA,         X86_ISET_GP, OP(Ev)),
    TABLE_ENTRY("nop",  X86_MN_NOP,     PF_NONE | PF_PFXNA,         X86_ISET_GP, OP(Ev)),
    TABLE_ENTRY("nop",  X86_MN_NOP,     PF_NONE | PF_PFXNA,         X86_ISET_GP, OP(Ev)),
    TABLE_ENTRY("nop",  X86_MN_NOP,     PF_NONE | PF_PFXNA,         X86_ISET_GP, OP(Ev)),
    TABLE_ENTRY("nop",  X86_MN_NOP,     PF_NONE | PF_PFXNA,         X86_ISET_GP, OP(Ev))

TABLE_END(F2_0F_1C)

TABLE_BEGIN(F2_0F_1D)

    /* modr/m byte reg/nnn field */
    TABLE_ENTRY("nop",  X86_MN_NOP,     PF_NONE | PF_PFXNA,         X86_ISET_GP, OP(Ev)),
    TABLE_ENTRY("nop",  X86_MN_NOP,     PF_NONE | PF_PFXNA,         X86_ISET_GP, OP(Ev)),
    TABLE_ENTRY("nop",  X86_MN_NOP,     PF_NONE | PF_PFXNA,         X86_ISET_GP, OP(Ev)),
    TABLE_ENTRY("nop",  X86_MN_NOP,     PF_NONE | PF_PFXNA,         X86_ISET_GP, OP(Ev)),
    TABLE_ENTRY("nop",  X86_MN_NOP,     PF_NONE | PF_PFXNA,         X86_ISET_GP, OP(Ev)),
    TABLE_ENTRY("nop",  X86_MN_NOP,     PF_NONE | PF_PFXNA,         X86_ISET_GP, OP(Ev)),
    TABLE_ENTRY("nop",  X86_MN_NOP,     PF_NONE | PF_PFXNA,         X86_ISET_GP, OP(Ev)),
    TABLE_ENTRY("nop",  X86_MN_NOP,     PF_NONE | PF_PFXNA,         X86_ISET_GP, OP(Ev))

TABLE_END(F2_0F_1D)

TABLE_BEGIN(F2_0F_1E)

    /* modr/m byte reg/nnn field */
    TABLE_ENTRY("nop",  X86_MN_NOP,     PF_NONE | PF_PFXNA,         X86_ISET_GP, OP(Ev)),
    TABLE_ENTRY("nop",  X86_MN_NOP,     PF_NONE | PF_PFXNA,         X86_ISET_GP, OP(Ev)),
    TABLE_ENTRY("nop",  X86_MN_NOP,     PF_NONE | PF_PFXNA,         X86_ISET_GP, OP(Ev)),
    TABLE_ENTRY("nop",  X86_MN_NOP,     PF_NONE | PF_PFXNA,         X86_ISET_GP, OP(Ev)),
    TABLE_ENTRY("nop",  X86_MN_NOP,     PF_NONE | PF_PFXNA,         X86_ISET_GP, OP(Ev)),
    TABLE_ENTRY("nop",  X86_MN_NOP,     PF_NONE | PF_PFXNA,         X86_ISET_GP, OP(Ev)),
    TABLE_ENTRY("nop",  X86_MN_NOP,     PF_NONE | PF_PFXNA,         X86_ISET_GP, OP(Ev)),
    TABLE_ENTRY("nop",  X86_MN_NOP,     PF_NONE | PF_PFXNA,         X86_ISET_GP, OP(Ev))

TABLE_END(F2_0F_1E)

TABLE_BEGIN(F2_0F_1F)

    /* modr/m byte reg/nnn field */
    TABLE_ENTRY("nop",  X86_MN_NOP,     PF_NONE | PF_PFXNA,         X86_ISET_GP, OP(Ev)),
    TABLE_ENTRY("nop",  X86_MN_NOP,     PF_NONE | PF_PFXNA,         X86_ISET_GP, OP(Ev)),
    TABLE_ENTRY("nop",  X86_MN_NOP,     PF_NONE | PF_PFXNA,         X86_ISET_GP, OP(Ev)),
    TABLE_ENTRY("nop",  X86_MN_NOP,     PF_NONE | PF_PFXNA,         X86_ISET_GP, OP(Ev)),
    TABLE_ENTRY("nop",  X86_MN_NOP,     PF_NONE | PF_PFXNA,         X86_ISET_GP, OP(Ev)),
    TABLE_ENTRY("nop",  X86_MN_NOP,     PF_NONE | PF_PFXNA,         X86_ISET_GP, OP(Ev)),
    TABLE_ENTRY("nop",  X86_MN_NOP,     PF_NONE | PF_PFXNA,         X86_ISET_GP, OP(Ev)),
    TABLE_ENTRY("nop",  X86_MN_NOP,     PF_NONE | PF_PFXNA,         X86_ISET_GP, OP(Ev))

TABLE_END(F2_0F_1F)

TABLE_BEGIN(F2_0F_71)

    /* modr/m byte reg/nnn field */

    /* 00h */
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID()

TABLE_END(F2_0F_71)

TABLE_BEGIN(F2_0F_72)

    //* modr/m byte reg/nnn field */

    /* 00h */
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID()

TABLE_END(F2_0F_72)

TABLE_BEGIN(F2_0F_73)

    /* modr/m byte reg/nnn field */

    /* 00h */
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID()

TABLE_END(F2_0F_73)

TABLE_BEGIN(F2_0F_AE)

    /* modr/m byte reg/nnn field */

    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID()

TABLE_END(F2_0F_AE)

/****************************************************************************

    Three-byte tables - no mandatory prefix

****************************************************************************/

TABLE_BEGIN(0F_38)

    /* 00h */
    TABLE_ENTRY("pshufb",       X86_MN_PSHUFB,      PF_NONE,        X86_ISET_SSSE3, OP(Pq, Qq)),
    TABLE_ENTRY("phaddw",       X86_MN_PHADDW,      PF_NONE,        X86_ISET_SSSE3, OP(Pq, Qq)),
    TABLE_ENTRY("phaddd",       X86_MN_PHADDD,      PF_NONE,        X86_ISET_SSSE3, OP(Pq, Qq)),
    TABLE_ENTRY("phaddsw",      X86_MN_PHADDSW,     PF_NONE,        X86_ISET_SSSE3, OP(Pq, Qq)),
    TABLE_ENTRY("pmaddubsw",    X86_MN_PMADDUBSW,   PF_NONE,        X86_ISET_SSSE3, OP(Pq, Qq)),
    TABLE_ENTRY("phsubw",       X86_MN_PHSUBW,      PF_NONE,        X86_ISET_SSSE3, OP(Pq, Qq)),
    TABLE_ENTRY("phsubd",       X86_MN_PHSUBD,      PF_NONE,        X86_ISET_SSSE3, OP(Pq, Qq)),
    TABLE_ENTRY("phsubsw",      X86_MN_PHSUBSW,     PF_NONE,        X86_ISET_SSSE3, OP(Pq, Qq)),
    TABLE_ENTRY("psignb",       X86_MN_PSIGNB,      PF_NONE,        X86_ISET_SSSE3, OP(Pq, Qq)),
    TABLE_ENTRY("psignw",       X86_MN_PSIGNW,      PF_NONE,        X86_ISET_SSSE3, OP(Pq, Qq)),
    TABLE_ENTRY("psignd",       X86_MN_PSIGND,      PF_NONE,        X86_ISET_SSSE3, OP(Pq, Qq)),
    TABLE_ENTRY("pmulhrsw",     X86_MN_PMULHRSW,    PF_NONE,        X86_ISET_SSSE3, OP(Pq, Qq)),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    /* 10h */
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY("pabsb",        X86_MN_PABSB,       PF_NONE,        X86_ISET_SSSE3, OP(Pq, Qq)),
    TABLE_ENTRY("pabsw",        X86_MN_PABSW,       PF_NONE,        X86_ISET_SSSE3, OP(Pq, Qq)),
    TABLE_ENTRY("pabsd",        X86_MN_PABSD,       PF_NONE,        X86_ISET_SSSE3, OP(Pq, Qq)),
    TABLE_ENTRY_INVALID(),

    /* 20h */
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    /* 30h */
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    /* 40h */
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    /* 50h */
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    /* 60h */
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    /* 70h */
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    /* 80h */
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    /* 90h */
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    /* A0h */
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    /* B0h */
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    /* C0h */
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    /* D0h */
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    /* E0h */
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    /* F0h */
    TABLE_ENTRY("movbe",        X86_MN_MOVBE,       PF_NONE,        X86_ISET_GP,    OP(Gy, My)),
    TABLE_ENTRY("movbe",        X86_MN_MOVBE,       PF_NONE,        X86_ISET_GP,    OP(My, Gy)),
    TABLE_ENTRY("andn",         X86_MN_ANDN,        PF_VEX,         X86_ISET_BMI1,  OP(Gy, By, Ey)),

    /* F3h: group 17; vex prefix is required */
    TABLE_ENTRY_ESCAPE(0F_38_F3),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY("bzhi",         X86_MN_BZHI,        PF_VEX,         X86_ISET_BMI2,  OP(Gy, Ey, By)),
    TABLE_ENTRY_ESCAPE(0F_38_F6),
    TABLE_ENTRY("bextr",        X86_MN_BEXTR,       PF_VEX,         X86_ISET_BMI1,  OP(Gy, Ey, By)),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID()

TABLE_END(0F_38)

TABLE_BEGIN(0F_38_F3)

    /* modr/m byte reg/nnn field */

    /* 00h */
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY("blsr",         X86_MN_BLSR,        PF_OVEX,        X86_ISET_BMI1,  OP(By, Ey)),
    TABLE_ENTRY("blsmsk",       X86_MN_BLSMSK,      PF_OVEX,        X86_ISET_BMI1,  OP(By, Ey)),
    TABLE_ENTRY("blsi",         X86_MN_BLSI,        PF_OVEX,        X86_ISET_BMI1,  OP(By, Ey)),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID()

TABLE_END(0F_38_F3)

TABLE_BEGIN(0F_38_F6)

    /* operand size: 0 = 16-bits, 1 = 32-bits, 2 = 64-bits */
    TABLE_ENTRY("wrssd",            X86_MN_WRSSD,           PF_NONE,        X86_ISET_CET,       OP(Md, Gd)),
    TABLE_ENTRY("wrssd",            X86_MN_WRSSD,           PF_NONE,        X86_ISET_CET,       OP(Md, Gd)),
    TABLE_ENTRY("wrssq",            X86_MN_WRSSQ,           PF_NONE,        X86_ISET_CET,       OP(Mq, Gq))

TABLE_END(0F_38_F6)

TABLE_BEGIN(0F_3A)

    /* 00h */
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY("palignr",      X86_MN_PALIGNR,     PF_NONE,        X86_ISET_SSSE3, OP(Pq, Qq, Ib)),

    /* 10h */
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    /* 20h */
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    /* 30h */
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    /* 40h */
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    /* 50h */
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    /* 60h */
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    /* 70h */
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    /* 80h */
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    /* 90h */
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    /* A0h */
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    /* B0h */
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    /* C0h */
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    /* D0h */
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    /* E0h */
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    /* F0h */
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID()

TABLE_END(0F_3A)

/****************************************************************************

    Three-byte tables - 66h mandatory prefix

****************************************************************************/

TABLE_BEGIN(66_0F_38)

    /* 00h */
    TABLE_ENTRY("pshufb",           X86_MN_PSHUFB,          PF_VEX,         X86_ISET_SSSE3,     OP(Vx, Hx, Wx)),
    TABLE_ENTRY("phaddw",           X86_MN_PHADDW,          PF_VEX,         X86_ISET_SSSE3,     OP(Vx, Hx, Wx)),
    TABLE_ENTRY("phaddd",           X86_MN_PHADDD,          PF_VEX,         X86_ISET_SSSE3,     OP(Vx, Hx, Wx)),
    TABLE_ENTRY("phaddsw",          X86_MN_PHADDSW,         PF_VEX,         X86_ISET_SSSE3,     OP(Vx, Hx, Wx)),
    TABLE_ENTRY("pmaddubsw",        X86_MN_PMADDUBSW,       PF_VEX,         X86_ISET_SSSE3,     OP(Vx, Hx, Wx)),
    TABLE_ENTRY("phsubw",           X86_MN_PHSUBW,          PF_VEX,         X86_ISET_SSSE3,     OP(Vx, Hx, Wx)),
    TABLE_ENTRY("phsubd",           X86_MN_PHSUBD,          PF_VEX,         X86_ISET_SSSE3,     OP(Vx, Hx, Wx)),
    TABLE_ENTRY("phsubsw",          X86_MN_PHSUBSW,         PF_VEX,         X86_ISET_SSSE3,     OP(Vx, Hx, Wx)),
    TABLE_ENTRY("psignb",           X86_MN_PSIGNB,          PF_VEX,         X86_ISET_SSSE3,     OP(Vx, Hx, Wx)),
    TABLE_ENTRY("psignw",           X86_MN_PSIGNW,          PF_VEX,         X86_ISET_SSSE3,     OP(Vx, Hx, Wx)),
    TABLE_ENTRY("psignd",           X86_MN_PSIGND,          PF_VEX,         X86_ISET_SSSE3,     OP(Vx, Hx, Wx)),
    TABLE_ENTRY("pmulhrsw",         X86_MN_PMULHRSW,        PF_VEX,         X86_ISET_SSSE3,     OP(Vx, Hx, Wx)),
    TABLE_ENTRY("permilps",         X86_MN_PERMILPS,        PF_OVEX,        X86_ISET_AVX,       OP(Vx, Hx, Wx)),
    TABLE_ENTRY("permilpd",         X86_MN_PERMILPD,        PF_OVEX,        X86_ISET_AVX,       OP(Vx, Hx, Wx)),
    TABLE_ENTRY("testps",           X86_MN_TESTPS,          PF_OVEX,        X86_ISET_AVX,       OP(Vx, Wx)),
    TABLE_ENTRY("testpd",           X86_MN_TESTPD,          PF_OVEX,        X86_ISET_AVX,       OP(Vx, Wx)),

    /* 10h */
    TABLE_ENTRY("pblendvb",         X86_MN_PBLENDVB,        PF_NONE,        X86_ISET_SSE41,     OP(Vdq, Wdq)),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY("cvtph2ps",         X86_MN_CVTPH2PS,        PF_VEX,         X86_ISET_F16C,      OP(Vx, Wx, Ib)),
    TABLE_ENTRY("blendvps",         X86_MN_BLENDVPS,        PF_NONE,        X86_ISET_SSE41,     OP(Vdq, Wdq)),
    TABLE_ENTRY("blendvpd",         X86_MN_BLENDVPD,        PF_NONE,        X86_ISET_SSE41,     OP(Vdq, Wdq)),
    TABLE_ENTRY("permps",           X86_MN_PERMPS,          PF_VEX,         X86_ISET_AVX,       OP(Vqq, Hqq, Wqq)),
    TABLE_ENTRY("ptest",            X86_MN_PTEST,           PF_VEX,         X86_ISET_SSE41,     OP(Vx, Wx)),

    TABLE_ENTRY("broadcastss",      X86_MN_BROADCASTSS,     PF_OVEX,        X86_ISET_AVX,       OP(Vx, Wd)),
    TABLE_ENTRY("broadcastsd",      X86_MN_BROADCASTSD,     PF_OVEX,        X86_ISET_AVX,       OP(Vqq, Wq)),
    TABLE_ENTRY("broadcastf128",    X86_MN_BROADCASTF128,   PF_OVEX,        X86_ISET_AVX,       OP(Vqq, Mdq)),
    TABLE_ENTRY_INVALID(),

    TABLE_ENTRY("pabsb",            X86_MN_PABSB,           PF_VEX,         X86_ISET_SSSE3,     OP(Vx, Wx)),
    TABLE_ENTRY("pabsw",            X86_MN_PABSW,           PF_VEX,         X86_ISET_SSSE3,     OP(Vx, Wx)),
    TABLE_ENTRY("pabsd",            X86_MN_PABSD,           PF_VEX,         X86_ISET_SSSE3,     OP(Vx, Wx)),
    TABLE_ENTRY_INVALID(),

    /* 20h */
    TABLE_ENTRY_ESCAPE(66_0F_38_20),
    TABLE_ENTRY_ESCAPE(66_0F_38_21),
    TABLE_ENTRY_ESCAPE(66_0F_38_22),
    TABLE_ENTRY_ESCAPE(66_0F_38_23),
    TABLE_ENTRY_ESCAPE(66_0F_38_24),
    TABLE_ENTRY_ESCAPE(66_0F_38_25),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    TABLE_ENTRY("pmuldq",           X86_MN_PMULDQ,          PF_VEX,         X86_ISET_SSE41,     OP(Vx, Hx, Wx)),
    TABLE_ENTRY("pcmpeqq",          X86_MN_PCMPEQQ,         PF_VEX,         X86_ISET_SSE41,     OP(Vx, Hx, Wx)),
    TABLE_ENTRY("movntdqa",         X86_MN_MOVNTDQA,        PF_VEX,         X86_ISET_SSE41,     OP(Vx, Mx)),
    TABLE_ENTRY("packusdw",         X86_MN_PACKUSDW,        PF_VEX,         X86_ISET_SSE41,     OP(Vx, Hx, Wx)),
    TABLE_ENTRY("maskmovps",        X86_MN_MASKMOVPS,       PF_OVEX,        X86_ISET_AVX,       OP(Vx, Hx, Mx)),
    TABLE_ENTRY("maskmovpd",        X86_MN_MASKMOVPD,       PF_OVEX,        X86_ISET_AVX,       OP(Vx, Hx, Mx)),
    TABLE_ENTRY("maskmovps",        X86_MN_MASKMOVPS,       PF_OVEX,        X86_ISET_AVX,       OP(Mx, Hx, Vx)),
    TABLE_ENTRY("maskmovpd",        X86_MN_MASKMOVPD,       PF_OVEX,        X86_ISET_AVX,       OP(Mx, Hx, Vx)),

    /* 30h */
    TABLE_ENTRY_ESCAPE(66_0F_38_30),
    TABLE_ENTRY_ESCAPE(66_0F_38_31),
    TABLE_ENTRY_ESCAPE(66_0F_38_32),
    TABLE_ENTRY_ESCAPE(66_0F_38_33),
    TABLE_ENTRY_ESCAPE(66_0F_38_34),
    TABLE_ENTRY_ESCAPE(66_0F_38_35),

    /* 36h */
    TABLE_ENTRY("permd",            X86_MN_PERMD,           PF_OVEX,        X86_ISET_AVX,       OP(Vqq, Hqq, Wqq)),
    TABLE_ENTRY("pcmpgtq",          X86_MN_PCMPGTQ,         PF_VEX,         X86_ISET_SSE42,     OP(Vx, Hx, Wx)),

    TABLE_ENTRY("pminsb",           X86_MN_PMINSB,          PF_VEX,         X86_ISET_SSE41,     OP(Vx, Hx, Wx)),
    TABLE_ENTRY("pminsd",           X86_MN_PMINSD,          PF_VEX,         X86_ISET_SSE41,     OP(Vx, Hx, Wx)),
    TABLE_ENTRY("pminuw",           X86_MN_PMINUW,          PF_VEX,         X86_ISET_SSE41,     OP(Vx, Hx, Wx)),
    TABLE_ENTRY("pminud",           X86_MN_PMINUD,          PF_VEX,         X86_ISET_SSE41,     OP(Vx, Hx, Wx)),
    TABLE_ENTRY("pmaxsb",           X86_MN_PMAXSB,          PF_VEX,         X86_ISET_SSE41,     OP(Vx, Hx, Wx)),
    TABLE_ENTRY("pmaxsd",           X86_MN_PMAXSD,          PF_VEX,         X86_ISET_SSE41,     OP(Vx, Hx, Wx)),
    TABLE_ENTRY("pmaxuw",           X86_MN_PMAXUW,          PF_VEX,         X86_ISET_SSE41,     OP(Vx, Hx, Wx)),
    TABLE_ENTRY("pmaxud",           X86_MN_PMAXUD,          PF_VEX,         X86_ISET_SSE41,     OP(Vx, Hx, Wx)),

    /* 40h */
    TABLE_ENTRY("pmulld",           X86_MN_PMULLD,          PF_VEX,         X86_ISET_SSE41,     OP(Vx, Hx, Wx)),
    TABLE_ENTRY("phminposuw",       X86_MN_PHMINPOSUW,      PF_VEX,         X86_ISET_SSE41,     OP(Vdq, Wdq)),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    /* 45h: operand size */
    TABLE_ENTRY_ESCAPE(66_0F_38_45),
    
    TABLE_ENTRY("psravd",           X86_MN_PSRAVD,          PF_OVEX,        X86_ISET_AVX,       OP(Vx, Hx, Wx)),
    
    /* 47h: operand size */
    TABLE_ENTRY_ESCAPE(66_0F_38_47),

    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    /* 50h */
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    TABLE_ENTRY("pbroadcastd",      X86_MN_PBROADCASTD,     PF_OVEX,        X86_ISET_AVX,       OP(Vx, Wx)),
    TABLE_ENTRY("pbroadcastq",      X86_MN_PBROADCASTQ,     PF_OVEX,        X86_ISET_AVX,       OP(Vx, Wx)),
    TABLE_ENTRY("broadcasti128",    X86_MN_BROADCASTI128,   PF_OVEX,        X86_ISET_AVX,       OP(Vqq, Mdq)),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    /* 60h */
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    /* 70h */
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    TABLE_ENTRY("pbroadcastb",      X86_MN_PBROADCASTB,     PF_OVEX,        X86_ISET_AVX,       OP(Vx, Wx)),
    TABLE_ENTRY("pbroadcastw",      X86_MN_PBROADCASTW,     PF_OVEX,        X86_ISET_AVX,       OP(Vx, Wx)),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    /* 80h */
    TABLE_ENTRY("invept",           X86_MN_INVEPT,          PF_NONE,        X86_ISET_VMX,       OP(Gy, Mdq)),
    TABLE_ENTRY("invvpid",          X86_MN_INVVPID,         PF_NONE,        X86_ISET_VMX,       OP(Gy, Mdq)),
    TABLE_ENTRY("invpcid",          X86_MN_INVPCID,         PF_NONE,        X86_ISET_VMX,       OP(Gy, Mdq)),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_ESCAPE(66_0F_38_8C),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_ESCAPE(66_0F_38_8E),
    TABLE_ENTRY_INVALID(),

    /* 90h */
    TABLE_ENTRY_ESCAPE(66_0F_38_90),
    TABLE_ENTRY_ESCAPE(66_0F_38_91),
    TABLE_ENTRY_ESCAPE(66_0F_38_92),
    TABLE_ENTRY_ESCAPE(66_0F_38_93),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_ESCAPE(66_0F_38_96),
    TABLE_ENTRY_ESCAPE(66_0F_38_97),

    TABLE_ENTRY_ESCAPE(66_0F_38_98),
    TABLE_ENTRY_ESCAPE(66_0F_38_99),
    TABLE_ENTRY_ESCAPE(66_0F_38_9A),
    TABLE_ENTRY_ESCAPE(66_0F_38_9B),
    TABLE_ENTRY_ESCAPE(66_0F_38_9C),
    TABLE_ENTRY_ESCAPE(66_0F_38_9D),
    TABLE_ENTRY_ESCAPE(66_0F_38_9E),
    TABLE_ENTRY_ESCAPE(66_0F_38_9F),

    /* A0h */
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_ESCAPE(66_0F_38_A6),
    TABLE_ENTRY_ESCAPE(66_0F_38_A7),

    TABLE_ENTRY_ESCAPE(66_0F_38_A8),
    TABLE_ENTRY_ESCAPE(66_0F_38_A9),
    TABLE_ENTRY_ESCAPE(66_0F_38_AA),
    TABLE_ENTRY_ESCAPE(66_0F_38_AB),
    TABLE_ENTRY_ESCAPE(66_0F_38_AC),
    TABLE_ENTRY_ESCAPE(66_0F_38_AD),
    TABLE_ENTRY_ESCAPE(66_0F_38_AE),
    TABLE_ENTRY_ESCAPE(66_0F_38_AF),

    /* B0h */
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_ESCAPE(66_0F_38_B6),
    TABLE_ENTRY_ESCAPE(66_0F_38_B7),

    TABLE_ENTRY_ESCAPE(66_0F_38_B8),
    TABLE_ENTRY_ESCAPE(66_0F_38_B9),
    TABLE_ENTRY_ESCAPE(66_0F_38_BA),
    TABLE_ENTRY_ESCAPE(66_0F_38_BB),
    TABLE_ENTRY_ESCAPE(66_0F_38_BC),
    TABLE_ENTRY_ESCAPE(66_0F_38_BD),
    TABLE_ENTRY_ESCAPE(66_0F_38_BE),
    TABLE_ENTRY_ESCAPE(66_0F_38_BF),

    /* C0h */
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    /* D0h */
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY("aesimc",           X86_MN_AESIMC,          PF_VEX128,      X86_ISET_AESNI,     OP(Vdq, Wdq)),
    TABLE_ENTRY("aesenc",           X86_MN_AESENC,          PF_VEX128,      X86_ISET_AESNI,     OP(Vdq, Hdq, Wdq)),
    TABLE_ENTRY("aesenclast",       X86_MN_AESENCLAST,      PF_VEX128,      X86_ISET_AESNI,     OP(Vdq, Hdq, Wdq)),
    TABLE_ENTRY("aesdec",           X86_MN_AESDEC,          PF_VEX128,      X86_ISET_AESNI,     OP(Vdq, Hdq, Wdq)),
    TABLE_ENTRY("aesdeclast",       X86_MN_AESDECLAST,      PF_VEX128,      X86_ISET_AESNI,     OP(Vdq, Hdq, Wdq)),

    /* E0h */
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    /* F0h */
    TABLE_ENTRY("movbe",            X86_MN_MOVBE,           PF_NONE,        X86_ISET_GP,        OP(Gw, Mw)),
    TABLE_ENTRY("movbe",            X86_MN_MOVBE,           PF_NONE,        X86_ISET_GP,        OP(Mw, Gw)),
    TABLE_ENTRY_INVALID(),

    /* F3h: group 17; vex prefix is required */
    TABLE_ENTRY_ESCAPE(0F_38_F3),
    TABLE_ENTRY_INVALID(),
    
    /* F5h: operand size */
    TABLE_ENTRY_ESCAPE(66_0F_38_F5),

    TABLE_ENTRY("adcx",             X86_MN_ADCX,            PF_NONE,        X86_ISET_ADX,       OP(Gy, Ey)),
    TABLE_ENTRY("shlx",             X86_MN_SHLX,            PF_OVEX,        X86_ISET_BMI2,      OP(Gy, Ey, By)),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID()

TABLE_END(66_0F_38)

TABLE_BEGIN(66_0F_38_20)

    /* modr/m byte mod field:  0 = !11b, 1 = 11b */
    TABLE_ENTRY("pmovsxbw",         X86_MN_PMOVSXBW,        PF_VEX,     X86_ISET_SSE41,     OP(Vx, Moq)),
    TABLE_ENTRY("pmovsxbw",         X86_MN_PMOVSXBW,        PF_VEX,     X86_ISET_SSE41,     OP(Vx, Uss))

TABLE_END(66_0F_38_20)

TABLE_BEGIN(66_0F_38_21)

    /* modr/m byte mod field:  0 = !11b, 1 = 11b */
    TABLE_ENTRY("pmovsxbd",         X86_MN_PMOVSXBD,        PF_VEX,     X86_ISET_SSE41,     OP(Vx, Moq)),
    TABLE_ENTRY("pmovsxbd",         X86_MN_PMOVSXBD,        PF_VEX,     X86_ISET_SSE41,     OP(Vx, Uss))

TABLE_END(66_0F_38_21)

TABLE_BEGIN(66_0F_38_22)

    /* modr/m byte mod field:  0 = !11b, 1 = 11b */
    TABLE_ENTRY("pmovsxbq",         X86_MN_PMOVSXBQ,        PF_VEX,     X86_ISET_SSE41,     OP(Vx, Moq)),
    TABLE_ENTRY("pmovsxbq",         X86_MN_PMOVSXBQ,        PF_VEX,     X86_ISET_SSE41,     OP(Vx, Uss))

TABLE_END(66_0F_38_22)

TABLE_BEGIN(66_0F_38_23)

    /* modr/m byte mod field:  0 = !11b, 1 = 11b */
    TABLE_ENTRY("pmovsxwd",         X86_MN_PMOVSXWD,        PF_VEX,     X86_ISET_SSE41,     OP(Vx, Moq)),
    TABLE_ENTRY("pmovsxwd",         X86_MN_PMOVSXWD,        PF_VEX,     X86_ISET_SSE41,     OP(Vx, Uss)),

TABLE_END(66_0F_38_23)

TABLE_BEGIN(66_0F_38_24)

    /* modr/m byte mod field:  0 = !11b, 1 = 11b */
    TABLE_ENTRY("pmovsxwq",         X86_MN_PMOVSXWQ,        PF_VEX,     X86_ISET_SSE41,     OP(Vx, Moq)),
    TABLE_ENTRY("pmovsxwq",         X86_MN_PMOVSXWQ,        PF_VEX,     X86_ISET_SSE41,     OP(Vx, Uss))

TABLE_END(66_0F_38_24)

TABLE_BEGIN(66_0F_38_25)

    /* modr/m byte mod field:  0 = !11b, 1 = 11b */
    TABLE_ENTRY("pmovsxdq",         X86_MN_PMOVSXDQ,        PF_VEX,     X86_ISET_SSE41,     OP(Vx, Moq)),
    TABLE_ENTRY("pmovsxdq",         X86_MN_PMOVSXDQ,        PF_VEX,     X86_ISET_SSE41,     OP(Vx, Uss))

TABLE_END(66_0F_38_25)

TABLE_BEGIN(66_0F_38_30)

    /* modr/m byte mod field:  0 = !11b, 1 = 11b */
    TABLE_ENTRY("pmovzxbw",         X86_MN_PMOVZXBW,        PF_VEX,     X86_ISET_SSE41,     OP(Vx, Moq)),
    TABLE_ENTRY("pmovzxbw",         X86_MN_PMOVZXBW,        PF_VEX,     X86_ISET_SSE41,     OP(Vx, Uss))

TABLE_END(66_0F_38_30)

TABLE_BEGIN(66_0F_38_31)

    /* modr/m byte mod field:  0 = !11b, 1 = 11b */
    TABLE_ENTRY("pmovzxbd",         X86_MN_PMOVZXBD,        PF_VEX,     X86_ISET_SSE41,     OP(Vx, Moq)),
    TABLE_ENTRY("pmovzxbd",         X86_MN_PMOVZXBD,        PF_VEX,     X86_ISET_SSE41,     OP(Vx, Uss))

TABLE_END(66_0F_38_31)

TABLE_BEGIN(66_0F_38_32)

    /* modr/m byte mod field:  0 = !11b, 1 = 11b */
    TABLE_ENTRY("pmovzxbq",         X86_MN_PMOVZXBQ,        PF_VEX,     X86_ISET_SSE41,     OP(Vx, Moq)),
    TABLE_ENTRY("pmovzxbq",         X86_MN_PMOVZXBQ,        PF_VEX,     X86_ISET_SSE41,     OP(Vx, Uss))

TABLE_END(66_0F_38_32)

TABLE_BEGIN(66_0F_38_33)

    /* modr/m byte mod field:  0 = !11b, 1 = 11b */
    TABLE_ENTRY("pmovzxwd",         X86_MN_PMOVZXWD,        PF_VEX,     X86_ISET_SSE41,     OP(Vx, Moq)),
    TABLE_ENTRY("pmovzxwd",         X86_MN_PMOVZXWD,        PF_VEX,     X86_ISET_SSE41,     OP(Vx, Uss)),

TABLE_END(66_0F_38_33)

TABLE_BEGIN(66_0F_38_34)

    /* modr/m byte mod field:  0 = !11b, 1 = 11b */
    TABLE_ENTRY("pmovzxwq",         X86_MN_PMOVZXWQ,        PF_VEX,     X86_ISET_SSE41,     OP(Vx, Moq)),
    TABLE_ENTRY("pmovzxwq",         X86_MN_PMOVZXWQ,        PF_VEX,     X86_ISET_SSE41,     OP(Vx, Uss))

TABLE_END(66_0F_38_34)

TABLE_BEGIN(66_0F_38_35)

    /* modr/m byte mod field:  0 = !11b, 1 = 11b */
    TABLE_ENTRY("pmovzxdq",         X86_MN_PMOVZXDQ,        PF_VEX,     X86_ISET_SSE41,     OP(Vx, Moq)),
    TABLE_ENTRY("pmovzxdq",         X86_MN_PMOVZXDQ,        PF_VEX,     X86_ISET_SSE41,     OP(Vx, Uss))

TABLE_END(66_0F_38_35)

TABLE_BEGIN(66_0F_38_45)

    /* operand size: 0 = vex.w0, 1 = vex.w1 */
    TABLE_ENTRY("psrlvd",           X86_MN_PSRLVD,          PF_OVEX,        X86_ISET_AVX,       OP(Vx, Hx, Wx)),
    TABLE_ENTRY("psrlvq",           X86_MN_PSRLVQ,          PF_OVEX,        X86_ISET_AVX,       OP(Vx, Hx, Wx))

TABLE_END(66_0F_38_45)

TABLE_BEGIN(66_0F_38_47)

    /* operand size: 0 = vex.w0, 1 = vex.w1 */
    TABLE_ENTRY("psllvd",           X86_MN_PSLLVD,          PF_OVEX,        X86_ISET_AVX,       OP(Vx, Hx, Wx)),
    TABLE_ENTRY("psllvq",           X86_MN_PSLLVQ,          PF_OVEX,        X86_ISET_AVX,       OP(Vx, Hx, Wx))

TABLE_END(66_0F_38_47)

TABLE_BEGIN(66_0F_38_8C)

    /* operand size: 0 = vex.w0, 1 = vex.w1 */
    TABLE_ENTRY("pmaskmovd",        X86_MN_PMASKMOVD,       PF_OVEX,        X86_ISET_AVX,       OP(Vx, Hx, Mx)),
    TABLE_ENTRY("pmaskmovq",        X86_MN_PMASKMOVQ,       PF_OVEX,        X86_ISET_AVX,       OP(Vx, Hx, Mx))

TABLE_END(66_0F_38_8C)

TABLE_BEGIN(66_0F_38_8E)

    /* operand size: 0 = vex.w0, 1 = vex.w1 */
    TABLE_ENTRY("pmaskmovd",        X86_MN_PMASKMOVD,       PF_OVEX,        X86_ISET_AVX,       OP(Mx, Hx, Vx)),
    TABLE_ENTRY("pmaskmovq",        X86_MN_PMASKMOVQ,       PF_OVEX,        X86_ISET_AVX,       OP(Mx, Hx, Vx))

TABLE_END(66_0F_38_8E)

TABLE_BEGIN(66_0F_38_90)

    /* operand size: 0 = vex.w0, 1 = vex.w1 */
    TABLE_ENTRY("gatherdd",         X86_MN_GATHERDD,        PF_OVEX | PF_VSIB_L,        X86_ISET_AVX,       OP(Vx, Wd, Hx)),
    TABLE_ENTRY("gatherdq",         X86_MN_GATHERDQ,        PF_OVEX | PF_VSIB_X,        X86_ISET_AVX,       OP(Vx, Wd, Hx))

TABLE_END(66_0F_38_90)

TABLE_BEGIN(66_0F_38_91)

    /* operand size: 0 = vex.w0, 1 = vex.w1 */
    TABLE_ENTRY("gatherqd",         X86_MN_GATHERQD,        PF_OVEX | PF_VSIB_L,        X86_ISET_AVX,       OP(Vx, Wq, Hx)),
    TABLE_ENTRY("gatherqq",         X86_MN_GATHERQQ,        PF_OVEX | PF_VSIB_L,        X86_ISET_AVX,       OP(Vx, Wq, Hx))

TABLE_END(66_0F_38_91)

TABLE_BEGIN(66_0F_38_92)

    /* operand size: 0 = vex.w0, 1 = vex.w1 */
    TABLE_ENTRY("gatherdps",        X86_MN_GATHERDPS,       PF_OVEX | PF_VSIB_L,        X86_ISET_AVX,       OP(Vx, Wd, Hx)),
    TABLE_ENTRY("gatherdpd",        X86_MN_GATHERDPD,       PF_OVEX | PF_VSIB_X,        X86_ISET_AVX,       OP(Vx, Wd, Hx))

TABLE_END(66_0F_38_92)

TABLE_BEGIN(66_0F_38_93)

    /* operand size: 0 = vex.w0, 1 = vex.w1 */
    TABLE_ENTRY("gatherqps",        X86_MN_GATHERQPS,       PF_OVEX | PF_VSIB_L,        X86_ISET_AVX,       OP(Vx, Wq, Hx)),
    TABLE_ENTRY("gatherqpd",        X86_MN_GATHERQPD,       PF_OVEX | PF_VSIB_L,        X86_ISET_AVX,       OP(Vx, Wq, Hx))

TABLE_END(66_0F_38_93)

TABLE_BEGIN(66_0F_38_96)

    /* operand size: 0 = vex.w0, 1 = vex.w1 */
    TABLE_ENTRY("fmaddsub132ps",    X86_MN_FMADDSUB132PS,   PF_OVEX,        X86_ISET_FMA,       OP(Vx, Hx, Wx)),
    TABLE_ENTRY("fmaddsub132pd",    X86_MN_FMADDSUB132PD,   PF_OVEX,        X86_ISET_FMA,       OP(Vx, Hx, Wx))

TABLE_END(66_0F_38_96)

TABLE_BEGIN(66_0F_38_97)

    /* operand size: 0 = vex.w0, 1 = vex.w1 */
    TABLE_ENTRY("fmsubadd132ps",    X86_MN_FMSUBADD132PS,   PF_OVEX,        X86_ISET_FMA,       OP(Vx, Hx, Wx)),
    TABLE_ENTRY("fmsubadd132pd",    X86_MN_FMSUBADD132PD,   PF_OVEX,        X86_ISET_FMA,       OP(Vx, Hx, Wx))

TABLE_END(66_0F_38_97)

TABLE_BEGIN(66_0F_38_98)

    /* operand size: 0 = vex.w0, 1 = vex.w1 */
    TABLE_ENTRY("fmadd132ps",       X86_MN_FMADD132PS,      PF_OVEX,        X86_ISET_FMA,       OP(Vx, Hx, Wx)),
    TABLE_ENTRY("fmadd132pd",       X86_MN_FMADD132PD,      PF_OVEX,        X86_ISET_FMA,       OP(Vx, Hx, Wx))

TABLE_END(66_0F_38_98)

TABLE_BEGIN(66_0F_38_99)

    /* operand size: 0 = vex.w0, 1 = vex.w1 */
    TABLE_ENTRY("fmadd132ss",       X86_MN_FMADD132SS,      PF_OVEX,        X86_ISET_FMA,       OP(Vx, Hx, Wx)),
    TABLE_ENTRY("fmadd132sd",       X86_MN_FMADD132SD,      PF_OVEX,        X86_ISET_FMA,       OP(Vx, Hx, Wx))

TABLE_END(66_0F_38_99)

TABLE_BEGIN(66_0F_38_9A)

    /* operand size: 0 = vex.w0, 1 = vex.w1 */
    TABLE_ENTRY("fmsub132ps",       X86_MN_FMSUB132PS,      PF_OVEX,        X86_ISET_FMA,       OP(Vx, Hx, Wx)),
    TABLE_ENTRY("fmsub132pd",       X86_MN_FMSUB132PD,      PF_OVEX,        X86_ISET_FMA,       OP(Vx, Hx, Wx))

TABLE_END(66_0F_38_9A)

TABLE_BEGIN(66_0F_38_9B)

    /* operand size: 0 = vex.w0, 1 = vex.w1 */
    TABLE_ENTRY("fmsub132ss",       X86_MN_FMSUB132SS,      PF_OVEX,        X86_ISET_FMA,       OP(Vx, Hx, Wx)),
    TABLE_ENTRY("fmsub132sd",       X86_MN_FMSUB132SD,      PF_OVEX,        X86_ISET_FMA,       OP(Vx, Hx, Wx))

TABLE_END(66_0F_38_9B)

TABLE_BEGIN(66_0F_38_9C)

    /* operand size: 0 = vex.w0, 1 = vex.w1 */
    TABLE_ENTRY("fnmadd132ps",      X86_MN_FNMADD132PS,     PF_OVEX,        X86_ISET_FMA,       OP(Vx, Hx, Wx)),
    TABLE_ENTRY("fnmadd132pd",      X86_MN_FNMADD132PD,     PF_OVEX,        X86_ISET_FMA,       OP(Vx, Hx, Wx))

TABLE_END(66_0F_38_9C)

TABLE_BEGIN(66_0F_38_9D)

    /* operand size: 0 = vex.w0, 1 = vex.w1 */
    TABLE_ENTRY("fnmadd132ss",      X86_MN_FNMADD132SS,     PF_OVEX,        X86_ISET_FMA,       OP(Vx, Hx, Wx)),
    TABLE_ENTRY("fnmadd132sd",      X86_MN_FNMADD132SD,     PF_OVEX,        X86_ISET_FMA,       OP(Vx, Hx, Wx))

TABLE_END(66_0F_38_9D)

TABLE_BEGIN(66_0F_38_9E)

    /* operand size: 0 = vex.w0, 1 = vex.w1 */
    TABLE_ENTRY("fnmsub132ps",      X86_MN_FNMSUB132PS,     PF_OVEX,        X86_ISET_FMA,       OP(Vx, Hx, Wx)),
    TABLE_ENTRY("fnmsub132pd",      X86_MN_FNMSUB132PD,     PF_OVEX,        X86_ISET_FMA,       OP(Vx, Hx, Wx))

TABLE_END(66_0F_38_9E)

TABLE_BEGIN(66_0F_38_9F)

    /* operand size: 0 = vex.w0, 1 = vex.w1 */
    TABLE_ENTRY("fnmsub132ss",      X86_MN_FNMSUB132SS,     PF_OVEX,        X86_ISET_FMA,       OP(Vx, Hx, Wx)),
    TABLE_ENTRY("fnmsub132sd",      X86_MN_FNMSUB132SD,     PF_OVEX,        X86_ISET_FMA,       OP(Vx, Hx, Wx))

TABLE_END(66_0F_38_9F)

TABLE_BEGIN(66_0F_38_A6)

    /* operand size: 0 = vex.w0, 1 = vex.w1 */
    TABLE_ENTRY("fmaddsub213ps",    X86_MN_FMADDSUB213PS,   PF_OVEX,        X86_ISET_FMA,       OP(Vx, Hx, Wx)),
    TABLE_ENTRY("fmaddsub213pd",    X86_MN_FMADDSUB213PD,   PF_OVEX,        X86_ISET_FMA,       OP(Vx, Hx, Wx))

TABLE_END(66_0F_38_A6)

TABLE_BEGIN(66_0F_38_A7)

    /* operand size: 0 = vex.w0, 1 = vex.w1 */
    TABLE_ENTRY("fmsubadd213ps",    X86_MN_FMSUBADD213PS,   PF_OVEX,        X86_ISET_FMA,       OP(Vx, Hx, Wx)),
    TABLE_ENTRY("fmsubadd213pd",    X86_MN_FMSUBADD213PD,   PF_OVEX,        X86_ISET_FMA,       OP(Vx, Hx, Wx))

TABLE_END(66_0F_38_A7)

TABLE_BEGIN(66_0F_38_A8)

    /* operand size: 0 = vex.w0, 1 = vex.w1 */
    TABLE_ENTRY("fmadd213ps",       X86_MN_FMADD213PS,      PF_OVEX,        X86_ISET_FMA,       OP(Vx, Hx, Wx)),
    TABLE_ENTRY("fmadd213pd",       X86_MN_FMADD213PD,      PF_OVEX,        X86_ISET_FMA,       OP(Vx, Hx, Wx))

TABLE_END(66_0F_38_A8)

TABLE_BEGIN(66_0F_38_A9)

    /* operand size: 0 = vex.w0, 1 = vex.w1 */
    TABLE_ENTRY("fmadd213ss",       X86_MN_FMADD213SS,      PF_OVEX,        X86_ISET_FMA,       OP(Vx, Hx, Wx)),
    TABLE_ENTRY("fmadd213sd",       X86_MN_FMADD213SD,      PF_OVEX,        X86_ISET_FMA,       OP(Vx, Hx, Wx))

TABLE_END(66_0F_38_A9)

TABLE_BEGIN(66_0F_38_AA)

    /* operand size: 0 = vex.w0, 1 = vex.w1 */
    TABLE_ENTRY("fmsub213ps",       X86_MN_FMSUB213PS,      PF_OVEX,        X86_ISET_FMA,       OP(Vx, Hx, Wx)),
    TABLE_ENTRY("fmsub213pd",       X86_MN_FMSUB213PD,      PF_OVEX,        X86_ISET_FMA,       OP(Vx, Hx, Wx))

TABLE_END(66_0F_38_AA)

TABLE_BEGIN(66_0F_38_AB)

    /* operand size: 0 = vex.w0, 1 = vex.w1 */
    TABLE_ENTRY("fmsub213ss",       X86_MN_FMSUB213SS,      PF_OVEX,        X86_ISET_FMA,       OP(Vx, Hx, Wx)),
    TABLE_ENTRY("fmsub213sd",       X86_MN_FMSUB213SD,      PF_OVEX,        X86_ISET_FMA,       OP(Vx, Hx, Wx))

TABLE_END(66_0F_38_AB)

TABLE_BEGIN(66_0F_38_AC)

    /* operand size: 0 = vex.w0, 1 = vex.w1 */
    TABLE_ENTRY("fnmadd213ps",      X86_MN_FNMADD213PS,     PF_OVEX,        X86_ISET_FMA,       OP(Vx, Hx, Wx)),
    TABLE_ENTRY("fnmadd213pd",      X86_MN_FNMADD213PD,     PF_OVEX,        X86_ISET_FMA,       OP(Vx, Hx, Wx))

TABLE_END(66_0F_38_AC)

TABLE_BEGIN(66_0F_38_AD)

    /* operand size: 0 = vex.w0, 1 = vex.w1 */
    TABLE_ENTRY("fnmadd213ss",      X86_MN_FNMADD213SS,     PF_OVEX,        X86_ISET_FMA,       OP(Vx, Hx, Wx)),
    TABLE_ENTRY("fnmadd213sd",      X86_MN_FNMADD213SD,     PF_OVEX,        X86_ISET_FMA,       OP(Vx, Hx, Wx))

TABLE_END(66_0F_38_AD)

TABLE_BEGIN(66_0F_38_AE)

    /* operand size: 0 = vex.w0, 1 = vex.w1 */
    TABLE_ENTRY("fnmsub213ps",      X86_MN_FNMSUB213PS,     PF_OVEX,        X86_ISET_FMA,       OP(Vx, Hx, Wx)),
    TABLE_ENTRY("fnmsub213pd",      X86_MN_FNMSUB213PD,     PF_OVEX,        X86_ISET_FMA,       OP(Vx, Hx, Wx))

TABLE_END(66_0F_38_AE)

TABLE_BEGIN(66_0F_38_AF)

    /* operand size: 0 = vex.w0, 1 = vex.w1 */
    TABLE_ENTRY("fnmsub213ss",      X86_MN_FNMSUB213SS,     PF_OVEX,        X86_ISET_FMA,       OP(Vx, Hx, Wx)),
    TABLE_ENTRY("fnmsub213sd",      X86_MN_FNMSUB213SD,     PF_OVEX,        X86_ISET_FMA,       OP(Vx, Hx, Wx))

TABLE_END(66_0F_38_AF)

TABLE_BEGIN(66_0F_38_B6)

    /* operand size: 0 = vex.w0, 1 = vex.w1 */
    TABLE_ENTRY("fmaddsub231ps",    X86_MN_FMADDSUB231PS,   PF_OVEX,        X86_ISET_FMA,       OP(Vx, Hx, Wx)),
    TABLE_ENTRY("fmaddsub231pd",    X86_MN_FMADDSUB231PD,   PF_OVEX,        X86_ISET_FMA,       OP(Vx, Hx, Wx))

TABLE_END(66_0F_38_B6)

TABLE_BEGIN(66_0F_38_B7)

    /* operand size: 0 = vex.w0, 1 = vex.w1 */
    TABLE_ENTRY("fmsubadd231ps",    X86_MN_FMSUBADD231PS,   PF_OVEX,        X86_ISET_FMA,       OP(Vx, Hx, Wx)),
    TABLE_ENTRY("fmsubadd231pd",    X86_MN_FMSUBADD231PD,   PF_OVEX,        X86_ISET_FMA,       OP(Vx, Hx, Wx))

TABLE_END(66_0F_38_B7)

TABLE_BEGIN(66_0F_38_B8)

    /* operand size: 0 = vex.w0, 1 = vex.w1 */
    TABLE_ENTRY("fmadd231ps",       X86_MN_FMADD231PS,      PF_OVEX,        X86_ISET_FMA,       OP(Vx, Hx, Wx)),
    TABLE_ENTRY("fmadd231pd",       X86_MN_FMADD231PD,      PF_OVEX,        X86_ISET_FMA,       OP(Vx, Hx, Wx))

TABLE_END(66_0F_38_B8)

TABLE_BEGIN(66_0F_38_B9)

    /* operand size: 0 = vex.w0, 1 = vex.w1 */
    TABLE_ENTRY("fmadd231ss",       X86_MN_FMADD231SS,      PF_OVEX,        X86_ISET_FMA,       OP(Vx, Hx, Wx)),
    TABLE_ENTRY("fmadd231sd",       X86_MN_FMADD231SD,      PF_OVEX,        X86_ISET_FMA,       OP(Vx, Hx, Wx))

TABLE_END(66_0F_38_B9)

TABLE_BEGIN(66_0F_38_BA)

    /* operand size: 0 = vex.w0, 1 = vex.w1 */
    TABLE_ENTRY("fmsub231ps",       X86_MN_FMSUB231PS,      PF_OVEX,        X86_ISET_FMA,       OP(Vx, Hx, Wx)),
    TABLE_ENTRY("fmsub231pd",       X86_MN_FMSUB231PD,      PF_OVEX,        X86_ISET_FMA,       OP(Vx, Hx, Wx))

TABLE_END(66_0F_38_BA)

TABLE_BEGIN(66_0F_38_BB)

    /* operand size: 0 = vex.w0, 1 = vex.w1 */
    TABLE_ENTRY("fmsub231ss",       X86_MN_FMSUB231SS,      PF_OVEX,        X86_ISET_FMA,       OP(Vx, Hx, Wx)),
    TABLE_ENTRY("fmsub231sd",       X86_MN_FMSUB231SD,      PF_OVEX,        X86_ISET_FMA,       OP(Vx, Hx, Wx))

TABLE_END(66_0F_38_BB)

TABLE_BEGIN(66_0F_38_BC)

    /* operand size: 0 = vex.w0, 1 = vex.w1 */
    TABLE_ENTRY("fnmadd231ps",      X86_MN_FNMADD231PS,     PF_OVEX,        X86_ISET_FMA,       OP(Vx, Hx, Wx)),
    TABLE_ENTRY("fnmadd231pd",      X86_MN_FNMADD231PD,     PF_OVEX,        X86_ISET_FMA,       OP(Vx, Hx, Wx))

TABLE_END(66_0F_38_BC)

TABLE_BEGIN(66_0F_38_BD)

    /* operand size: 0 = vex.w0, 1 = vex.w1 */
    TABLE_ENTRY("fnmadd231ss",      X86_MN_FNMADD231SS,     PF_OVEX,        X86_ISET_FMA,       OP(Vx, Hx, Wx)),
    TABLE_ENTRY("fnmadd231sd",      X86_MN_FNMADD231SD,     PF_OVEX,        X86_ISET_FMA,       OP(Vx, Hx, Wx))

TABLE_END(66_0F_38_BD)

TABLE_BEGIN(66_0F_38_BE)

    /* operand size: 0 = vex.w0, 1 = vex.w1 */
    TABLE_ENTRY("fnmsub231ps",      X86_MN_FNMSUB231PS,     PF_OVEX,        X86_ISET_FMA,       OP(Vx, Hx, Wx)),
    TABLE_ENTRY("fnmsub231pd",      X86_MN_FNMSUB231PD,     PF_OVEX,        X86_ISET_FMA,       OP(Vx, Hx, Wx))

TABLE_END(66_0F_38_BE)

TABLE_BEGIN(66_0F_38_BF)

    /* operand size: 0 = vex.w0, 1 = vex.w1 */
    TABLE_ENTRY("fnmsub231ss",      X86_MN_FNMSUB231SS,     PF_OVEX,        X86_ISET_FMA,       OP(Vx, Hx, Wx)),
    TABLE_ENTRY("fnmsub231sd",      X86_MN_FNMSUB231SD,     PF_OVEX,        X86_ISET_FMA,       OP(Vx, Hx, Wx))

TABLE_END(66_0F_38_BF)

TABLE_BEGIN(66_0F_38_F5)

    /* operand size: 0 = 16-bits, 1 = 32-bits, 2 = 64-bits */
    TABLE_ENTRY("wrussd",           X86_MN_WRUSSD,          PF_NONE,        X86_ISET_CET,       OP(Md, Gd)),
    TABLE_ENTRY("wrussd",           X86_MN_WRUSSD,          PF_NONE,        X86_ISET_CET,       OP(Md, Gd)),
    TABLE_ENTRY("wrussq",           X86_MN_WRUSSQ,          PF_NONE,        X86_ISET_CET,       OP(Mq, Gq))

TABLE_END(66_0F_38_F5)

TABLE_BEGIN(66_0F_3A)

    /* 00h */
    TABLE_ENTRY("permq",            X86_MN_PERMQ,           PF_OVEX,        X86_ISET_AVX2,      OP(Vqq, Wqq, Ib)),
    TABLE_ENTRY("permpd",           X86_MN_PERMPD,          PF_OVEX,        X86_ISET_AVX2,      OP(Vqq, Wqq, Ib)),
    TABLE_ENTRY("pblendd",          X86_MN_PBLENDD,         PF_OVEX,        X86_ISET_AVX2,      OP(Vx, Hx, Wx, Ib)),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY("permilps",         X86_MN_PERMILPS,        PF_OVEX,        X86_ISET_AVX2,      OP(Vx, Wx, Ib)),
    TABLE_ENTRY("permilpd",         X86_MN_PERMILPD,        PF_OVEX,        X86_ISET_AVX2,      OP(Vx, Wx, Ib)),
    TABLE_ENTRY("perm2f128",        X86_MN_PERM2F128,       PF_OVEX,        X86_ISET_AVX2,      OP(Vqq, Hqq, Wqq, Ib)),
    TABLE_ENTRY_INVALID(),

    TABLE_ENTRY("roundps",          X86_MN_ROUNDPS,         PF_VEX,         X86_ISET_SSE41,     OP(Vx, Wx, Ib)),
    TABLE_ENTRY("roundpd",          X86_MN_ROUNDPD,         PF_VEX,         X86_ISET_SSE41,     OP(Vx, Wx, Ib)),
    TABLE_ENTRY("roundss",          X86_MN_ROUNDSS,         PF_VEX128,      X86_ISET_SSE41,     OP(Vss, Wss, Ib)),
    TABLE_ENTRY("roundsd",          X86_MN_ROUNDSD,         PF_VEX128,      X86_ISET_SSE41,     OP(Vsd, Wq, Ib)),
    TABLE_ENTRY("blendps",          X86_MN_BLENDPS,         PF_VEX,         X86_ISET_SSE41,     OP(Vx, Hx, Wx, Ib)),
    TABLE_ENTRY("blendpd",          X86_MN_BLENDPD,         PF_VEX,         X86_ISET_SSE41,     OP(Vx, Hx, Wx, Ib)),
    TABLE_ENTRY("pblendw",          X86_MN_PBLENDW,         PF_VEX,         X86_ISET_SSE41,     OP(Vx, Hx, Wx, Ib)),
    TABLE_ENTRY("palignr",          X86_MN_PALIGNR,         PF_VEX,         X86_ISET_SSSE3,     OP(Vx, Hx, Wx, Ib)),

    /* 10h */
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_ESCAPE(66_0F_3A_14),
    TABLE_ENTRY_ESCAPE(66_0F_3A_15),
    TABLE_ENTRY_ESCAPE(66_0F_3A_16),
    TABLE_ENTRY("extractps",        X86_MN_EXTRACTPS,       PF_VEX,         X86_ISET_SSE41,     OP(Ed, Vdq, Ib)),

    TABLE_ENTRY("insertf128",       X86_MN_INSERTF128,      PF_VEX256,      X86_ISET_AVX,       OP(Vqq, Hqq, Wqq, Ib)),
    TABLE_ENTRY("extractf128",      X86_MN_EXTRACTF128,     PF_VEX256,      X86_ISET_AVX,       OP(Wdq, Vqq, Ib)),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY("cvtps2ph",         X86_MN_CVTPS2PH,        PF_OVEX,        X86_ISET_F16C,      OP(Woq, Vx, Ib)), // Woq
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    /* 20h */
    TABLE_ENTRY_ESCAPE(66_0F_3A_20),
    TABLE_ENTRY_ESCAPE(66_0F_3A_21),
    TABLE_ENTRY_ESCAPE(66_0F_3A_22),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    /* 30h */
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    TABLE_ENTRY("inserti128",       X86_MN_INSERTI128,      PF_VEX256,      X86_ISET_AVX2,      OP(Vqq, Hqq, Wqq, Ib)),
    TABLE_ENTRY("extracti128",      X86_MN_EXTRACTI128,     PF_VEX256,      X86_ISET_AVX2,      OP(Wdq, Vqq, Ib)),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    /* 40h */
    TABLE_ENTRY("dpps",             X86_MN_DPPS,            PF_VEX,         X86_ISET_SSE41,     OP(Vx, Hx, Wx, Ib)), 
    TABLE_ENTRY("dppd",             X86_MN_DPPD,            PF_VEX,         X86_ISET_SSE41,     OP(Vdq, Hdq, Wdq, Ib)),
    TABLE_ENTRY("mpsadbw",          X86_MN_MPSADBW,         PF_VEX,         X86_ISET_SSE41,     OP(Vx, Hx, Wx, Ib)),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY("pclmulqdq",        X86_MN_PCLMULQDQ,       PF_VEX,         X86_ISET_PCLMULQDQ, OP(Vdq, Hdq, Wdq, Ib)),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY("perm2i128",        X86_MN_PERM2I128,       PF_VEX256,      X86_ISET_AVX2,      OP(Vqq, Hqq, Wqq, Ib)),
    TABLE_ENTRY_INVALID(),

    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY("blendvps",         X86_MN_BLENDVPS,        PF_VEX,         X86_ISET_SSE41,     OP(Vx, Hx, Wx, Lx)), 
    TABLE_ENTRY("blendvpd",         X86_MN_BLENDVPD,        PF_VEX,         X86_ISET_SSE41,     OP(Vx, Hx, Wx, Lx)), 
    TABLE_ENTRY("pblendvb",         X86_MN_PBLENDVB,        PF_VEX,         X86_ISET_SSE41,     OP(Vx, Hx, Wx, Lx)), 
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    /* 50h */
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_ESCAPE(66_0F_3A_5C),
    TABLE_ENTRY_ESCAPE(66_0F_3A_5D),
    TABLE_ENTRY_ESCAPE(66_0F_3A_5E),
    TABLE_ENTRY_ESCAPE(66_0F_3A_5F),

    /* 60h */
    TABLE_ENTRY("pcmpestrm",        X86_MN_PCMPESTRM,       PF_VEX,         X86_ISET_SSE42,     OP(Vdq, Wdq, Ib)),
    TABLE_ENTRY("pcmpestri",        X86_MN_PCMPESTRI,       PF_VEX,         X86_ISET_SSE42,     OP(Vdq, Wdq, Ib)),
    TABLE_ENTRY("pcmpistrm",        X86_MN_PCMPISTRM,       PF_VEX,         X86_ISET_SSE42,     OP(Vdq, Wdq, Ib)),
    TABLE_ENTRY("pcmpistri",        X86_MN_PCMPISTRI,       PF_VEX,         X86_ISET_SSE42,     OP(Vdq, Wdq, Ib)),

    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    TABLE_ENTRY_ESCAPE(66_0F_3A_68),
    TABLE_ENTRY_ESCAPE(66_0F_3A_69),
    TABLE_ENTRY_ESCAPE(66_0F_3A_6A),
    TABLE_ENTRY_ESCAPE(66_0F_3A_6B),
    TABLE_ENTRY_ESCAPE(66_0F_3A_6C),
    TABLE_ENTRY_ESCAPE(66_0F_3A_6D),
    TABLE_ENTRY_ESCAPE(66_0F_3A_6E),
    TABLE_ENTRY_ESCAPE(66_0F_3A_6F),

    /* 70h */
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    TABLE_ENTRY_ESCAPE(66_0F_3A_78),
    TABLE_ENTRY_ESCAPE(66_0F_3A_79),
    TABLE_ENTRY_ESCAPE(66_0F_3A_7A),
    TABLE_ENTRY_ESCAPE(66_0F_3A_7B),
    TABLE_ENTRY_ESCAPE(66_0F_3A_7C),
    TABLE_ENTRY_ESCAPE(66_0F_3A_7D),
    TABLE_ENTRY_ESCAPE(66_0F_3A_7E),
    TABLE_ENTRY_ESCAPE(66_0F_3A_7F),

    /* 80h */
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    /* 90h */
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    /* A0h */
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    /* B0h */
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    /* C0h */
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    /* D0h */
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY("aeskeygenassist",      X86_MN_AESKEYGENASSIST,     PF_VEX,         X86_ISET_AESNI,     OP(Vdq, Wdq, Ib)),

    /* E0h */
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    /* F0h */
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID()

TABLE_END(66_0F_3A)

TABLE_BEGIN(66_0F_3A_14)

    /* modr/m byte mod field:  0 = !11b, 1 = 11b */
    TABLE_ENTRY("pextrb",       X86_MN_PEXTRB,          PF_VEX128,          X86_ISET_SSE41,     OP(Mb, Vdq, Ib)),
    TABLE_ENTRY("pextrb",       X86_MN_PEXTRB,          PF_VEX128,          X86_ISET_SSE41,     OP(Rb, Vdq, Ib))

TABLE_END(66_0F_3A_14)

TABLE_BEGIN(66_0F_3A_15)

    /* modr/m byte mod field:  0 = !11b, 1 = 11b */
    TABLE_ENTRY("pextrw",       X86_MN_PEXTRW,          PF_VEX128,          X86_ISET_SSE41,     OP(Mw, Vdq, Ib)),
    TABLE_ENTRY("pextrw",       X86_MN_PEXTRW,          PF_VEX128,          X86_ISET_SSE41,     OP(Rw, Vdq, Ib))

TABLE_END(66_0F_3A_15)

TABLE_BEGIN(66_0F_3A_16)

    /* operand size: 0 = 16-bits, 1 = 32-bits, 2 = 64-bits */
    TABLE_ENTRY("pextrd",       X86_MN_PEXTRD,          PF_VEX128,          X86_ISET_SSE41,     OP(Ey, Vdq, Ib)),
    TABLE_ENTRY("pextrd",       X86_MN_PEXTRD,          PF_VEX128,          X86_ISET_SSE41,     OP(Ey, Vdq, Ib)),
    TABLE_ENTRY("pextrq",       X86_MN_PEXTRQ,          PF_VEX128,          X86_ISET_SSE41,     OP(Ey, Vdq, Ib))

TABLE_END(66_0F_3A_16)

TABLE_BEGIN(66_0F_3A_20)

    /* modr/m byte mod field:  0 = !11b, 1 = 11b */
    TABLE_ENTRY("pinsrb",       X86_MN_PINSRB,          PF_VEX128,          X86_ISET_SSE41,     OP(Vdq, Hdq, Mb, Ib)),
    TABLE_ENTRY("pinsrb",       X86_MN_PINSRB,          PF_VEX128,          X86_ISET_SSE41,     OP(Vdq, Hdq, Ry, Ib))

TABLE_END(66_0F_3A_20)

TABLE_BEGIN(66_0F_3A_21)

    /* modr/m byte mod field:  0 = !11b, 1 = 11b */
    TABLE_ENTRY("insertps",     X86_MN_INSERTPS,        PF_VEX128,          X86_ISET_SSE41,     OP(Vdq, Hdq, Md, Ib)),
    TABLE_ENTRY("insertps",     X86_MN_INSERTPS,        PF_VEX128,          X86_ISET_SSE41,     OP(Vdq, Hdq, Udq, Ib))

TABLE_END(66_0F_3A_21)

TABLE_BEGIN(66_0F_3A_22)

    /* operand size: 0 = 16-bits, 1 = 32-bits, 2 = 64-bits */
    TABLE_ENTRY("pinsrd",       X86_MN_PINSRD,          PF_VEX128,          X86_ISET_SSE41,     OP(Vdq, Hdq, Ey, Ib)),
    TABLE_ENTRY("pinsrd",       X86_MN_PINSRD,          PF_VEX128,          X86_ISET_SSE41,     OP(Vdq, Hdq, Ey, Ib)),
    TABLE_ENTRY("pinsrq",       X86_MN_PINSRQ,          PF_VEX128,          X86_ISET_SSE41,     OP(Vdq, Hdq, Ey, Ib))

TABLE_END(66_0F_3A_22)

#if 1

// FMA4!

TABLE_BEGIN(66_0F_3A_5C)

    /* operand size: 0 = vex.w0, 1 = vex.w1 */
    TABLE_ENTRY("fmaddsubps",   X86_MN_FMADDSUBPS,      PF_OVEX,            X86_ISET_FMA4,      OP(Vx, Hx, Wx, Lx)),
    TABLE_ENTRY("fmaddsubps",   X86_MN_FMADDSUBPS,      PF_OVEX,            X86_ISET_FMA4,      OP(Vx, Hx, Lx, Wx))

TABLE_END(66_0F_3A_5C)

TABLE_BEGIN(66_0F_3A_5D)

    /* operand size: 0 = vex.w0, 1 = vex.w1 */
    TABLE_ENTRY("fmaddsubpd",   X86_MN_FMADDSUBPD,      PF_OVEX,            X86_ISET_FMA4,      OP(Vx, Hx, Wx, Lx)),
    TABLE_ENTRY("fmaddsubpd",   X86_MN_FMADDSUBPD,      PF_OVEX,            X86_ISET_FMA4,      OP(Vx, Hx, Lx, Wx))

TABLE_END(66_0F_3A_5D)

TABLE_BEGIN(66_0F_3A_5E)

    /* operand size: 0 = vex.w0, 1 = vex.w1 */
    TABLE_ENTRY("fmsubaddps",   X86_MN_FMSUBADDPS,      PF_OVEX,            X86_ISET_FMA4,      OP(Vx, Hx, Wx, Lx)),
    TABLE_ENTRY("fmsubaddps",   X86_MN_FMSUBADDPS,      PF_OVEX,            X86_ISET_FMA4,      OP(Vx, Hx, Lx, Wx))

TABLE_END(66_0F_3A_5E)

TABLE_BEGIN(66_0F_3A_5F)

    /* operand size: 0 = vex.w0, 1 = vex.w1 */
    TABLE_ENTRY("fmsubaddpd",   X86_MN_FMSUBADDPD,      PF_OVEX,            X86_ISET_FMA4,      OP(Vx, Hx, Wx, Lx)),
    TABLE_ENTRY("fmsubaddpd",   X86_MN_FMSUBADDPD,      PF_OVEX,            X86_ISET_FMA4,      OP(Vx, Hx, Lx, Wx))

TABLE_END(66_0F_3A_5F)

TABLE_BEGIN(66_0F_3A_68)

    /* operand size: 0 = vex.w0, 1 = vex.w1 */
    TABLE_ENTRY("fmaddps",      X86_MN_FMADDPS,         PF_OVEX,            X86_ISET_FMA4,      OP(Vx, Hx, Wx, Lx)),
    TABLE_ENTRY("fmaddps",      X86_MN_FMADDPS,         PF_OVEX,            X86_ISET_FMA4,      OP(Vx, Hx, Lx, Wx))

TABLE_END(66_0F_3A_68)

TABLE_BEGIN(66_0F_3A_69)

    /* operand size: 0 = vex.w0, 1 = vex.w1 */
    TABLE_ENTRY("fmaddpd",      X86_MN_FMADDPD,         PF_OVEX,            X86_ISET_FMA4,      OP(Vx, Hx, Wx, Lx)),
    TABLE_ENTRY("fmaddpd",      X86_MN_FMADDPD,         PF_OVEX,            X86_ISET_FMA4,      OP(Vx, Hx, Lx, Wx))

TABLE_END(66_0F_3A_69)

TABLE_BEGIN(66_0F_3A_6A)

    /* operand size: 0 = vex.w0, 1 = vex.w1 */
    TABLE_ENTRY("fmaddss",      X86_MN_FMADDSS,         PF_OVEX,            X86_ISET_FMA4,      OP(Vx, Hx, Wd, Lx)),
    TABLE_ENTRY("fmaddss",      X86_MN_FMADDSS,         PF_OVEX,            X86_ISET_FMA4,      OP(Vx, Hx, Lx, Wd))

TABLE_END(66_0F_3A_6A)

TABLE_BEGIN(66_0F_3A_6B)

    /* operand size: 0 = vex.w0, 1 = vex.w1 */
    TABLE_ENTRY("fmaddsd",      X86_MN_FMADDSD,         PF_OVEX,            X86_ISET_FMA4,      OP(Vx, Hx, Wq, Lx)),
    TABLE_ENTRY("fmaddsd",      X86_MN_FMADDSD,         PF_OVEX,            X86_ISET_FMA4,      OP(Vx, Hx, Lx, Wq))

TABLE_END(66_0F_3A_6B)

TABLE_BEGIN(66_0F_3A_6C)

    /* operand size: 0 = vex.w0, 1 = vex.w1 */
    TABLE_ENTRY("fmsubps",      X86_MN_FMSUBPS,         PF_OVEX,            X86_ISET_FMA4,      OP(Vx, Hx, Wx, Lx)),
    TABLE_ENTRY("fmsubps",      X86_MN_FMSUBPS,         PF_OVEX,            X86_ISET_FMA4,      OP(Vx, Hx, Lx, Wx))

TABLE_END(66_0F_3A_6C)

TABLE_BEGIN(66_0F_3A_6D)

    /* operand size: 0 = vex.w0, 1 = vex.w1 */
    TABLE_ENTRY("fmsubpd",      X86_MN_FMSUBPD,         PF_OVEX,            X86_ISET_FMA4,      OP(Vx, Hx, Wx, Lx)),
    TABLE_ENTRY("fmsubpd",      X86_MN_FMSUBPD,         PF_OVEX,            X86_ISET_FMA4,      OP(Vx, Hx, Lx, Wx))

TABLE_END(66_0F_3A_6D)

TABLE_BEGIN(66_0F_3A_6E)

    /* operand size: 0 = vex.w0, 1 = vex.w1 */
    TABLE_ENTRY("fmsubss",      X86_MN_FMSUBSS,         PF_OVEX,            X86_ISET_FMA4,      OP(Vx, Hx, Wd, Lx)),
    TABLE_ENTRY("fmsubss",      X86_MN_FMSUBSS,         PF_OVEX,            X86_ISET_FMA4,      OP(Vx, Hx, Lx, Wd))

TABLE_END(66_0F_3A_6E)

TABLE_BEGIN(66_0F_3A_6F)

    /* operand size: 0 = vex.w0, 1 = vex.w1 */
    TABLE_ENTRY("fmsubsd",      X86_MN_FMSUBSD,         PF_OVEX,            X86_ISET_FMA4,      OP(Vx, Hx, Wq, Lx)),
    TABLE_ENTRY("fmsubsd",      X86_MN_FMSUBSD,         PF_OVEX,            X86_ISET_FMA4,      OP(Vx, Hx, Lx, Wq))

TABLE_END(66_0F_3A_6F)

TABLE_BEGIN(66_0F_3A_78)

    /* operand size: 0 = vex.w0, 1 = vex.w1 */
    TABLE_ENTRY("fnmaddps",     X86_MN_FNMADDPS,        PF_OVEX,            X86_ISET_FMA4,      OP(Vx, Hx, Wx, Lx)),
    TABLE_ENTRY("fnmaddps",     X86_MN_FNMADDPS,        PF_OVEX,            X86_ISET_FMA4,      OP(Vx, Hx, Lx, Wx))

TABLE_END(66_0F_3A_78)

TABLE_BEGIN(66_0F_3A_79)

    /* operand size: 0 = vex.w0, 1 = vex.w1 */
    TABLE_ENTRY("fnmaddpd",     X86_MN_FNMADDPD,        PF_OVEX,            X86_ISET_FMA4,      OP(Vx, Hx, Wx, Lx)),
    TABLE_ENTRY("fnmaddpd",     X86_MN_FNMADDPD,        PF_OVEX,            X86_ISET_FMA4,      OP(Vx, Hx, Lx, Wx))

TABLE_END(66_0F_3A_79)

TABLE_BEGIN(66_0F_3A_7A)

    /* operand size: 0 = vex.w0, 1 = vex.w1 */
    TABLE_ENTRY("fnmaddss",     X86_MN_FNMADDSS,        PF_OVEX,            X86_ISET_FMA4,      OP(Vx, Hx, Wd, Lx)),
    TABLE_ENTRY("fnmaddss",     X86_MN_FNMADDSS,        PF_OVEX,            X86_ISET_FMA4,      OP(Vx, Hx, Lx, Wd))

TABLE_END(66_0F_3A_7A)

TABLE_BEGIN(66_0F_3A_7B)

    /* operand size: 0 = vex.w0, 1 = vex.w1 */
    TABLE_ENTRY("fnmaddsd",     X86_MN_FNMADDSD,        PF_OVEX,            X86_ISET_FMA4,      OP(Vx, Hx, Wq, Lx)),
    TABLE_ENTRY("fnmaddsd",     X86_MN_FNMADDSD,        PF_OVEX,            X86_ISET_FMA4,      OP(Vx, Hx, Lx, Wq))

TABLE_END(66_0F_3A_7B)

TABLE_BEGIN(66_0F_3A_7C)

    /* operand size: 0 = vex.w0, 1 = vex.w1 */
    TABLE_ENTRY("fnmsubps",     X86_MN_FNMSUBPS,        PF_OVEX,            X86_ISET_FMA4,      OP(Vx, Hx, Wx, Lx)),
    TABLE_ENTRY("fnmsubps",     X86_MN_FNMSUBPS,        PF_OVEX,            X86_ISET_FMA4,      OP(Vx, Hx, Lx, Wx))

TABLE_END(66_0F_3A_7C)

TABLE_BEGIN(66_0F_3A_7D)

    /* operand size: 0 = vex.w0, 1 = vex.w1 */
    TABLE_ENTRY("fnmsubpd",     X86_MN_FNMSUBPD,        PF_OVEX,            X86_ISET_FMA4,      OP(Vx, Hx, Wx, Lx)),
    TABLE_ENTRY("fnmsubpd",     X86_MN_FNMSUBPD,        PF_OVEX,            X86_ISET_FMA4,      OP(Vx, Hx, Lx, Wx))

TABLE_END(66_0F_3A_7D)

TABLE_BEGIN(66_0F_3A_7E)

    /* operand size: 0 = vex.w0, 1 = vex.w1 */
    TABLE_ENTRY("fnmsubss",     X86_MN_FNMSUBSS,        PF_OVEX,            X86_ISET_FMA4,      OP(Vx, Hx, Wd, Lx)),
    TABLE_ENTRY("fnmsubss",     X86_MN_FNMSUBSS,        PF_OVEX,            X86_ISET_FMA4,      OP(Vx, Hx, Lx, Wd))

TABLE_END(66_0F_3A_7E)

TABLE_BEGIN(66_0F_3A_7F)

    /* operand size: 0 = vex.w0, 1 = vex.w1 */
    TABLE_ENTRY("fnmsubsd",     X86_MN_FNMSUBSD,        PF_OVEX,            X86_ISET_FMA4,      OP(Vx, Hx, Wq, Lx)),
    TABLE_ENTRY("fnmsubsd",     X86_MN_FNMSUBSD,        PF_OVEX,            X86_ISET_FMA4,      OP(Vx, Hx, Lx, Wq))

TABLE_END(66_0F_3A_7F)

#endif

/****************************************************************************

    Three-byte tables - F2h mandatory prefix

****************************************************************************/

TABLE_BEGIN(F2_0F_38)

    /* 00h */
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    /* 10h */
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    /* 20h */
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    /* 30h */
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    /* 40h */
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    /* 50h */
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    /* 60h */
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    /* 70h */
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    /* 80h */
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    /* 90h */
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    /* A0h */
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    /* B0h */
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    /* C0h */
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    /* D0h */
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    /* E0h */
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    /* F0h */
    TABLE_ENTRY("crc32",        X86_MN_CRC32,       PF_NONE,    X86_ISET_SSE42,     OP(Gy, Eb)),
    TABLE_ENTRY("crc32",        X86_MN_CRC32,       PF_NONE,    X86_ISET_SSE42,     OP(Gy, Ev)),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY("pdep",         X86_MN_PDEP,        PF_OVEX,    X86_ISET_BMI2,      OP(Gy, By, Ey)),
    TABLE_ENTRY("mulx",         X86_MN_MULX,        PF_OVEX,    X86_ISET_BMI2,      OP(By, Gy, rDX, Ey)),
    TABLE_ENTRY("shrx",         X86_MN_SHRX,        PF_OVEX,    X86_ISET_BMI2,      OP(Gy, Ey, By)),

    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID()

TABLE_END(F2_0F_38)

TABLE_BEGIN(F2_0F_3A)

    /* 00h */
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    /* 10h */
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    /* 20h */
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    /* 30h */
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    /* 40h */
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    /* 50h */
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    /* 60h */
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    /* 70h */
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    /* 80h */
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    /* 90h */
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    /* A0h */
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    /* B0h */
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    /* C0h */
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    /* D0h */
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    /* E0h */
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    /* F0h */
    TABLE_ENTRY("rorx",     X86_MN_RORX,    PF_OVEX,    X86_ISET_BMI2,  OP(Gy, Ey, Ib)),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID()

TABLE_END(F2_0F_3A)

/****************************************************************************

    Three-byte tables - F3h mandatory prefix

****************************************************************************/

TABLE_BEGIN(F3_0F_38)

    /* 00h */
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    /* 10h */
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    /* 20h */
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    /* 30h */
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    /* 40h */
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    /* 50h */
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    /* 60h */
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    /* 70h */
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    /* 80h */
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    /* 90h */
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    /* A0h */
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    /* B0h */
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    /* C0h */
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    /* D0h */
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    /* E0h */
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    /* F0h */
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY("pext",     X86_MN_PEXT,    PF_OVEX,    X86_ISET_BMI2,      OP(Gy, By, Ey)),
    TABLE_ENTRY("adox",     X86_MN_ADOX,    PF_NONE,    X86_ISET_ADX,       OP(Gy, Ey)),
    TABLE_ENTRY("sarx",     X86_MN_SARX,    PF_OVEX,    X86_ISET_BMI2,      OP(Gy, Ey, By)),

    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID()

TABLE_END(F3_0F_38)

TABLE_BEGIN(F3_0F_3A)

    /* 00h */
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    /* 10h */
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    /* 20h */
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    /* 30h */
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    /* 40h */
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    /* 50h */
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    /* 60h */
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    /* 70h */
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    /* 80h */
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    /* 90h */
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    /* A0h */
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    /* B0h */
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    /* C0h */
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    /* D0h */
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    /* E0h */
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    /* F0h */
    TABLE_ENTRY("rorx",     X86_MN_RORX,    PF_OVEX,    X86_ISET_BMI2,  OP(Gy, Ey, Ib)),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID()

TABLE_END(F3_0F_3A)

/****************************************************************************

    XOP tables

****************************************************************************/

TABLE_BEGIN(XOP_8)

    /* 00h */
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    /* 10h */
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    /* 20h */
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    /* 30h */
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    /* 40h */
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    /* 50h */
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    /* 60h */
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    /* 70h */
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    /* 80h */
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY("vpmacssww",    X86_MN_VPMACSSWW,   PF_NONE,    X86_ISET_XOP,   OP(Vss, Hss, Wss, Lss)),
    TABLE_ENTRY("vpmacsswd",    X86_MN_VPMACSSWD,   PF_NONE,    X86_ISET_XOP,   OP(Vss, Hss, Wss, Lss)),
    TABLE_ENTRY("vpmacssdql",   X86_MN_VPMACSSDQL,  PF_NONE,    X86_ISET_XOP,   OP(Vss, Hss, Wss, Lss)),

    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY("vpmacssdd",    X86_MN_VPMACSSDD,   PF_NONE,    X86_ISET_XOP,   OP(Vss, Hss, Wss, Lss)),
    TABLE_ENTRY("vpmacssdqh",   X86_MN_VPMACSSDQH,  PF_NONE,    X86_ISET_XOP,   OP(Vss, Hss, Wss, Lss)),

    /* 90h */
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY("vpmacsww",     X86_MN_VPMACSWW,    PF_NONE,    X86_ISET_XOP,   OP(Vss, Hss, Wss, Lss)),
    TABLE_ENTRY("vpmacswd",     X86_MN_VPMACSWD,    PF_NONE,    X86_ISET_XOP,   OP(Vss, Hss, Wss, Lss)),
    TABLE_ENTRY("vpmacsdql",    X86_MN_VPMACSDQL,   PF_NONE,    X86_ISET_XOP,   OP(Vss, Hss, Wss, Lss)),

    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY("vpmacsdd",     X86_MN_VPMACSDD,    PF_NONE,    X86_ISET_XOP,   OP(Vss, Hss, Wss, Lss)),
    TABLE_ENTRY("vpmacsdqh",    X86_MN_VPMACSDQH,   PF_NONE,    X86_ISET_XOP,   OP(Vss, Hss, Wss, Lss)),

    /* A0h */
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_ESCAPE(XOP_8_A2),
    TABLE_ENTRY_ESCAPE(XOP_8_A3),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY("vpmadcsswd",   X86_MN_VPMADCSSWD,  PF_NONE,    X86_ISET_XOP,   OP(Vss, Hss, Wss, Lss)),
    TABLE_ENTRY_INVALID(),

    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    /* B0h */
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY("vpmadcswd",    X86_MN_VPMADCSWD,   PF_NONE,    X86_ISET_XOP,   OP(Vss, Hss, Wss, Lss)),
    TABLE_ENTRY_INVALID(),

    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    /* C0h */
    TABLE_ENTRY("vprotb",       X86_MN_VPROTB,      PF_NONE,    X86_ISET_XOP,   OP(Vss, Wss, Ib)),
    TABLE_ENTRY("vprotw",       X86_MN_VPROTW,      PF_NONE,    X86_ISET_XOP,   OP(Vss, Wss, Ib)),
    TABLE_ENTRY("vprotd",       X86_MN_VPROTD,      PF_NONE,    X86_ISET_XOP,   OP(Vss, Wss, Ib)),
    TABLE_ENTRY("vprotq",       X86_MN_VPROTQ,      PF_NONE,    X86_ISET_XOP,   OP(Vss, Wss, Ib)),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY("vpcom*b",      X86_MN_VPCOMB,      PF_XOPCC,   X86_ISET_XOP,   OP(Vss, Hss, Wss, Ib)),
    TABLE_ENTRY("vpcom*w",      X86_MN_VPCOMW,      PF_XOPCC,   X86_ISET_XOP,   OP(Vss, Hss, Wss, Ib)),
    TABLE_ENTRY("vpcom*d",      X86_MN_VPCOMD,      PF_XOPCC,   X86_ISET_XOP,   OP(Vss, Hss, Wss, Ib)),
    TABLE_ENTRY("vpcom*q",      X86_MN_VPCOMQ,      PF_XOPCC,   X86_ISET_XOP,   OP(Vss, Hss, Wss, Ib)),

    /* D0h */
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    /* E0h */
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY("vpcom*ub",     X86_MN_VPCOMUB,     PF_XOPCC,   X86_ISET_XOP,   OP(Vss, Hss, Wss, Ib)),
    TABLE_ENTRY("vpcom*uw",     X86_MN_VPCOMUW,     PF_XOPCC,   X86_ISET_XOP,   OP(Vss, Hss, Wss, Ib)),
    TABLE_ENTRY("vpcom*ud",     X86_MN_VPCOMUD,     PF_XOPCC,   X86_ISET_XOP,   OP(Vss, Hss, Wss, Ib)),
    TABLE_ENTRY("vpcom*uq",     X86_MN_VPCOMUQ,     PF_XOPCC,   X86_ISET_XOP,   OP(Vss, Hss, Wss, Ib)),

    /* F0h */
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID()

TABLE_END(XOP_8)

TABLE_BEGIN(XOP_8_A2)

    /* operand size: 0 = xop.w0, 1 = xop.w1 */
    TABLE_ENTRY("vpcmov",           X86_MN_VPCMOV,          PF_NONE,    X86_ISET_XOP,   OP(Vx, Hx, Wx, Lx)),
    TABLE_ENTRY("vpcmov",           X86_MN_VPCMOV,          PF_NONE,    X86_ISET_XOP,   OP(Vx, Hx, Lx, Wx))

TABLE_END(XOP_8_A2)

TABLE_BEGIN(XOP_8_A3)

    /* operand size: 0 = xop.w0, 1 = xop.w1 */
    TABLE_ENTRY("vpperm",           X86_MN_VPPERM,          PF_NONE,    X86_ISET_XOP,   OP(Vss, Hss, Wss, Lss)),
    TABLE_ENTRY("vpperm",           X86_MN_VPPERM,          PF_NONE,    X86_ISET_XOP,   OP(Vss, Hss, Lss, Wss))

TABLE_END(XOP_8_A3)

TABLE_BEGIN(XOP_9)

    /* 00h */
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_ESCAPE(XOP_9_1),
    TABLE_ENTRY_ESCAPE(XOP_9_2),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    /* 10h */
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_ESCAPE(XOP_9_12),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    /* 20h */
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    /* 30h */
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    /* 40h */
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    /* 50h */
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    /* 60h */
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    /* 70h */
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    /* 80h */
    TABLE_ENTRY("vfrczps",          X86_MN_VFRCZPS,         PF_NONE,    X86_ISET_XOP,   OP(Vx, Wx)),
    TABLE_ENTRY("vfrczpd",          X86_MN_VFRCZPD,         PF_NONE,    X86_ISET_XOP,   OP(Vx, Wx)),
    TABLE_ENTRY("vfrczss",          X86_MN_VFRCZSS,         PF_NONE,    X86_ISET_XOP,   OP(Vss, Wss)),
    TABLE_ENTRY("vfrczsd",          X86_MN_VFRCZSD,         PF_NONE,    X86_ISET_XOP,   OP(Vss, Wsd)),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    /* 90h */
    TABLE_ENTRY_ESCAPE(XOP_9_90),
    TABLE_ENTRY_ESCAPE(XOP_9_91),
    TABLE_ENTRY_ESCAPE(XOP_9_92),
    TABLE_ENTRY_ESCAPE(XOP_9_93),
    TABLE_ENTRY_ESCAPE(XOP_9_94),
    TABLE_ENTRY_ESCAPE(XOP_9_95),
    TABLE_ENTRY_ESCAPE(XOP_9_96),
    TABLE_ENTRY_ESCAPE(XOP_9_97),

    TABLE_ENTRY_ESCAPE(XOP_9_98),
    TABLE_ENTRY_ESCAPE(XOP_9_99),
    TABLE_ENTRY_ESCAPE(XOP_9_9A),
    TABLE_ENTRY_ESCAPE(XOP_9_9B),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    /* A0h */
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    /* B0h */
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    /* C0h */
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY("vphaddbw",         X86_MN_VPHADDBW,        PF_NONE,    X86_ISET_XOP,   OP(Vss, Wss)),
    TABLE_ENTRY("vphaddbd",         X86_MN_VPHADDBD,        PF_NONE,    X86_ISET_XOP,   OP(Vss, Wss)),
    TABLE_ENTRY("vphaddbq",         X86_MN_VPHADDBQ,        PF_NONE,    X86_ISET_XOP,   OP(Vss, Wss)),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY("vphaddwd",         X86_MN_VPHADDWD,        PF_NONE,    X86_ISET_XOP,   OP(Vss, Wss)),
    TABLE_ENTRY("vphaddwq",         X86_MN_VPHADDWQ,        PF_NONE,    X86_ISET_XOP,   OP(Vss, Wss)),

    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY("vphadddq",         X86_MN_VPHADDDQ,        PF_NONE,    X86_ISET_XOP,   OP(Vss, Wss)),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    /* D0h */
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY("vphaddubwd",       X86_MN_VPHADDUBWD,      PF_NONE,    X86_ISET_XOP,   OP(Vss, Wss)),
    TABLE_ENTRY("vphaddubd",        X86_MN_VPHADDUBD,       PF_NONE,    X86_ISET_XOP,   OP(Vss, Wss)),
    TABLE_ENTRY("vphaddubq",        X86_MN_VPHADDUBQ,       PF_NONE,    X86_ISET_XOP,   OP(Vss, Wss)),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY("vphadduwd",        X86_MN_VPHADDUWD,       PF_NONE,    X86_ISET_XOP,   OP(Vss, Wss)),
    TABLE_ENTRY("vphadduwq",        X86_MN_VPHADDUWQ,       PF_NONE,    X86_ISET_XOP,   OP(Vss, Wss)),

    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY("vphaddudq",        X86_MN_VPHADDUDQ,       PF_NONE,    X86_ISET_XOP,   OP(Vss, Wss)),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    /* E0h */
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY("vphsubbw",         X86_MN_VPHSUBBW,        PF_NONE,    X86_ISET_XOP,   OP(Vss, Wss)),
    TABLE_ENTRY("vphsubwd",         X86_MN_VPHSUBWD,        PF_NONE,    X86_ISET_XOP,   OP(Vss, Wss)),
    TABLE_ENTRY("vphsubdq",         X86_MN_VPHSUBDQ,        PF_NONE,    X86_ISET_XOP,   OP(Vss, Wss)),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    /* F0h */
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID()

TABLE_END(XOP_9)

TABLE_BEGIN(XOP_9_1)

    /* modr/m byte reg/nnn field */

    /* 00h */
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY("blcfill",          X86_MN_BLCFILL,     PF_NONE,    X86_ISET_XOP,   OP(By, Ey)),
    TABLE_ENTRY("blsfill",          X86_MN_BLSFILL,     PF_NONE,    X86_ISET_XOP,   OP(By, Ey)),
    TABLE_ENTRY("blcs",             X86_MN_BLCS,        PF_NONE,    X86_ISET_XOP,   OP(By, Ey)),
    TABLE_ENTRY("tzmsk",            X86_MN_TZMSK,       PF_NONE,    X86_ISET_XOP,   OP(By, Ey)),
    TABLE_ENTRY("blcic",            X86_MN_BLCIC,       PF_NONE,    X86_ISET_XOP,   OP(By, Ey)),
    TABLE_ENTRY("blcsic",           X86_MN_BLCSIC,      PF_NONE,    X86_ISET_XOP,   OP(By, Ey)),
    TABLE_ENTRY("t1mskc",           X86_MN_T1MSKC,      PF_NONE,    X86_ISET_XOP,   OP(By, Ey))

TABLE_END(XOP_9_1)

TABLE_BEGIN(XOP_9_2)

    /* modr/m byte reg/nnn field */

    /* 00h */
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY("blcmsk",           X86_MN_BLCMSK,      PF_NONE,    X86_ISET_XOP,   OP(By, Ey)),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID()

TABLE_END(XOP_9_2)

TABLE_BEGIN(XOP_9_12)

    /* modr/m byte reg/nnn field */

    /* 00h */
    TABLE_ENTRY("llwpcb",           X86_MN_LLWPCB,      PF_NONE,    X86_ISET_XOP,   OP(Ry)),
    TABLE_ENTRY("slwpcb",           X86_MN_SLWPCB,      PF_NONE,    X86_ISET_XOP,   OP(Ry)),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID()

TABLE_END(XOP_9_12)

TABLE_BEGIN(XOP_9_90)

    /* operand size: 0 = xop.w0, 1 = xop.w1 */
    TABLE_ENTRY("vprotb",       X86_MN_VPROTB,      PF_NONE,    X86_ISET_XOP,   OP(Vss, Wss, Hss)),
    TABLE_ENTRY("vprotb",       X86_MN_VPROTB,      PF_NONE,    X86_ISET_XOP,   OP(Vss, Hss, Wss))

TABLE_END(XOP_9_90)

TABLE_BEGIN(XOP_9_91)

    /* operand size: 0 = xop.w0, 1 = xop.w1 */
    TABLE_ENTRY("vprotw",       X86_MN_VPROTW,      PF_NONE,    X86_ISET_XOP,   OP(Vss, Wss, Hss)),
    TABLE_ENTRY("vprotw",       X86_MN_VPROTW,      PF_NONE,    X86_ISET_XOP,   OP(Vss, Hss, Wss))

TABLE_END(XOP_9_91)

TABLE_BEGIN(XOP_9_92)

    /* operand size: 0 = xop.w0, 1 = xop.w1 */
    TABLE_ENTRY("vprotd",       X86_MN_VPROTD,      PF_NONE,    X86_ISET_XOP,   OP(Vss, Wss, Hss)),
    TABLE_ENTRY("vprotd",       X86_MN_VPROTD,      PF_NONE,    X86_ISET_XOP,   OP(Vss, Hss, Wss))

TABLE_END(XOP_9_92)

TABLE_BEGIN(XOP_9_93)

    /* operand size: 0 = xop.w0, 1 = xop.w1 */
    TABLE_ENTRY("vprotq",       X86_MN_VPROTQ,      PF_NONE,    X86_ISET_XOP,   OP(Vss, Wss, Hss)),
    TABLE_ENTRY("vprotq",       X86_MN_VPROTQ,      PF_NONE,    X86_ISET_XOP,   OP(Vss, Hss, Wss))

TABLE_END(XOP_9_93)

TABLE_BEGIN(XOP_9_94)

    /* operand size: 0 = xop.w0, 1 = xop.w1 */
    TABLE_ENTRY("vpshlb",       X86_MN_VPSHLB,      PF_NONE,    X86_ISET_XOP,   OP(Vss, Wss, Hss)),
    TABLE_ENTRY("vpshlb",       X86_MN_VPSHLB,      PF_NONE,    X86_ISET_XOP,   OP(Vss, Hss, Wss))

TABLE_END(XOP_9_94)

TABLE_BEGIN(XOP_9_95)

    /* operand size: 0 = xop.w0, 1 = xop.w1 */
    TABLE_ENTRY("vpshlw",       X86_MN_VPSHLW,      PF_NONE,    X86_ISET_XOP,   OP(Vss, Wss, Hss)),
    TABLE_ENTRY("vpshlw",       X86_MN_VPSHLW,      PF_NONE,    X86_ISET_XOP,   OP(Vss, Hss, Wss))

TABLE_END(XOP_9_95)

TABLE_BEGIN(XOP_9_96)

    /* operand size: 0 = xop.w0, 1 = xop.w1 */
    TABLE_ENTRY("vpshld",       X86_MN_VPSHLD,      PF_NONE,    X86_ISET_XOP,   OP(Vss, Wss, Hss)),
    TABLE_ENTRY("vpshld",       X86_MN_VPSHLD,      PF_NONE,    X86_ISET_XOP,   OP(Vss, Hss, Wss))

TABLE_END(XOP_9_96)

TABLE_BEGIN(XOP_9_97)

    /* operand size: 0 = xop.w0, 1 = xop.w1 */
    TABLE_ENTRY("vpshlq",       X86_MN_VPSHLQ,      PF_NONE,    X86_ISET_XOP,   OP(Vss, Wss, Hss)),
    TABLE_ENTRY("vpshlq",       X86_MN_VPSHLQ,      PF_NONE,    X86_ISET_XOP,   OP(Vss, Hss, Wss))

TABLE_END(XOP_9_97)

TABLE_BEGIN(XOP_9_98)

    /* operand size: 0 = xop.w0, 1 = xop.w1 */
    TABLE_ENTRY("vpshab",       X86_MN_VPSHAB,      PF_NONE,    X86_ISET_XOP,   OP(Vss, Wss, Hss)),
    TABLE_ENTRY("vpshab",       X86_MN_VPSHAB,      PF_NONE,    X86_ISET_XOP,   OP(Vss, Hss, Wss))

TABLE_END(XOP_9_98)

TABLE_BEGIN(XOP_9_99)

    /* operand size: 0 = xop.w0, 1 = xop.w1 */
    TABLE_ENTRY("vpshaw",       X86_MN_VPSHAW,      PF_NONE,    X86_ISET_XOP,   OP(Vss, Wss, Hss)),
    TABLE_ENTRY("vpshaw",       X86_MN_VPSHAW,      PF_NONE,    X86_ISET_XOP,   OP(Vss, Hss, Wss))

TABLE_END(XOP_9_99)

TABLE_BEGIN(XOP_9_9A)

    /* operand size: 0 = xop.w0, 1 = xop.w1 */
    TABLE_ENTRY("vpshad",       X86_MN_VPSHAD,      PF_NONE,    X86_ISET_XOP,   OP(Vss, Wss, Hss)),
    TABLE_ENTRY("vpshad",       X86_MN_VPSHAD,      PF_NONE,    X86_ISET_XOP,   OP(Vss, Hss, Wss))

TABLE_END(XOP_9_9A)

TABLE_BEGIN(XOP_9_9B)

    /* operand size: 0 = xop.w0, 1 = xop.w1 */
    TABLE_ENTRY("vpshaq",       X86_MN_VPSHAQ,      PF_NONE,    X86_ISET_XOP,   OP(Vss, Wss, Hss)),
    TABLE_ENTRY("vpshaq",       X86_MN_VPSHAQ,      PF_NONE,    X86_ISET_XOP,   OP(Vss, Hss, Wss))

TABLE_END(XOP_9_9B)

TABLE_BEGIN(XOP_A)

    /* 00h */
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    /* 10h */
    TABLE_ENTRY("bextr",        X86_MN_BEXTR,       PF_NONE,    X86_ISET_XOP,   OP(Gy, Ey, Id)),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_ESCAPE(XOP_A_12),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    /* 20h */
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    /* 30h */
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    /* 40h */
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    /* 50h */
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    /* 60h */
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    /* 70h */
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    /* 80h */
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    /* 90h */
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    /* A0h */
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    /* B0h */
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    /* C0h */
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    /* D0h */
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    /* E0h */
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    /* F0h */
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID()

TABLE_END(XOP_A)

TABLE_BEGIN(XOP_A_12)

    /* modr/m byte reg/nnn field */

    /* 00h */
    TABLE_ENTRY("lwpins",           X86_MN_LWPINS,      PF_NONE,    X86_ISET_XOP,   OP(By, Ed, Id)),
    TABLE_ENTRY("lwpval",           X86_MN_LWPVAL,      PF_NONE,    X86_ISET_XOP,   OP(By, Ed, Id)),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID()

TABLE_END(XOP_A_12)

/****************************************************************************

    3DNow! table

****************************************************************************/

TABLE_BEGIN(0F_0F)

    /* 00h */
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY("pi2fw",        X86_MN_PI2FW,       PF_NONE,    X86_ISET_3DNOW, OP(Pq, Qq)),
    TABLE_ENTRY("pi2fd",        X86_MN_PI2FD,       PF_NONE,    X86_ISET_3DNOW, OP(Pq, Qq)),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    /* 10h */
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY("pf2iw",        X86_MN_PF2IW,       PF_NONE,    X86_ISET_3DNOW, OP(Pq, Qq)),
    TABLE_ENTRY("pf2id",        X86_MN_PF2ID,       PF_NONE,    X86_ISET_3DNOW, OP(Pq, Qq)),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    /* 20h */
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    /* 30h */
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    /* 40h */
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    /* 50h */
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    /* 60h */
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    /* 70h */
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    /* 80h */
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY("pfnacc",       X86_MN_PFNACC,      PF_NONE,    X86_ISET_3DNOW, OP(Pq, Qq)),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY("pfpnacc",      X86_MN_PFPNACC,     PF_NONE,    X86_ISET_3DNOW, OP(Pq, Qq)),
    TABLE_ENTRY_INVALID(),

    /* 90h */
    TABLE_ENTRY("pfcmpge",      X86_MN_PFCMPGE,     PF_NONE,    X86_ISET_3DNOW, OP(Pq, Qq)),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY("pfmin",        X86_MN_PFMIN,       PF_NONE,    X86_ISET_3DNOW, OP(Pq, Qq)),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY("pfrcp",        X86_MN_PFRCP,       PF_NONE,    X86_ISET_3DNOW, OP(Pq, Qq)),
    TABLE_ENTRY("pfrsqrt",      X86_MN_PFRSQRT,     PF_NONE,    X86_ISET_3DNOW, OP(Pq, Qq)),

    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY("pfsub",        X86_MN_PFSUB,       PF_NONE,    X86_ISET_3DNOW, OP(Pq, Qq)),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY("pfadd",        X86_MN_PFADD,       PF_NONE,    X86_ISET_3DNOW, OP(Pq, Qq)),
    TABLE_ENTRY_INVALID(),

    /* A0h */
    TABLE_ENTRY("pfcmpgt",      X86_MN_PFCMPGT,     PF_NONE,    X86_ISET_3DNOW, OP(Pq, Qq)),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY("pfmax",        X86_MN_PFMAX,       PF_NONE,    X86_ISET_3DNOW, OP(Pq, Qq)),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY("pfrcpit1",     X86_MN_PFRCPIT1,    PF_NONE,    X86_ISET_3DNOW, OP(Pq, Qq)),
    TABLE_ENTRY("pfrsqit1",     X86_MN_PFRSQIT1,    PF_NONE,    X86_ISET_3DNOW, OP(Pq, Qq)),

    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY("pfsubr",       X86_MN_PFSUBR,      PF_NONE,    X86_ISET_3DNOW, OP(Pq, Qq)),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY("pfacc",        X86_MN_PFACC,       PF_NONE,    X86_ISET_3DNOW, OP(Pq, Qq)),
    TABLE_ENTRY_INVALID(),

    /* B0h */
    TABLE_ENTRY("pfcmpeq",      X86_MN_PFCMPEQ,     PF_NONE,    X86_ISET_3DNOW, OP(Pq, Qq)),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY("pfmul",        X86_MN_PFMUL,       PF_NONE,    X86_ISET_3DNOW, OP(Pq, Qq)),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY("pfrcpit2",     X86_MN_PFRCPIT2,    PF_NONE,    X86_ISET_3DNOW, OP(Pq, Qq)),
    TABLE_ENTRY("pmulhrw",      X86_MN_PMULHRW,     PF_NONE,    X86_ISET_3DNOW, OP(Pq, Qq)),

    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY("pswapd",       X86_MN_PSWAPD,      PF_NONE,    X86_ISET_3DNOW, OP(Pq, Qq)),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY("pavgusb",      X86_MN_PAVGUSB,     PF_NONE,    X86_ISET_3DNOW, OP(Pq, Qq)),

    /* C0h */
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    /* D0h */
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    /* E0h */
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    /* F0h */
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),

    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID(),
    TABLE_ENTRY_INVALID()

TABLE_END(0F_0F)

/****************************************************************************

    Master table

****************************************************************************/

const x86_table_link_t x86_table_links[] =
{
    TABLE_LINK(1B, BYTE),
    TABLE_LINK(60, OSIZE),
    TABLE_LINK(61, OSIZE),
    TABLE_LINK(63, DMODE),
    TABLE_LINK(6D, OSIZE),
    TABLE_LINK(6F, OSIZE),
    TABLE_LINK(80, MODRM_REG),
    TABLE_LINK(81, MODRM_REG),
    TABLE_LINK(82, MODRM_REG),
    TABLE_LINK(83, MODRM_REG),
    TABLE_LINK(8F, MODRM_REG),
    TABLE_LINK(98, OSIZE),
    TABLE_LINK(99, OSIZE),
    TABLE_LINK(9C, OSIZE),
    TABLE_LINK(9D, OSIZE),
    TABLE_LINK(A5, OSIZE),
    TABLE_LINK(A7, OSIZE),
    TABLE_LINK(AB, OSIZE),
    TABLE_LINK(AD, OSIZE),
    TABLE_LINK(AF, OSIZE),
    TABLE_LINK(C0, MODRM_REG),
    TABLE_LINK(C1, MODRM_REG),
    TABLE_LINK(C6, MODRM_REG),
    TABLE_LINK(C7, MODRM_REG),
    TABLE_LINK(CF, OSIZE),
    TABLE_LINK(D0, MODRM_REG),
    TABLE_LINK(D1, MODRM_REG),
    TABLE_LINK(D2, MODRM_REG),
    TABLE_LINK(D3, MODRM_REG),
    TABLE_LINK(D8, MODRM_MOD),
    TABLE_LINK(D9, MODRM_MOD),
    TABLE_LINK(DA, MODRM_MOD),
    TABLE_LINK(DB, MODRM_MOD),
    TABLE_LINK(DC, MODRM_MOD),
    TABLE_LINK(DD, MODRM_MOD),
    TABLE_LINK(DE, MODRM_MOD),
    TABLE_LINK(DF, MODRM_MOD),
    TABLE_LINK(E3, ASIZE), 
    TABLE_LINK(F6, MODRM_REG),
    TABLE_LINK(F7, MODRM_REG),
    TABLE_LINK(FE, MODRM_REG),
    TABLE_LINK(FF, MODRM_REG),
    TABLE_LINK(D8_MOD_N11, MODRM_REG),
    TABLE_LINK(D8_MOD_11, FPU),
    TABLE_LINK(D9_MOD_N11, MODRM_REG),
    TABLE_LINK(D9_MOD_11, FPU),
    TABLE_LINK(DA_MOD_N11, MODRM_REG),
    TABLE_LINK(DA_MOD_11, FPU),
    TABLE_LINK(DB_MOD_N11, MODRM_REG),
    TABLE_LINK(DB_MOD_11, FPU),
    TABLE_LINK(DC_MOD_N11, MODRM_REG),
    TABLE_LINK(DC_MOD_11, FPU),
    TABLE_LINK(DD_MOD_N11, MODRM_REG),
    TABLE_LINK(DD_MOD_11, FPU),
    TABLE_LINK(DE_MOD_N11, MODRM_REG),
    TABLE_LINK(DE_MOD_11, FPU),
    TABLE_LINK(DF_MOD_N11, MODRM_REG), 
    TABLE_LINK(DF_MOD_11, FPU),

    TABLE_LINK(0F, BYTE),
    TABLE_LINK(0F_00, MODRM_REG),
    TABLE_LINK(0F_01, MODRM_REG),
    TABLE_LINK(0F_0D, MODRM_REG),
    TABLE_LINK(0F_0F, 3DNOW),
    TABLE_LINK(0F_12, MODRM_MOD),
    TABLE_LINK(0F_16, MODRM_MOD),
    TABLE_LINK(0F_18, MODRM_REG),
    TABLE_LINK(0F_19, MODRM_REG),
    TABLE_LINK(0F_1C, MODRM_REG),
    TABLE_LINK(0F_1D, MODRM_REG),
    TABLE_LINK(0F_1E, MODRM_REG),
    TABLE_LINK(0F_1F, MODRM_REG),
    TABLE_LINK(0F_6E, OSIZE),
    TABLE_LINK(0F_71, MODRM_REG),
    TABLE_LINK(0F_71_REG2, MODRM_MOD),
    TABLE_LINK(0F_71_REG4, MODRM_MOD),
    TABLE_LINK(0F_71_REG6, MODRM_MOD),
    TABLE_LINK(0F_72, MODRM_REG),
    TABLE_LINK(0F_72_REG2, MODRM_MOD),
    TABLE_LINK(0F_72_REG4, MODRM_MOD),
    TABLE_LINK(0F_72_REG6, MODRM_MOD), 
    TABLE_LINK(0F_73, MODRM_REG),
    TABLE_LINK(0F_73_REG2, MODRM_MOD),
    TABLE_LINK(0F_73_REG6, MODRM_MOD), 
    TABLE_LINK(0F_7E, OSIZE), 
    TABLE_LINK(0F_AE, MODRM_REG),
    TABLE_LINK(0F_B9, MODRM_REG),
    TABLE_LINK(0F_BA, MODRM_REG),
    TABLE_LINK(0F_C4, MODRM_MOD),
    TABLE_LINK(0F_C7, MODRM_REG),
    TABLE_LINK(0F_00_REG0, MODRM_MOD),
    TABLE_LINK(0F_00_REG1, MODRM_MOD),
    TABLE_LINK(0F_01_REG0, MODRM_MOD),
    TABLE_LINK(0F_01_REG1, MODRM_MOD),
    TABLE_LINK(0F_01_REG2, MODRM_MOD),
    TABLE_LINK(0F_01_REG3, MODRM_MOD),
    TABLE_LINK(0F_01_REG4, MODRM_MOD),
    TABLE_LINK(0F_01_REG7, MODRM_MOD),
    TABLE_LINK(0F_01_REG0_RM, MODRM_RM),
    TABLE_LINK(0F_01_REG1_RM, MODRM_RM),
    TABLE_LINK(0F_01_REG2_RM, MODRM_RM),
    TABLE_LINK(0F_01_REG3_RM, MODRM_RM),
    TABLE_LINK(0F_01_REG7_RM, MODRM_RM),
    TABLE_LINK(0F_18_REG0, MODRM_MOD),
    TABLE_LINK(0F_18_REG1, MODRM_MOD),
    TABLE_LINK(0F_18_REG2, MODRM_MOD),
    TABLE_LINK(0F_18_REG3, MODRM_MOD),
    TABLE_LINK(0F_AE_REG0, MODRM_MOD),
    TABLE_LINK(0F_AE_REG1, MODRM_MOD),
    TABLE_LINK(0F_AE_REG2, MODRM_MOD),
    TABLE_LINK(0F_AE_REG3, MODRM_MOD),
    TABLE_LINK(0F_AE_REG4, MODRM_MOD),
    TABLE_LINK(0F_AE_REG5, MODRM_MOD),
    TABLE_LINK(0F_AE_REG6, MODRM_MOD),
    TABLE_LINK(0F_AE_REG7, MODRM_MOD),
    TABLE_LINK(0F_C7_REG1, OSIZE),
    TABLE_LINK(0F_C7_REG3, OSIZE),
    TABLE_LINK(0F_C7_REG4, OSIZE),
    TABLE_LINK(0F_C7_REG5, OSIZE),
    TABLE_LINK(0F_C7_REG6, MODRM_MOD),
    TABLE_LINK(0F_C7_REG7, MODRM_MOD),
    TABLE_LINK(66_0F, BYTE),
    TABLE_LINK(66_0F_19, MODRM_REG),
    TABLE_LINK(66_0F_1C, MODRM_REG),
    TABLE_LINK(66_0F_1D, MODRM_REG),
    TABLE_LINK(66_0F_1E, MODRM_REG),
    TABLE_LINK(66_0F_1F, MODRM_REG),
    TABLE_LINK(66_0F_6E, OSIZE),
    TABLE_LINK(66_0F_71, MODRM_REG),
    TABLE_LINK(66_0F_71_REG2, MODRM_MOD),
    TABLE_LINK(66_0F_71_REG4, MODRM_MOD),
    TABLE_LINK(66_0F_71_REG6, MODRM_MOD),
    TABLE_LINK(66_0F_72, MODRM_REG),
    TABLE_LINK(66_0F_72_REG2, MODRM_MOD),
    TABLE_LINK(66_0F_72_REG4, MODRM_MOD),
    TABLE_LINK(66_0F_72_REG6, MODRM_MOD),
    TABLE_LINK(66_0F_73, MODRM_REG), 
    TABLE_LINK(66_0F_73_REG2, MODRM_MOD), 
    TABLE_LINK(66_0F_73_REG3, MODRM_MOD),
    TABLE_LINK(66_0F_73_REG6, MODRM_MOD),
    TABLE_LINK(66_0F_73_REG7, MODRM_MOD),
    TABLE_LINK(66_0F_78, MODRM_REG),
    TABLE_LINK(66_0F_7E, OSIZE),
    TABLE_LINK(66_0F_AE, MODRM_REG), 
    TABLE_LINK(66_0F_C4, MODRM_MOD),
    TABLE_LINK(66_0F_C7, MODRM_REG),
    TABLE_LINK(66_0F_C7_REG6, MODRM_MOD),
    TABLE_LINK(F3_0F, BYTE),
    TABLE_LINK(F3_0F_01, MODRM_REG),
    TABLE_LINK(F3_0F_01_REG5, MODRM_MOD),
    TABLE_LINK(F3_0F_01_REG5_RM, MODRM_RM),
    TABLE_LINK(F3_0F_19, MODRM_REG),
    TABLE_LINK(F3_0F_1C, MODRM_REG),
    TABLE_LINK(F3_0F_1D, MODRM_REG),
    TABLE_LINK(F3_0F_1E, MODRM_REG),
    TABLE_LINK(F3_0F_1E_REG1, OSIZE),
    TABLE_LINK(F3_0F_1E_REG7_RM, MODRM_RM),
    TABLE_LINK(F3_0F_1F, MODRM_REG),
    TABLE_LINK(F3_0F_71, MODRM_REG),
    TABLE_LINK(F3_0F_72, MODRM_REG),
    TABLE_LINK(F3_0F_73, MODRM_REG),
    TABLE_LINK(F3_0F_AE, MODRM_REG),
    TABLE_LINK(F3_0F_AE_REG5, OSIZE),
    TABLE_LINK(F3_0F_C7, MODRM_REG),
    TABLE_LINK(F3_0F_C7_REG6, MODRM_MOD),
    TABLE_LINK(F3_0F_C7_REG7, MODRM_MOD),
    TABLE_LINK(F2_0F, BYTE),
    TABLE_LINK(F2_0F_19, MODRM_REG),
    TABLE_LINK(F2_0F_1C, MODRM_REG),
    TABLE_LINK(F2_0F_1D, MODRM_REG),
    TABLE_LINK(F2_0F_1E, MODRM_REG),
    TABLE_LINK(F2_0F_1F, MODRM_REG),
    TABLE_LINK(F2_0F_71, MODRM_REG),
    TABLE_LINK(F2_0F_72, MODRM_REG),
    TABLE_LINK(F2_0F_73, MODRM_REG),
    TABLE_LINK(F2_0F_AE, MODRM_REG),

    TABLE_LINK(0F_38, BYTE),
    TABLE_LINK(0F_38_F3, MODRM_REG),
    TABLE_LINK(0F_38_F6, OSIZE),
    TABLE_LINK(0F_3A, BYTE),
    TABLE_LINK(66_0F_38, BYTE),
    TABLE_LINK(66_0F_38_20, MODRM_MOD),
    TABLE_LINK(66_0F_38_21, MODRM_MOD),
    TABLE_LINK(66_0F_38_22, MODRM_MOD),
    TABLE_LINK(66_0F_38_23, MODRM_MOD),
    TABLE_LINK(66_0F_38_24, MODRM_MOD),
    TABLE_LINK(66_0F_38_25, MODRM_MOD),
    TABLE_LINK(66_0F_38_30, MODRM_MOD),
    TABLE_LINK(66_0F_38_31, MODRM_MOD),
    TABLE_LINK(66_0F_38_32, MODRM_MOD),
    TABLE_LINK(66_0F_38_33, MODRM_MOD),
    TABLE_LINK(66_0F_38_34, MODRM_MOD),
    TABLE_LINK(66_0F_38_35, MODRM_MOD),
    TABLE_LINK(66_0F_38_45, OSIZE_VEX),
    TABLE_LINK(66_0F_38_47, OSIZE_VEX),
    TABLE_LINK(66_0F_38_8C, OSIZE_VEX),
    TABLE_LINK(66_0F_38_8E, OSIZE_VEX),
    TABLE_LINK(66_0F_38_90, OSIZE_VEX),
    TABLE_LINK(66_0F_38_91, OSIZE_VEX),
    TABLE_LINK(66_0F_38_92, OSIZE_VEX),
    TABLE_LINK(66_0F_38_93, OSIZE_VEX),
    TABLE_LINK(66_0F_38_96, OSIZE_VEX),
    TABLE_LINK(66_0F_38_97, OSIZE_VEX),
    TABLE_LINK(66_0F_38_98, OSIZE_VEX), 
    TABLE_LINK(66_0F_38_99, OSIZE_VEX),
    TABLE_LINK(66_0F_38_9A, OSIZE_VEX),
    TABLE_LINK(66_0F_38_9B, OSIZE_VEX), 
    TABLE_LINK(66_0F_38_9C, OSIZE_VEX),
    TABLE_LINK(66_0F_38_9D, OSIZE_VEX),
    TABLE_LINK(66_0F_38_9E, OSIZE_VEX),
    TABLE_LINK(66_0F_38_9F, OSIZE_VEX),
    TABLE_LINK(66_0F_38_A6, OSIZE_VEX),
    TABLE_LINK(66_0F_38_A7, OSIZE_VEX),
    TABLE_LINK(66_0F_38_A8, OSIZE_VEX),
    TABLE_LINK(66_0F_38_A9, OSIZE_VEX),
    TABLE_LINK(66_0F_38_AA, OSIZE_VEX),
    TABLE_LINK(66_0F_38_AB, OSIZE_VEX),
    TABLE_LINK(66_0F_38_AC, OSIZE_VEX),
    TABLE_LINK(66_0F_38_AD, OSIZE_VEX),
    TABLE_LINK(66_0F_38_AE, OSIZE_VEX),
    TABLE_LINK(66_0F_38_AF, OSIZE_VEX),
    TABLE_LINK(66_0F_38_B6, OSIZE_VEX),
    TABLE_LINK(66_0F_38_B7, OSIZE_VEX),
    TABLE_LINK(66_0F_38_B8, OSIZE_VEX),
    TABLE_LINK(66_0F_38_B9, OSIZE_VEX),
    TABLE_LINK(66_0F_38_BA, OSIZE_VEX), 
    TABLE_LINK(66_0F_38_BB, OSIZE_VEX),
    TABLE_LINK(66_0F_38_BC, OSIZE_VEX),
    TABLE_LINK(66_0F_38_BD, OSIZE_VEX),
    TABLE_LINK(66_0F_38_BE, OSIZE_VEX),
    TABLE_LINK(66_0F_38_BF, OSIZE_VEX),
    TABLE_LINK(66_0F_38_F5, OSIZE),
    TABLE_LINK(66_0F_3A, BYTE),
    TABLE_LINK(66_0F_3A_14, MODRM_MOD),
    TABLE_LINK(66_0F_3A_15, MODRM_MOD),
    TABLE_LINK(66_0F_3A_16, OSIZE),
    TABLE_LINK(66_0F_3A_20, MODRM_MOD),
    TABLE_LINK(66_0F_3A_21, MODRM_MOD),
    TABLE_LINK(66_0F_3A_22, OSIZE),
    TABLE_LINK(F3_0F_38, BYTE),
    TABLE_LINK(F3_0F_3A, BYTE),
    TABLE_LINK(F2_0F_38, BYTE),
    TABLE_LINK(F2_0F_3A, BYTE),
    TABLE_LINK(XOP_8, BYTE),
    TABLE_LINK(XOP_8_A2, OSIZE_XOP),
    TABLE_LINK(XOP_8_A3, OSIZE_XOP),
    TABLE_LINK(XOP_9, BYTE), 
    TABLE_LINK(XOP_9_1, MODRM_REG),
    TABLE_LINK(XOP_9_2, MODRM_REG),
    TABLE_LINK(XOP_9_12, MODRM_REG),
    TABLE_LINK(XOP_9_90, OSIZE_XOP),
    TABLE_LINK(XOP_9_91, OSIZE_XOP),
    TABLE_LINK(XOP_9_92, OSIZE_XOP), 
    TABLE_LINK(XOP_9_93, OSIZE_XOP), 
    TABLE_LINK(XOP_9_94, OSIZE_XOP),
    TABLE_LINK(XOP_9_95, OSIZE_XOP),
    TABLE_LINK(XOP_9_96, OSIZE_XOP),
    TABLE_LINK(XOP_9_97, OSIZE_XOP),
    TABLE_LINK(XOP_9_98, OSIZE_XOP),
    TABLE_LINK(XOP_9_99, OSIZE_XOP),
    TABLE_LINK(XOP_9_9A, OSIZE_XOP),
    TABLE_LINK(XOP_9_9B, OSIZE_XOP),
    TABLE_LINK(XOP_A, BYTE),
    TABLE_LINK(XOP_A_12, MODRM_REG),

    TABLE_LINK(66_0F_3A_69, OSIZE_VEX),
    TABLE_LINK(66_0F_3A_5C, OSIZE_VEX),
    TABLE_LINK(66_0F_3A_5D, OSIZE_VEX),
    TABLE_LINK(66_0F_3A_5E, OSIZE_VEX),
    TABLE_LINK(66_0F_3A_5F, OSIZE_VEX),
    TABLE_LINK(66_0F_3A_68, OSIZE_VEX),
    TABLE_LINK(66_0F_3A_6A, OSIZE_VEX),
    TABLE_LINK(66_0F_3A_6B, OSIZE_VEX),
    TABLE_LINK(66_0F_3A_6C, OSIZE_VEX),
    TABLE_LINK(66_0F_3A_6D, OSIZE_VEX),
    TABLE_LINK(66_0F_3A_6E, OSIZE_VEX),
    TABLE_LINK(66_0F_3A_6F, OSIZE_VEX),
    TABLE_LINK(66_0F_3A_78, OSIZE_VEX),
    TABLE_LINK(66_0F_3A_79, OSIZE_VEX),
    TABLE_LINK(66_0F_3A_7A, OSIZE_VEX),
    TABLE_LINK(66_0F_3A_7B, OSIZE_VEX),
    TABLE_LINK(66_0F_3A_7C, OSIZE_VEX),
    TABLE_LINK(66_0F_3A_7D, OSIZE_VEX),
    TABLE_LINK(66_0F_3A_7E, OSIZE_VEX),
    TABLE_LINK(66_0F_3A_7F, OSIZE_VEX)
};

/****************************************************************************

    EFLAGS table

****************************************************************************/

const x86_eflags_t x86_eflags_table[] =
{
/*         X86_MN_ADD */ EFLAGS_ENTRY(  M,  NA,  NA,   M,   M,   M,   M,   M),
/*          X86_MN_OR */ EFLAGS_ENTRY(CLR,  NA,  NA,   M,   M,   M,   M, CLR),
/*         X86_MN_ADC */ EFLAGS_ENTRY(  M,  NA,  NA,   M,   M,   M,   M,  TM),
/*         X86_MN_SBB */ EFLAGS_ENTRY(  M,  NA,  NA,   M,   M,   M,   M,  TM),
/*         X86_MN_AND */ EFLAGS_ENTRY(CLR,  NA,  NA,   M,   M,   M,   M, CLR),
/*         X86_MN_DAA */ EFLAGS_ENTRY(  M,  NA,  NA,   M,   M,  TM,   M,  TM),
/*         X86_MN_SUB */ EFLAGS_ENTRY(  M,  NA,  NA,   M,   M,   M,   M,   M),
/*         X86_MN_DAS */ EFLAGS_ENTRY(  M,  NA,  NA,   M,   M,  TM,   M,  TM),
/*         X86_MN_XOR */ EFLAGS_ENTRY(CLR,  NA,  NA,   M,   M,   M,   M, CLR),
/*         X86_MN_AAA */ EFLAGS_ENTRY(  M,  NA,  NA,   M,   M,  TM,   M,   M),
/*         X86_MN_CMP */ EFLAGS_ENTRY(  M,  NA,  NA,   M,   M,   M,   M,   M),
/*         X86_MN_AAS */ EFLAGS_ENTRY(  M,  NA,  NA,   M,   M,  TM,   M,   M),
/*         X86_MN_INC */ EFLAGS_ENTRY(  M,  NA,  NA,   M,   M,   M,   M,  NA),
/*         X86_MN_DEC */ EFLAGS_ENTRY(  M,  NA,  NA,   M,   M,   M,   M,  NA),
/*       X86_MN_BOUND */ EFLAGS_ENTRY( NA,  NA, CLR,  NA,  NA,  NA,  NA,  NA),
/*        X86_MN_ARPL */ EFLAGS_ENTRY( NA,  NA,  NA,  NA,   M,  NA,  NA,  NA),
/*        X86_MN_IMUL */ EFLAGS_ENTRY(  M,  NA,  NA,   M,   M,   M,   M,   M),
/*        X86_MN_INSB */ EFLAGS_ENTRY(  M,   T,  NA,   M,   M,   M,   M,   M),
/*        X86_MN_INSW */ EFLAGS_ENTRY( NA,   T,  NA,  NA,  NA,  NA,  NA,  NA),
/*        X86_MN_INSD */ EFLAGS_ENTRY( NA,   T,  NA,  NA,  NA,  NA,  NA,  NA),
/*       X86_MN_OUTSB */ EFLAGS_ENTRY( NA,   T,  NA,  NA,  NA,  NA,  NA,  NA),
/*       X86_MN_OUTSW */ EFLAGS_ENTRY( NA,   T,  NA,  NA,  NA,  NA,  NA,  NA),
/*       X86_MN_OUTSD */ EFLAGS_ENTRY( NA,   T,  NA,  NA,  NA,  NA,  NA,  NA),
/*          X86_MN_JO */ EFLAGS_ENTRY(  T,  NA,  NA,  NA,  NA,  NA,  NA,  NA),
/*         X86_MN_JNO */ EFLAGS_ENTRY(  T,  NA,  NA,  NA,  NA,  NA,  NA,  NA),
/*          X86_MN_JC */ EFLAGS_ENTRY( NA,  NA,  NA,  NA,  NA,  NA,  NA,   T),
/*         X86_MN_JNC */ EFLAGS_ENTRY( NA,  NA,  NA,  NA,  NA,  NA,  NA,   T),
/*          X86_MN_JE */ EFLAGS_ENTRY( NA,  NA,  NA,  NA,   T,  NA,  NA,  NA),
/*         X86_MN_JNE */ EFLAGS_ENTRY( NA,  NA,  NA,  NA,   T,  NA,  NA,  NA),
/*         X86_MN_JNA */ EFLAGS_ENTRY( NA,  NA,  NA,  NA,   T,  NA,  NA,   T),
/*          X86_MN_JA */ EFLAGS_ENTRY( NA,  NA,  NA,  NA,   T,  NA,  NA,   T),
/*          X86_MN_JS */ EFLAGS_ENTRY( NA,  NA,  NA,   T,  NA,  NA,  NA,  NA),
/*         X86_MN_JNS */ EFLAGS_ENTRY( NA,  NA,  NA,   T,  NA,  NA,  NA,  NA),
/*         X86_MN_JPE */ EFLAGS_ENTRY( NA,  NA,  NA,  NA,  NA,  NA,   T,  NA),
/*         X86_MN_JPO */ EFLAGS_ENTRY( NA,  NA,  NA,  NA,  NA,  NA,   T,  NA),
/*        X86_MN_JNGE */ EFLAGS_ENTRY(  T,  NA,  NA,   T,  NA,  NA,  NA,  NA),
/*         X86_MN_JGE */ EFLAGS_ENTRY(  T,  NA,  NA,   T,  NA,  NA,  NA,  NA),
/*         X86_MN_JNG */ EFLAGS_ENTRY(  T,  NA,  NA,   T,   T,  NA,  NA,  NA),
/*          X86_MN_JG */ EFLAGS_ENTRY(  T,  NA,  NA,   T,   T,  NA,  NA,  NA),
/*        X86_MN_TEST */ EFLAGS_ENTRY(CLR,  NA,  NA,   M,   M,   M,   M, CLR),
/*        X86_MN_SAHF */ EFLAGS_ENTRY(CLR,  NA,  NA,   M,   M,   M,   M,   M),
/*        X86_MN_LAHF */ EFLAGS_ENTRY( NA,  NA,  NA,   T,   T,   T,   T,   T),
/*       X86_MN_MOVSB */ EFLAGS_ENTRY( NA,   T,  NA,  NA,  NA,  NA,  NA,  NA),
/*       X86_MN_MOVSW */ EFLAGS_ENTRY( NA,   T,  NA,  NA,  NA,  NA,  NA,  NA),
/*       X86_MN_MOVSD */ EFLAGS_ENTRY( NA,   T,  NA,  NA,  NA,  NA,  NA,  NA),
/*       X86_MN_MOVSQ */ EFLAGS_ENTRY( NA,   T,  NA,  NA,  NA,  NA,  NA,  NA),
/*       X86_MN_CMPSB */ EFLAGS_ENTRY(  M,   T,  NA,   M,   M,   M,   M,   M),
/*       X86_MN_CMPSW */ EFLAGS_ENTRY(  M,   T,  NA,   M,   M,   M,   M,   M),
/*       X86_MN_CMPSD */ EFLAGS_ENTRY(  M,   T,  NA,   M,   M,   M,   M,   M),
/*       X86_MN_CMPSQ */ EFLAGS_ENTRY(  M,   T,  NA,   M,   M,   M,   M,   M),
/*       X86_MN_STOSB */ EFLAGS_ENTRY(CLR,   T,  NA,   M,   M,   M,   M, CLR),
/*       X86_MN_STOSW */ EFLAGS_ENTRY( NA,   T,  NA,  NA,  NA,  NA,  NA,  NA),
/*       X86_MN_STOSD */ EFLAGS_ENTRY( NA,   T,  NA,  NA,  NA,  NA,  NA,  NA),
/*       X86_MN_STOSQ */ EFLAGS_ENTRY( NA,   T,  NA,  NA,  NA,  NA,  NA,  NA),
/*       X86_MN_LODSB */ EFLAGS_ENTRY( NA,   T,  NA,  NA,  NA,  NA,  NA,  NA),
/*       X86_MN_LODSW */ EFLAGS_ENTRY( NA,   T,  NA,  NA,  NA,  NA,  NA,  NA),
/*       X86_MN_LODSD */ EFLAGS_ENTRY( NA,   T,  NA,  NA,  NA,  NA,  NA,  NA),
/*       X86_MN_LODSQ */ EFLAGS_ENTRY( NA,   T,  NA,  NA,  NA,  NA,  NA,  NA),
/*       X86_MN_SCASB */ EFLAGS_ENTRY(  M,   T,  NA,   M,   M,   M,   M,   M),
/*       X86_MN_SCASW */ EFLAGS_ENTRY(  M,   T,  NA,   M,   M,   M,   M,   M),
/*       X86_MN_SCASD */ EFLAGS_ENTRY(  M,   T,  NA,   M,   M,   M,   M,   M),
/*       X86_MN_SCASQ */ EFLAGS_ENTRY(  M,   T,  NA,   M,   M,   M,   M,   M),
/*         X86_MN_ROL */ EFLAGS_ENTRY(  M,  NA,  NA,   M,   M,   M,   M,   M),
/*         X86_MN_ROR */ EFLAGS_ENTRY(  M,  NA,  NA,   M,   M,   M,   M,   M),
/*         X86_MN_RCL */ EFLAGS_ENTRY(  M,  NA,  NA,   M,   M,   M,   M,  TM),
/*         X86_MN_RCR */ EFLAGS_ENTRY(  M,  NA,  NA,   M,   M,   M,   M,  TM),
/*         X86_MN_SAL */ EFLAGS_ENTRY(  M,  NA,  NA,   M,   M,   M,   M,   M),
/*         X86_MN_SHR */ EFLAGS_ENTRY(  M,  NA,  NA,   M,   M,   M,   M,   M),
/*         X86_MN_SHL */ EFLAGS_ENTRY(  M,  NA,  NA,   M,   M,   M,   M,   M),
/*         X86_MN_SAR */ EFLAGS_ENTRY(  M,  NA,  NA,   M,   M,   M,   M,   M),
/*         X86_MN_INT */ EFLAGS_ENTRY( NA,  NA, CLR,  NA,  NA,  NA,  NA,  NA),
/*        X86_MN_INTO */ EFLAGS_ENTRY(  T,  NA, CLR,  NA,  NA,  NA,  NA,  NA),
/*         X86_MN_AAM */ EFLAGS_ENTRY(  M,  NA,  NA,   M,   M,   M,   M,   M),
/*         X86_MN_AMX */ EFLAGS_ENTRY(  M,  NA,  NA,   M,   M,   M,   M,   M),
/*         X86_MN_AAD */ EFLAGS_ENTRY(  M,  NA,  NA,   M,   M,   M,   M,   M),
/*         X86_MN_ADX */ EFLAGS_ENTRY(  M,  NA,  NA,   M,   M,   M,   M,   M),
/*      X86_MN_SETALC */ EFLAGS_ENTRY( NA,  NA,  NA,  NA,  NA,  NA,  NA,   T),
/*      X86_MN_FCMOVB */ EFLAGS_ENTRY( NA,  NA,  NA,  NA,  NA,  NA,  NA,   T),
/*      X86_MN_FCMOVE */ EFLAGS_ENTRY( NA,  NA,  NA,  NA,   T,  NA,  NA,  NA),
/*     X86_MN_FCMOVBE */ EFLAGS_ENTRY( NA,  NA,  NA,  NA,   T,  NA,  NA,  NA),
/*      X86_MN_FCMOVU */ EFLAGS_ENTRY( NA,  NA,  NA,  NA,  NA,  NA,   T,  NA),
/*     X86_MN_FCMOVNB */ EFLAGS_ENTRY( NA,  NA,  NA,  NA,  NA,  NA,  NA,   T),
/*     X86_MN_FCMOVNE */ EFLAGS_ENTRY( NA,  NA,  NA,  NA,   T,  NA,  NA,  NA),
/*    X86_MN_FCMOVNBE */ EFLAGS_ENTRY( NA,  NA,  NA,  NA,   T,  NA,  NA,  NA),
/*     X86_MN_FCMOVNU */ EFLAGS_ENTRY( NA,  NA,  NA,  NA,  NA,  NA,   T,  NA),
/*      X86_MN_FUCOMI */ EFLAGS_ENTRY(CLR,  NA,  NA,  NA,   M,  NA,   M,   M),
/*       X86_MN_FCOMI */ EFLAGS_ENTRY(CLR,  NA,  NA,  NA,   M,  NA,   M,   M),
/*     X86_MN_FUCOMIP */ EFLAGS_ENTRY(CLR,  NA,  NA,  NA,   M,  NA,   M,   M),
/*      X86_MN_FCOMIP */ EFLAGS_ENTRY(CLR,  NA,  NA,  NA,   M,  NA,   M,   M),
/*      X86_MN_LOOPNE */ EFLAGS_ENTRY( NA,  NA,  NA,  NA,   T,  NA,  NA,  NA),
/*       X86_MN_LOOPE */ EFLAGS_ENTRY( NA,  NA,  NA,  NA,   T,  NA,  NA,  NA),
/*       X86_MN_ICEBP */ EFLAGS_ENTRY( NA,  NA, CLR,  NA,   T,  NA,  NA,  NA),
/*         X86_MN_CMC */ EFLAGS_ENTRY( NA,  NA,  NA,  NA,   T,  NA,  NA,  TM),
/*         X86_MN_NEG */ EFLAGS_ENTRY(  M,  NA,  NA,   M,   M,   M,   M,   M),
/*         X86_MN_MUL */ EFLAGS_ENTRY(  M,  NA,  NA,   M,   M,   M,   M,   M),
/*         X86_MN_DIV */ EFLAGS_ENTRY(  M,  NA,  NA,   M,   M,   M,   M,   M),
/*        X86_MN_IDIV */ EFLAGS_ENTRY(  M,  NA,  NA,   M,   M,   M,   M,   M),
/*         X86_MN_CLC */ EFLAGS_ENTRY( NA,  NA,  NA,  NA,  NA,  NA,  NA, CLR),
/*         X86_MN_STC */ EFLAGS_ENTRY( NA,  NA,  NA,  NA,  NA,  NA,  NA, SET),
/*         X86_MN_CLI */ EFLAGS_ENTRY( NA,  NA, CLR,  NA,  NA,  NA,  NA,  NA),
/*         X86_MN_STI */ EFLAGS_ENTRY( NA,  NA, SET,  NA,  NA,  NA,  NA,  NA),
/*         X86_MN_CLD */ EFLAGS_ENTRY( NA, CLR,  NA,  NA,  NA,  NA,  NA,  NA),
/*         X86_MN_STD */ EFLAGS_ENTRY( NA, SET,  NA,  NA,  NA,  NA,  NA,  NA),
/*        X86_MN_VERR */ EFLAGS_ENTRY( NA,  NA,  NA,  NA,   M,  NA,  NA,  NA),
/*        X86_MN_VERW */ EFLAGS_ENTRY( NA,  NA,  NA,  NA,   M,  NA,  NA,  NA),
/*      X86_MN_VMCALL */ EFLAGS_ENTRY(  M,  NA,  NA,   M,   M,   M,   M,   M),
/*    X86_MN_VMLAUNCH */ EFLAGS_ENTRY(  M,  NA,  NA,   M,   M,   M,   M,   M),
/*    X86_MN_VMRESUME */ EFLAGS_ENTRY(  M,  NA,  NA,   M,   M,   M,   M,   M),
/*      X86_MN_VMXOFF */ EFLAGS_ENTRY(  M,  NA,  NA,   M,   M,   M,   M,   M),
/*         X86_MN_LAR */ EFLAGS_ENTRY( NA,  NA,  NA,  NA,   M,  NA,  NA,  NA),
/*         X86_MN_LSL */ EFLAGS_ENTRY( NA,  NA,  NA,  NA,   M,  NA,  NA,  NA),
/*         X86_MN_MOV */ EFLAGS_ENTRY(  M,  NA,  NA,   M,   M,   M,   M,   M),
/*     X86_MN_UCOMISS */ EFLAGS_ENTRY(  M,  NA,  NA,   M,   M,   M,   M,   M),
/*     X86_MN_UCOMISD */ EFLAGS_ENTRY( NA,  NA,  NA,  NA,   M,  NA,   M,   M),
/*      X86_MN_COMISS */ EFLAGS_ENTRY( NA,  NA,  NA,  NA,   M,  NA,   M,   M),
/*      X86_MN_COMISD */ EFLAGS_ENTRY( NA,  NA,  NA,  NA,   M,  NA,   M,   M),
/*    X86_MN_SYSENTER */ EFLAGS_ENTRY( NA,  NA, CLR,  NA,  NA,  NA,  NA,  NA),
/*       X86_MN_PTEST */ EFLAGS_ENTRY(CLR,  NA, CLR, CLR,   M, CLR, CLR,   M),
/*      X86_MN_INVEPT */ EFLAGS_ENTRY(  M,  NA,  NA,   M,   M,   M,   M,   M),
/*     X86_MN_INVVPID */ EFLAGS_ENTRY(  M,  NA,  NA,   M,   M,   M,   M,   M),
/*   X86_MN_PCMPESTRM */ EFLAGS_ENTRY(  M,  NA,  NA,   M,   M, CLR, CLR,   M),
/*   X86_MN_PCMPESTRI */ EFLAGS_ENTRY(  M,  NA,  NA,   M,   M, CLR, CLR,   M),
/*   X86_MN_PCMPISTRM */ EFLAGS_ENTRY(  M,  NA,  NA,   M,   M, CLR, CLR,   M),
/*   X86_MN_PCMPISTRI */ EFLAGS_ENTRY(  M,  NA,  NA,   M,   M, CLR, CLR,   M),
/*       X86_MN_CMOVO */ EFLAGS_ENTRY(  T,  NA,  NA,  NA,  NA,  NA,  NA,  NA),
/*      X86_MN_CMOVNO */ EFLAGS_ENTRY(  T,  NA,  NA,  NA,  NA,  NA,  NA,  NA),
/*       X86_MN_CMOVC */ EFLAGS_ENTRY( NA,  NA,  NA,  NA,  NA,  NA,  NA,   T),
/*      X86_MN_CMOVNC */ EFLAGS_ENTRY( NA,  NA,  NA,  NA,  NA,  NA,  NA,   T),
/*       X86_MN_CMOVE */ EFLAGS_ENTRY( NA,  NA,  NA,  NA,   T,  NA,  NA,  NA),
/*      X86_MN_CMOVNE */ EFLAGS_ENTRY( NA,  NA,  NA,  NA,   T,  NA,  NA,  NA),
/*      X86_MN_CMOVNA */ EFLAGS_ENTRY( NA,  NA,  NA,  NA,   T,  NA,  NA,   T),
/*       X86_MN_CMOVA */ EFLAGS_ENTRY( NA,  NA,  NA,  NA,   T,  NA,  NA,   T),
/*       X86_MN_CMOVS */ EFLAGS_ENTRY( NA,  NA,  NA,   T,  NA,  NA,  NA,  NA),
/*      X86_MN_CMOVNS */ EFLAGS_ENTRY( NA,  NA,  NA,   T,  NA,  NA,  NA,  NA),
/*      X86_MN_CMOVPE */ EFLAGS_ENTRY( NA,  NA,  NA,  NA,  NA,  NA,   T,  NA),
/*      X86_MN_CMOVPO */ EFLAGS_ENTRY( NA,  NA,  NA,  NA,  NA,  NA,   T,  NA),
/*     X86_MN_CMOVNGE */ EFLAGS_ENTRY(  T,  NA,  NA,   T,  NA,  NA,  NA,  NA),
/*      X86_MN_CMOVGE */ EFLAGS_ENTRY(  T,  NA,  NA,   T,  NA,  NA,  NA,  NA),
/*      X86_MN_CMOVNG */ EFLAGS_ENTRY(  T,  NA,  NA,   T,   T,  NA,  NA,  NA),
/*       X86_MN_CMOVG */ EFLAGS_ENTRY(  T,  NA,  NA,   T,   T,  NA,  NA,  NA),
/*      X86_MN_VMREAD */ EFLAGS_ENTRY(  M,  NA,  NA,   M,   M,   M,   M,   M),
/*     X86_MN_VMWRITE */ EFLAGS_ENTRY(  M,  NA,  NA,   M,   M,   M,   M,   M),
/*        X86_MN_SETO */ EFLAGS_ENTRY(  T,  NA,  NA,  NA,  NA,  NA,  NA,  NA),
/*       X86_MN_SETNO */ EFLAGS_ENTRY(  T,  NA,  NA,  NA,  NA,  NA,  NA,  NA),
/*        X86_MN_SETC */ EFLAGS_ENTRY( NA,  NA,  NA,  NA,  NA,  NA,  NA,   T),
/*       X86_MN_SETNC */ EFLAGS_ENTRY( NA,  NA,  NA,  NA,  NA,  NA,  NA,   T),
/*        X86_MN_SETE */ EFLAGS_ENTRY( NA,  NA,  NA,  NA,   T,  NA,  NA,  NA),
/*       X86_MN_SETNE */ EFLAGS_ENTRY( NA,  NA,  NA,  NA,   T,  NA,  NA,  NA),
/*       X86_MN_SETNA */ EFLAGS_ENTRY( NA,  NA,  NA,  NA,   T,  NA,  NA,   T),
/*        X86_MN_SETA */ EFLAGS_ENTRY( NA,  NA,  NA,  NA,   T,  NA,  NA,   T),
/*        X86_MN_SETS */ EFLAGS_ENTRY( NA,  NA,  NA,   T,  NA,  NA,  NA,  NA),
/*       X86_MN_SETNS */ EFLAGS_ENTRY( NA,  NA,  NA,   T,  NA,  NA,  NA,  NA),
/*       X86_MN_SETPE */ EFLAGS_ENTRY( NA,  NA,  NA,  NA,  NA,  NA,   T,  NA),
/*       X86_MN_SETPO */ EFLAGS_ENTRY( NA,  NA,  NA,  NA,  NA,  NA,   T,  NA),
/*      X86_MN_SETNGE */ EFLAGS_ENTRY(  T,  NA,  NA,   T,  NA,  NA,  NA,  NA),
/*       X86_MN_SETGE */ EFLAGS_ENTRY(  T,  NA,  NA,   T,  NA,  NA,  NA,  NA),
/*       X86_MN_SETNG */ EFLAGS_ENTRY(  T,  NA,  NA,   T,   T,  NA,  NA,  NA),
/*        X86_MN_SETG */ EFLAGS_ENTRY(  T,  NA,  NA,   T,   T,  NA,  NA,  NA),
/*          X86_MN_BT */ EFLAGS_ENTRY(  M,  NA,  NA,   M,   M,   M,   M,   M),
/*        X86_MN_SHLD */ EFLAGS_ENTRY(  M,  NA,  NA,   M,   M,   M,   M,   M),
/*         X86_MN_BTS */ EFLAGS_ENTRY(  M,  NA,  NA,   M,   M,   M,   M,   M),
/*        X86_MN_SHRD */ EFLAGS_ENTRY(  M,  NA,  NA,   M,   M,   M,   M,   M),
/*     X86_MN_CMPXCHG */ EFLAGS_ENTRY(  M,  NA,  NA,   M,   M,   M,   M,   M),
/*         X86_MN_BTR */ EFLAGS_ENTRY(  M,  NA,  NA,   M,   M,   M,   M,   M),
/*      X86_MN_POPCNT */ EFLAGS_ENTRY(CLR,  NA,  NA, CLR,   M, CLR, CLR, CLR),
/*         X86_MN_BTC */ EFLAGS_ENTRY(  M,  NA,  NA,   M,   M,   M,   M,   M),
/*         X86_MN_BSF */ EFLAGS_ENTRY(  M,  NA,  NA,   M,   M,   M,   M,   M),
/*         X86_MN_BSR */ EFLAGS_ENTRY(  M,  NA,  NA,   M,   M,   M,   M,   M),
/*        X86_MN_XADD */ EFLAGS_ENTRY(  M,  NA,  NA,   M,   M,   M,   M,   M),
/*   X86_MN_CMPXCHG8B */ EFLAGS_ENTRY(  M,  NA,  NA,   M,   M,   M,   M,   M),
/*  X86_MN_CMPXCHG16B */ EFLAGS_ENTRY( NA,  NA,  NA,  NA,   M,  NA,  NA,  NA),
/*     X86_MN_VMPTRLD */ EFLAGS_ENTRY(  M,  NA,  NA,   M,   M,   M,   M,   M),
/*     X86_MN_VMCLEAR */ EFLAGS_ENTRY(  M,  NA,  NA,   M,   M,   M,   M,   M),
/*       X86_MN_VMXON */ EFLAGS_ENTRY(  M,  NA,  NA,   M,   M,   M,   M,   M),
/*     X86_MN_VMPTRST */ EFLAGS_ENTRY(  M,  NA,  NA,   M,   M,   M,   M,   M),
/*        X86_MN_POPF */ EFLAGS_ENTRY(  M,   M,   M,   M,   M,   M,   M,   M)
};

x86_eflags_t
x86_eflags_get(uint16_t mnem)
{
    int i;

    /* default return */
    x86_eflags_t default_result = 
    {
        X86_FLAG_NA, X86_FLAG_NA, X86_FLAG_NA, X86_FLAG_NA, 
        X86_FLAG_NA, X86_FLAG_NA, X86_FLAG_NA, X86_FLAG_NA
    };

    switch(mnem)
    {
        case         X86_MN_ADD: i = 0; break; 
        case          X86_MN_OR: i = 1; break; 
        case         X86_MN_ADC: i = 2; break; 
        case         X86_MN_SBB: i = 3; break; 
        case         X86_MN_AND: i = 4; break; 
        case         X86_MN_DAA: i = 5; break; 
        case         X86_MN_SUB: i = 6; break; 
        case         X86_MN_DAS: i = 7; break; 
        case         X86_MN_XOR: i = 8; break; 
        case         X86_MN_AAA: i = 9; break; 
        case         X86_MN_CMP: i = 10; break; 
        case         X86_MN_AAS: i = 11; break; 
        case         X86_MN_INC: i = 12; break; 
        case         X86_MN_DEC: i = 13; break; 
        case       X86_MN_BOUND: i = 14; break; 
        case        X86_MN_ARPL: i = 15; break; 
        case        X86_MN_IMUL: i = 16; break; 
        case        X86_MN_INSB: i = 17; break; 
        case        X86_MN_INSW: i = 18; break; 
        case        X86_MN_INSD: i = 19; break; 
        case       X86_MN_OUTSB: i = 20; break; 
        case       X86_MN_OUTSW: i = 21; break; 
        case       X86_MN_OUTSD: i = 22; break; 
        case          X86_MN_JO: i = 23; break; 
        case         X86_MN_JNO: i = 24; break; 
        case          X86_MN_JC: i = 25; break; 
        case         X86_MN_JNC: i = 26; break; 
        case          X86_MN_JZ: i = 27; break; 
        case         X86_MN_JNZ: i = 28; break; 
        case         X86_MN_JNA: i = 29; break; 
        case          X86_MN_JA: i = 30; break; 
        case          X86_MN_JS: i = 31; break; 
        case         X86_MN_JNS: i = 32; break; 
        case          X86_MN_JP: i = 33; break; 
        case         X86_MN_JNP: i = 34; break; 
        case          X86_MN_JL: i = 35; break; 
        case         X86_MN_JNL: i = 36; break; 
        case         X86_MN_JNG: i = 37; break; 
        case          X86_MN_JG: i = 38; break; 
        case        X86_MN_TEST: i = 39; break; 
        case        X86_MN_SAHF: i = 40; break; 
        case        X86_MN_LAHF: i = 41; break; 
        case       X86_MN_MOVSB: i = 42; break; 
        case       X86_MN_MOVSW: i = 43; break; 
        case       X86_MN_MOVSD: i = 44; break; 
        case       X86_MN_MOVSQ: i = 45; break; 
        case       X86_MN_CMPSB: i = 46; break; 
        case       X86_MN_CMPSW: i = 47; break; 
        case       X86_MN_CMPSD: i = 48; break; 
        case       X86_MN_CMPSQ: i = 49; break; 
        case       X86_MN_STOSB: i = 50; break; 
        case       X86_MN_STOSW: i = 51; break; 
        case       X86_MN_STOSD: i = 52; break; 
        case       X86_MN_STOSQ: i = 53; break; 
        case       X86_MN_LODSB: i = 54; break; 
        case       X86_MN_LODSW: i = 55; break; 
        case       X86_MN_LODSD: i = 56; break; 
        case       X86_MN_LODSQ: i = 57; break; 
        case       X86_MN_SCASB: i = 58; break; 
        case       X86_MN_SCASW: i = 59; break; 
        case       X86_MN_SCASD: i = 60; break; 
        case       X86_MN_SCASQ: i = 61; break; 
        case         X86_MN_ROL: i = 62; break; 
        case         X86_MN_ROR: i = 63; break; 
        case         X86_MN_RCL: i = 64; break; 
        case         X86_MN_RCR: i = 65; break; 
        case         X86_MN_SAL: i = 66; break; 
        case         X86_MN_SHR: i = 67; break; 
        case         X86_MN_SHL: i = 68; break; 
        case         X86_MN_SAR: i = 69; break; 
        case         X86_MN_INT: i = 70; break; 
        case        X86_MN_INTO: i = 71; break; 
        case         X86_MN_AAM: i = 72; break; 
        case         X86_MN_AMX: i = 73; break; 
        case         X86_MN_AAD: i = 74; break; 
        case         X86_MN_ADX: i = 75; break; 
        case        X86_MN_SALC: i = 76; break; 
        case      X86_MN_FCMOVB: i = 77; break; 
        case      X86_MN_FCMOVE: i = 78; break; 
        case     X86_MN_FCMOVBE: i = 79; break; 
        case      X86_MN_FCMOVU: i = 80; break; 
        case     X86_MN_FCMOVNB: i = 81; break; 
        case     X86_MN_FCMOVNE: i = 82; break; 
        case    X86_MN_FCMOVNBE: i = 83; break; 
        case     X86_MN_FCMOVNU: i = 84; break; 
        case      X86_MN_FUCOMI: i = 85; break; 
        case       X86_MN_FCOMI: i = 86; break; 
        case     X86_MN_FUCOMIP: i = 87; break; 
        case      X86_MN_FCOMIP: i = 88; break; 
        case      X86_MN_LOOPNZ: i = 89; break; 
        case        X86_MN_LOOP: i = 90; break; 
        case       X86_MN_ICEBP: i = 91; break; 
        case         X86_MN_CMC: i = 92; break; 
        case         X86_MN_NEG: i = 93; break; 
        case         X86_MN_MUL: i = 94; break; 
        case         X86_MN_DIV: i = 95; break; 
        case        X86_MN_IDIV: i = 96; break; 
        case         X86_MN_CLC: i = 97; break; 
        case         X86_MN_STC: i = 98; break; 
        case         X86_MN_CLI: i = 99; break; 
        case         X86_MN_STI: i = 100; break; 
        case         X86_MN_CLD: i = 101; break; 
        case         X86_MN_STD: i = 102; break; 
        case        X86_MN_VERR: i = 103; break; 
        case        X86_MN_VERW: i = 104; break; 
        case      X86_MN_VMCALL: i = 105; break; 
        case    X86_MN_VMLAUNCH: i = 106; break; 
        case    X86_MN_VMRESUME: i = 107; break; 
        case      X86_MN_VMXOFF: i = 108; break; 
        case         X86_MN_LAR: i = 109; break; 
        case         X86_MN_LSL: i = 110; break; 
        case     X86_MN_UCOMISS: i = 112; break; 
        case     X86_MN_UCOMISD: i = 113; break; 
        case      X86_MN_COMISS: i = 114; break; 
        case      X86_MN_COMISD: i = 115; break; 
        case    X86_MN_SYSENTER: i = 116; break; 
        case       X86_MN_PTEST: i = 117; break; 
        case      X86_MN_INVEPT: i = 118; break; 
        case     X86_MN_INVVPID: i = 119; break; 
        case   X86_MN_PCMPESTRM: i = 120; break; 
        case   X86_MN_PCMPESTRI: i = 121; break; 
        case   X86_MN_PCMPISTRM: i = 122; break; 
        case   X86_MN_PCMPISTRI: i = 123; break; 
        case       X86_MN_CMOVO: i = 124; break; 
        case      X86_MN_CMOVNO: i = 125; break; 
        case       X86_MN_CMOVC: i = 126; break; 
        case      X86_MN_CMOVNC: i = 127; break; 
        case       X86_MN_CMOVZ: i = 128; break; 
        case      X86_MN_CMOVNZ: i = 129; break; 
        case      X86_MN_CMOVNA: i = 130; break; 
        case       X86_MN_CMOVA: i = 131; break; 
        case       X86_MN_CMOVS: i = 132; break; 
        case      X86_MN_CMOVNS: i = 133; break; 
        case       X86_MN_CMOVP: i = 134; break; 
        case      X86_MN_CMOVPO: i = 135; break; 
        case       X86_MN_CMOVL: i = 136; break; 
        case      X86_MN_CMOVNL: i = 137; break; 
        case      X86_MN_CMOVNG: i = 138; break; 
        case       X86_MN_CMOVG: i = 139; break; 
        case      X86_MN_VMREAD: i = 140; break; 
        case     X86_MN_VMWRITE: i = 141; break; 
        case        X86_MN_SETO: i = 142; break; 
        case       X86_MN_SETNO: i = 143; break; 
        case        X86_MN_SETC: i = 144; break; 
        case       X86_MN_SETNC: i = 145; break; 
        case        X86_MN_SETZ: i = 146; break; 
        case       X86_MN_SETNZ: i = 147; break; 
        case       X86_MN_SETNA: i = 148; break; 
        case        X86_MN_SETA: i = 149; break; 
        case        X86_MN_SETS: i = 150; break; 
        case       X86_MN_SETNS: i = 151; break; 
        case        X86_MN_SETP: i = 152; break; 
        case       X86_MN_SETPO: i = 153; break; 
        case        X86_MN_SETL: i = 154; break; 
        case       X86_MN_SETNL: i = 155; break; 
        case       X86_MN_SETNG: i = 156; break; 
        case        X86_MN_SETG: i = 157; break; 
        case          X86_MN_BT: i = 158; break; 
        case        X86_MN_SHLD: i = 159; break; 
        case         X86_MN_BTS: i = 160; break; 
        case        X86_MN_SHRD: i = 161; break; 
        case     X86_MN_CMPXCHG: i = 162; break; 
        case         X86_MN_BTR: i = 163; break; 
        case      X86_MN_POPCNT: i = 164; break; 
        case         X86_MN_BTC: i = 165; break; 
        case         X86_MN_BSF: i = 166; break; 
        case         X86_MN_BSR: i = 167; break; 
        case        X86_MN_XADD: i = 168; break; 
        case   X86_MN_CMPXCHG8B: i = 169; break; 
        case  X86_MN_CMPXCHG16B: i = 170; break; 
        case     X86_MN_VMPTRLD: i = 171; break; 
        case     X86_MN_VMCLEAR: i = 172; break; 
        case       X86_MN_VMXON: i = 173; break; 
        case     X86_MN_VMPTRST: i = 174; break;

        case        X86_MN_POPF: 
        case       X86_MN_POPFD:
        case       X86_MN_POPFQ: i = 175; break;

        default: 
            i = -1;
            break;
    }

    /* check if the instruction has an eflags entry */
    if(i < 0)
    {
        return default_result;
    }
    else
    {
        return x86_eflags_table[i];
    }
}

#endif // DASM_IMPLEMENTATION
