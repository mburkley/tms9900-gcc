git restore build
rm -rf build/gcc-4.4.0/libgcc/config/tms9900
rm -rf build/gcc-4.4.0/gcc/config/tms9900
rm build/.gcc_patched 
rm build/.binutils_patched 
rm build/binutils-2.19.1/bfd/cpu-tms9900.*
rm build/binutils-2.19.1/bfd/elf32-tms9900.*
rm build/binutils-2.19.1/gas/config/tc-tms9900.*
rm build/binutils-2.19.1/include/elf/tms9900.*
rm build/binutils-2.19.1/ld/emulparams/tms9900.*
rm build/binutils-2.19.1/ld/scripttempl/tms9900.*
rm build/binutils-2.19.1/opcodes/tms9900-dis.c
