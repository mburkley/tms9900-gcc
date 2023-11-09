/* this is tc-tms9900.h
   Copyright 2009 Eric Welser

   This file is part of GAS, the GNU Assembler.

   GAS is free software; you can redistribute it and/or modify
   it under the terms of .the GNU General Public License as published by
   the Free Software Foundation; either version 3, or (at your option)
   any later version.

   GAS is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with GAS; see the file COPYING.  If not, write to the Free
   Software Foundation, 51 Franklin Street - Fifth Floor, Boston, MA
   02110-1301, USA.  */

#ifndef TC_TMS9900
#define TC_TMS9900

#define TARGET_ARCH   bfd_arch_tms9900
#define BFD_ARCH      TARGET_ARCH
#define COFF_MAGIC    0x5A80
#define TARGET_MACH   bfd_mach_tms9900
#define TARGET_BYTES_BIG_ENDIAN  0
#define LABELS_WITHOUT_COLONS 1


#define TARGET_FORMAT "elf32-tms9900"

/* If you define this macro, GAS will warn about the
   use of nonstandard escape sequences in a string.  */
#define ONLY_STANDARD_ESCAPES

/* This should just call either `number_to_chars_bigendian' or
   `number_to_chars_littleendian', whichever is appropriate.  On
   targets like the MIPS which support options to change the
   endianness, which function to call is a runtime decision.  On
   other targets, `md_number_to_chars' can be a simple macro.  */
#define md_number_to_chars number_to_chars_littleendian

/* This is called when an unrecognized symbol is encountered while
   parsing an expression. */
#define md_parse_name(name, exp, mode, c) tms9900_parse_name (name, exp)

/* This macro permits the processor to specify all characters which
   may appears in an operand.  This will prevent the scrubber from
   discarding meaningful whitespace in certain cases. */
extern const char tms9900_symbol_chars[];
#define tc_symbol_chars tms9900_symbol_chars

extern const char tms9900_comment_chars[];
#define tc_comment_chars tms9900_comment_chars


#define TC_COUNT_RELOC(x) 1

#define TC_COFF_FIX2RTYPE(fixP) tc_coff_fix2rtype (fixP)
#define md_convert_frag(b,s,f)   as_fatal ("convert_frag called\n")
#define md_estimate_size_before_relax(f,s) \
  (as_fatal (_("estimate_size_before_relax called")), 1)

/* Define some functions to be called by generic code.  */
#define md_end               tms9900_md_end
#define md_start_line_hook() { if (tms9900_start_line_hook ()) continue; }
#define TC_CONS_FIX_NEW tms9900_cons_fix_new

extern void tms9900_md_end (void);
extern int tms9900_start_line_hook (void);
extern void tms9900_cons_fix_new (fragS *, int, int, expressionS *);
extern int tms9900_parse_name(const char *, struct expressionS *);

/* We don't need to handle a broken .word directive. */
#define WORKING_DOT_WORD

/* If you define this macro, it means that `tc_gen_reloc' may return
   multiple relocation entries for a single fixup.  In this case, the
   return value of `tc_gen_reloc' is a pointer to a null terminated
   array.  */
#undef RELOC_EXPANSION_POSSIBLE

/* No shared lib support, so we don't need to ensure
   externally visible symbols can be overridden.  */
#define EXTERN_FORCE_RELOC 0

/* Values passed to md_apply_fix3 don't include the symbol value.  */
#define MD_APPLY_SYM_VALUE(FIX) 0

#define LISTING_WORD_SIZE 2

/* A single '=' is accepted as a comparison operator.  */
#define O_SINGLE_EQ O_eq

/* A '$' is used to refer to the current location or as a hex. prefix.  */
#define DOLLAR_DOT
#define DOLLAR_AMBIGU                1
#define LOCAL_LABELS_FB              1
#define LITERAL_PREFIXPERCENT_BIN
#define NUMBERS_WITH_SUFFIX          1
#define NO_PSEUDO_DOT                1
/* We allow single quotes to delimit character constants as
   well, but it is cleaner to handle that in tc-tms9900.c.  */
#define SINGLE_QUOTE_STRINGS
#define ONLY_STANDARD_ESCAPES

/* An `.lcomm' directive with no explicit alignment parameter will
   use this macro to set P2VAR to the alignment that a request for
   SIZE bytes will have.  The alignment is expressed as a power of
   two.  If no alignment should take place, the macro definition
   should do nothing.  Some targets define a `.bss' directive that is
   also affected by this macro.  The default definition will set
   P2VAR to the truncated power of two of sizes up to eight bytes.  */
#define TC_IMPLICIT_LCOMM_ALIGNMENT(SIZE, P2VAR) (P2VAR) = 0

#endif
