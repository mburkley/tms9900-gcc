/* Subroutines for insn-output.c for TMS9900.
   Copyright (C) 1987, 1994, 1995, 1997, 1998, 1999, 2000, 2001, 2002,
   2004, 2005, 2006, 2007, 2008
   Free Software Foundation, Inc.

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

#include "insn-modes.h"
#include <stdio.h>
#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "tm.h"
#include "rtl.h"
#include "tree.h"
#include "tm_p.h"
#include "regs.h"
#include "hard-reg-set.h"
#include "real.h"
#include "insn-config.h"
#include "conditions.h"
#include "output.h"
#include "insn-attr.h"
#include "flags.h"
#include "recog.h"
#include "expr.h"
#include "libfuncs.h"
#include "toplev.h"
#include "basic-block.h"
#include "function.h"
#include "ggc.h"
#include "reload.h"
#include "target.h"
#include "target-def.h"
#include "df.h"
#include "dfp.h"
#include "decimal128.h"
#include "decNumber.h"

/* Define this to put insn debugs into output files */
#undef TMS9900_DEBUG
// #define TMS9900_DEBUG 1

static bool tms9900_pass_by_reference (CUMULATIVE_ARGS *,
                                       enum machine_mode, const_tree, bool);
static bool tms9900_asm_integer(rtx x, unsigned int size, int aligned_p);

static int tms9900_dwarf_label_counter;

/* Define data types */
#undef  TARGET_ASM_BYTE_OP
#define TARGET_ASM_BYTE_OP "\tbyte\t"

#undef  TARGET_ASM_ALIGNED_HI_OP
#define TARGET_ASM_ALIGNED_HI_OP "\tdata\t"

#undef  TARGET_PASS_BY_REFERENCE
#define TARGET_PASS_BY_REFERENCE tms9900_pass_by_reference

#undef  TARGET_ASM_INTEGER
#define TARGET_ASM_INTEGER tms9900_asm_integer

#undef  TARGET_FUNCTION_OK_FOR_SIBCALL
#define TARGET_FUNCTION_OK_FOR_SIBCALL tms9900_ok_for_sibcall

static bool
tms9900_ok_for_sibcall (tree decl, tree exp)
{
  return true;
}

#define TARGET_ASM_ALIGNED_HI_OP "\tdata\t"
#define TARGET_ASM_ALIGNED_SI_OP NULL
#define TARGET_ASM_ALIGNED_DI_OP NULL
#define TARGET_ASM_ALIGNED_TI_OP NULL

struct gcc_target targetm = TARGET_INITIALIZER;

/* Set global variables as needed for the options enabled.  */

static void
tms9900_encode_real (const struct real_format *fmt, long *buf,
		    const REAL_VALUE_TYPE *r)
{
  printf("%s val uexp=%u expb=%d sigsz=%d sig[n]=",
         __func__, r->uexp, EXP_BITS, SIGSZ);

  // unsigned char *c = (unsigned char*) r->sig;
  // int i;

  char a[256];
  decimal128ToString((const decimal128*)r->sig, a);
  printf("str=%s\n", a);

/* atof.c
   This method converts an ascii representation of a float to a float.
   change log:
   06/23/2023 mrvan initial version
*/

// #include <stdlib.h>
// #include <ctype.h>
// #include <string_ext.h>

// converts an ascii representation of a float to a float

   // The TI-99/4A uses RADIX-100 for doubles. RADIX is also known as base. So the doubles are
   // stored in base 100. Doubles are stored as 8 bytes, where the number is in scientific notation,
   // but again as radix 100:
   // - The first byte is the exponent (a radix 100 exponent, not base 10 exponent).
   // - Subsequent bytes store the mantissa. The decimal point is after the first mantissa digit.
   // - Negative numbers are stored with the exponent and first digit negated.
   // - Zero has an exponent and first digit set to 0 and 0, respectively.
   // - Exponents are offset by 0x40.
   // - Note that there are excellent explanations online, but even those have some incorrect info.
   // - https://www.unige.ch/medecine/nouspikel/ti99/reals.htm
   // - For instance, one example gets the matissa correct but the author inadvertantly used base
   // - 10 for the exponent, rather than base 100.

   char *s = a;
   unsigned char d[8];                // return value
   // unsigned char *d = (unsigned char *)buf;
   char * pd = (char *) d; // pointer to the return value

   // staging values for final double value
   int exp     = 0;                                            // base 100 exponent
   int neg_pos = 1;                                            // pos = 1, neg = -1
   int v[14]   = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }; // represented as base 10 later to be compbined as base 100
   int pos     = 0;                                            // write pos in v[]
   int dp      = 0;                                            // decimal point position relative to v[]

   int b10_exp_neg_pos = 1;
   int b10_exp = 0;
   int b10_exp_have_digit = 0;

   int n;
   int have_pos_digit = 0;
   int i;

   // skip whitespace
   // s = skip_whitespace (s);

   // capture the negative sign if present
   if (*s == '-') {
      neg_pos = -1;
      s++;
   }

   // run through the digits before any decimal point
   while ( (*s) && (*s >= '0' && *s <= '9') && (pos < 14) ) {
      n = *s;
      n -= 48;
      if (n >= 1) have_pos_digit = 1;
      if (have_pos_digit) {
         v[pos] = n;
         pos++;
      }
      s++;
   }

   // capture the decimal point position, even if implicit due to lack of one
   if (*s == '.') {
      s++;
   }
   dp = pos;

   // run through the fractional digits before any exponent designation
   while ( (*s) && (*s >= '0' && *s <= '9') && (pos < 14) ) {
      n = *s;
      n = n - '0';
      if (n >= 1) have_pos_digit = 1;
      if (have_pos_digit) {
         v[pos] = n;
         pos++;
      } else {
         dp--;
      }
      s++;
   }

   // handle specified exponent
   if ( (*s == 'e') || (*s == 'E') ) {
      s++;
      if (*s == '-') {
         b10_exp_neg_pos = -1;
         s++;
      }
      while ( (*s) && (*s >= '0' && *s <= '9') ) {
         if (b10_exp_have_digit) {
            b10_exp *= 10;
         }
         n = *s;
         b10_exp += n - '0';
         b10_exp_have_digit = 1;
         s++;
      }
      dp += b10_exp * b10_exp_neg_pos;
   }

   // process the collected value if at least one positive digit was found, otherwise the value is zero
   if (have_pos_digit) {
      // calculate the exponent, which is base 100
      exp = 0x3F + dp / 2;
      if ( (dp > 0) && (dp % 2) ) exp++;

      // shift the base 10 digits right by one if not exp aligned for base 100
      if (dp % 2) {
         for (i = 13; i > 0; i--) {
            v[i] = v[i - 1];
         }
         v[0] = 0;
      }
   }

   // finally, convert the collected and processed value into a double

   // generate the exponent, write and advance. Note that this is a base 100 exponent
   if (neg_pos == -1) exp++;
   n = exp * neg_pos;
   *pd = n;
   pd++;

   // generate the first mantissa digit, that might need to be negated, write and advance
   n = (v[0] * 10 + v[1]) * neg_pos; // base 100 generated by combining the v[0] and v[1] values
   *pd = n;
   pd++;

   // process and write the remaining base 100 digits, combining the adjacent base 10 digits
   for (i = 2; i < 14; i += 2) {
      n = v[i] * 10 + v[i + 1];
      *pd = n;
      pd++;
   }

  // Convert 8-bytes to 3-longs ????

  buf[0] = (d[0] << 24) | (d[1] << 16) | (d[2] << 8) | d[3];
  buf[1] = (d[4] << 24) | (d[5] << 16) | (d[6] << 8) | d[7];
  buf[2] = 0;

  unsigned char *x=(unsigned char*)buf;
  for (i = 0; i < SIGSZ*8 ; i++)
      printf (" %02X", x[i]);

  printf("\n");
}

/*  Decode a TI real to REAL_VALUE_TYPE by first converting it to a string.  The
 *  r->sig member of the real value is a decimal128 struct */
static void
tms9900_decode_real (const struct real_format *fmt, REAL_VALUE_TYPE *r,
		    const long *buf)
{
    char s[32];
    char *p = s;
    unsigned char *bytes=(unsigned char*)buf;
    short first = bytes[0] << 8 | bytes[1];
    int exp;
    int i;
    
  for (i = 0; i < 8; i++)
      printf (" %02X", bytes[i]);

      printf ("\n");
    if (first < 0)
    {
        *p++='-';
        first =- first;
    }

    exp = first >> 8;
    first &= 0xff;

    printf("exp=%d first=%d\n", exp, first);
    if (first>9) *p++=(first/10) + '0';
    *p++=(first%10)+'0';
    *p++='.';
    for (i = 2; i < 8; i++)
    {
        *p++=(bytes[i]/10)+'0';
        *p++=(bytes[i]%10)+'0';
    }

    exp -= 0x40;

    if (exp != 0)
    {
        *p++='E';

        if (exp<0)
        {
            *p++='-';
            exp=-exp;
        }

        exp*=2;
        if (exp>9)
        {
            *p++=(exp/10)+'0';
            exp/=10;
        }
        *p++=exp+'0';
    }

  printf ("%s encoding %s\n", __func__, s);
  #if 0
  decNumber dn;
  decContext set;
  decContextDefault (&set, DEC_INIT_DECIMAL128);

  // decimal128FromString((const decimal128*) r->sig, a);

  decNumberFromString (&dn, s, &set);
  decimal_from_decnumber (r, &dn, &set);
  #endif
    decimal_real_from_string (r, s);

}

const struct real_format tms9900_real_format =
  {
    tms9900_encode_real,
    tms9900_decode_real,
    10,         // base 10
    14,         // 14 digits
    14,         // 14 digits
    -63,        // lowest exp
    64,         // highest exp
    63,
    63,
    false,      // Don't round toward 0
    false,      // Don't have sign dependent rounding
    false,      // Don't have NAN
    false,      // Don't have INF
    false,      // Don't have denorm whatever that is
    true,       // Do have signed zero
    false,      // Don't have qnan
    false       // Don't have that last thing
  };


void override_options (void)
{
  /* We use TI99 floating point, not IEEE floating point.  */
    REAL_MODE_FORMAT (DFmode) = &tms9900_real_format;
}

/* Non-volatile registers to be saved across function calls */
static int nvolregs[]={
   HARD_LR_REGNUM,
   HARD_BP_REGNUM,
   HARD_LW_REGNUM,
   HARD_LP_REGNUM,
   HARD_LS_REGNUM,
   0};


/* If defined, a C expression which determines whether, and in which direction,
   to pad out an argument with extra space.  The value should be of type
   `enum direction': either `upward' to pad above the argument,
   `downward' to pad below, or `none' to inhibit padding.

   Structures are stored left shifted in their argument slot.  */
int tms9900_function_arg_padding (enum machine_mode mode,
                                  const_tree type)
{
  if (type != 0 && AGGREGATE_TYPE_P (type))
  {
    // printf ("%s upward\n", __func__);
    return upward;
  }

  /* Fall back to the default.  */
  return DEFAULT_FUNCTION_ARG_PADDING (mode, type);
}


/* Update the data in CUM to advance over an argument
   of mode MODE and data type TYPE.
   (TYPE is null for libcalls where that information may not be available.)  */
void tms9900_function_arg_advance (CUMULATIVE_ARGS *cum, 
                                   enum machine_mode mode,
                                   tree type, 
                                   int named ATTRIBUTE_UNUSED)
{
  int arg_bytes;
  if(mode == BLKmode)
  {
     arg_bytes = int_size_in_bytes (type);
  }
  else
  {
     arg_bytes = GET_MODE_SIZE (mode);
  }
  cum->nregs += ((arg_bytes + 1)/ UNITS_PER_WORD) * REGS_PER_WORD;
  // printf("%s bytes=%d\n", __func__, arg_bytes);
  return;
}


/* Initialize a variable CUM of type CUMULATIVE_ARGS
   for a call to a function whose data type is FNTYPE.
   For a library call, FNTYPE is NULL.  */
void tms9900_init_cumulative_args (CUMULATIVE_ARGS *cum, 
                                   tree fntype ATTRIBUTE_UNUSED,
                                   rtx libname ATTRIBUTE_UNUSED)
{
  /* Varargs vectors are treated the same as long long. Using
     named_count avoids having to change the way 'named' is handled */
  cum->named_count = 0;
  cum->nregs = 0;
}


/* Determine where to put an argument to a function.
   Value is zero to push the argument on the stack,
   or a hard register in which to store the argument.

   MODE is the argument's machine mode.
   TYPE is the data type of the argument (as a tree).
      This is null for libcalls where that information may
      not be available.
   CUM is a variable of type CUMULATIVE_ARGS which gives info about
      the preceding args and about the function being called.
   NAMED is nonzero if this argument is a named parameter
      (otherwise it is an extra parameter matching an ellipsis).  */
rtx tms9900_function_arg (CUMULATIVE_ARGS *cum, 
                          enum machine_mode mode,
		          tree type,
                          int named)
{
  if (mode == VOIDmode)
    /* Pick an arbitrary value for operand 2 of the call insn.  */
    return const0_rtx;
  
  if (/* Vararg argument, must live on stack */
      !named ||
      /* No more argument registers left */
      cum->nregs >= TMS9900_ARG_REGS ||
      /* Argument doesn't completely fit in arg registers */      
      GET_MODE_SIZE(mode) + cum->nregs > TMS9900_ARG_REGS)
    {
    // printf ("%s alloc on stack\n", __func__);
    return NULL_RTX;
    }

  /* Allocate registers for argument */
  // printf ("%s alloc in reg %d\n", __func__, cum->nregs+1);
  return gen_rtx_REG (mode, cum->nregs + HARD_R1_REGNUM);
}


/* Output all constant addresses using hex values */
static void tms9900_output_addr_const(FILE *file, rtx addr)
{
  if(CONST_INT_P(addr))
    fprintf(file, ">%X", (int)(INTVAL(addr)) & 0xFFFF);
  else
    output_addr_const(file, addr);
}

static FILE *outputFile;

/* Construct string expression matching an address operand */
void print_operand_address (FILE *file,
                            register rtx addr)
{
  if (!outputFile) outputFile=file;

  retry:
  switch (GET_CODE (addr))
    {
    case MEM:
      addr = XEXP (addr, 0);
      goto retry;

    case REG:
      fprintf (file, "*%s", reg_names[REGNO (addr)]);
      break;

    case POST_MODIFY:
    case POST_INC:
      fprintf (file, "*%s+", reg_names[REGNO (XEXP (addr, 0))]);
      break;

    case PLUS:
      /* @xxxx(R0) */
      if (GET_CODE (addr) == PLUS            &&
          GET_CODE (XEXP (addr, 0)) == REG   &&
          REG_OK_FOR_BASE_P (XEXP (addr, 0)) &&
          CONSTANT_ADDRESS_P (XEXP (addr, 1)))
      {
        fprintf(file, "@");
        tms9900_output_addr_const (file, XEXP (addr, 1));
        fprintf(file, "(%s)", reg_names[REGNO (XEXP (addr, 0))]);
      }

      /* @(symbol+xxxx)(R0) */
      else if(GET_CODE (addr) == PLUS )
      {
        rtx base   = 0;
        rtx offset = 0;
        int regno  = 0;
        if(GET_CODE(XEXP(addr,0)) == MEM)
        {
          rtx op = XEXP(addr,0);
          op = XEXP(op, 0);
          if(GET_CODE (op) == PLUS             &&
             GET_CODE (XEXP (op, 0)) == REG    &&
             REG_OK_FOR_BASE_P (XEXP (op, 0))  &&
             CONSTANT_ADDRESS_P(XEXP (op, 1))  &&
             CONSTANT_ADDRESS_P(XEXP(addr, 1)))
          {
            base   = XEXP(addr, 1);
            offset = XEXP(op, 1);
            regno  = REGNO(XEXP(op, 0));
            if(CONST_INT_P(base) && CONST_INT_P(offset))
            {
              fprintf(file, "@>%X(%s)", (int)(INTVAL(base) + INTVAL(offset)) & 0xFFFF, reg_names[regno]);
            }
            else
            {
              fprintf(file, "@(");
              tms9900_output_addr_const (file, base);
              fprintf(file, "+>%X)(%s)", ((int)INTVAL(offset)) & 0xFFFF, reg_names[regno]);
            }
          }
        }
      }
      else
      {
        gcc_assert (0);
      }
      break;

    default:
      fprintf(file, "@");
      tms9900_output_addr_const (file, addr);
    }
}


/* Should we save this register? */ 
int tms9900_should_save_reg(int regno)
{
   // printf("should I save %d ? ever_live=%d used=%d leaf=%d\n",
   //        regno, df_regs_ever_live_p(regno),
   //        call_used_regs[regno],
   //        current_function_is_leaf);
          
   return(
      /* Save non-volatile registers or r11 if used */
      (df_regs_ever_live_p(regno) &&
          (call_used_regs[regno] == 0 || regno == HARD_LR_REGNUM)) ||
      /* Save r11 if this is not a leaf function */
      (regno == HARD_LR_REGNUM && !current_function_is_leaf)
   );
}


/* Get number of bytes used to save registers in the current stack frame */
static int tms9900_get_saved_reg_size(void)
{
   int idx = 0;
   int size = 0;
   while(nvolregs[idx] != 0)
   {      
      int regno = nvolregs[idx];     
      if(tms9900_should_save_reg(regno))
      {
         size += 2;
      }
      idx++;
   }
   return(size);
}

static void print_arg_offset (int from)
{
    switch (from)
    {
    case ARG_POINTER_REGNUM: printf ("%d=ARG_PTR_R ", from); break;
    case HARD_SP_REGNUM: printf ("%d=HARD_SP_R ", from); break;
    case FRAME_POINTER_REGNUM: printf ("%d=FR_PTR_R ", from); break;
    default: printf ("%d=dunno? ", from); break;
    }
}

/* Define the offset between two registers, one to be eliminated, and the
   other its replacement, at the start of a routine.
   MGB return values as they are AFTER the prolog */
int tms9900_initial_elimination_offset (int from,
                                        int to)
{
  /*  
      [argn]
      .
      [arg0]
      . <- arg pointer
      [saved regs]
      [frame]
      . <- stack pointer
      . <- frame pointer
  */

  // printf("%s savedregs=%d frame=%d ", __func__,
  //     tms9900_get_saved_reg_size(), get_frame_size());
  // print_arg_offset (from);
  // print_arg_offset (to);
  int ret = 0;
  if (from == ARG_POINTER_REGNUM && to == HARD_SP_REGNUM)
  {
    ret =(tms9900_get_saved_reg_size()+
           get_frame_size ());
  }
  if (from == FRAME_POINTER_REGNUM && to == HARD_SP_REGNUM)
  {
    ret = 0;
  }
  if (from == ARG_POINTER_REGNUM && to == FRAME_POINTER_REGNUM)
  {
    ret =(tms9900_get_saved_reg_size()+
           get_frame_size ());
  }
  // printf ("%s res=%d\n", __func__, ret);
  return ret;
}


/* Determine if an address is represented using a valid expression */
int legitimate_address_p (enum machine_mode mode,
                          rtx address)
{
    GO_IF_LEGITIMATE_ADDRESS(mode, address, win);   
    return 0;
    
  win:
    return 1;
}


/* Determine the memory operand type
   returns : 0 - Not a memory operand
             1 - Register indirect : *Rn
             2 - Post increment    : *Rn+
             3 - Indexed register  : @INDEX(Rn)
*/
int tms9900_address_type(rtx op,
                         enum machine_mode mode ATTRIBUTE_UNUSED)
{
  rtx addr;

  /* Eliminate non-memory operations */
  if (GET_CODE (op) != MEM)
    return 0;

  /* Decode the address now.  */
indirection:
  addr = XEXP (op, 0);
  switch (GET_CODE (addr))
  {
    case REG:
      /* Register indirect: *Rn */
      return 1;
	
    case POST_INC:
      /* Post increment: *Rn+ */
      return 2;
	
    case MEM:
      /* Yes, we know this is a memory expression.
         Strip this code and try again */
      op=addr;
      goto indirection;
	
    case CONST_INT:
    case LABEL_REF:	       
    case CONST:
    case SYMBOL_REF:
      /* Indexed register : @ADDRESS(R0) - extra word required */

    case PLUS:
      /* Indexed register : @INDEX(Rn) - extra word required */
      return 3;

    default:
      break;
  }
    
  /* We should never get here, but we need to return something */
  return 0;
}


/* Emit a branch condtional instruction */
const char* output_branch (const char *pos, const char *neg, int length)
{
    static int label_id = 0;
    
    static char buf[1000];
    length -= 10;
    if(length == 2)
    {
        sprintf(buf, "%s  %%l0",pos);
    }
    else if(length == 4)
    {
        if(*pos == 'L')
            sprintf(buf, "jlt  %%l0\n"
                         "\tjeq  %%l0");
        else if(*pos == 'G')
            sprintf(buf, "jgt  %%l0\n"
                         "\tjeq  %%l0");
        else
            gcc_unreachable();
    }
    else if(length == 6)
    {
	sprintf(buf, "%s  JMP_%d\n"
                     "\tb    @%%l0\n"
                     "JMP_%d", neg, label_id, label_id);	
	label_id++;
    }
    else if(length == 8)
    {
        if(*neg == 'L')
            sprintf(buf, "jlt  JMP_%d\n"
                         "\tjeq  JMP_%d\n"
                         "\tb    @%%l0\n"
                         "JMP_%d", label_id, label_id, label_id);
        else if(*neg == 'G')
            sprintf(buf, "jgt  JMP_%d\n"
                         "\tjeq  JMP_%d\n"
                         "\tb    @%%l0\n"
                         "JMP_%d", label_id, label_id, label_id);
        else
            gcc_unreachable();

	label_id++;
    }
    else
    {
	gcc_unreachable();
    }    
    return buf;
}


/* Emit a jump instrcution */
const char* output_jump (int length)
{
    length -= 10;
    switch(length)    
    {
        case 2: return("jmp  %l0");
        case 4: return("b    @%l0");
        default: gcc_unreachable();
    }
}


/* Determine if an instruction will update the conditional
   flag as a side effect. This is used to eliminate unnneded
   comparison instructions */
void notice_update_cc_on_set(rtx exp, rtx insn ATTRIBUTE_UNUSED)
{
  if (GET_CODE (SET_DEST (exp)) == CC0)
  {
    cc_status.flags = 0;					
    cc_status.value1 = SET_DEST (exp);			
    cc_status.value2 = SET_SRC (exp);			
  }
  else if((GET_MODE (SET_DEST(exp)) == HImode ||
           GET_MODE (SET_DEST(exp)) == QImode)
          &&
          (GET_CODE(SET_SRC(exp)) == PLUS  ||
           GET_CODE(SET_SRC(exp)) == MINUS ||
           GET_CODE(SET_SRC(exp)) == AND   ||
           GET_CODE(SET_SRC(exp)) == IOR   ||
           GET_CODE(SET_SRC(exp)) == XOR   ||
           GET_CODE(SET_SRC(exp)) == NOT   ||
           GET_CODE(SET_SRC(exp)) == NEG   ||
           GET_CODE(SET_SRC(exp)) == ABS   ||
           GET_CODE(SET_SRC(exp)) == REG   ||
           GET_CODE(SET_SRC(exp)) == MEM))
  {
    cc_status.flags = 0;
    cc_status.value1 = SET_SRC (exp);
    cc_status.value2 = SET_DEST (exp);
  }
  else
  {  
    /* This last else is a bit paranoid, but since nearly all
       instructions play with condition codes, it's reasonable. */
    CC_STATUS_INIT;
  }		        
}


/* Determine where the return value from a function will lie */
rtx tms9900_function_value (const_tree valtype) /*, 
                            const_tree func ATTRIBUTE_UNUSED, 
                            bool outgoing ATTRIBUTE_UNUSED) */
{
  return gen_rtx_REG (TYPE_MODE (valtype), HARD_R1_REGNUM);
}


/* A C statement to output to the stdio stream STREAM an assembler
   instruction to assemble a string constant containing the LEN bytes
   at PTR.  PTR will be a C expression of type `char *' and LEN a C
   expression of type `int'. */
void tms9900_output_ascii(FILE* stream, const char* ptr, int len)
{
   int i;
   int in_text = 0;
   int count = 0;
   for (i = 0; i < len; i++)
   {
      int c = *ptr++;
      if (ISPRINT(c))
      {
         /* End TEXT statement */
         if (count==64)
         {
            fprintf (stream, "'\n");
            in_text = 0;
            count = 0;
         }

         /* Start TEXT statement */
         if(in_text == 0)
         {
            fprintf (stream, "\ttext '");
            in_text = 1;
         }
         putc (c, stream);
         count++;
         if(c == '\'') putc (c, stream);
      }
      else
      {
         /* Close TEXT statement */
         if(in_text == 1)
         {
            fprintf (stream, "'\n");
            in_text = 0;
         }

         /* Handle non-printable characters by inlining BYTE constants*/
         fprintf (stream, "\tbyte %d\n", (unsigned char)c);
      }
   }

   /*  If we ran out of input before we had closed the text, close it now */
   if(in_text == 1)
      fprintf (stream, "'\n");
}


void tms9900_expand_prologue (void)
{
   /* Registers to save in this frame */
   int saveregs[5];
   int regcount = 0;
   int idx = 0;
   int frame_size = get_frame_size();
   int dst_reg = HARD_SP_REGNUM;

   /* Find non-volatile registers which need to be saved */
   while(nvolregs[idx] != 0)
   {      
      int regno = nvolregs[idx];
      saveregs[idx] = 0;
      if(tms9900_should_save_reg(regno))
      {
         /* Mark this register to be saved */
         saveregs[idx] = 1;
         regcount++;
      }
      idx++;
   }

  // printf("%s saving regs=%d frame=%d ", __func__, regcount, frame_size);
   /* Allocate stack space for saved regs */
   if(regcount > 0)
   {
      /* Emit "ai sp, -regcount*2" */
      emit_insn(gen_addhi3(stack_pointer_rtx, stack_pointer_rtx, 
                          GEN_INT(-regcount * 2)));
   }

   /* Actually save registers */
      /*
      Form 1:
      ai sp, -regs*2        4      14+8+8   = 30
      mov sp, r0            2      14+8     = 22
      mov r9 , *r0+         2      14+8+8+8 = 38
      mov r13, *r0+         2      14+8+8+8 = 38
      mov r14, *r0+         2      14+8+8+8 = 38
      mov r15, *r0+         2      14+8+8+8 = 38
      mov r11, *r0          2      14+8+8+8 = 34
      ai sp, -frame         4      14+8+8   = 30
      */

      /* If saving more than 1 reg, then copy sp to r0 and use r0 as the dest
       * with auto-inc */
      if (regcount>1)
      {
        /* Emit "mov sp, r0" */
        emit_insn(gen_movhi(gen_rtx_REG(HImode, HARD_R0_REGNUM),
                            stack_pointer_rtx));
        dst_reg = HARD_R0_REGNUM;
      }

      idx = 0;
      while(nvolregs[idx] != 0)
      {
         if(saveregs[idx]!=0)
         {
            int regno = nvolregs[idx];
            /*  Don't postinc if last reg */
            if(regcount==1)
            {
               /* Emit "mov Rx, *R0" */
               emit_insn(gen_movhi(
                  gen_rtx_MEM(HImode, gen_rtx_REG(HImode, dst_reg)),
                  gen_rtx_REG(HImode, regno)));
            }
            else
            {
               /* Emit "mov Rx, *R0+" */
               emit_insn(gen_movhi(
                  gen_rtx_MEM(HImode, 
                              gen_rtx_POST_INC(HImode, 
                                 gen_rtx_REG(HImode, dst_reg))),
                  gen_rtx_REG(HImode, regno)));
            }
            regcount--;
         }
         idx++;
      }

   if(frame_size > 0)
   {
      // printf("%s alloc frame\n", __func__);
      /* Emit "ai sp, -framesize" */
      emit_insn(gen_addhi3(stack_pointer_rtx, stack_pointer_rtx, 
                          GEN_INT(-frame_size)));
   }

   /* Set frame pointer */
   if(frame_pointer_needed)
   {
      // printf("%s set fp\n", __func__);
      emit_insn(gen_movhi(gen_rtx_REG(HImode, FRAME_POINTER_REGNUM),
                          stack_pointer_rtx));
   }

   // printf("%s done\n", __func__);
}


void tms9900_expand_epilogue (bool is_sibcall)
{
   /*
   There is one only form for the epilogue.

   ai sp, frame         4      14+8+8   = 30
   mov *sp+, r9         2      14+8+8+8 = 38
   mov *sp+, r13        2      14+8+8+8 = 38
   mov *sp+, r14        2      14+8+8+8 = 38
   mov *sp+, r15        2      14+8+8+8 = 38
   mov *sp+, r11        2      14+8+8+8 = 38
   */

   int saveregs[5];
   int restored;

   /* Find frame size to restore */
   int frame_size = get_frame_size();

   if(frame_size != 0)
   {
      // printf("%s delete frame\n", __func__);
      /* Emit "ai sp, frame_size" */
      emit_insn(gen_addhi3(stack_pointer_rtx, stack_pointer_rtx, 
                           GEN_INT(frame_size)));
   }

   /* Find non-volatile registers which need to be restored */
   int idx = 0;
   int regcount = 0;
   while(nvolregs[idx] != 0)
   {      
      int regno = nvolregs[idx];
      saveregs[idx] = 0;
      if(tms9900_should_save_reg(regno))
      {
         /* Mark this register to be restored */
         saveregs[idx] = 1;
         regcount++;
      }
      idx++;
   }

   // printf("%s restore %d regs\n", __func__, regcount);
   idx = 0;
   restored = 0;
   while(nvolregs[idx] != 0)
   {      
      int regno = nvolregs[idx];
      if(saveregs[idx] != 0)
      {
         /* Restore this register */
         int regno = nvolregs[idx];
         restored++;
        /* Emit "mov *SP+, Rx" */
        emit_insn(gen_movhi(
           gen_rtx_REG(HImode, regno),
           gen_rtx_MEM(HImode, 
                       gen_rtx_POST_INC(HImode, stack_pointer_rtx))));
      }
      idx++;
   }
   
   if(!is_sibcall)
   {
      // printf("%s return\n", __func__);
      /* Emit the return instruction "b *R11" */
      emit_insn(gen_rtx_UNSPEC(HImode, 
                               gen_rtvec (1, gen_rtx_REG(HImode, HARD_R11_REGNUM)),
                               UNSPEC_RETURN));
   }
   // printf("%s done\n", __func__);
}


/* All registers may be used as a base, except R0 or 
   pseudoregs when we are in strict mod */
int tms9900_reg_ok_for_base(int strict, rtx reg)
{
  return(!strict || 
         (REGNO(reg) !=0 && REGNO(reg) <= HARD_R15_REGNUM));
}


/* Determine if the specified address is a valid operand */
int tms9900_go_if_legitimate_address(enum machine_mode mode ATTRIBUTE_UNUSED, rtx operand, int strict)
{
  /* Accept *R0 */
  if (GET_CODE (operand) == REG &&
      tms9900_reg_ok_for_base(strict, operand))
    return 1;

  /* Accept *R0+ */
  if (GET_CODE (operand) == POST_INC      &&
      GET_CODE (XEXP (operand, 0)) == REG &&
      tms9900_reg_ok_for_base(strict, XEXP (operand, 0)))
    return 1;

  /* Accept @xxxx */
  if (CONSTANT_ADDRESS_P (operand))
    return 1;

  /* Accept @xxxx(R0)*/
  if (GET_CODE (operand) == PLUS            &&
      GET_CODE (XEXP (operand, 0)) == REG   &&
      tms9900_reg_ok_for_base(strict, XEXP (operand, 0)) &&
      CONSTANT_ADDRESS_P (XEXP (operand, 1)))
    return 1;

  /* Accept @(symbol+xxxx)(R0)*/
/*
  if(GET_CODE (operand) == PLUS )
  {
    if(GET_CODE(XEXP(operand,0)) == MEM)
    {
      rtx op = XEXP(operand,0);
      op = XEXP(op, 0);
      if(GET_CODE (op) == PLUS             &&
         GET_CODE (XEXP (op, 0)) == REG    &&
         tms9900_reg_ok_for_base(strict, XEXP (op, 0))  &&
         CONSTANT_ADDRESS_P (XEXP (op, 1)) &&
         CONSTANT_ADDRESS_P(XEXP(operand, 1)))
      {
        return 1;
      }
    }
  }
*/
  /* Anything else is invalid */
  return 0;
}


/* All aggregate types or types larger than four bytes which are
   to be passsed by value are silently copied to the stack and 
   then passed by reference. */
static bool
tms9900_pass_by_reference (CUMULATIVE_ARGS *cum ATTRIBUTE_UNUSED,
                       enum machine_mode mode ATTRIBUTE_UNUSED,
                       const_tree type, bool named ATTRIBUTE_UNUSED)
{
  unsigned int size;
  if (type)
    {
      if (AGGREGATE_TYPE_P (type))
        return(true);
      size = int_size_in_bytes (type);
    }
  else
    size = GET_MODE_SIZE (mode);
  return(size > 4);
}


/* Output a difference of two labels that will be an assembly time
   constant if the two labels are local.  (.long lab1-lab2 will be
   very different if lab1 is at the boundary between two sections; it
   will be relocated according to the second section, not the first,
   so one ends up with a difference between labels in different
   sections, which is bad in the dwarf2 eh context for instance.)  */
void
tms9900_asm_output_dwarf_delta (FILE *file, int size,
                               const char *lab1, const char *lab2)
{
  int islocaldiff = (lab1[0] == '*' && lab1[1] == 'L'
                     && lab2[0] == '*' && lab2[1] == 'L');
  const char *directive = "data";
  islocaldiff=1;
  if(size > 2) fprintf(file,"\n\tdata 0\n",size);
  if (islocaldiff)
    fprintf (file, "\t.set L$set$%d,", tms9900_dwarf_label_counter);
  else
    fprintf (file, "\t%s\t", directive);
  assemble_name_raw (file, lab1);
  fprintf (file, "-");
  assemble_name_raw (file, lab2);
  if (islocaldiff)
    fprintf (file, "\n\t%s L$set$%d", directive, tms9900_dwarf_label_counter++);
}


/* Output an offset from a label for use in a dwarf record */
void tms9900_asm_output_dwarf_offset (FILE *file, int size, const char * lab,
                                section *base)
{
  char sname[64];
  sprintf(sname, "%s",&base->named.name[0]);
  tms9900_asm_output_dwarf_delta (file, size, lab, sname);
}


/* Output an integer value of a specified size and alignemnt */
static bool
tms9900_asm_integer(rtx x, unsigned int size, int aligned_p)
{
  if(!aligned_p)
  {
    if(GET_CODE(x) == CONST_INT)
    {
      unsigned int value = INTVAL(x);
      int i;
      fprintf(asm_out_file, "\tbyte\t");
      for(i = size-1; i>=1; i--)
      {
        fprintf(asm_out_file, "0x%X,", (value >> (i*8)) & 0xFF);        
      }
      fprintf(asm_out_file, "0x%X", value & 0xFF);
      return true;
    }
  }
  return default_assemble_integer(x,size,1);
}


//==================================================================
// Code for tms9900_subreg pass


#include "tree-pass.h"
#include "basic-block.h"
#include "rtl.h"

static void
tms9900_extract_subreg(rtx insn, rtx arg, rtx* parg)
{
  // printf("%s\n", __func__);
  if(BINARY_P(arg))
  {
    // printf("%s recurse\n", __func__);
    /* Recurse until we find a leaf expression */
    tms9900_extract_subreg(insn, XEXP(arg,0), &XEXP(arg,0));
    tms9900_extract_subreg(insn, XEXP(arg,1), &XEXP(arg,1));
  }
  else

  {
    if(GET_CODE(arg) == SUBREG && GET_MODE(arg) == QImode)
    {
      // printf ("%s creating extract\n", __func__);
      /* Found a subreg expression we need to extract.
         Place it in a seperate instruction before this one */
      rtx temp_reg = gen_reg_rtx(QImode);
      rtx extract = gen_rtx_SET(QImode, temp_reg, arg);

      if(dump_file)
      {
        fprintf(dump_file, "\nModifying insn %d, extracting subreg to new instruction %d\n",
                INSN_UID(insn), INSN_UID(extract));
      }

      /* Replace expression in instruction with our new temp register */
      memcpy(parg, &temp_reg, sizeof(rtx));
      emit_insn_before(extract, insn);

      if(dump_file)
      {
        fprintf(dump_file, "New sequence:\n");
        print_rtl_single(dump_file, extract);
        print_rtl_single(dump_file, insn);
      }
    }
  }
}

static bool
gate_tms9900_subreg (void)
{
  return true;
}

static unsigned int
tms9900_subreg (void)
{
  // printf("%s\n", __func__);
  return 0;
  basic_block bb;
  rtx insn;

  FOR_EACH_BB (bb)
    FOR_BB_INSNS (bb, insn)
    // printf("%s looping\n", __func__);
    if (INSN_P (insn))
    {
      // printf("%s get single_set\n", __func__);
      rtx set=single_set (insn);
      if(set !=NULL)
      {
        /* We only need to handle cases where there may be
           subreg expressions in an operation in the source
           argument. Unary expressions are already handled
           in the machine description. */
        rtx src=SET_SRC(set);
        if(BINARY_P(src))
        {
          tms9900_extract_subreg(insn, XEXP(src,0), &XEXP(src,0));
          tms9900_extract_subreg(insn, XEXP(src,1), &XEXP(src,1));
        }
      }
    }
    // else
    //   printf("%s not INSN_P\n", __func__);

  // printf("%s done\n", __func__);
  return 0;
}


struct rtl_opt_pass pass_tms9900_subreg =
{
 {
  RTL_PASS,
  "tms9900_subreg",                     /* name */
  gate_tms9900_subreg,                  /* gate */
  tms9900_subreg,                       /* execute */
  NULL,                                 /* sub */
  NULL,                                 /* next */
  0,                                    /* static_pass_number */
  0,                                    /* tv_id */
  0,                                    /* properties_required */
  0,                                    /* properties_provided */
  0,                                    /* properties_destroyed */
  0,                                    /* todo_flags_start */
  TODO_dump_func |
  TODO_ggc_collect                      /* todo_flags_finish */
 }
};

//==================================================================
// Code for tms9900_postinc pass

/* last instruction to use target register */
struct reg_last_used {
  rtx insn;
  int regnum;
  int is_deref;
  int mode;
  rtx parent;
  int argnum;
};

static struct reg_last_used reg_last_insn[1024] = {0};


static bool
gate_tms9900_postinc(void)
{
  return true;
}


/* Find an instruction which uses a form like *(register+constant) we could 
   merge via pointer postincrement in an earlier instruction.*/
static void
tms9900_find_merge_insn(rtx insn, rtx parent, int argnum, rtx arg)
{
  if(MEM_P(arg))
  {
    rtx expr = XEXP(arg,0);
    if(GET_CODE(expr) == PLUS)
    {
      rtx val1 = XEXP(expr,0);
      rtx val2 = XEXP(expr,1);
      int offset;
      int regnum = -1;
      rtx reg;

      /* Isolate regnum and offset in sum */
      if(CONST_INT_P(val1) && REG_P(val2))
      {
         offset = INTVAL(val1);
         regnum = REGNO(val2);
         reg = val2;
      }
      else if(CONST_INT_P(val2) && REG_P(val1))
      {
         offset = INTVAL(val2);
         regnum = REGNO(val1);
         reg = val1;
      }

      if((regnum >= 0) && 
         (offset == 2 || offset == 1) &&
         (find_regno_note(insn, REG_DEAD, regnum)))
      {
        /* Found an indexed address with a small offset, investigate further */
        struct reg_last_used *last = &reg_last_insn[regnum];
        if(dump_file)
        {
          fprintf(dump_file,"\n\nPossible merge candidate:\n");
          print_rtl_single(dump_file, insn);
          fprintf(dump_file,"\nLast use of reg %d was in insn %d:\n", regnum, INSN_UID(last->insn));
          print_rtl_single(dump_file, last->insn);
        }

        if((last->is_deref) && (GET_MODE_SIZE(last->mode) == offset))
        {
          /* Modify previous instruction to use postincrement */
          rtx temp_inc = gen_rtx_POST_INC(last->mode, reg);
          rtx temp_arg = gen_rtx_MEM(last->mode,temp_inc); 
          memcpy(XEXP(last->parent, last->argnum), temp_arg, rtx_size(temp_arg));

          /* Modify this instruction to remove index */
          temp_arg = gen_rtx_MEM(last->mode,reg); 
          memcpy(XEXP(parent, argnum), temp_arg, rtx_size(temp_arg));

          if(dump_file) 
          {
            fprintf(dump_file,"\nModified instruction %d:\n",INSN_UID(last->insn));
            print_rtl_single(dump_file,last->insn);
          }
        }
        else if(dump_file) fprintf(dump_file, "\nCannot merge\n");
        return;
      }
    }
  }

  if(BINARY_P(arg))
  {
    /* This is a binary expression, check the children */
    tms9900_find_merge_insn(insn, arg, 0, XEXP(arg,0));
    tms9900_find_merge_insn(insn, arg, 1, XEXP(arg,1));
  }
  else
  {
    /* This is a leaf expression. Note the register used here for later */
    int is_deref = 0;
    if(MEM_P(arg))
    {
      arg = XEXP(arg,0);
      is_deref = 1;
    }
    if(REG_P(arg))
    {
      int index = REGNO(arg);
      reg_last_insn[index].insn = insn;
      reg_last_insn[index].is_deref = is_deref;
      reg_last_insn[index].mode = GET_MODE(arg);     
      reg_last_insn[index].regnum = REGNO(arg);
      reg_last_insn[index].parent = parent;
      reg_last_insn[index].argnum = argnum;
    }
  }
}


static unsigned int
tms9900_postinc (void)
{
  basic_block bb;
  rtx insn;
  int i;

  FOR_EACH_BB (bb)
  {
    memset(reg_last_insn, 0, sizeof(reg_last_insn));
    FOR_BB_INSNS (bb, insn)
    {
      if (INSN_P (insn))
      {
        rtx set=single_set (insn);
        if(set !=NULL)
        {
          /* Look for memory references of the form *(costant+reg) */
          tms9900_find_merge_insn(insn, set, 0, SET_DEST(set));
          tms9900_find_merge_insn(insn, set, 1, SET_SRC(set));
        }
      }
    }
  }
  return 0;
}


struct rtl_opt_pass pass_tms9900_postinc =
{
 {
  RTL_PASS,
  "tms9900_postinc",                    /* name */
  gate_tms9900_postinc,                 /* gate */
  tms9900_postinc,                      /* execute */
  NULL,                                 /* sub */
  NULL,                                 /* next */
  0,                                    /* static_pass_number */
  0,                                    /* tv_id */
  0,                                    /* properties_required */
  0,                                    /* properties_provided */
  0,                                    /* properties_destroyed */
  0,                                    /* todo_flags_start */
  TODO_dump_func |
  TODO_ggc_collect                      /* todo_flags_finish */
 }
};

// MGB additions start here - mostly for debug

#include <stdlib.h>
#include <stdarg.h>

extern void tms9900_inline_debug (const char *fmt,...)
{
#ifndef TMS9900_DEBUG
    return;
#endif

    FILE *file = outputFile?outputFile:stdout;

    va_list ap;

    va_start (ap, fmt);
    vfprintf (file, fmt, ap);
    va_end (ap);
}

extern void tms9900_debug_operands (const char *name, rtx ops[], int count)
{
#ifndef TMS9900_DEBUG
    return;
#endif

    FILE *file = outputFile?outputFile:stdout;

    static int refcount;
    fprintf(file, "\n; %s-%d\n", name, ++refcount);
    for (int i = 0; i < count; i++)
    {
        fprintf(file, "; OP%d : ", i);

        /* For print_inline_rtx to prefix its output with a comment indicator.
         * This is similar to passing -dP to gcc but more specific to our needs
         */
        extern const char *print_rtx_head;
        print_rtx_head = "; ";

        print_inline_rtx (file, ops[i], 0);
        fprintf (file, "code=[%s:%s]\n", GET_RTX_NAME(GET_CODE(ops[i])),
                 GET_MODE_NAME (GET_MODE (ops[i])));
    }
    fprintf (file, "\n");
}

