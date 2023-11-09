/* Print TMS9900 instructions
   Copyright 2009 Eric Welser

   This file is part of the GNU opcodes library.

   This library is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free software Foundation; either version 3, or (at your option)
   any later version.

   It is distributed in the hope that it will be useful, but WITHOUT
   ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
   or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public
   License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street - Fifth Floor, Boston,
   MA 02110-1301, USA.  */

#include "sysdep.h"
#include "dis-asm.h"
#include <stdio.h>

struct buffer
{
  bfd_vma base;
  unsigned short opcode;
  int n_fetch;
  unsigned char data[6];
} ;

typedef void (*parse_func)(struct buffer *, disassemble_info *, char *);

struct parse_table
{
  parse_func func;
  char* text;
};

static int
fetch_data (struct buffer *buf, disassemble_info * info, int n)
{
  int r;

  if (buf->n_fetch + n > 6)
    abort ();

  r = info->read_memory_func (buf->base + buf->n_fetch,
			      (unsigned char*) buf->data + buf->n_fetch,
			      n, info);
  if (r == 0)
    buf->n_fetch += n;
  return !r;
}

static void parse_error(struct buffer *buffer, disassemble_info *info, char *text ATTRIBUTE_UNUSED)
{
  info->fprintf_func (info->stream, "data %02X%02X", 
                      buffer->data[0],  buffer->data[1]);  
}

static void parse_mode(struct buffer *buffer, disassemble_info *info, char *arg, int code)
{
  int mode = (code >> 4) & 0x3;
  int reg = code & 0xF;
  if(mode == 2)
    {
      unsigned char *data = &buffer->data[buffer->n_fetch];
      unsigned short value;
      if (! fetch_data (buffer, info, 2))
        return;
      value = ((unsigned short)data[0] << 8) | data[1];
      if(reg == 0)
        sprintf(arg, "@>%04X",value);
      else
        sprintf(arg, "@>%04X(r%d)", value, reg);
    }
  else
    {
      char *format[] = { "r%d", "*r%d", "", "*r%d+" };
      sprintf(arg, format[mode], reg);
    }
}

static void parse_type_1(struct buffer *buffer, disassemble_info *info, char *text)
{
  //                  |15|14|13|12|11|10| 9| 8| 7| 6| 5| 4| 3| 2| 1| 0|
  // source, dest     |Opcode  |B |Td   |Dest. Reg. |Ts   |Source Reg.|

  char source[16];
  char dest[16];

  parse_mode(buffer, info, source, (buffer->opcode & 0x3F));
  parse_mode(buffer, info, dest, ((buffer->opcode >> 6) & 0x3F));

  info->fprintf_func (info->stream, "%s %s, %s", text, source, dest);
}

static void parse_type_2(struct buffer *buffer ATTRIBUTE_UNUSED, disassemble_info *info, char *text)
{
  //                  |15|14|13|12|11|10| 9| 8| 7| 6| 5| 4| 3| 2| 1| 0|
  // PC-rel           |Opcode                 |Offset                 |

  char disp = buffer->opcode & 0xFF;
  info->fprintf_func (info->stream, "%s %d", text, (int)(disp) * 2);
}

static void parse_type_2a(struct buffer *buffer ATTRIBUTE_UNUSED, disassemble_info *info, char *text)
{
  //                  |15|14|13|12|11|10| 9| 8| 7| 6| 5| 4| 3| 2| 1| 0|
  // CRU disp         |Opcode                 |Offset                 |

  char disp = buffer->opcode & 0xFF;
  info->fprintf_func (info->stream, "%s %d", text, (int)(disp));
}

static void parse_type_3(struct buffer *buffer, disassemble_info *info, char *text)
{
  //                  |15|14|13|12|11|10| 9| 8| 7| 6| 5| 4| 3| 2| 1| 0|
  // source, reg      |Opcode           |Register   |Ts   |Source Reg.|

  char source[16];
  parse_mode(buffer, info, source, (buffer->opcode & 0x3F));
  info->fprintf_func (info->stream, "%s %s, r%d", 
                      text, source, (buffer->opcode >> 6) & 0xF);
}

static void parse_type_4(struct buffer *buffer, disassemble_info *info, char *text)
{
  //                  |15|14|13|12|11|10| 9| 8| 7| 6| 5| 4| 3| 2| 1| 0|
  // source, nbits    |Opcode           |NBits      |Ts   |Source Reg.|

  char source[16];
  parse_mode(buffer, info, source, (buffer->opcode & 0x3F));
  info->fprintf_func (info->stream, "%s %s, %d", 
                      text, source, (buffer->opcode >> 6) & 0xF);
}

static void parse_type_5(struct buffer *buffer, disassemble_info *info, char *text)
{
  //                  |15|14|13|12|11|10| 9| 8| 7| 6| 5| 4| 3| 2| 1| 0|
  // register, count  |Opcode                 |Count      |Register   |

  info->fprintf_func (info->stream, "%s r%d, %d", 
                      text, buffer->opcode & 0xF, (buffer->opcode >> 4) & 0xF);
}

static void parse_type_6(struct buffer *buffer, disassemble_info *info, char *text)
{
  //                  |15|14|13|12|11|10| 9| 8| 7| 6| 5| 4| 3| 2| 1| 0|
  // dest             |Opcode                       |Ts   |Source Reg.|

  char source[16];
  parse_mode(buffer, info, source, (buffer->opcode & 0x3F));
  info->fprintf_func (info->stream, "%s %s", text, source);
}

static void parse_type_7(struct buffer *buffer ATTRIBUTE_UNUSED, disassemble_info *info, char *text)
{
  //                  |15|14|13|12|11|10| 9| 8| 7| 6| 5| 4| 3| 2| 1| 0|
  // dest             |Opcode                             | 0  0  0  0|

  info->fprintf_func (info->stream, "%s", text);
}

static void parse_type_8(struct buffer *buffer, disassemble_info *info, char *text)
{
  //                  |15|14|13|12|11|10| 9| 8| 7| 6| 5| 4| 3| 2| 1| 0|
  // register, count  |Opcode                          | 0|Register   |
  // immediate        |Data                                           |

  unsigned char *data = &buffer->data[buffer->n_fetch];
  unsigned short value;
  if (! fetch_data (buffer, info, 2))
    return;
  value = ((unsigned short)data[0] << 8) | data[1];

  info->fprintf_func (info->stream, "%s r%d, >%04X", 
                      text, buffer->opcode & 0xF, value);
}

static void parse_type_8a(struct buffer *buffer, disassemble_info *info, char *text)
{
  //                  |15|14|13|12|11|10| 9| 8| 7| 6| 5| 4| 3| 2| 1| 0|
  // register, count  |Opcode                          | 0|Register   |

  info->fprintf_func (info->stream, "%s r%d", 
                      text, buffer->opcode & 0xF);
}

static void parse_type_8b(struct buffer *buffer, disassemble_info *info, char *text)
{
  //                  |15|14|13|12|11|10| 9| 8| 7| 6| 5| 4| 3| 2| 1| 0|
  // opcode           |Opcode                          | 0            |
  // immediate        |Data                                           |

  unsigned char *data = &buffer->data[buffer->n_fetch];
  unsigned short value;
  if (! fetch_data (buffer, info, 2))
    return;
  value = ((unsigned short)data[0] << 8) | data[1];

  info->fprintf_func (info->stream, "%s >%04X", 
                      text, value);
}

static struct parse_table level_5[] =
{
  {parse_type_6, "blwp"},
  {parse_type_6, "b"},
  {parse_type_6, "x"},
  {parse_type_6, "clr"},
  {parse_type_6, "neg"},
  {parse_type_6, "inv"},
  {parse_type_6, "inc"},
  {parse_type_6, "inct"},
  {parse_type_6, "dec"},
  {parse_type_6, "dect"},
  {parse_type_6, "bl"},
  {parse_type_6, "swpb"},
  {parse_type_6, "seto"},
  {parse_type_6, "abs"},
  {parse_error,  ""},
  {parse_error,  ""}
};

static void parse_level_5(struct buffer *buffer, disassemble_info *info, char *text)
{
  unsigned int code = (buffer->opcode >> 6) & 0xF;
  text = level_5[code].text;
  level_5[code].func(buffer, info, text);
}

static struct parse_table level_4[] =
{
  {parse_type_8,  "li"},
  {parse_error,   ""},
  {parse_type_8,  "ai"},
  {parse_error,   ""},
  {parse_type_8,  "andi"},
  {parse_error,   ""},
  {parse_type_8,  "ori"},
  {parse_error,   ""},
  {parse_type_8,  "ci"},
  {parse_error,   ""},
  {parse_type_8a, "stwp"},
  {parse_error,   ""},
  {parse_type_8a, "stst"},
  {parse_error,   ""},
  {parse_type_8b, "lwpi"},
  {parse_error,   ""},
  {parse_type_8b, "limi"},
  {parse_error,   ""},
  {parse_type_7,  "idle"},
  {parse_error,   ""},
  {parse_type_7,  "rset"},
  {parse_error,   ""},
  {parse_type_7,  "rtwp"},
  {parse_error,   ""},
  {parse_type_7,  "ckon"},
  {parse_error,   ""},
  {parse_type_7,  "ckof"},
  {parse_error,   ""},
  {parse_type_7,  "lrex"},
  {parse_error,   ""},
  {parse_error,   ""},
  {parse_error,   ""}
};

static void parse_level_4(struct buffer *buffer, disassemble_info *info, char *text)
{
  unsigned int code = (buffer->opcode >> 4) & 0x1F;
  text = level_4[code].text;
  level_4[code].func(buffer, info, text);
}

static struct parse_table level_3[] =
{
  {parse_error,   ""},
  {parse_error,   ""},
  {parse_level_4, ""},
  {parse_level_4, ""},
  {parse_level_5, ""},
  {parse_level_5, ""},
  {parse_level_5, ""},
  {parse_level_5, ""},
  {parse_type_5,  "sra"},
  {parse_type_5,  "srl"},
  {parse_type_5,  "sla"},
  {parse_type_5,  "src"},
  {parse_error,   ""},
  {parse_error,   ""},
  {parse_error,   ""},
  {parse_error,   ""}
};

static void parse_level_3(struct buffer *buffer, disassemble_info *info, char *text)
{
  unsigned int code = (buffer->opcode >> 8) & 0xF;
  text = level_3[code].text;
  level_3[code].func(buffer, info, text);
}

static struct parse_table level_2[] =
{
  {parse_type_2,  "jmp"},
  {parse_type_2,  "jlt"},
  {parse_type_2,  "jle"},
  {parse_type_2,  "jeq"},
  {parse_type_2,  "jhe"},
  {parse_type_2,  "jgt"},
  {parse_type_2,  "jne"},
  {parse_type_2,  "jnc"},
  {parse_type_2,  "joc"},
  {parse_type_2,  "jno"},
  {parse_type_2,  "jl"},
  {parse_type_2,  "jh"},
  {parse_type_2,  "jop"},
  {parse_type_2a, "sbo"},
  {parse_type_2a, "sbz"},
  {parse_type_2a, "tb"}
};

static void parse_level_2(struct buffer *buffer, disassemble_info *info, char *text)
{
  unsigned int code = (buffer->opcode >> 8) & 0xF;
  text = level_2[code].text;
  level_2[code].func(buffer, info, text);
}

static struct parse_table level_1[] =
{
  {parse_type_3, "coc"},
  {parse_type_3, "czc"},
  {parse_type_3, "xor"},
  {parse_type_4, "xop"},
  {parse_type_4, "ldcr"},
  {parse_type_4, "stcr"},
  {parse_type_3, "mpy"},
  {parse_type_3, "div"}
};

static void parse_level_1(struct buffer *buffer, disassemble_info *info, char *text)
{
  unsigned int code = (buffer->opcode >> 10) & 0x7;
  text = level_1[code].text;
  level_1[code].func(buffer, info, text);
}

static struct parse_table level_0[] =
{
  {parse_level_3, ""},
  {parse_level_2, ""},
  {parse_level_1, ""},
  {parse_level_1, ""},
  {parse_type_1,  "szc"},
  {parse_type_1,  "szcb"},
  {parse_type_1,  "s"},
  {parse_type_1,  "sb"},
  {parse_type_1,  "c"},
  {parse_type_1,  "cb"},
  {parse_type_1,  "a"},
  {parse_type_1,  "ab"},
  {parse_type_1,  "mov"},
  {parse_type_1,  "movb"},
  {parse_type_1,  "soc"},
  {parse_type_1,  "socb"}
};

int
print_insn_tms9900 (bfd_vma addr, disassemble_info * info)
{
  struct buffer buf;
  unsigned int code;
  
  buf.base = addr;
  buf.n_fetch = 0;

  if (! fetch_data (& buf, info, 2))
    return -1;
  buf.opcode = (((unsigned int)buf.data[0]) << 8) | buf.data[1];
  code = (buf.opcode >> 12) & 0xF;
  level_0[code].func(&buf, info, level_0[code].text);
  return(buf.n_fetch);
}

