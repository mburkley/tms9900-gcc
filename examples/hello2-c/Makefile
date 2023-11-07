# Paths to TMS9900 compilation tools
GAS=tms9900-as
LD=tms9900-ld
CC=tms9900-gcc
CXX=tms9900-c++
OBJCOPY=tms9900-objcopy

# Flags used during linking
# Refer to the linker rules in an external file
LDFLAGS=\
  --script=linkfile

# List of compiled objects used in executable
OBJECT_LIST=\
  cart_header.o\
  crt0.o\
  main.o

# List of all files needed in executable
PREREQUISITES=\
  $(OBJECT_LIST)
  
# Recipe to compile the executable
all: hello.elf
	$(OBJCOPY) -O binary -j .text -j .data -j .ctors hello.elf hello.cart

hello.elf: $(PREREQUISITES)
	$(LD) $(OBJECT_LIST) $(LDFLAGS) -o $@

# Recipe to clean all compiled objects
.phony clean:
	rm -f *.o
	rm -f *.elf
	rm -f *.cart

# Recipes to compile individual files
%.o: %.asm
	$(GAS) $< -o $@
%.o: %.c
	$(CC) -c $< -O2 -o $@
%.o: %.cpp
	$(CXX) -c $< -O2 -o $@

