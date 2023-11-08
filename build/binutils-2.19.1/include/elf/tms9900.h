/* ix86 ELF support for BFD.
   Copyright 1998, 1999, 2000, 2002, 2004, 2005, 2006
   Free Software Foundation, Inc.

   This file is part of BFD, the Binary File Descriptor library.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software Foundation,
   Inc., 51 Franklin Street - Fifth Floor, Boston, MA 02110-1301, USA.  */

#ifndef _ELF_TMS9900_H
#define _ELF_TMS9900_H

#include "elf/reloc-macros.h"

START_RELOC_NUMBERS (elf_tms9900_reloc_type)
     RELOC_NUMBER (R_TMS9900_NONE,      0)	/* No reloc */
     RELOC_NUMBER (R_TMS9900_PC8,       1)	/* PC relative 8 bit */
     RELOC_NUMBER (R_TMS9900_16,        2)	/* Direct 16 bit  */
END_RELOC_NUMBERS (R_TMS9900_max)

#endif
