GAS=/home/mark/tms9900-gcc/tms9900-gcc/bin/tms9900-as
LD=/home/mark/tms9900-gcc/tms9900-gcc/bin/tms9900-ld
CC=/home/mark/tms9900-gcc/tms9900-gcc/bin/tms9900-gcc
LIBTI99?=/home/mark/ti99/libti99
LIBGCC=/home/mark/tms9900-gcc/tms9900-gcc/lib/gcc/tms9900/4.4.0/libgcc.a
# LIBGCC=libgcc.a
ELF2CART=elf2cart

#FNAME=test
FNAME=float

LDFLAGS=\
  --section-start .text=6000 --section-start .data=2080
#  --section-start .text=6000 --section-start .data=8320

# CRT0 = crt0.o
CRT0 = $(LIBTI99)/crt0_ea5.o


OBJECT_LIST=\
  header.o\
  $(CRT0)\
  $(FNAME).o \
  tap.o

all: $(FNAME).bin

#  Build on host.  Used to verify tests
host-test: test.c
	gcc -o host-test test.c tap.c

#  Test real number conversions on host
DECNUMDIR=../dev/gcc-4.4.0/libdecnumber/
LIBGCCDIR=../dev/gcc-4.4.0/libgcc
DECNUMLIBDIR=../dev/gcc-4.4.0/build/libdecnumber
tireal: tireal.c
	gcc -o tireal -I $(DECNUMDIR) -I $(LIBGCCDIR) tireal.c\
        -L $(DECNUMLIBDIR) \
        -ldecnumber

$(FNAME).bin: $(FNAME).elf
	$(ELF2CART) $(FNAME).elf $(FNAME).bin

$(FNAME).elf: $(OBJECT_LIST)
	$(LD) $(OBJECT_LIST) $(LDFLAGS) -L$(LIBTI99) -lti99 -o $(FNAME).elf \
	-Map=mapfile $(LIBGCC)

.phony clean:
	rm -f *.o
	rm -f *.elf
	rm -f *.bin
	rm -f *.i
	rm -f *.s
	rm -f mapfile
	rm -f gccdump.*

%.o: %.asm
	$(GAS) $< -o $@


%.o: %.c
	$(CC) -c $< -std=c99 --save-temp -I$(LIBTI99) -o $@
#	$(CC) -c $< -std=c99 -O2 --save-temp -I$(LIBTI99) -o $@
