GAS=/home/eric/dev/tios/toolchain/bin/tms9900-as
LD=/home/eric/dev/tios/toolchain/bin/tms9900-ld
CC=/home/eric/dev/tios/toolchain/bin/tms9900-gcc

ELF2CART=/home/eric/dev/tios/toolchain/bin/elf2cart

LDFLAGS=\
  --section-start .text=6000 --section-start .data=2000

OBJECT_LIST=\
  header.o\
  crt0.o\
  main.o

PREREQUISITES=\
  $(OBJECT_LIST)
  
all: $(PREREQUISITES)
	$(LD) $(OBJECT_LIST) $(LDFLAGS) -o hello.elf
	$(ELF2CART) hello.elf hello.cart

.phony clean:
	rm *.o
	rm *.elf
	rm *.cart

%.o: %.asm
	$(GAS) $< -o $@

%.o: %.c
	$(CC) -c $< -O2 -o $@

