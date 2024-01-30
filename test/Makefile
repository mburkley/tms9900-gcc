LIBTI99DIR?=../libti99

# GCCDIR=/opt/tms9900-gcc/bin/
GCCDIR=../tms9900-gcc/bin/

GAS=$(GCCDIR)tms9900-as
LD=$(GCCDIR)tms9900-ld
CC=$(GCCDIR)tms9900-gcc
LIBGCC=$(GCCDIR)../lib/gcc/tms9900/4.4.0/libgcc.a
ELF2CART=elf2cart

FNAME=test

# Uncomment one test file here.  Tests are split up so as to fit into an 8K bin file
# to make a cartridge bin.  Note some tests will not fit into a cartridge
# without optimisation.  Check the bin file is less than 8192 bytes before
# running.

# TODO make these targets

# TEST_SRC=test_char_short
# TEST_SRC=test_long
# TEST_SRC=test_float
TEST_SRC=test_misc
# TEST_SRC=test_one
# TEST_SRC=zap

LDFLAGS=\
  --section-start .text=6000 --section-start .data=2080

CFLAGS=-std=c99 --save-temp -I$(LIBTI99DIR)

#  test with and without -O2.
#
#  NOTE some tests are useless with -O2 as the compiler optimises away any
#  arithmetic or comparisons with constants.
# CFLAGS+=-O2
# CFLAGS+=-Os

# Uncomment to turn on unit test debug output
# CFLAGS+=-D_DEBUG=1

# Uncomment to build binary for use with emulator
# CFLAGS+=-DEMUL_TEST=1

CRT0 = $(LIBTI99DIR)/crt0_ea5.o

LIBTI99=-lti99

OBJECT_LIST=header.o

# OBJECT_LIST=temu_hdr.o

OBJECT_LIST+=\
  $(CRT0)\
  $(TEST_SRC).o \
  tap.o

all: $(FNAME).bin

#  Build on host.  Used to verify tests
host-test: test.c
	gcc -o host-test test.c tap.c

# TODO add a check that bin file has not exceeded cartridge size
#	size=$(wc -c < $(FNAME).bin)
#        if (size > 8191) $(error

$(FNAME).bin: $(FNAME).elf
	$(ELF2CART) $(FNAME).elf $(FNAME).bin


$(FNAME).elf: $(OBJECT_LIST)
	$(LD) $(OBJECT_LIST) $(LDFLAGS) -L$(LIBTI99DIR) $(LIBTI99) -o $(FNAME).elf \
	-Map=mapfile $(LIBGCC)

.phony clean:
	rm -f *.o
	rm -f *.elf
	rm -f *.bin
	rm -f *.i
	rm -f *.s
	rm -f mapfile
	rm -f gccdump.*
	rm -f *.c.*r.*

%.o: %.asm
	$(GAS) $< -o $@

%.o: %.c
	$(CC) -c $< $(CFLAGS) -o $@
