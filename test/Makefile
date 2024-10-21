##
#  Directories where tools are located.  Edit to reflect your setup
##
LIBTI99DIR?=~/work/ti99/libTi99All/buildti
# GCCDIR=/opt/tms9900-gcc/bin/
GCCDIR=../gcc4/bin/

##
#  Locations of tools relative to directories defined above
##
GAS=$(GCCDIR)tms9900-as
LD=$(GCCDIR)tms9900-ld
CC=$(GCCDIR)tms9900-gcc
LIBGCC=$(GCCDIR)../lib/gcc/tms9900/4.4.0/libgcc.a
CRT0=$(LIBTI99DIR)/crt0_ea5.o

##
# Define test targets
##

# Tests are split up so as to fit into an 8K bin file
# to make a cartridge bin.  Note some tests will not fit into a cartridge
# without optimisation.  Check the bin file is less than 8192 bytes before
# running.

# test_float is not tested by this framework as it uses the console ROM
# floating point routines
NONTARGET=test_float

TARGETS=\
test_char_short \
test_long \
test_misc \
test_subreg \
test_array

BINTARGETS=$(TARGETS:=.bin)
ELFTARGETS=$(TARGETS:=.elf)

.PHONY: test clean harness
all: $(BINTARGETS) harness

##
#  Define make flags
##
ifdef EA5
ELF2BIN=../elfutils/elf2ea5
LDFLAGS=--section-start .text=a000 --section-start .data=2080 -M
else
ELF2BIN=../elfutils/elf2cart
LDFLAGS=--section-start .text=6000 --section-start .data=2080
HEADER=header.o
endif

OBJECTS=tap.o params.o

CFLAGS=-std=c99 --save-temp -I$(LIBTI99DIR)

#  All tests should be run with and without -O2 and -Os for complete coverage
#
#  NOTE some tests are useless with -O2 as the compiler optimises away any
#  arithmetic or comparisons with constants.
# CFLAGS+=-O2
# CFLAGS+=-Os

# Uncomment to dump gcc insns into assembly output file
# CFLAGS+=-minline_rtl

# Uncomment to turn on unit test debug output
# CFLAGS+=-D_DEBUG=1

# Uncomment to build binary for use with emulator
CFLAGS+=-DEMUL_TEST=1

LIBTI99=-lti99

##
#  Define make rules
##
harness:
	$(MAKE) -C harness

test: $(BINTARGETS) harness
	@for target in $(BINTARGETS); do \
            harness/harness -f $$target ; \
            count=$$(($$count + $$?)) ; \
        done ; \
	if [ $$count -gt 0 ]; then \
	echo "\n*** $$count tests FAILED\n" ; \
	else \
	echo "\n*** all tests passed\n" ; \
	fi

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

$(ELFTARGETS): %.elf: %.o $(OBJECTS) $(HEADER)
	$(LD) $(HEADER) $(CRT0) $< $(OBJECTS) -o $@ $(LDFLAGS) -L $(LIBTI99DIR) $(LIBTI99) \
	-Map=$(patsubst %.elf,%.map,$@) $(LIBGCC)

clean:
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
