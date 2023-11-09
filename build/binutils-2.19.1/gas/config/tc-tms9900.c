/* tc-tms9900.c -- Assemble code for the Texas Instruments TMS9900
   Copyright 2009 Eric Welser

   This file is part of GAS, the GNU Assembler.

   GAS is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
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

#include "as.h"
#include "safe-ctype.h"
#include "subsegs.h"


/* Exported constants.  */
const char line_separator_chars[] = "\0";
const char EXP_CHARS[] = "eE\0";
const char FLT_CHARS[] = "RrFf\0";
const char line_comment_chars[] = "#\0";

/* The TMS9900 assembly format specifies "*" for the start of comments.
   Unfortunately, that character is also used in register expressions.
   So we have to manually handle comments here, and cannot rely on GAS's
   normal comment parsing */
const char tms9900_comment_chars[] = "#;\0";

/* List of chars besides those in app.c:symbol_chars that can start an
   operand.  Used to prevent the scrubber eating vital white-space.
   We use this to determine if the asterisk is the start of a comment */
const char tms9900_symbol_chars[] = "+*\0";

/* For machine specific options.  */
const char * md_shortopts = ""; /* None yet.  */

struct option md_longopts[] =
{
  { NULL, no_argument, NULL, 0 }
};

size_t md_longopts_size = sizeof (md_longopts);

extern int coff_flags;


int
md_parse_option (int c, char* arg ATTRIBUTE_UNUSED)
{
  switch (c)
    {
    default:
      return 0;
    }
}


void
md_show_usage (FILE * f)
{
  fprintf(f,"\n");
}

static symbolS * zero;


void
md_begin (void)
{
  expressionS nul;
  char * p;

  p = input_line_pointer;
  input_line_pointer = "0";
  nul.X_md=0;
  expression (& nul);
  input_line_pointer = p;
  zero = make_expr_symbol (& nul);
  /* We do not use relaxation (yet).  */
  linkrelax = 0;
}


void
tms9900_md_end (void)
{
  bfd_set_arch_mach (stdoutput, TARGET_ARCH, bfd_mach_tms9900);
}


static const char *
skip_space (const char *s)
{
  while (*s == ' ' || *s == '\t')
    ++s;
  return s;
}


symbolS *
md_undefined_symbol (char *name ATTRIBUTE_UNUSED)
{
  return NULL;
}


char *
md_atof (int type ATTRIBUTE_UNUSED, char *litP ATTRIBUTE_UNUSED,
	 int *sizeP ATTRIBUTE_UNUSED)
{
  return _("floating point numbers are not implemented");
}


valueT
md_section_align (segT seg ATTRIBUTE_UNUSED, valueT size)
{
  return size;
}


long
md_pcrel_from (fixS * fixp)
{
  return fixp->fx_where +
    fixp->fx_frag->fr_address + 1;
}


/* Compares the key for structs that start with a char * to the key.  */
static int
key_cmp (const void * a, const void * b)
{
  const char *str_a, *str_b;

  str_a = *((const char**)a);
  str_b = *((const char**)b);
  return strcmp (str_a, str_b);
}


static int
key_regcmp (const void * a, const void * b)
{
  const char *str_a, *str_b;

  str_a = *((const char**)a);
  str_b = *((const char**)b);
  return strncasecmp (str_a, str_b, strlen(str_b));
}


#define BUFLEN 8 /* Large enough for any keyword.  */

char buf[BUFLEN];
const char *key = buf;

static const struct reg_entry
{
  char* name;
  int number;
} regtable[] =
{
  {"cb",  12 },  /* CRU base */
  {"lp",  14 },  /* Old Program Counter register after BLWP */
  {"lr",  11 },  /* Return address after BL */
  {"ls",  15 },  /* Old Status Register after BLWP */
  {"lw",  13 },  /* Old Workspace Register after BLWP */
  {"r0",   0 },
  {"r1",   1 },
  {"r10", 10 },
  {"r11", 11 },
  {"r12", 12 },
  {"r13", 13 },
  {"r14", 14 },
  {"r15", 15 },
  {"r2",   2 },
  {"r3",   3 },
  {"r4",   4 },
  {"r5",   5 },
  {"r6",   6 },
  {"r7",   7 },
  {"r8",   8 },
  {"r9",   9 },
  {"sp",  10 }   /* Stack pointer */
} ;

/* Prevent an error on a line from also generating
   a "junk at end of line" error message.  */
static char err_flag;

static void tms9900_bss (int unused ATTRIBUTE_UNUSED);


int
tms9900_start_line_hook (void)
{
  /* Elements of code grammar */
  enum
    {
      ST_START,         /* Start of line */
      ST_LABEL,         /* Code label */
      ST_PRE_DIRECTIVE, /* Space before directive or mnemonic*/
      ST_DIRECTIVE,     /* Assembly directive */
      ST_PRE_OPTION,    /* Space before directive option */
      ST_OPTION,        /* Directive option */
      ST_MNEMONIC,      /* Opcode mnemonic */
      ST_PRE_ARGUMENT,  /* Space before argument */
      ST_ARGUMENT,      /* Non-string argument */
      ST_POST_ARGUMENT, /* Space or comma after argument */
      ST_TI_STRING,     /* TI-style string */
      ST_GAS_STRING,    /* GAS-style string */
      ST_POST_STRING,   /* Space or comma after string */
      ST_COMMENT        /* Comment at end of line */
    };
  int state = ST_START;
  char *p;

  for (p = input_line_pointer; *p && *p != '\n'; ++p)
    {
      switch(state)
        {
          case ST_START:
          if(strchr(" \t",*p)) state = ST_PRE_DIRECTIVE;
          else if(*p == '*') state = ST_COMMENT;
          else state = ST_LABEL;
          p--;
          break;

          case ST_LABEL:
          if(strchr(" \t",p[1])) state = ST_PRE_DIRECTIVE;
          break;

          case ST_PRE_DIRECTIVE:
          if(strchr(" \t",p[1]) == NULL)
            {
              if(p[1] == '.') state = ST_DIRECTIVE;
              else if(p[1] == '*') state = ST_COMMENT;
              else state = ST_MNEMONIC;
            }
          break;

          case ST_DIRECTIVE:
          if(strchr(" \t",p[1])) state = ST_PRE_OPTION;
          break;

          case ST_PRE_OPTION:
          if(strchr(" \t",p[1]) == NULL) state = ST_OPTION;
          break;

          case ST_OPTION:
          if(strchr(" \t",p[1])) state = ST_OPTION;
          if(p[0] == '*') {p--; state = ST_COMMENT;}
          break;

          case ST_MNEMONIC:
          if(strchr(" \t",p[1])) state = ST_PRE_ARGUMENT;
          break;

          case ST_PRE_ARGUMENT:
          if(strchr(" \t",p[0]) == NULL)
            {
              if(p[0] == '\'') state = ST_TI_STRING;
              else if(p[0] == '"') state = ST_GAS_STRING;              
              else {state = ST_ARGUMENT; p--;}
            }
          break;

          case ST_ARGUMENT:
          if(p[0] == '>' && strchr("0123456789abcdefABCDEF",p[1])) p[0] = '$';
          if(strchr(" \t,",p[1]))
            {
              if(p[0] == '*') {p--; state = ST_COMMENT;}
              else state = ST_POST_ARGUMENT;
            }
          break;

          case ST_POST_ARGUMENT:
          if(strchr(" \t",p[0]) == NULL)
            {
              if(p[0] == ',') state = ST_PRE_ARGUMENT;
              else {p--; state = ST_COMMENT;}
            }
          break;

          case ST_TI_STRING:
          if(p[0] == '\'' && p[1] == '\'') p++;  /* Skip escaped quote */
          else if(p[0] == '\'') state = ST_POST_STRING;
          break;

          case ST_GAS_STRING:
          if(p[0] == '\\' && p[1] == '"') p++;  /* Skip escaped quote */
          else if(p[0] == '"') state = ST_POST_STRING;
          break;

          case ST_POST_STRING:
          if(strchr(" \t",*p) == NULL)
            {
              if(*p == ',') state = ST_PRE_ARGUMENT;
              else {p--; state = ST_COMMENT;}
            }
          break;

          case ST_COMMENT:
          memset(p, ' ', strcspn(p,"\n"));
          state = ST_PRE_ARGUMENT;
          break;
        }
    }
  /* Remove trailing whitespace */
  p = input_line_pointer;
  p += strcspn(p,"\n");
  char *line_end = p;
  while(p > input_line_pointer && strchr(" \t\n",p[0])) p--;
  if(p > input_line_pointer)
    {
      /* Remove characters occupied by trailing whitespace */
      p++;
      memmove(p,line_end,strlen(line_end)+1);
      memset(&p[strlen(p)],0,line_end-p);
    }

  /* Check for <label>[:] [.](EQU|BES) <value>.  */
  if (is_name_beginner (*input_line_pointer))
    {
      char c, *rest, *line_start;
      int code;
      int len = 3;  /* Length of directive */
      symbolS * symbolP;

      line_start = input_line_pointer;
      LISTING_NEWLINE ();
      if (ignore_input ())
	return 0;

      c = get_symbol_end ();
      rest = input_line_pointer + 1;

      if (*rest == ':')
	++rest;
      if (*rest == ' ' || *rest == '\t')
	++rest;
      if (*rest == '.')
	++rest;
      if (strncasecmp (rest, "EQU", 3) == 0)
	code = 1;
      else if (strncasecmp (rest, "BES", 3) == 0)
        code = 2;
      else
	code = 0;

      if (code && (rest[len] == ' ' || rest[len] == '\t'))
	{
          if ( code == 1)
            {
              /* This is an EQU operative */
              /* Handle assignment here.  */
              input_line_pointer = rest + len;
              if (line_start[-1] == '\n')
                bump_line_counters ();
              /* Make sure we don't redefine an existing equate */
              if ((symbolP = symbol_find (line_start)) != NULL) 
                {
                  if (S_IS_DEFINED (symbolP) || symbol_equated_p (symbolP))
                    as_bad (_("symbol `%s' is already defined"), line_start);
                }
              equals (line_start, 1);
              return 1;
            }
          else if ( code == 2)
            {
              /* This is a BES operative */
              /* Allocate space */
              input_line_pointer = rest + 3;
              tms9900_bss(0);
              /* Move label to after allocation */
              while(rest-- > line_start)
                {
                  (input_line_pointer--)[-1] = rest[-1];
                }
            }
	}
      else
	{
	  /* Restore line and pointer.  */
	  *input_line_pointer = c;
	  input_line_pointer = line_start;
	}
    }
  return 0;
}


int tms9900_parse_name(const char* name, struct expressionS* expr)
{
    struct reg_entry* regp;
    regp = bsearch (&name, regtable, ARRAY_SIZE (regtable),
                    sizeof (regtable[0]), key_regcmp);
    if(regp && strcmp(name, regp->name)==0)
    {
      expr->X_op = O_register;
      expr->X_add_number = regp->number;
      input_line_pointer = (char*)name + strlen(regp->name);
      return(1); 
    }
  return(0);
}


static char* assemble_mode(const char *line, short* opcode, expressionS* exp)
{
/*
 Encodings for the Ts field of the opcode
 00 -  REG
 01 - *REG
 11 - *REG+
 10 - @EXP      reg=0
 10 - @EXP(REG)
*/
  char *p = (char*)line;
  int open = 0;  /* No open paren around register */
  int maybe_increment = 0;
  *opcode = 0;
  exp->X_op = O_absent;

  if(*p == '*')
    {
      /* Indirect by register */
      *opcode = 0x10;
      maybe_increment = 1;
      p++;
    }
  else if(*p == '@')
    {
      /* Indirect by expression */
      *opcode = 0x20;
      /* This is an expression, parse it */
      input_line_pointer = ++p;
      expression(exp);
      p=input_line_pointer;
      if(*p == '(')
        {
          open = 1;
          p++;
        }
    }

  if(*p != '*' &&
     *p != ',' &&
     *p != 0)
    {
      /* This term uses a register, let's find out which one */
      struct reg_entry* regp;
      regp = bsearch (&p, regtable, ARRAY_SIZE (regtable),
                      sizeof (regtable[0]), key_regcmp);
      if(regp)
        {
          *opcode |= regp->number;
          p += strlen(regp->name);
        }
      else
        {
          /* Handle registers in numeric format */
          unsigned int regnum=256;
          if(sscanf(p,"%u", &regnum) && regnum < 16)
            {
              p++;
              if(regnum > 9) p++;
              *opcode |= regnum;
            }
          else
            {
              printf("Invalid register starting at \"%s\"\n",p);
            }
        }
    }

  /* Do we have register post increment? */
  if(maybe_increment == 1 && *p == '+')
    {
      *opcode |= 0x20;
      p++;
    }
  
  /* Cleanup end of parameter */
  if(open == 1)
    {
      if(*p == ')')  p++;
        else printf("Unclosed paren around register\n");
    }

  input_line_pointer = p;        
  return(p);
}


void tms9900_cons_fix_new (fragS *frag_p, int offset, int nbytes, expressionS *exp)
{
  bfd_reloc_code_real_type type = BFD_RELOC_16;
  int pcrel = 0;
  switch(nbytes)
    {
      case 1:
        type = BFD_RELOC_8_PCREL;
        pcrel = 1;
      case 2:
        fix_new_exp (frag_p, offset, nbytes, exp, pcrel, type);
        break;

      case 4:
        /* Place relocation into low word. Leave high word as zero */
        fix_new_exp (frag_p, offset+2, nbytes, exp, 0, type);
        break;

      default:
        as_bad (_("unsupported BFD relocation size %u"), nbytes);
    }
}


enum
{
  TEXT_DATAONLY = -1,
  TEXT_PRESIZE  = -2,
  TEXT_ZEROTERM = -3
};


static void
tms9900_data (int size)
{
  const char *p;
  char *u, quote;
  int cnt;
  expressionS exp;

  if (is_it_end_of_statement ())
    {
      /* Handle instructions which only allocate space */
      demand_empty_rest_of_line ();
      if(size > 0)
        {
          u = frag_more (size);
          memset (u, 0, size);
        }
      return;
    }

  /* Skip to data value */  
  p = skip_space (input_line_pointer);

  do
    {
      if (*p == '\"' || *p == '\'')
	{
          /* String constant, remove escape codes from string */
          char q[1024];
          char *out = q;
          quote = *p++;
          if(quote == '\'')
          {
            /* TI-style quote, only handle escaped quote character */
            while(p[0] && (p[0] != '\'' || (p[0] == '\'' && p[1] == '\'')))
              {
                if(p[0] == '\'' && p[1] == '\'') p++;
                *out++ = *p++;
              }
            if(p[0] == '\'') p++;
          }
          else
          {
             /* GAS-style quote, handle standard escape sequences */
             int c;
             input_line_pointer = p;
             while (is_a_char (c = next_char_of_string ()))
             {
               *out++ = c;
             }
             p = input_line_pointer;
          }
          *out = 0;
          cnt = out - q;

          /* Check for length and validity */
          if(size > 0)
            {
              if(cnt > size)
                {
                  as_warn(_("string constant too long"));
                  cnt = size;
                }
              u = frag_more (size);
              if(size == 2 && cnt == 1) u[1] = q[0];
                else  memcpy (u, q, cnt);
            }
          else if(size == TEXT_DATAONLY)
            {
              u = frag_more (cnt);
              memcpy (u, q, cnt);
            }
          else if(size == TEXT_PRESIZE)
            {
              if(cnt > 255)
                {
                  as_warn(_("string constant too long"));
                  cnt = 255;
                }
              u = frag_more (cnt+1);
              *u = cnt;             
              memcpy (u + 1, q, cnt);
            }
          else if(size == TEXT_ZEROTERM)
            {
              u = frag_more (cnt + 1);
              memcpy (u, q, cnt);
              u[cnt] = 0;
            }
          if (!*p)
            as_warn (_("unterminated string"));
	}
      else
	{
          /* This is a non-string value */
          input_line_pointer = (char*)p;
	  expression(&exp);
          p = (const char*)input_line_pointer;
	  if (exp.X_op == O_register)
	    {
	      break;
	    }
	  if (exp.X_op == O_constant)
            {
              int max;
              int val = exp.X_add_number;
              if(size < 0)
                {
                  break;
                }
              max = (1 << (size * 8)) - 1;
              if(val > max || val < -max)
                {
                  as_warn(_("constant too big"));
                  val = val & ((1 << (size * 8)) - 1);
                }
              u = frag_more (size);
              if(size == 1)
                {
                  u[0] = val & 0xFF;
                }
              else
                {
                  u[0] = (val >> 8) & 0xFF;
                  u[1] = val & 0xFF;
                }
            }
          else if(exp.X_op == O_symbol &&
                  size == 2)
            {
              u = frag_more(2);
              fix_new_exp (frag_now,
                           u - frag_now->fr_literal,
                           2, &exp, 0,
                           BFD_RELOC_16);

            }
	}
       p = skip_space (p);
    }
  while (*p++ == ',');
  input_line_pointer = (char *)(p-1);
}

static void
tms9900_even (int unused ATTRIBUTE_UNUSED)
{
  /* Align to 16 bit boundary, fill with zeroes */
  s_align_ptwo(1);
}

static void
tms9900_bss (int unused ATTRIBUTE_UNUSED)
{
  expressionS exp;
  expression(&exp);
  if(exp.X_op == O_constant)
    {
      char* p = frag_more(exp.X_add_number);
      memset(p, 0, exp.X_add_number);      
    }
  else
    {
      as_bad (_("junk at end of line, first unrecognized character is `%c'"),
              *input_line_pointer);
    }
}

static void
tms9900_seg_bss (int unused ATTRIBUTE_UNUSED)
{
  subseg_set (bss_section,  now_subseg);
  demand_empty_rest_of_line ();
}

static void
tms9900_seg_data (int unused ATTRIBUTE_UNUSED)
{
  subseg_set (data_section,  now_subseg);
  demand_empty_rest_of_line ();
}

static void
tms9900_seg_text (int unused ATTRIBUTE_UNUSED)
{
  subseg_set (text_section,  now_subseg);
  demand_empty_rest_of_line ();
}

/* Port specific pseudo ops.  */
const pseudo_typeS md_pseudo_table[] =
{
  { ".bss",  tms9900_seg_bss, 0},
  { ".data", tms9900_seg_data, 0},
  { ".text", tms9900_seg_text, 0},


  /* AORG is a commonly-used directive, but we will have to handle that
     in the linker, since we can't assign a value for the target address
     from within GAS. */
  { "aorg",  s_ignore, 0},

  /* TI convention for segments */
  { "pseg",  tms9900_seg_text, 0},
  { "pend",  s_ignore, 0},
  { "cseg",  tms9900_seg_bss, 0},
  { "cend",  tms9900_seg_text, 0},
  { "dseg",  tms9900_seg_data, 0},
  { "dend",  tms9900_seg_text, 0},

  /* Reservation for data values */
  { "bes",   tms9900_bss, 0},
  { "bss",   tms9900_bss, 0},
  { "byte",  tms9900_data, 1},
  { "data",  tms9900_data, 2}, 
  { "end",   s_ignore, 0},
  { "def",   s_globl, 0},
  { "even",  tms9900_even, 0},
  { "ref",   s_ignore, 0},
  
  /* The SYM directive displays a list of all symbols, and their values */
  { "sym",   s_ignore, 0},
  { "ntext", tms9900_data, TEXT_PRESIZE},
  { "text",  tms9900_data, TEXT_DATAONLY},
  { "textz", tms9900_data, TEXT_ZEROTERM},

  /* Aliases for the text drirectives */
  { "nstring", tms9900_data, TEXT_PRESIZE},
  { "string",  tms9900_data, TEXT_DATAONLY},
  { "stringz", tms9900_data, TEXT_ZEROTERM},

  { NULL, 0, 0 }
} ;

enum
{
  ARG_NONE,       /* No argument            */
  ARG_MODE,       /* Addressing mode        */
  ARG_OFFSET,     /* Branch offset          */
  ARG_REGISTER,   /* Register number        */
  ARG_IMMEDIATE,  /* 16-bit immediate value */
  ARG_SHIFT,      /* Bit shift count        */
  ARG_CRU_COUNT,  /* CRU bit count          */
  ARG_CRU_DISP    /* CRU displacement       */
};

typedef struct _table_t
{
  char* name;
  short opcode;
  int arg[2];
} table_t;

static table_t instab[] =
{
  { "a",    0xA000, {ARG_MODE,      ARG_MODE}},
  { "ab",   0xB000, {ARG_MODE,      ARG_MODE}},
  { "abs",  0x0740, {ARG_MODE,      ARG_NONE}},
  { "ai",   0x0220, {ARG_REGISTER,  ARG_IMMEDIATE}},
  { "andi", 0x0240, {ARG_REGISTER,  ARG_IMMEDIATE}},
  { "b",    0x0440, {ARG_MODE,      ARG_NONE}},
  { "bl",   0x0680, {ARG_MODE,      ARG_NONE}},
  { "blwp", 0x0400, {ARG_MODE,      ARG_NONE}},
  { "c",    0x8000, {ARG_MODE,      ARG_MODE}},
  { "cb",   0x9000, {ARG_MODE,      ARG_MODE}},
  { "ci",   0x0280, {ARG_REGISTER,  ARG_IMMEDIATE}},
  { "ckof", 0x03C0, {ARG_NONE,      ARG_NONE}},
  { "ckon", 0x03A0, {ARG_NONE,      ARG_NONE}},
  { "clr",  0x04C0, {ARG_MODE,      ARG_NONE}},
  { "coc",  0x2000, {ARG_MODE,      ARG_REGISTER}},
  { "czc",  0x2400, {ARG_MODE,      ARG_REGISTER}},
  { "dec",  0x0600, {ARG_MODE,      ARG_NONE}},
  { "dect", 0x0640, {ARG_MODE,      ARG_NONE}},
  { "div",  0x3C00, {ARG_MODE,      ARG_REGISTER}},
  { "idle", 0x0340, {ARG_NONE,      ARG_NONE}},
  { "inc",  0x0580, {ARG_MODE,      ARG_NONE}},
  { "inct", 0x05C0, {ARG_MODE,      ARG_NONE}},
  { "inv",  0x0540, {ARG_MODE,      ARG_NONE}},
  { "jeq",  0x1300, {ARG_OFFSET,    ARG_NONE}},
  { "jgt",  0x1500, {ARG_OFFSET,    ARG_NONE}},
  { "jh",   0x1B00, {ARG_OFFSET,    ARG_NONE}},
  { "jhe",  0x1400, {ARG_OFFSET,    ARG_NONE}},
  { "jl",   0x1A00, {ARG_OFFSET,    ARG_NONE}},
  { "jle",  0x1200, {ARG_OFFSET,    ARG_NONE}},
  { "jlt",  0x1100, {ARG_OFFSET,    ARG_NONE}},
  { "jmp",  0x1000, {ARG_OFFSET,    ARG_NONE}},
  { "jnc",  0x1700, {ARG_OFFSET,    ARG_NONE}},
  { "jne",  0x1600, {ARG_OFFSET,    ARG_NONE}},
  { "jno",  0x1900, {ARG_OFFSET,    ARG_NONE}},
  { "joc",  0x1800, {ARG_OFFSET,    ARG_NONE}},
  { "jop",  0x1C00, {ARG_OFFSET,    ARG_NONE}},
  { "ldcr", 0x3000, {ARG_MODE,      ARG_CRU_COUNT}},
  { "li",   0x0200, {ARG_REGISTER,  ARG_IMMEDIATE}},
  { "limi", 0x0300, {ARG_IMMEDIATE, ARG_NONE}},
  { "lrex", 0x03E0, {ARG_NONE,      ARG_NONE}},
  { "lwpi", 0x02E0, {ARG_IMMEDIATE, ARG_NONE}},
  { "mov",  0xC000, {ARG_MODE,      ARG_MODE}},
  { "movb", 0xD000, {ARG_MODE,      ARG_MODE}},
  { "mpy",  0x3800, {ARG_MODE,      ARG_REGISTER}},
  { "neg",  0x0500, {ARG_MODE,      ARG_NONE}},
  { "nop",  0x1000, {ARG_NONE,      ARG_NONE}},  /* Encoded as "jmp 0"  */
  { "or",   0xE000, {ARG_MODE,      ARG_MODE}},  /* Encoded as "soc"    */
  { "orb",  0xF000, {ARG_MODE,      ARG_MODE}},  /* Encoded as "socb"   */
  { "ori",  0x0260, {ARG_REGISTER,  ARG_IMMEDIATE}},
  { "rset", 0x0360, {ARG_NONE,      ARG_NONE}},
  { "rt",   0x045B, {ARG_NONE,      ARG_NONE}},  /* Encoded as "b *r11" */
  { "rtwp", 0x0380, {ARG_NONE,      ARG_NONE}},
  { "s",    0x6000, {ARG_MODE,      ARG_MODE}},
  { "sb",   0x7000, {ARG_MODE,      ARG_MODE}},
  { "sbo",  0x1D00, {ARG_CRU_DISP,  ARG_NONE}},
  { "sbz",  0x1E00, {ARG_CRU_DISP,  ARG_NONE}},
  { "seto", 0x0700, {ARG_MODE,      ARG_NONE}},
  { "sla",  0x0A00, {ARG_REGISTER,  ARG_SHIFT}},
  { "soc",  0xE000, {ARG_MODE,      ARG_MODE}},
  { "socb", 0xF000, {ARG_MODE,      ARG_MODE}},
  { "sra",  0x0800, {ARG_REGISTER,  ARG_SHIFT}},
  { "src",  0x0B00, {ARG_REGISTER,  ARG_SHIFT}},
  { "srl",  0x0900, {ARG_REGISTER,  ARG_SHIFT}},
  { "stcr", 0x3400, {ARG_MODE,      ARG_CRU_COUNT}},
  { "stst", 0x02C0, {ARG_REGISTER,  ARG_NONE}},
  { "stwp", 0x02A0, {ARG_REGISTER,  ARG_IMMEDIATE}},
  { "swpb", 0x06C0, {ARG_MODE,      ARG_NONE}},
  { "szc",  0x4000, {ARG_MODE,      ARG_MODE}},
  { "szcb", 0x5000, {ARG_MODE,      ARG_MODE}},
  { "tb",   0x1F00, {ARG_CRU_DISP,  ARG_NONE}},
  { "x",    0x0480, {ARG_MODE,      ARG_NONE}},
  { "xop",  0x2C00, {ARG_MODE,      ARG_REGISTER}},
  { "xor",  0x2800, {ARG_MODE,      ARG_REGISTER}}
};

static int
assemble_arg(int argtype, short *opcode, int *op_size, expressionS *exp)
{
  /* argtype - type of argument to parse
     opcode  - bits to insert into opcode
     op_size - size of opcode bits
     expr    - expression for branch target or immediate value
  */

  char *start = input_line_pointer;

  exp->X_op = O_absent;
  *opcode = 0;
  *op_size = 0;  
  int valid = 0;

  if(argtype == ARG_MODE)
    {
      assemble_mode(input_line_pointer, opcode, exp);
      *op_size = 6;
      valid = 1;
    }
  else if(argtype != ARG_NONE)
    {
      char *ptr = input_line_pointer;
      expression(exp);
      switch(argtype)
        {
          /* Branch offset */
          case ARG_OFFSET:     
            if(exp->X_op == O_constant ||
               exp->X_op == O_symbol)
              {
                valid = 1;
              }
            if(exp->X_op == O_constant)
              {
                *opcode = (exp->X_add_number >> 1) & 0xFF;
                *op_size = 8;
                exp->X_op = O_absent;
              }
            break;

          /* Register number */
          case ARG_REGISTER:
            if(exp->X_op == O_register ||
               /* The expression parser cannot handle numeric registers.
                  Instructions like "mov 1,2" must be handled here
               */
               (exp->X_op == O_constant &&
                  (unsigned int)exp->X_add_number < 16))
              {
                *opcode = exp->X_add_number;
                *op_size = 4;
                exp->X_op = O_absent;
                valid = 1;
              }
            else
              {
                as_bad (_("not a register '%s'"), start);
              }
            break;

          /* 16-bit immediate value */
          case ARG_IMMEDIATE:
            if(exp->X_op == O_constant && *ptr++ == '\'')
              {
                /* Could be a constant like 'a' or 'ab'
                   These values can be no longer than two characters.
                   Since TI escapes the single quote by duplication,
                   the code below is uglier than it needs to be.

                   Expected translations:
                     ''     -> 0x0027
                     ''''   -> 0x0027
                     '''''' -> 0x2727
                */
                int i, val = 0;
                for(i = 0; i < 2; i++)
                  {
                    if(*ptr == '\'' && ptr[1] == '\'') ptr++;
                    val = (val << 8) | *ptr++;
                    if((*ptr == '\'' && ptr[1]!='\'') ||
                       *ptr == 0    ||
                       *ptr == '\n') break;
                  }
                if(*ptr == '\n' ||
                   *ptr == 0) as_warn(_("unterminated string"));
                else if(*ptr != '\'' ||
                        (*ptr == '\'' && ptr[1] == '\''))
                  {
                    as_warn(_("constant too big"));
                  }

                /* Use the value we found */
                exp->X_add_number = val;
                valid = 1;
              }
            else if(exp->X_op == O_constant ||
                    exp->X_op == O_symbol)
              {
                valid = 1;
              }
            else
              {
                as_bad (_("not a constant expression '%s'"), start);
              }
            break;

          /* Bit shift count */
          case ARG_SHIFT:
            if(exp->X_op == O_constant ||
               (exp->X_op == O_register && exp->X_add_number == 0))
              {
                *opcode = exp->X_add_number;
                *op_size = 4;
                exp->X_op = O_absent;
                valid = 1;
              }
            break;

          /* CRU bit count */
          case ARG_CRU_COUNT:
            if(exp->X_op == O_constant)
              {
                *opcode = exp->X_add_number;
                *op_size = 4;
                exp->X_op = O_absent;
                valid = 1;
              }
            else
              {
                as_bad (_("not a constant expression '%s'"), input_line_pointer);
              }
            break;

          /* CRU displacement */
          case ARG_CRU_DISP:
            if(exp->X_op == O_constant)
              {
                *opcode = exp->X_add_number;
                *op_size = 8;
                exp->X_op = O_absent;
                valid = 1;
              }
            else
              {
                as_bad (_("not a constant expression '%s'"), input_line_pointer);
              }
            break;
        }
    }

  if(input_line_pointer == start)
    {
      as_bad (_("missing argument"));
      return(0);
    }

  /* If we found an invalid argument, revert everything done here */
  if(valid == 0)
    {
      exp->X_op = O_absent;
      input_line_pointer = start;
    }
  return(valid);
}

void
md_assemble (char* str)
{
  const char *p;
  char * old_ptr;
  int i;
  table_t *insp;
  short opcode[2];
  int op_size[2];
  expressionS exp[2];              
  char *out;
  short insn;
  int check_unex = 0;

  err_flag = 0;
  old_ptr = input_line_pointer;
  p = skip_space (str);

  /* Convert opcode to lowercase */
  for (i = 0; (i < BUFLEN) && (ISALPHA (*p));)
    buf[i++] = TOLOWER (*p++);

  if (i == BUFLEN)
    {
      /* Opcode too long */
      buf[BUFLEN-3] = buf[BUFLEN-2] = '.'; /* Mark opcode as abbreviated.  */
      buf[BUFLEN-1] = 0;
      as_bad (_("unknown instruction '%s'"), buf);
    }
  else if ((*p) && (!ISSPACE (*p)))
    as_bad (_("syntax error"));
  else 
    {
      /* Check for valid opcode */
      buf[i] = 0;
      p = skip_space (p);
      key = buf;
      
      insp = bsearch (&key, instab, ARRAY_SIZE (instab),
		    sizeof (instab[0]), key_cmp);
      if (!insp)
	as_bad (_("unknown instruction '%s'"), buf);
      else
	{
          /* Skip to start of arguments, if we have any */
          input_line_pointer = str + strlen(insp->name);
          input_line_pointer = (char*)skip_space (input_line_pointer);

          /* Initialize arguments */
          for(i=0;i<2;i++)
            {
              exp[i].X_op = O_absent;
              op_size[i] = 0;
              opcode[i] = 0;
            }

          /* Parse arguments */
          if(insp->arg[0] != ARG_NONE)
            {
              /* Parse first argument */
              if(assemble_arg(insp->arg[0], &opcode[0], &op_size[0], &exp[0]) != 0)
                {
                  check_unex = 1;
                  if(insp->arg[1] != ARG_NONE)
                    {
                      /* Parse second argument */
                      if(*input_line_pointer != ',')
                        {
                          if(*input_line_pointer == 0)
                            {
                              as_bad(_("unexpected end of line"));
                              check_unex = 0;
                            }
                          else
                            {
                              char str[64];
                              sprintf(str, "unexpected character at \"%s\"",
                                      input_line_pointer);
                              as_bad(str);
                              check_unex = 0;
                            }
                        }
                      else
                        {
                          input_line_pointer++;
                          if(assemble_arg(insp->arg[1], &opcode[1], &op_size[1], &exp[1]))
                              /* Second argument is valid, rest is comment */
                              check_unex = 0;
                        }
                    }
                }           
            }

          /* Check for unexpected characters */
          if(check_unex != 0 && 
             *input_line_pointer != 0 && 
             strspn(input_line_pointer, " *") == 0)
            {
              char str[64];
              sprintf(str, "unexpected character at \"%s\"", input_line_pointer);
              as_bad(str);
            }

          /* Make output instruction */
          insn = insp->opcode | 
                 opcode[0]    |
                 (opcode[1] << op_size[0]);

          out = frag_more(2);
          *out++ = ((insn >> 8) & 0xFF);
          *out   =  (insn       & 0xFF);

          /* Insert extra words and fixups */
          for(i=0; i<2; i++)
            {
              if(exp[i].X_op != O_absent)
                {
                  /* Assume no fixup required */
                  int reloc_type = BFD_RELOC_NONE;
                  int fix_size = 0;

                  if(insp->arg[i] == ARG_OFFSET)
                    {
                      /* 8-bit PC-relative offset, stored in instruction */
                      reloc_type = BFD_RELOC_8_PCREL;
                      fix_size = 1;
                    }
                  else
                    {
                      /* 16-bit constant value, stored after instruction */
                      unsigned short val = 0;
                      out = frag_more(2);
                      reloc_type = BFD_RELOC_16;
                      fix_size = 2;
                      if(exp[i].X_op == O_constant)
                        {
                          val = (exp[i].X_add_number & 0xFFFF);
                          fix_size = 0;
                        }
                      out[0] = ((val >> 8) & 0xFF);
                      out[1] =  (val       & 0xFF);      
                    }

                  if(fix_size != 0)
                    {
                      /* Fixup used for this instruction, configure it */
                      fix_new_exp (frag_now,
                                   out - frag_now->fr_literal,
                                   fix_size,
                                   &exp[i],
                                   0,
                                   reloc_type);
                    }
                }
            }
        }
    }

  input_line_pointer = old_ptr;
}


void
md_operand (expressionS* exp ATTRIBUTE_UNUSED)
{
  printf("Unrecognized expression: \"%s\"\n", input_line_pointer);
}


void
md_apply_fix (fixS * fixP, valueT* valP, segT seg ATTRIBUTE_UNUSED)
{
  long val = * (long *) valP;
  char *buf = fixP->fx_where + fixP->fx_frag->fr_literal;

  switch (fixP->fx_r_type)
    {
    case BFD_RELOC_8_PCREL:
      if (fixP->fx_addsy)
        {
          fixP->fx_no_overflow = 1;
          fixP->fx_done = 0;
        }
      else
        {
	  fixP->fx_no_overflow = (-128 <= val && val < 128);
	  if (!fixP->fx_no_overflow)
            as_bad_where (fixP->fx_file, fixP->fx_line,
			  _("relative jump out of range"));
	  *buf++ = val;
          fixP->fx_done = 1;
        }
      break;

    case BFD_RELOC_16:
      *buf++ = (val >> 8);
      *buf++ = val;
      fixP->fx_no_overflow = 1; 
      if (fixP->fx_addsy == NULL)
	fixP->fx_done = 1;
      break;

    default:
      printf (_("md_apply_fix: unknown r_type 0x%x\n"), fixP->fx_r_type);
      abort ();
    }
}

/* GAS will call this to generate a reloc.  GAS will pass the
   resulting reloc to `bfd_install_relocation'.  This currently works
   poorly, as `bfd_install_relocation' often does the wrong thing, and
   instances of `tc_gen_reloc' have been written to work around the
   problems, which in turns makes it difficult to fix
   `bfd_install_relocation'.  */

/* If while processing a fixup, a reloc really
   needs to be created then it is done here.  */

arelent *
tc_gen_reloc (asection *seg ATTRIBUTE_UNUSED , fixS *fixp)
{
  arelent *reloc;

  if (! bfd_reloc_type_lookup (stdoutput, fixp->fx_r_type))
    {
      as_bad_where (fixp->fx_file, fixp->fx_line,
		    _("reloc %d not supported by object file format"),
		    (int) fixp->fx_r_type);
      return NULL;
    }

  reloc               = xmalloc (sizeof (arelent));
  reloc->sym_ptr_ptr  = xmalloc (sizeof (asymbol *));
  *reloc->sym_ptr_ptr = symbol_get_bfdsym (fixp->fx_addsy);
  reloc->address      = fixp->fx_frag->fr_address + fixp->fx_where;
  reloc->howto        = bfd_reloc_type_lookup (stdoutput, fixp->fx_r_type);
  reloc->addend       = fixp->fx_offset;

  return reloc;
}

