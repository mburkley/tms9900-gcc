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

/* Perform a relocation for a final link.  */

static bfd_reloc_status_type
tms9900_final_link_relocate (reloc_howto_type *howto,
			     bfd *input_bfd,
			     bfd *output_bfd ATTRIBUTE_UNUSED,
			     asection *input_section,
			     bfd_byte *contents,
			     bfd_vma offset,
			     bfd_vma value,
			     bfd_vma addend,
			     struct bfd_link_info *info ATTRIBUTE_UNUSED,
			     asection *sym_sec ATTRIBUTE_UNUSED,
			     bfd_boolean is_local ATTRIBUTE_UNUSED)
{
  bfd_byte *hit_data = contents + offset;

  value += addend;

  switch (howto->type)
    {
    case R_TMS9900_NONE:
      return bfd_reloc_ok;

    case R_TMS9900_16:
      if (value > 0xffff)
	return bfd_reloc_overflow;
      bfd_put_16 (input_bfd, value, hit_data);
      return bfd_reloc_ok;

    case R_TMS9900_PC8:
      {
	/* The 8-bit branch offset lives in the low byte of the branch
	   instruction.  The CPU adds it, in words, to the address of the
	   following instruction.  */
	bfd_vma site = (input_section->output_section->vma
			+ input_section->output_offset
			+ offset);
	bfd_signed_vma disp = (bfd_signed_vma) (value - (site + 1));

	disp >>= 1;
	if (disp < -128 || disp > 127)
	  return bfd_reloc_overflow;

	bfd_put_8 (input_bfd, (bfd_vma) disp & 0xff, hit_data);
	return bfd_reloc_ok;
      }

    default:
      return bfd_reloc_notsupported;
    }
}

/* Relocate a section for the ELF linker.  Defining this hook is what makes
   bfd_elf_final_link (rather than the generic linker) handle this target;
   the ELF linker is the one that implements --emit-relocs.  */

static bfd_boolean
tms9900_elf_relocate_section (bfd *output_bfd,
			      struct bfd_link_info *info,
			      bfd *input_bfd,
			      asection *input_section,
			      bfd_byte *contents,
			      Elf_Internal_Rela *relocs,
			      Elf_Internal_Sym *local_syms,
			      asection **local_sections)
{
  Elf_Internal_Shdr *symtab_hdr;
  struct elf_link_hash_entry **sym_hashes;
  Elf_Internal_Rela *rel;
  Elf_Internal_Rela *relend;

  symtab_hdr = &elf_tdata (input_bfd)->symtab_hdr;
  sym_hashes = elf_sym_hashes (input_bfd);

  rel = relocs;
  relend = relocs + input_section->reloc_count;
  for (; rel < relend; rel++)
    {
      int r_type;
      reloc_howto_type *howto;
      unsigned long r_symndx;
      Elf_Internal_Sym *sym;
      asection *sec;
      struct elf_link_hash_entry *h;
      bfd_vma relocation;
      bfd_reloc_status_type r;

      r_symndx = ELF32_R_SYM (rel->r_info);
      r_type = ELF32_R_TYPE (rel->r_info);
      howto = elf_howto_table + r_type;

      if (r_type == R_TMS9900_NONE)
	{
	  rel->r_info = 0;
	  rel->r_addend = 0;
	  continue;
	}

      h = NULL;
      sym = NULL;
      sec = NULL;
      if (r_symndx < symtab_hdr->sh_info)
	{
	  sym = local_syms + r_symndx;
	  sec = local_sections[r_symndx];
	  relocation = _bfd_elf_rela_local_sym (output_bfd, sym, &sec, rel);
	}
      else
	{
	  bfd_boolean unresolved_reloc, warned;

	  RELOC_FOR_GLOBAL_SYMBOL (info, input_bfd, input_section, rel,
				   r_symndx, symtab_hdr, sym_hashes,
				   h, sec, relocation,
				   unresolved_reloc, warned);
	}

      if (sec != NULL && elf_discarded_section (sec))
	{
	  /* For relocs against symbols from removed linkonce sections,
	     or sections discarded by a linker script, we just want the
	     section contents zeroed.  Avoid any special processing.  */
	  _bfd_clear_contents (howto, input_bfd, contents + rel->r_offset);
	  rel->r_info = 0;
	  rel->r_addend = 0;
	  continue;
	}

      if (info->relocatable)
	continue;

      r = tms9900_final_link_relocate (howto, input_bfd, output_bfd,
				       input_section,
				       contents, rel->r_offset,
				       relocation, rel->r_addend,
				       info, sec, h == NULL);

      if (r != bfd_reloc_ok)
	{
	  const char *name;
	  const char *msg = (const char *) 0;

	  if (h != NULL)
	    name = h->root.root.string;
	  else
	    {
	      name = (bfd_elf_string_from_elf_section
		      (input_bfd, symtab_hdr->sh_link, sym->st_name));
	      if (name == NULL || *name == '\0')
		name = bfd_section_name (input_bfd, sec);
	    }

	  switch (r)
	    {
	    case bfd_reloc_overflow:
	      if (! ((*info->callbacks->reloc_overflow)
		     (info, (h ? &h->root : NULL), name, howto->name,
		      (bfd_vma) 0, input_bfd, input_section,
		      rel->r_offset)))
		return FALSE;
	      break;

	    case bfd_reloc_undefined:
	      if (! ((*info->callbacks->undefined_symbol)
		     (info, name, input_bfd, input_section,
		      rel->r_offset, TRUE)))
		return FALSE;
	      break;

	    case bfd_reloc_outofrange:
	      msg = _("internal error: out of range error");
	      goto common_error;

	    case bfd_reloc_notsupported:
	      msg = _("internal error: unsupported relocation error");
	      goto common_error;

	    case bfd_reloc_dangerous:
	      msg = _("internal error: dangerous error");
	      goto common_error;

	    default:
	      msg = _("internal error: unknown error");
	      /* Fall through.  */
	    common_error:
	      if (! ((*info->callbacks->warning)
		     (info, msg, name, input_bfd, input_section,
		      rel->r_offset)))
		return FALSE;
	      break;
	    }
	}
    }

  return TRUE;
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
#define elf_backend_relocate_section          tms9900_elf_relocate_section
#define elf_backend_rela_normal               1

#include "elf32-target.h"
