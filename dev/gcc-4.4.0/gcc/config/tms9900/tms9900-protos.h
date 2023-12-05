/* Definitions of target machine for GNU compiler, for the TMS9900

Copyright 2009 Eric Welser (EMW)
Copyright 2023 Mark Burkley (MGB)

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
<http://www.gnu.org/licenses/>.  */

/* declarations */
#ifdef RTX_CODE
extern int simple_memory_operand (rtx, enum machine_mode);
extern void print_operand_address (FILE *, rtx);
extern int legitimate_address_p (enum machine_mode, rtx);
extern void notice_update_cc_on_set (rtx, rtx);
#endif /* RTX_CODE */

extern const char *output_branch (const char *, const char *, int);
extern const char *output_jump (int);

extern int tms9900_function_arg_padding (enum machine_mode mode, 
                                         const_tree type);

extern void tms9900_function_arg_advance (CUMULATIVE_ARGS *cum, 
                                          enum machine_mode mode,
                                          tree type,
                                          int named ATTRIBUTE_UNUSED);

extern void tms9900_init_cumulative_args (CUMULATIVE_ARGS *cum,
                                          tree fntype ATTRIBUTE_UNUSED,
                                          rtx libname ATTRIBUTE_UNUSED);

extern rtx tms9900_function_arg (CUMULATIVE_ARGS *cum, 
                                 enum machine_mode mode,
                                 tree type ATTRIBUTE_UNUSED,
                                 int named);

extern int tms9900_initial_elimination_offset (int from, int to);
extern rtx tms9900_function_value (const_tree valtype);
extern void tms9900_output_ascii(FILE* stream, const char* ptr, int len);
extern void tms9900_expand_prologue (void);
extern void tms9900_expand_epilogue (bool is_sibcall);
extern int tms9900_starting_frame_offset(void);
extern int tms9900_reg_ok_for_base(int strict, rtx reg);
extern int tms9900_go_if_legitimate_address(enum machine_mode mode ATTRIBUTE_UNUSED, rtx operand, int strict);

extern void tms9900_register_mode_set (rtx operand, int mode);
extern void tms9900_register_convert (rtx operand, int mode, int sign);
extern void tms9900_inline_debug (const char *fmt,...);
extern void tms9900_debug_operands (const char *name, rtx ops[], int count);
