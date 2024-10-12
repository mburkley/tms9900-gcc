LIBTI99DIR?=../libti99

# GCCDIR=/opt/tms9900-gcc/bin/
GCCDIR=../gcc4/bin/

GAS=$(GCCDIR)tms9900-as
LD=$(GCCDIR)tms9900-ld
CC=$(GCCDIR)tms9900-gcc
LIBGCC=$(GCCDIR)../lib/gcc/tms9900/4.4.0/libgcc.a

# Tests are split up so as to fit into an 8K bin file
# to make a cartridge bin.  Note some tests will not fit into a cartridge
# without optimisation.  Check the bin file is less than 8192 bytes before
# running.

TARGETS=\
test_char_short \
test_long \
test_float \
test_misc \
test_subreg \
test_one

BINTARGETS=$(TARGETS:=.bin)

all: $(BINTARGETS)

ifdef EA5
ELF2BIN=../elfutils/elf2ea5
LDFLAGS=--section-start .text=a000 --section-start .data=2080 -M
FNAME=test.ea5
else
ELF2BIN=../elfutils/elf2cart
LDFLAGS=--section-start .text=6000 --section-start .data=2080
HEADER=header.o
FNAME=test.bin
endif

CFLAGS=-std=c99 --save-temp -I$(LIBTI99DIR)

#  test with and without -O2.
#
#  NOTE some tests are useless with -O2 as the compiler optimises away any
#  arithmetic or comparisons with constants.
# CFLAGS+=-O2
CFLAGS+=-Os

# Uncomment to dump gcc insns into assembly output file
# CFLAGS+=-minline_rtl

# Uncomment to turn on unit test debug output
# CFLAGS+=-D_DEBUG=1

# Uncomment to build binary for use with emulator
CFLAGS+=-DEMUL_TEST=1

HEADER+=$(LIBTI99DIR)/crt0_ea5.o

LIBTI99=-lti99

# OBJECT_LIST=temu_hdr.o

OBJECTS=tap.o params.o

#  Build on host.  Used to verify tests
host-test: test.c
	gcc -o host-test test.c tap.c

# TODO add a check that bin file has not exceeded cartridge size
#	size=$(wc -c < $(FNAME).bin)
#        if (size > 8191) $(error

$(BINTARGETS): %.bin: %.elf
	$(ELF2BIN) $< $@

#  NOTE link order is important.  Headers must be first and target can't be last.  There is some issue with byte
#  alignment if target is last.

%.elf: %.o $(HEADER) $(OBJECTS)
	$(LD) $(HEADER) $< $(OBJECTS) -o $@ $(LDFLAGS) -L$(LIBTI99DIR) $(LIBTI99) \
	-Map=$(patsubst %.elf,%.map,$@) $(LIBGCC)
#	-Map=$@.map $(LIBGCC)

.phony clean:
	rm -f *.o
	rm -f *.elf
	rm -f *.bin
	rm -f *.i
	rm -f *.s
	rm -f *.map
	rm -f mapfile
	rm -f gccdump.*
	rm -f *.c.*r.*

%.o: %.asm
	$(GAS) $< -o $@

%.o: %.c
	$(CC) -c $< $(CFLAGS) -o $@
