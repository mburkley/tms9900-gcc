/* BFD library support routines for the TMS9900 architecture.
   Copyright 2009 Eric Welser

   This file is part of BFD, the Binary File Descriptor library.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street - Fifth Floor, Boston,
   MA 02110-1301, USA.  */

#include "sysdep.h"
#include "bfd.h"
#include "libbfd.h"

const bfd_arch_info_type bfd_tms9900_arch;

/* This routine is provided two arch_infos and
   returns whether they'd be compatible.  */

static const bfd_arch_info_type *
compatible (const bfd_arch_info_type *a, const bfd_arch_info_type *b)
{
  if (a->arch != b->arch)
    return NULL;

  if (a->mach == b->mach)
    return a;

  return (a->arch == bfd_arch_tms9900) ? & bfd_tms9900_arch : NULL;
}


const bfd_arch_info_type bfd_tms9900_arch = 
{
  16,                /* Bits per word */
  16,                /* Bits per address */
  8,                 /* Bits per byte */
  bfd_arch_tms9900,  /* Architecture ID */
  bfd_mach_tms9900,  /* Machine ID */
  "tms9900",         /* Architecture name */
  "tms9900",         /* Printable name */
  0,                 /* Section align power */
  TRUE,              /* Is this the default machine for this arch? */
  compatible,        /* Function to determine object file compatibility */
  bfd_default_scan,  /* Scan function */
  NULL               /* Next record */
};
