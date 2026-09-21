SCRIPT_NAME=elf
OUTPUT_FORMAT="elf32-tms9900"
OUTPUT_ARCH="tms9900"
TEXT_START_ADDR=0xA000
TARGET_PAGE_SIZE=512
ARCH=tms9900

# gcc at -O2 puts main in .text.startup (and cold code in .text.unlikely), which
# the generic ELF script places ahead of .text.  The cartridge header (in
# header.o's .text) must stay first, so place all plain .text input sections,
# in input order, at the start of the output .text before those.
TEXT_START_SYMBOLS='*(.text)'
