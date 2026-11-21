/* Definitions of target machine for GNU compiler.
   Texas Instruments TMS9900
   Copyright (C) 2009 Eric Welser
   Copyright (C) 2023 Mark Burkley

This file is part of GCC.

GCC is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 3, or (at your option)
any later version.

GCC is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with GCC; see the file COPYING3.  If not see
<http://www.gnu.org/licenses/>.
*/

/*****************************************************************************
**
** Controlling the Compilation Driver, `gcc'
**
*****************************************************************************/

#undef ENDFILE_SPEC

/* Define this for front-ended changes */
#define TMS9900

/* Options to pass to the assembler */
#ifndef ASM_SPEC
#define ASM_SPEC ""
#endif

/* Options for the linker. 
   We need to tell the linker the target elf format.
   This can be overridden by -Wl option of gcc.  */
#ifndef LINK_SPEC
#define LINK_SPEC "-m tms9900"
#endif

/* More linker options, these are used at the beginning of the command */
#undef STARTFILE_SPEC
/*#define STARTFILE_SPEC "crt1%O%s"*/
#define STARTFILE_SPEC ""

/* More linker options, used at the end of the command string */
#ifndef LIB_SPEC
#define LIB_SPEC       ""
#endif

/* Options to pass to CC1 and other language front ends */
#ifndef CC1_SPEC
#define CC1_SPEC       ""
#endif

/* Options to pass to the C Preprocessor */
#ifndef CPP_SPEC
#define CPP_SPEC ""
#endif

/* Names to predefine in the preprocessor for this target machine.  */
#define TARGET_CPU_CPP_BUILTINS()		\
  do						\
    {						\
      builtin_define ("__tms9900__");		\
      builtin_define_std ("cpu=tms9900");	\
    }						\
  while (0)

/* Nonzero if ELF.  */
#define TARGET_ELF 1

/* As an embedded target, we have no libc.
 * MGB removed.  With gcc-13 the compiler fails to build with unknown functons
 * from gcov-io.h that are prototyped only if this flag is not set*/
// #ifndef inhibit_libc
// #  define inhibit_libc
// #endif

/* Default target_flags if no switches specified.  */

#ifndef TARGET_DEFAULT
#define TARGET_DEFAULT (MASK_UNIX_ASM)
#endif

#if 0
/* Forward type declaration for prototypes definitions.
   rtx_ptr is equivalent to rtx. Can't use the same name.  */
struct rtx_def;
typedef struct rtx_def *rtx_ptr;

union tree_node;
typedef union tree_node *tree_ptr;

/* We can't declare enum machine_mode forward nor include 'machmode.h' here.
   Prototypes defined here will use an int instead. It's better than no
   prototype at all.  */
typedef int enum_machine_mode;
#endif

/*****************************************************************************
**
** Run-time Target Specification
**
*****************************************************************************/

/* Run-time compilation parameters selecting different hardware subsets.  */

extern short *reg_renumber;	/* def in local_alloc.c */

#define TARGET_OP_TIME		(optimize && optimize_size == 0)
#define TARGET_RELAX            (TARGET_NO_DIRECT_MODE)

/* Default target_flags if no switches specified.  */
#ifndef TARGET_DEFAULT
# define TARGET_DEFAULT		0
#endif

/* Define this macro as a C expression for the initializer of an
   array of string to tell the driver program which options are
   defaults for this target and thus do not need to be handled
   specially when using `MULTILIB_OPTIONS'.  */
#ifndef MULTILIB_DEFAULTS
# define MULTILIB_DEFAULTS { "tms9900" }
#endif

/* Print subsidiary information on the compiler version in use.  */
// #define TARGET_VERSION	fprintf (stderr, " (TMS9900)")

/* Target machine storage layout */

/* Define this as 1 if most significant byte of a word is the lowest numbered.  */
#define BYTES_BIG_ENDIAN 	1
/* target machine storage layout */

/* Define this if most significant bit is lowest numbered
   in instructions that operate on numbered bit-fields.  */
#define BITS_BIG_ENDIAN 1

/* Define this if most significant word of a multiword number is first.  */
#define WORDS_BIG_ENDIAN 1

#define FLOAT_WORDS_BIG_ENDIAN 1

/* Width of a word, in units (bytes). 

   UNITS OR BYTES - seems like units */
#define UNITS_PER_WORD 2


/* Definition of size_t.  This is really an unsigned short as the
   TMS9900 only handles a 64K address space.  */
#define SIZE_TYPE               "short unsigned int"

/* A C expression for a string describing the name of the data type
   to use for the result of subtracting two pointers.  The typedef
   name `ptrdiff_t' is defined using the contents of the string.
   The TMS9900 only has a 64K address space.  */
#define PTRDIFF_TYPE            "short int"

/* Allocation boundary (bits) for storing pointers in memory.  */
#define POINTER_BOUNDARY	16

/* Normal alignment required for function parameters on the stack, in bits.
   This can't be less than BITS_PER_WORD */
#define PARM_BOUNDARY		(BITS_PER_WORD)

/* Boundary (bits) on which stack pointer should be aligned.  */
#define STACK_BOUNDARY		16

/* Allocation boundary (bits) for the code of a function.  */
#define FUNCTION_BOUNDARY	16

/* Biggest alignment which, if violated, may cause a fault */
#define BIGGEST_ALIGNMENT	16

/* Alignment of field after `int : 0' in a structure.  */
#define EMPTY_FIELD_BOUNDARY	16

/* Every structure's size must be a multiple of this.  */
#define STRUCTURE_SIZE_BOUNDARY 16

/* Define this as 1 if instructions will fail to work if given data not
   on the nominal alignment.  If instructions will merely go slower
   in that case, do not define this macro.  */
#define STRICT_ALIGNMENT	1

/* An integer expression for the size in bits of the largest integer
   machine mode that should actually be used.  All integer machine modes of
   this size or smaller can be used for structures and unions with the
   appropriate sizes.  */
#define MAX_FIXED_MODE_SIZE	64

/* target machine storage layout */

/* Size (bits) of the type "int" on target machine */
#define INT_TYPE_SIZE           16

/* Size (bits) of the type "short" on target machine */
#define SHORT_TYPE_SIZE		16

/* Size (bits) of the type "long" on target machine */
#define LONG_TYPE_SIZE		32

/* Size (bits) of the type "long long" on target machine */
#define LONG_LONG_TYPE_SIZE     64

/* A C expression for the size in bits of the type `float' on the
   target machine. If you don't define this, the default is one word.
   Don't use default: a word is only 16.  */
#define FLOAT_TYPE_SIZE         32

/* A C expression for the size in bits of the type double on the target
   machine. If you don't define this, the default is two words.
   Be IEEE compliant.  */
#define DOUBLE_TYPE_SIZE        64

#define LONG_DOUBLE_TYPE_SIZE   64

/* Define this as 1 if `char' should by default be signed; else as 0.  */
#define DEFAULT_SIGNED_CHAR	1

/* A C expression for a string describing the name of the data type
   to use for wide characters.  The typedef name `wchar_t' is defined
   using the contents of the string.
   
   Define these to avoid dependence on meaning of `int'.
   Note that WCHAR_TYPE_SIZE is used in cexp.y,
   where TARGET_SHORT is not available.  */
#define WCHAR_TYPE              "short int"
#define WCHAR_TYPE_SIZE         16

/* Standard register usage.  */

#define HARD_REG_SIZE           (2)

#define REGS_PER_WORD (UNITS_PER_WORD / HARD_REG_SIZE)

#if 0
// MGB Moved to tms9900.md
/* Assign names to real TMS9900 registers.  ST and WP registers have been added
 * to this list for completeness */
#define HARD_R0_REGNUM		0
#define HARD_R1_REGNUM		1
#define HARD_R2_REGNUM		2
#define HARD_R3_REGNUM		3
#define HARD_R4_REGNUM		4
#define HARD_R5_REGNUM		5
#define HARD_R6_REGNUM		6
#define HARD_R7_REGNUM		7
#define HARD_R8_REGNUM		8
#define HARD_R9_REGNUM		9
#define HARD_R10_REGNUM		10
#define HARD_R11_REGNUM		11
#define HARD_R12_REGNUM		12
#define HARD_R13_REGNUM		13
#define HARD_R14_REGNUM		14
#define HARD_R15_REGNUM		15
#define HARD_ST_REGNUM		16  // Status register
#define HARD_WP_REGNUM		17  // Workspace pointer

/* Shift count register */
#define HARD_SC_REGNUM		HARD_R0_REGNUM
/* Arg pointer */
#define HARD_AP_REGNUM		HARD_R8_REGNUM
/* Base pointer */
#define HARD_BP_REGNUM		HARD_R9_REGNUM
/* Stack pointer */
#define HARD_SP_REGNUM		HARD_R10_REGNUM
/* Old PC after BL instruction */
#define HARD_LR_REGNUM		HARD_R11_REGNUM
/* CRU base address */
#define HARD_CB_REGNUM		HARD_R12_REGNUM
/* Old workspace after BLWP instruction */
#define HARD_LW_REGNUM		HARD_R13_REGNUM
/* Old PC after BLWP instruction */
#define HARD_LP_REGNUM		HARD_R14_REGNUM
/* Old status register after BLWP instruction */
#define HARD_LS_REGNUM		HARD_R15_REGNUM

/* Number of actual hardware registers. The hardware registers are assigned
   numbers for the compiler from 0 to just below FIRST_PSEUDO_REGISTER. 
   All registers that the compiler knows about must be given numbers, even
   those that are not normally considered general registers.  */
#define FIRST_PSEUDO_REGISTER	(19)

/* Register to use for pushing function arguments.  */
#define STACK_POINTER_REGNUM		HARD_SP_REGNUM

/* Base register for access to local variables of the function.  */
#define FRAME_POINTER_REGNUM		HARD_R9_REGNUM

/* Base register for access to arguments of the function.  */
#define ARG_POINTER_REGNUM		HARD_R8_REGNUM

/* Register in which static-chain is passed to a function.  */
#define STATIC_CHAIN_REGNUM	        HARD_R7_REGNUM


#endif

/* How to refer to registers in assembler output.  This sequence is indexed
   by compiler's hard-register-number (see above). */
#define REGISTER_NAMES \
{ "r0",  "r1", "r2",  "r3",  "r4",  "r5",  "r6",  "r7",  \
  "r8",  "r9", "r10", "r11", "r12", "r13", "r14", "r15", \
  "st",  "wp", "pc"  }

/* 1 for registers that have pervasive standard uses and are not available
   for the register allocator.

*/
#define FIXED_REGISTERS \
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 1, 1, 1}
/* SC 1  2  3  4  5  6  7  AP BP SP LR CB LW LP LS ST WP PC*/

/* 0 for registers which must be preserved across function call boundaries */
/* MGB TODO seems excessive to always preserve R13,R14,R15 as these will only
 * have values to be saved if we were invokved by a BLWP which is never emitted
 * by this backend.  If someone is writing an ISR / DSR which is invoked by a
 * BLWP then we can ask them to save R13/R14/R15 themselves.
 */
#define CALL_USED_REGISTERS \
  {1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 0, 0, 1, 1, 1}
/* SC 1  2  3  4  5  6  7  AP BP SP LR CB LW LP LS ST WP PC*/

/* Define this macro to change register usage conditional on target flags. */
// #define CONDITIONAL_REGISTER_USAGE 

/* List the order in which to allocate registers.  Each register must be
   listed once, even those in FIXED_REGISTERS.  */
#define REG_ALLOC_ORDER	\
   {R1_REGNUM,  R2_REGNUM,  R3_REGNUM,  R4_REGNUM,\
    R5_REGNUM,  R6_REGNUM,  R7_REGNUM,  R8_REGNUM,\
    R12_REGNUM, R13_REGNUM, R14_REGNUM, R15_REGNUM,\
    R8_REGNUM,  R9_REGNUM,  R11_REGNUM, R10_REGNUM,\
    ST_REGNUM,  WP_REGNUM,  PC_REGNUM }

/* A C expression for the number of consecutive hard registers,
   starting at register number REGNO, required to hold a value of
   mode MODE.  */
#if 0
/* MGB removed for gcc-13 */
#define HARD_REGNO_NREGS(REGNO, MODE) \
   ((GET_MODE_SIZE (MODE) + HARD_REG_SIZE - 1) / HARD_REG_SIZE)
#endif

#if 0
/* Value is 1 if hard register REGNO (or starting with REGNO) can hold a value of machine-mode MODE
 *
 *  MGB TODO could include 64-bit as well
 *  removed for gcc-13 */
#define HARD_REGNO_MODE_OK(REGNO, MODE) \
   (!(MODE == SImode && REGNO==HARD_R15_REGNUM))
#endif

/* A C expression that is nonzero if hard register number REGNO2 can be
   considered for use as a rename register for REGNO1 */
#define HARD_REGNO_RENAME_OK(REGNO1,REGNO2) 1

/* Value is 1 if it is a good idea to tie two pseudo registers when one has
   mode MODE1 and one has mode MODE2.  If HARD_REGNO_MODE_OK could produce
   different values for MODE1 and MODE2, for any hard reg, then this must be
   0 for correct output. */
/* MGB removed for gcc-13 */
// #define MODES_TIEABLE_P(MODE1, MODE2) 0

/* Define the classes of registers for register constraints in the
   machine description.  Also define ranges of constants.

   One of the classes must always be named ALL_REGS and include all hard regs.
   If there is more than one class, another class must be named NO_REGS
   and contain no registers.

   The classes must be numbered in nondecreasing order; that is,
   a larger-numbered class must never be contained completely
   in a smaller-numbered class.

   For any two classes, it is very desirable that there be another
   class that represents their union.

   MGB TODO remove CRU.  No CRU code is emitted by this back end
*/
enum reg_class
{
  NO_REGS,
  SHIFT_REGS,     /* Register used for variable shift (SC) */
  CRU_REGS,       /* Register used for CRU access (CB) */
  ST_REGS,        /* Status register for CC */
  WP_REGS,        /* Workspace pointer */
  PC_REGS,        /* PC */
  BASE_REGS,      /* Registers which may be used as a memory base */
  ALL_REGS,       /* All registers, including fakes */
  LIM_REG_CLASSES
};

/* The name GENERAL_REGS must be the name of a class (or an alias for
   another name such as ALL_REGS).  This is the class of registers
   that is allowed by "g" or "r" in a register constraint.
   Also, registers outside this class are allocated only when
   instructions express preferences for them. */
#define GENERAL_REGS	ALL_REGS

/* The number of distict register classes */
#define N_REG_CLASSES	(int) LIM_REG_CLASSES

/* Give names of register classes as strings for dump file.  */
#define REG_CLASS_NAMES \
{ "NO_REGS",       \
  "SHIFT_REGS",    \
  "CRU_REGS",      \
  "ST_REGS",       \
  "WP_REGS",       \
  "PC_REGS",       \
  "BASE_REGS",     \
  "ALL_REGS" }

/* An initializer containing the contents of the register classes,
   as integers which are bit masks.  The Nth integer specifies the
   contents of class N.  The way the integer MASK is interpreted is
   that register R is in the class if `MASK & (1 << R)' is 1.  */

/*--------------------------------------------------------------
   SC      0x00000001
   R1      0x00000002
   R2      0x00000004
   R3      0x00000008
   R4      0x00000010
   R5      0x00000020
   R6      0x00000040
   R7      0x00000080
   AP      0x00000100
   BP      0x00000200
   SP      0x00000400
   LR      0x00000800
   CB      0x00001000
   LW      0x00002000
   LP      0x00004000
   LS      0x00008000
   ST      0x00010000
   WP      0x00020000
   PC      0x00040000
--------------------------------------------------------------*/

#define REG_CLASS_CONTENTS \
/* NO_REGS       */  {{ 0x00000000 }, \
/* SHIFT_REGS    */   { 0x00000001 }, /* SC */ \
/* CRU_REGS      */   { 0x00001000 }, /* CB */ \
/* ST_REGS       */   { 0x00010000 }, /* ST */ \
/* WP_REGS       */   { 0x00020000 }, /* WP */ \
/* PC_REGS       */   { 0x00040000 }, /* PC */ \
/* BASE_REGS     */   { 0x0000FFFE }, \
/* ALL_REGS      */   { 0x0007FFFF }}

/* Set up a C expression whose value is a register class containing hard
   register REGNO */
#define REGNO_REG_CLASS(REGNO) \
   (REGNO == SC_REGNUM  ? SHIFT_REGS : \
    REGNO == CB_REGNUM  ? CRU_REGS   : \
    REGNO == ST_REGNUM  ? ST_REGS   : \
    REGNO == WP_REGNUM  ? WP_REGS   : \
    REGNO == PC_REGNUM  ? PC_REGS   : \
    REGNO <= R15_REGNUM ? ALL_REGS   : \
    NO_REGS)

#if 0
/* MGB moved to constraints.md for gcc-13 */
/* Get register class from a letter in the machine description. */
#define REG_CLASS_FROM_LETTER(C) \
   ((C) == 'S' ? SHIFT_REGS : \
    (C) == 'C' ? CRU_REGS   : \
    (C) == 'T' ? ALL_REGS   : \
    NO_REGS)
#endif

/* A C expression that places additional restrictions of the register
   class to use when it is necessary to copy value X into a register
   in class CLASS. Some values may require the use of a more restrictive
   class.*/
#define PREFERRED_RELOAD_CLASS(X,CLASS)	CLASS

/* Return the maximum number of consecutive registers needed to represent
   mode MODE in a register of class CLASS.  */
#define CLASS_MAX_NREGS(CLASS, MODE) \
   ((GET_MODE_SIZE (MODE) + UNITS_PER_WORD - 1) / UNITS_PER_WORD)

#if 0
/* MGB removed for gcc-13 */
/* The letters I, J, K, L and M in a register constraint string
   can be used to stand for particular ranges of immediate operands.
   This macro defines what the ranges are.
   C is the letter, and VALUE is a constant value.
   Return 1 if VALUE is in the range specified by C.

   'I' is for 32-bit value xxxx0000
   'J' is for 32-bit value 0000xxxx
   'K' is for 32-bit value xxxxxxxx
   'L' is for 1
   'M' is for -1
   'N' is for 0
   'O' is for 2 or -2
   'P' is for 16-bit value 00ff
*/
#define CONST_OK_FOR_LETTER_P(VALUE, C) \
  ((C) == 'L' ? ((VALUE) == 2 || (VALUE) == -2): \
   (C) == 'M' ? ((VALUE) == -1): \
   (C) == 'N' ? ((VALUE) == 1): \
   (C) == 'O' ? ((VALUE) == 0): \
   (C) == 'P' ? ((VALUE) == 0x00ff): \
   (C) == 'I' ? ((VALUE) & 0xffff0000) == 0: \
   (C) == 'J' ? ((VALUE) & 0x0000ffff) == 0: \
   (C) == 'K' ? (((VALUE) & 0xffff0000) != 0 && \
		 ((VALUE) & 0x0000ffff) != 0): \
   0)
#endif

#if 0
/* MGB removed for gcc-13 */
/* Similar, but for floating constants, and defining letters G and H.

   `G' is for 0.0.  */
#define CONST_DOUBLE_OK_FOR_LETTER_P(VALUE, C) \
  ((C) == 'G' ? (GET_MODE_CLASS (GET_MODE (VALUE)) == MODE_FLOAT \
		 && VALUE == CONST0_RTX (GET_MODE (VALUE))) : 0) 
#endif

#if 0
/* MGB removed for gcc-13 */
/* Letters in the range `Q' through `U' may be defined in a
   machine-dependent fashion to stand for arbitrary operand types. 
   The machine description macro `EXTRA_CONSTRAINT' is passed the
   operand as its first argument and the constraint letter as its
   second operand.

   This macro returns 1 if the provided CODE matches the provided OP

   `Q'	is for memory references that require an extra word after the opcode.
   `R'	is for memory references which are encoded within the opcode.  */
#define EXTRA_CONSTRAINT(OP,CODE)					\
  ((GET_CODE (OP) != MEM) ? 0						\
   : !legitimate_address_p (GET_MODE (OP), XEXP (OP, 0)) ? 0		\
   : ((CODE) == 'Q') ? (tms9900_address_type (OP, GET_MODE (OP)) == 3)  \
   : ((CODE) == 'R') ? (tms9900_address_type (OP, GET_MODE (OP)) == 1)	\
   : 0)
#endif

/* Stack layout; function entry, exit and calling.  */

/* Define this if pushing a word on the stack
   makes the stack pointer a smaller address.  */
#define STACK_GROWS_DOWNWARD 1

/* Define this to nonzero if the nominal address of the stack frame
   is at the high-address end of the local variables;
   that is, each additional local variable allocated
   goes at a more negative offset in the frame.

   Define to 0 for TMS9900, the frame pointer is the bottom
   of local variables.  */
#define FRAME_GROWS_DOWNWARD 0

/* Offset within stack frame to start allocating local variables at.
   If FRAME_GROWS_DOWNWARD, this is the offset to the END of the
   first local allocated.  Otherwise, it is the offset to the BEGINNING
   of the first local allocated.  */
/* MGB removed for gcc-13 */
// #define STARTING_FRAME_OFFSET 0

/* Offset of first parameter from the argument pointer register value.  */
#define FIRST_PARM_OFFSET(FNDECL)	0

/* After the prologue, RA is at 0(AP) in the current frame.  */
#define RETURN_ADDR_RTX(COUNT, FRAME)					\
  ((COUNT) == 0								\
   ? gen_rtx_MEM (Pmode, arg_pointer_rtx)                               \
   : 0)

/* Before the prologue, the top of the frame is at 2(sp).  */
#define INCOMING_FRAME_SP_OFFSET        0

/* Definitions for register eliminations.

   This is an array of structures.  Each structure initializes one pair
   of eliminable registers.  The "from" register number is given first,
   followed by "to".  Eliminations of the same "from" register are listed
   in order of preference.

   The pseudo arg pointer and pseudo frame pointer registers can always
   be eliminated; they are replaced with either the stack or the real
   frame pointer.  */
#define ELIMINABLE_REGS \
  {{ARG_POINTER_REGNUM,   STACK_POINTER_REGNUM},\
   {ARG_POINTER_REGNUM,   FRAME_POINTER_REGNUM},\
   {FRAME_POINTER_REGNUM, STACK_POINTER_REGNUM}}

/* Value should be nonzero if functions must have frame pointers.
   Zero means the frame pointer need not be set up (and parms may be
   accessed via the stack pointer) in functions that seem suitable.
   This is computed in `reload', in reload1.c.  */
/* MGB removed for gcc-13 */
// #define FRAME_POINTER_REQUIRED	0

/* MGB removed for gcc-13 */
// #define CAN_DEBUG_WITHOUT_FP 1

/* Given FROM and TO register numbers, say whether this elimination is allowed.
   Frame pointer elimination is automatically handled.

   All other eliminations are valid.  */
/* MGB removed for gcc-13 */
// #define CAN_ELIMINATE(FROM, TO)		1

/* Define the offset between two registers, one to be eliminated, and the other
   its replacement, at the start of a routine.  */
#define INITIAL_ELIMINATION_OFFSET(FROM, TO, OFFSET) \
    { OFFSET = tms9900_initial_elimination_offset (FROM, TO); }

/* Passing Function Arguments on the Stack.  */

/* A C expression.  If nonzero, push insns will be used to pass
   outgoing arguments.  If the target machine does not have a push
   instruction, set it to zero.  That directs GCC to use an alternate
   strategy: to allocate the entire argument block and then store the
   arguments into it.  When `PUSH_ARGS' is nonzero, `PUSH_ROUNDING'
   must be defined too. */
#define PUSH_ARGS 0

/* Value is 1 if returning from a function call automatically pops the
   arguments described by the number-of-args field in the call. FUNTYPE is
   the data type of the function (as a tree), or for a library call it is
   an identifier node for the subroutine name. */
/* MGB removed for gcc-13 */
// #define RETURN_POPS_ARGS(FUNDECL,FUNTYPE,SIZE)	0

/* Passing Arguments in Registers.  */


/* The number of argument registers we can use (R1..R6) */
#define TMS9900_ARG_REGS (R7_REGNUM - R1_REGNUM)

/* Define a data type for recording info about an argument list
   during the scan of that argument list.  This data type should
   hold all necessary information about the function itself
   and about the args processed so far, enough to enable macros
   such as FUNCTION_ARG to determine where the next arg should go.  */
typedef struct tms9900_args
{
  int nregs;        /* Number of registers used so far */
  int named_count;  /* Number of named arguments (for varargs) */  
} CUMULATIVE_ARGS;

#undef PAD_VARARGS_DOWN
#define PAD_VARARGS_DOWN \
  (targetm.calls.function_arg_padding (TYPE_MODE (type), type) == PAD_DOWNWARD)

/* Initialize a variable CUM of type CUMULATIVE_ARGS for a call to a
   function whose data type is FNTYPE. For a library call, FNTYPE is 0.  */
#define INIT_CUMULATIVE_ARGS(CUM, FNTYPE, LIBNAME, FNDECL, N_NAMED_ARGS) \
   (tms9900_init_cumulative_args (&CUM, FNTYPE, LIBNAME, FNDECL))

/* This target hook should return `true' if an argument at the
   position indicated by CUM should be passed by reference.  This
   predicate is queried after target independent reasons for being
   passed by reference, such as `TREE_ADDRESSABLE (type)'.

   If the hook returns true, a copy of that argument is made in
   memory and a pointer to the argument is passed instead of the
   argument itself.  The pointer is passed in whatever way is
   appropriate for passing a pointer to that type. */
/*
#define TARGET_PASS_BY_REFERENCE(CUM,MODE,TYPE,NAMED) \
 (TYPE && TREE_CODE (TYPE_SIZE (TYPE)) != INTEGER_CST)
*/

/* Define the profitability of saving registers around calls.

   Disable this because the saving instructions generated by
   caller-save need a reload and the way it is implemented,
   it forbids all spill registers at that point.  Enabling
   caller saving results in spill failure.  */
/* MGB removed for gcc-13 */
// #define CALLER_SAVE_PROFITABLE(REFS,CALLS) 0

/* 1 if N is a possible register number for function argument passing. */
#define FUNCTION_ARG_REGNO_P(N)	\
     (((N) >= R1_REGNUM) && ((N) <= R6_REGNUM))

/* 8- and 16-bit values are returned in R1, 32-bit values are
   passed in R1+R2, The high word is in R1. */
#define FUNCTION_VALUE(VALTYPE, FUNC) tms9900_function_value(VALTYPE)

/* 8- and 16-bit values are returned in R1, 32-bit values are
   passed in R1+R2, The high word is in R1. */
#define LIBCALL_VALUE(MODE)						\
     gen_rtx_REG (MODE, R1_REGNUM)

/* 1 if N is a possible register number for a function value.  */
#define FUNCTION_VALUE_REGNO_P(N) \
     ((N) == R1_REGNUM)

/* EXIT_IGNORE_STACK should be nonzero if, when returning from a function,
   the stack pointer does not matter.  The value is tested only in functions
   that have frame pointers. No definition is equivalent to always zero.  */
#define EXIT_IGNORE_STACK	0

/* Generating Code for Profiling.  */

/* Output assembler code to FILE to increment profiler label # LABELNO
   for profiling a function entry.  */
#define TMS9900_FUNCTION_PROFILER_NAME "mcount"
#define FUNCTION_PROFILER(FILE, LABELNO) \
     fprintf (FILE, "\tldy\tLP%d\n\tjsr mcount\n", (LABELNO))

/* Let's see whether this works as trampoline:
     LI Rn, @STATIC	0x0200	0x0000 <- STATIC; Y = STATIC_CHAIN_REGNUM
     B  FUNCTION	0x0820  0x0000 <- FUNCTION
*/
/* MGB removed for gcc-13 */
#if 0
#define TRAMPOLINE_TEMPLATE(FILE)	\
{					\
  assemble_aligned_integer (2, GEN_INT (0x0200+STATIC_CHAIN_REGNUM));	\
  assemble_aligned_integer (2, const0_rtx);				\
  assemble_aligned_integer (2, GEN_INT(0x0820));			\
  assemble_aligned_integer (2, const0_rtx);				\
}

/* Emit RTL insns to initialize the variable parts of a trampoline.
   FNADDR is an RTX for the address of the function's pure code.
   CXT is an RTX for the static chain value for the function.  */
#define INITIALIZE_TRAMPOLINE(TRAMP,FNADDR,CXT)	\
{					\
  emit_move_insn (gen_rtx_MEM (HImode, plus_constant (TRAMP, 2)), CXT); \
  emit_move_insn (gen_rtx_MEM (HImode, plus_constant (TRAMP, 6)), FNADDR); \
}
#endif

#define TRAMPOLINE_SIZE 8
#define TRAMPOLINE_ALIGNMENT 16

/* The TMS9900 can only do post increment */
#define HAVE_POST_INCREMENT  (1)
#define HAVE_PRE_INCREMENT   (0)
#define HAVE_POST_DECREMENT  (0)
#define HAVE_PRE_DECREMENT   (0)
#define HAVE_POST_MODIFY_REG (1)

/* The name of the class to which a valid base register must belong.
   A base register is one used in an address which is the register
   value plus a displacement. */
#define BASE_REG_CLASS	BASE_REGS

/* The class value for index registers. */
#define INDEX_REG_CLASS	NO_REGS

/* A C expression which is nonzero if register number NUM is suitable
   for use as a base register in operand addresses.  It may be either
   a suitable hard register or a pseudo register that has been
   allocated such a hard register. 
   Any hard register except R0 is a valid base */
#define REGNO_OK_FOR_BASE_P(NUM) \
   ((NUM) >= R1_REGNUM && (NUM) <= R15_REGNUM)

/* A C expression which is nonzero if register number NUM is suitable
   for use as an index register in operand addresses.  It may be
   either a suitable hard register or a pseudo register that has been
   allocated such a hard register. The difference between an index
   register and a base register is that the index register may be scaled. */
#define REGNO_OK_FOR_INDEX_P(NUM) 0

/* 1 if X is an rtx for a constant that is a valid address.  */
/*
#define CONSTANT_ADDRESS_P(X)	\
  (GET_CODE (X) == LABEL_REF || GET_CODE (X) == SYMBOL_REF \
          || CONST_INT_P (X) || GET_CODE (X) == CONST      \
          || GET_CODE (X) == HIGH)
*/
#define CONSTANT_ADDRESS_P(X)  CONSTANT_P(X)

/* Maximum number of registers that can appear in a valid memory address */
#define MAX_REGS_PER_ADDRESS	1

/* The behavior of several macros depend on whether or not we are in 
   strict mode. Define a constant for this */
#ifdef REG_OK_STRICT
#define IS_STRICT_P 1
#else
#define IS_STRICT_P 0
#endif

/* GO_IF_LEGITIMATE_ADDRESS recognizes an RTL expression that is a
   valid memory address for an instruction. The MODE argument is the
   machine mode for the MEM expression that wants to use this address.  */

/*--------------------------------------------------------------
   Valid addresses are either direct or indirect (MEM) versions
   of the following forms:
	constant		N
	register		X
	indexed			N,X
--------------------------------------------------------------*/

/* GO_IF_LEGITIMATE_ADDRESS recognizes an RTL expression
   that is a valid memory address for an instruction.
   The MODE argument is the machine mode for the MEM expression
   that wants to use this address. */
#define GO_IF_LEGITIMATE_ADDRESS(mode, operand, ADDR) \
  { \
  if(tms9900_go_if_legitimate_address(mode, operand, IS_STRICT_P)) \
    { \
    goto ADDR; \
    } \
  }

  // fail: ;

/* The macros REG_OK_FOR..._P assume that the arg is a REG rtx and check its
   validity for a certain class.  We have two alternate definitions for each
   of them.  The usual definition accepts all pseudo regs; the other rejects
   them unless they have been allocated suitable hard regs.  The symbol
   REG_OK_STRICT causes the latter definition to be used.
  
   Most source files want to accept pseudo regs in the hope that they will
   get allocated to the class that the insn wants them to be in. Source files
   for reload pass need to be strict. After reload, it makes no difference,
   since pseudo regs have been eliminated by then.  */
#define REG_OK_FOR_BASE_P(X) (tms9900_reg_ok_for_base(IS_STRICT_P, X)) 

/* Nonzero if X is a hard reg that can be used as an index.  */
#define REG_OK_FOR_INDEX_P(X) (0)


/* Try machine-dependent ways of modifying an illegitimate address
   to be legitimate.  If we find one, return the new, valid address.
   This macro is used in only one place: `memory_address' in explow.c.
  
   OLDX is the address as it was before break_out_memory_refs was called.
   In some cases it is useful to look at this to decide what needs to be done.
  
   MODE and WIN are passed so that this macro can use
   GO_IF_LEGITIMATE_ADDRESS.
  
   It is always safe for this macro to do nothing.
   It exists to recognize opportunities to optimize the output.  */
/* MGB removed for gcc-13 */
// #define LEGITIMIZE_ADDRESS(X,OLDX,MODE,WIN)
  
/* Go to LABEL if ADDR (a legitimate address expression)
   has an effect that depends on the machine mode it is used for.  */
/* MGB removed for gcc-13 */
// #define GO_IF_MODE_DEPENDENT_ADDRESS(ADDR,LABEL)

/* Nonzero if the constant value X is a legitimate general operand.
   It is given that X satisfies CONSTANT_P or is a CONST_DOUBLE.  */
/* MGB removed for gcc-13 */
// #define LEGITIMATE_CONSTANT_P(X)	1

/* Tell final.c how to eliminate redundant test instructions.  */
/* MGB removed for gcc-13 */
#if 0
#define NOTICE_UPDATE_CC(EXP, INSN) \
{ if (GET_CODE (EXP) == SET)					\
    {								\
      notice_update_cc_on_set(EXP, INSN);			\
    }								\
  else if (GET_CODE (EXP) == PARALLEL				\
	   && GET_CODE (XVECEXP (EXP, 0, 0)) == SET)		\
    {								\
      notice_update_cc_on_set(XVECEXP (EXP, 0, 0), INSN);	\
    }								\
  else if (GET_CODE (EXP) == CALL)				\
    { /* all bets are off */ CC_STATUS_INIT; }			\
  if (cc_status.value1 && GET_CODE (cc_status.value1) == REG	\
      && cc_status.value2					\
      && reg_overlap_mentioned_p (cc_status.value1, cc_status.value2)) \
    { 								\
      cc_status.value2 = 0;					\
    }								\
}
#endif

/*   A C expression for the cost of moving data of mode MODE from a
     register in class FROM to one in class TO.  The classes are
     expressed using the enumeration values such as `GENERAL_REGS'.  A
     value of 2 is the default; other values are interpreted relative to
     that.

     It is not required that the cost always equal 2 when FROM is the
     same as TO; on some machines it is expensive to move between
     registers if they are not general registers.

     If reload sees an insn consisting of a single `set' between two
     hard registers, and if `REGISTER_MOVE_COST' applied to their
     classes returns a value of 2, reload does not check to ensure that
     the constraints of the insn are met.  Setting a cost of other than
     2 will allow reload to verify that the constraints are met.  You
     should do this if the `movM' pattern's constraints do not allow
     such copying. */
#define REGISTER_MOVE_COST(MODE, FROM, TO) 4

/* A C expression for the cost of moving data of mode MODE between a
   register of class CLASS and memory; IN is zero if the value is to
   be written to memory, nonzero if it is to be read in.  This cost
   is relative to those in `REGISTER_MOVE_COST'.  If moving between
   registers and memory is more expensive than between two registers,
   you should define this macro to express the relative cost

   For the TMS9900, memory access is four times slower than registers */
#define MEMORY_MOVE_COST(MODE,CLASS,IN)	16

/* A C expression for the cost of a branch instruction.  A value of 1
   is the default; other values are interpreted relative to that.

   Pretend branches are cheap because GCC generates sub-optimal code
   for the default value.  */
#define BRANCH_COST(speed_p, predictable_p) 0

/* Nonzero if access to memory by bytes is slow and undesirable.  */
#define SLOW_BYTE_ACCESS 0

/* Give a comparison code (EQ, NE etc) and the first operand of a COMPARE,
   return the mode to be used for the comparison.
   MGB fixed at CCmode for now */

#define SELECT_CC_MODE(OP,X,Y) CCmode

/* Enable compare elimination pass.  */
#undef TARGET_FLAGS_REGNUM
#define TARGET_FLAGS_REGNUM ST_REGNUM

/* Specify the CC registers.  */
#undef TARGET_FIXED_CONDITION_CODE_REGS
#define TARGET_FIXED_CONDITION_CODE_REGS ST_REGNUM

/* Defining the Output Assembler Language.  */

/* A default list of other sections which we might be "in" at any given
   time.  For targets that use additional sections (e.g. .tdesc) you
   should override this definition in the target-specific file which
   includes this file.  */

/* Output before read-only data.  */
#define TEXT_SECTION_ASM_OP	("\tpseg")

/* Output before writable data.  */
#define DATA_SECTION_ASM_OP	("\tdseg")

/* Output before uninitialized data.  */
#define BSS_SECTION_ASM_OP 	("\tcseg")

/* Define this macro to be an expression with a nonzero value if jump tables
   (for tablejump insns) should be output in the text section, along with the
   assembler instructions. Otherwise, the readonly data section is used. */
#define JUMP_TABLES_IN_TEXT_SECTION 1


/* Define the pseudo-ops used to switch to the .ctors and .dtors sections.

   Same as config/elfos.h but don't mark these section SHF_WRITE since
   there is no shared library problem.  */
/*EMW - Neglect C++ for now...
#undef CTORS_SECTION_ASM_OP
#define CTORS_SECTION_ASM_OP	"\t.section\t.ctors,\"a\""

#undef DTORS_SECTION_ASM_OP
#define DTORS_SECTION_ASM_OP	"\t.section\t.dtors,\"a\""

#define TARGET_ASM_CONSTRUCTOR  tms9900_asm_out_constructor
#define TARGET_ASM_DESTRUCTOR   tms9900_asm_out_destructor
EMW*/

/* Comment character */
#define ASM_COMMENT_START	"*"

/* Output to assembler file text saying following lines
   may contain character constants, extra white space, comments, etc.  */
#define ASM_APP_ON 		"* Begin inline assembler code\n"

/* Output to assembler file text saying following lines
   no longer contain unusual constructs.  */
#define ASM_APP_OFF 		"* End of inline assembler code\n"

/* output external reference */
#undef ASM_OUTPUT_EXTERNAL
#define ASM_OUTPUT_EXTERNAL(FILE,DECL,NAME) \
  {fputs ("\n\tref\t", FILE); \
  assemble_name (FILE, NAME); \
  fputs ("\n", FILE);}

#define ASM_OUTPUT_LABEL(FILE,NAME) \
  {							\
    assemble_name ((FILE), (NAME));			\
    fputc ('\n', (FILE));				\
  }

#define ASM_OUTPUT_INTERNAL_LABEL(FILE,NAME) \
  {							\
    assemble_name ((FILE), (NAME));			\
    fputc ('\n', (FILE));				\
  }

/* Print operand X (an rtx) in assembler syntax to file FILE.
   CODE is a letter or dot (`z' in `%z0') or 0 if no letter was specified.
   For `%' followed by punctuation, CODE is the punctuation and X is null. */
#define PRINT_OPERAND(FILE, X, CODE)  		\
{ if (CODE == '#') fprintf (FILE, "#");		\
  else if (GET_CODE (X) == REG)			\
    fprintf (FILE, "%s", reg_names[REGNO (X)]);	\
  else if (GET_CODE (X) == MEM)			\
    output_address (GET_MODE(x), XEXP (X, 0));		\
  else if (GET_CODE (X) == PC)			\
    fprintf (FILE, "$");			\
  else if (GET_CODE (X) == CONST_INT)		\
    fprintf (FILE, ">%X", (unsigned short)(INTVAL(X) & 0xFFFF));	\
  else output_addr_const (FILE, X);}

/* Print a memory operand whose address is ADDR, on file FILE.  */
#define PRINT_OPERAND_ADDRESS(FILE, ADDR) \
   print_operand_address (FILE, ADDR)

/* This is how to output an insn to push/pop a register on the stack.
   It need not be very fast code.  

   Don't define because we don't know how to handle that with
   the STATIC_CHAIN_REGNUM (soft register).  Saving the static
   chain must be made inside FUNCTION_PROFILER.  */
#undef ASM_OUTPUT_REG_PUSH
#undef ASM_OUTPUT_REG_POP

/* This is how to output an element of a case-vector that is relative.  */
#define ASM_OUTPUT_ADDR_DIFF_ELT(FILE, BODY, VALUE, REL) \
  fprintf (FILE, "\t%s\tL%d-L%d\n", integer_asm_op (2, TRUE), VALUE, REL)

/* This is how to output an element of a case-vector that is absolute.  */
#define ASM_OUTPUT_ADDR_VEC_ELT(FILE, VALUE) \
  fprintf (FILE, "\t%s\tL%d\n", integer_asm_op (2, TRUE), VALUE)

/* Advance to the next word boundary */
#undef ALIGN_ASM_OP
#define ALIGN_ASM_OP "\teven\t"

/* This is how to output an assembler line that says to advance the
   location counter to a multiple of 2**LOG bytes.  */
#define ASM_OUTPUT_ALIGN(FILE,LOG)      \
  switch (LOG)                          \
    {                                   \
      case 0:                           \
        break;                          \
      case 1:                           \
        fprintf (FILE, "\teven\n");     \
        break;                          \
      default:                          \
        gcc_unreachable ();             \
    }

/* Assembler Commands for Exception Regions.  */

/* Default values provided by GCC should be ok. Assuming that DWARF-2
   frame unwind info is ok for this platform.  */
#define DWARF2_DEBUGGING_INFO 1

/* Prefer dwarf format for debugging info */
#undef PREFERRED_DEBUGGING_TYPE
#define PREFERRED_DEBUGGING_TYPE DWARF2_DEBUG

/* Use 16-bit values for dwarf address pointers */
#define DWARF2_ADDR_SIZE 2

/* Enable support for source line debugging in emitted assembly code */
#define HAVE_AS_DWARF2_DEBUG_LINE 1

/* Only emit ".file" and ".loc" directives if debugging is enabled */
#define DWARF2_ASM_LINE_DEBUG_INFO (write_symbols != NO_DEBUG)

/* This flag is true if the target supports `TARGET_ASM_NAMED_SECTION'. */
#define TARGET_HAVE_NAMED_SECTIONS 1

/* Output assembly directives to switch to section NAME.  The section
   should have attributes as specified by FLAGS, which is a bit mask
   of the `SECTION_*' flags defined in `output.h'.  If ALIGN is
   nonzero, it contains an alignment in bytes to be used for the
   section, otherwise some target default should be used.  Only
   targets that must specify an alignment within the section
   directive need pay attention to ALIGN - we will still use
   `ASM_OUTPUT_ALIGN'. */
#define TARGET_ASM_NAMED_SECTION  default_elf_asm_named_section

/* The prefix for local labels.  You should be able to define this as
   an empty string, or any arbitrary string (such as ".", ".L%", etc)
   without having to make any other changes to account for the specific
   definition.  Note it is a string literal, not interpreted by printf
   and friends.  */
/*#define LOCAL_LABEL_PREFIX "."*/

/* Directive to give a symbol global scope */
#define GLOBAL_ASM_OP   "\n\tdef\t"

/* Miscellaneous Parameters.  */

/* Specify the machine mode that this machine uses
   for the index in the tablejump instruction.  */
#define CASE_VECTOR_MODE	Pmode

/* This flag, if defined, says the same insns that convert to a signed fixnum
   also convert validly to an unsigned one.  */
/* MGB removed for gcc-13 */
// #define FIXUNS_TRUNC_LIKE_FIX_TRUNC

/* Max number of bytes we can move from memory to memory in one
   reasonably fast instruction.  */
#define MOVE_MAX 		2

/* MOVE_RATIO is the number of move instructions that is better than a
   block move.  Make this small, since the code size grows very
   large with each move.  */
#define MOVE_RATIO(speed)	3

/* Define if shifts truncate the shift count which implies one can omit
   a sign-extension or zero-extension of a shift count.  */
#define SHIFT_COUNT_TRUNCATED	1

/* Value is 1 if truncating an integer of INPREC bits to OUTPREC bits
   is done just by pretending it is already truncated.  */
/* MGB removed for gcc-13 */
// #define TRULY_NOOP_TRUNCATION(OUTPREC, INPREC)	0

/* Specify the machine mode that pointers have. After generation of rtl, the
   compiler makes no further distinction between pointers and any other
   objects of this machine mode.  */
#define Pmode			HImode

/* An alias for the machine mode used for memory references to
   functions being called, in `call' RTL expressions.  On most CISC
   machines, where an instruction can begin at any byte address, this
   should be `QImode'.  On most RISC machines, where all instructions
   have fixed size and alignment, this should be a mode with the same
   size and alignment as the machine instruction words - typically
   `SImode' or `HImode'. */
#define FUNCTION_MODE		HImode

/* A C statement (sans semicolon) to output a reference to
   `SYMBOL_REF' SYM.  If not defined, `assemble_name' will be used to
   output the name of the symbol.  This macro may be used to modify
   the way a symbol is referenced depending on information encoded by
   `TARGET_ENCODE_SECTION_INFO'. */
#define ASM_GENERATE_INTERNAL_LABEL(STRING, PREFIX, NUM) \
  sprintf (STRING, "*%s%ld", PREFIX, (long)(NUM))

/* A C statement to output to the stdio stream STREAM an assembler
   instruction to advance the location counter by NBYTES bytes.
   Those bytes should be zero when loaded.  NBYTES will be a C
   expression of type `unsigned HOST_WIDE_INT'. */
#define ASM_OUTPUT_SKIP(STREAM, NBYTES) \
   fprintf(STREAM, "\tbss %lu\n", NBYTES);

/* A C statement (sans semicolon) to output to the stdio stream
   STREAM the assembler definition of uninitialized global DECL named
   NAME whose size is SIZE bytes.  The variable ROUNDED is the size
   rounded up to whatever alignment the caller wants. */
#define ASM_OUTPUT_ALIGNED_BSS(STREAM, DECL, NAME, SIZE, ALIGNMENT)     \
  if(ALIGNMENT > 1) fprintf ((STREAM), "\n\teven\n");                   \
  asm_output_aligned_bss ((STREAM), (DECL), (NAME), (SIZE), (ALIGNMENT));

/* A C statement (sans semicolon) to output to the stdio stream
   STREAM the assembler definition of a common-label named NAME whose
   size is SIZE bytes.  The variable ROUNDED is the size rounded up
   to whatever alignment the caller wants. */
#define ASM_OUTPUT_ALIGNED_COMMON(STREAM, NAME, SIZE, ALIGNMENT)           \
do {                                                                       \
     switch_to_section (bss_section);                                      \
     if(ALIGNMENT > 1) fprintf ((STREAM), "\n\teven");                     \
     fprintf ((STREAM), "\n\tdef %s\n", (NAME));                           \
     assemble_name ((STREAM), (NAME));                                     \
     fprintf ((STREAM), "\n\tbss %u\n", (int)(SIZE));                      \
} while (0)

/* A C statement (sans semicolon) to output to the stdio stream
   STREAM the assembler definition of a local-common-label named NAME
   whose size is SIZE bytes.  The variable ROUNDED is the size
   rounded up to whatever alignment the caller wants. */
#define ASM_OUTPUT_ALIGNED_LOCAL(STREAM, NAME, SIZE, ALIGNMENT)            \
do {                                                                       \
     switch_to_section (bss_section);                                      \
     if(ALIGNMENT > 1) fprintf ((STREAM), "\n\teven\n");                   \
     assemble_name ((STREAM), (NAME));                                     \
     fprintf ((STREAM), "\n\tbss %u\n", (int)(SIZE));                      \
} while (0)

/* A C statement to output to the stdio stream STREAM an assembler
   instruction to assemble a string constant containing the LEN bytes
   at PTR.  PTR will be a C expression of type `char *' and LEN a C
   expression of type `int'. */
#define ASM_OUTPUT_ASCII(STREAM, PTR, LEN) \
   tms9900_output_ascii(STREAM, PTR, LEN)

#define ASM_OUTPUT_DWARF_DELTA(FILE,SIZE,LABEL1,LABEL2)  \
   tms9900_asm_output_dwarf_delta (FILE, SIZE, LABEL1, LABEL2)

#define ASM_OUTPUT_DWARF_OFFSET(FILE,SIZE,LABEL,OFFSET,BASE)  \
   tms9900_asm_output_dwarf_offset (FILE, SIZE, LABEL, OFFSET,BASE)

/* Put references to global constructors in a .init section. The crt0 code
   will invoke these constructors at startup, before calling main. */
/* MGB gcc-13 expects a value here so if we have none, don't define it*/
// #define INIT_SECTION_ASM_OP

#undef  SIZE_ASM_OP
#undef  TYPE_ASM_OP
#define SIZE_ASM_OP     "\t.size\t"
#define TYPE_ASM_OP     "\t.type\t"

#undef TYPE_OPERAND_FMT
#define TYPE_OPERAND_FMT        "@%s"

#define ASM_DECLARE_FUNCTION_SIZE(FILE, FNAME, DECL)                    \
  do {                                                                  \
     if (!flag_inhibit_size_directive)                                  \
      ASM_OUTPUT_MEASURED_SIZE (FILE, FNAME);                           \
  } while (0)

#define ASM_DECLARE_OBJECT_NAME(FILE, NAME, DECL)                       \
do {                                                                    \
  ASM_OUTPUT_TYPE_DIRECTIVE (FILE, NAME, "object");                     \
  size_directive_output = 0;                                            \
  if (!flag_inhibit_size_directive && DECL_SIZE (DECL))                 \
    {                                                                   \
      size_directive_output = 1;                                        \
      ASM_OUTPUT_SIZE_DIRECTIVE (FILE, NAME,                            \
                                 int_size_in_bytes (TREE_TYPE (DECL))); \
    }                                                                   \
  ASM_OUTPUT_LABEL(FILE, NAME);                                         \
} while (0)


#undef ASM_FINISH_DECLARE_OBJECT
#define ASM_FINISH_DECLARE_OBJECT(FILE, DECL, TOP_LEVEL, AT_END)        \
  do {                                                                  \
    const char *name = XSTR (XEXP (DECL_RTL (DECL), 0), 0);             \
    HOST_WIDE_INT size;                                                 \
    if (!flag_inhibit_size_directive                                    \
        && DECL_SIZE (DECL)                                             \
        && ! AT_END && TOP_LEVEL                                        \
        && DECL_INITIAL (DECL) == error_mark_node                       \
        && !size_directive_output                                       \
        && (size = int_size_in_bytes (TREE_TYPE (DECL))) > 0)           \
      {                                                                 \
        size_directive_output = 1;                                      \
        ASM_OUTPUT_SIZE_DIRECTIVE (FILE, name, size);                   \
      }                                                                 \
  } while (0)


#ifdef FILE
void tms9900_asm_output_dwarf_delta (FILE *file, int size,
                               const char *lab1, const char *lab2);
#endif

