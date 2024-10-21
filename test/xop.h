/* XOP instruction definitions used as traps to the emulator */

#define XOP_START       1
#define XOP_EXECUTE     2
#define XOP_REPORT      3
#define XOP_PRINT       4

#define ASM_XOP(x) "li r0," #x "\n\txop r0,15\n"
#define STR(x) #x
