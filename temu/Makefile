
OBJECTS=cpu.o \
unasm.o \
parse.o \
temu-mem.o 

LIBS=\
-lreadline \
-lm 

CFLAGS=-Wall -ggdb3
# LDFLAGS=

all:  temu

temu: $(OBJECTS) temu.o
	@echo "\t[LD] $@..."
	@$(CXX) $(LDFLAGS) -o $@ temu.o $(OBJECTS) $(LIBS)

%.o: %.cc
	@echo "\t[CC] $<..."
	@$(CXX) -c $(CCFLAGS) $< -o $@
