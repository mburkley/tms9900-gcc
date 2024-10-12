
OBJECTS=cpu.o \
unasm.o \
parse.o \
harness-mem.o 

LIBS=\
-lreadline \
-lm 

CFLAGS=-Wall -ggdb3
# LDFLAGS=

all:  harness

harness: $(OBJECTS) harness.o
	@echo "\t[LD] $@..."
	@$(CXX) $(LDFLAGS) -o $@ harness.o $(OBJECTS) $(LIBS)

%.o: %.cc
	@echo "\t[CC] $<..."
	@$(CXX) -c $(CCFLAGS) $< -o $@

.phony clean:
	rm *.o
	rm harness
