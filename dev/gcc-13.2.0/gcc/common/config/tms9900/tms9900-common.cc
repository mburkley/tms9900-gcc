/* Common hooks for tms9900.
   Copyright (C) 1994-2023 Free Software Foundation, Inc.

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

#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "tm.h"
#include "common/common-target.h"
#include "common/common-target-def.h"
#include "opts.h"
#include "flags.h"

/* Implement TARGET_HANDLE_OPTION.  */

static bool
tms9900_handle_option (struct gcc_options *opts,
		     struct gcc_options *opts_set ATTRIBUTE_UNUSED,
		     const struct cl_decoded_option *decoded,
		     location_t loc ATTRIBUTE_UNUSED)
{
  size_t code = decoded->opt_index;

  switch (code)
    {
    default:
      return true;
    }
}

/* Implement TARGET_OPTION_INIT_STRUCT.  */

static void
tms9900_option_init_struct (struct gcc_options *opts)
{
  opts->x_flag_finite_math_only = 0;
  opts->x_flag_trapping_math = 0;
  // opts->x_flag_signaling_nans = 0;
}

#undef TARGET_DEFAULT_TARGET_FLAGS
#define TARGET_DEFAULT_TARGET_FLAGS 0
#undef TARGET_HANDLE_OPTION
#define TARGET_HANDLE_OPTION tms9900_handle_option
#undef TARGET_OPTION_INIT_STRUCT
#define TARGET_OPTION_INIT_STRUCT tms9900_option_init_struct

struct gcc_targetm_common targetm_common = TARGETM_COMMON_INITIALIZER;
