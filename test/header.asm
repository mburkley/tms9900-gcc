# ROM header
  byte 0xAA, 0x01, 0x00, 0x00

# Start of power-up chain (unused)
  data 0x0000

# Start of program chain
  data program_record

# Start of DSR chain (unused)
  data 0x0000

  def mcolor
  def scolor
  def fcolor

# Color bytes
  TEXT 'COLOR'
mcolor:
  byte 0x17
scolor:
  byte 0x12
fcolor:
  byte 0x16
  even

# Start of subprogram list (unused)
# This doubles as the terminator of the program chain
program_record:
#  data  program_record2    # Next program chain record
  data  0
  data  _start    # Entry point for program
  nstring "GCC-4.4.0 TEST"  # Name of program
  even

