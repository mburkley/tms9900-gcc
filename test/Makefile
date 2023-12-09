GAS=/home/mark/tms9900-gcc/tms9900-gcc/bin/tms9900-as
LD=/home/mark/tms9900-gcc/tms9900-gcc/bin/tms9900-ld
CC=/home/mark/tms9900-gcc/tms9900-gcc/bin/tms9900-gcc
LIBTI99?=/home/mark/ti99/libti99
LIBGCC=/home/mark/tms9900-gcc/tms9900-gcc/lib/gcc/tms9900/4.4.0/libgcc.a
# LIBGCC=libgcc.a
ELF2CART=elf2cart

FNAME=test

# select one test file here.  Tests are broken up so as to fit into an 8K bin file
# to make a cartridge bin
# TODO make these targets

# TEST_SRC=test_char_short
TEST_SRC=test_long
# TEST_SRC=test_float
# TEST_SRC=test_misc
# TEST_SRC=test_one

LDFLAGS=\
  --section-start .text=6000 --section-start .data=2080

CRT0 = $(LIBTI99)/crt0_ea5.o

OBJECT_LIST=\
  header.o\
  $(CRT0)\
  $(TEST_SRC).o \
  tap.o

all: $(FNAME).bin

#  Build on host.  Used to verify tests
host-test: test.c
	gcc -o host-test test.c tap.c

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

# test with and without -O2

%.o: %.c
	$(CC) -c $< -std=c99 --save-temp -I$(LIBTI99) -o $@
#	$(CC) -c $< -std=c99 -O2 --save-temp -I$(LIBTI99) -o $@
