#include "sysdep.h"
#include "bfd.h"
#include "bfdlink.h"
#include "libbfd.h"
#include "elf-bfd.h"
#include "bfd_stdint.h"

#include "elf/tms9900.h"


#ifdef DEBUG_GEN_RELOC
#define TRACE(str) \
  fprintf (stderr, "tms9900 bfd reloc lookup %d (%s)\n", code, str)
#else
#define TRACE(str)
#endif

extern const bfd_target bfd_elf32_tms9900_vec;

static reloc_howto_type elf_howto_table[]=
{
  /* Invalid record, do nothing */
  HOWTO(R_TMS9900_NONE,             /* type */
        0,                          /* rightshift */
        0,                          /* size (0 = byte, 1 = short, 2 = long) */
        0,                          /* bitsize */
        FALSE,                      /* pc_relative */
        0,                          /* bitpos */
        complain_overflow_bitfield, /* complain on overflow */
	bfd_elf_generic_reloc,      /* special_function */
        "R_TMS9900_NONE",           /* name */
	FALSE,                      /* partial_inplace */
        0x00000000,                 /* src_mask */
        0x00000000,                 /* dst_mask */
        FALSE),                     /* pcrel_offset */

  /* 8-bit offset from current PC, used in jump commands */
  HOWTO(R_TMS9900_PC8,              /* type */
        1,                          /* rightshift */
        0,                          /* size (0 = byte, 1 = short, 2 = long) */
        8,                          /* bitsize */
        TRUE,                       /* pc_relative */
        0,                          /* bitpos */
        complain_overflow_signed,   /* complain on overflow */
        bfd_elf_generic_reloc,      /* special_function */
        "R_TMS9900_PC8",            /* name */
        FALSE,                      /* partial_inplace */
        0x00000000,                 /* src_mask */
        0x000000FF,                 /* dst_mask */
        TRUE),                      /* pcrel_offset */

  /* Generic 16-bit constant */
  HOWTO(R_TMS9900_16,               /* type */
        0,                          /* rightshift */
        1,                          /* size (0 = byte, 1 = short, 2 = long) */
        16,                         /* bitsize */
        FALSE,                      /* pc_relative */
        0,                          /* bitpos */
        complain_overflow_bitfield, /* complain on overflow */
        bfd_elf_generic_reloc,      /* special_function */
        "R_TMS9900_16",             /* name */
        FALSE,                      /* partial_inplace */
        0x00000000,                 /* src_mask */
        0x0000FFFF,                 /* dst_mask */
        FALSE),                     /* pcrel_offset */
};  


static reloc_howto_type *
elf_tms9900_reloc_type_lookup (bfd *abfd ATTRIBUTE_UNUSED,
				bfd_reloc_code_real_type code)
{
  switch (code)
    {
    case BFD_RELOC_NONE:
      TRACE ("BFD_RELOC_NONE");
      return &elf_howto_table[R_TMS9900_NONE];

    case BFD_RELOC_16:
      TRACE ("BFD_RELOC_16");
      return &elf_howto_table[R_TMS9900_16];

    case BFD_RELOC_8_PCREL:
      TRACE ("BFD_RELOC_8_PCREL");
      return &elf_howto_table[R_TMS9900_PC8];

    default:
      break;
    }

  TRACE ("Unknown");
  return 0;
}

static reloc_howto_type *
elf_tms9900_reloc_name_lookup (bfd *abfd ATTRIBUTE_UNUSED,
				const char *r_name)
{
  unsigned int i;

  for (i = 0; i < sizeof (elf_howto_table) / sizeof (elf_howto_table[0]); i++)
    if (elf_howto_table[i].name != NULL
	&& strcasecmp (elf_howto_table[i].name, r_name) == 0)
      return &elf_howto_table[i];

  return NULL;
}

static void
tms9900_elf_info_to_howto (bfd *abfd ATTRIBUTE_UNUSED,
		           arelent *cache_ptr,
		           Elf_Internal_Rela *dst)
{
  cache_ptr->howto = &elf_howto_table[ELF32_R_TYPE (dst->r_info)];
}

#define TARGET_BIG_SYM			bfd_elf32_tms9900_vec
#define TARGET_BIG_NAME			"elf32-tms9900"
#define ELF_ARCH			bfd_arch_tms9900
#define ELF_MACHINE_CODE		EM_TMS9900
#define ELF_MAXPAGESIZE			0x200

#define bfd_elf32_bfd_reloc_type_lookup	      elf_tms9900_reloc_type_lookup
#define bfd_elf32_bfd_reloc_name_lookup	      elf_tms9900_reloc_name_lookup
#define elf_info_to_howto                     tms9900_elf_info_to_howto
#define elf_info_to_howto_rel                 _bfd_elf_no_info_to_howto

#include "elf32-target.h"
