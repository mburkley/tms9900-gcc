UTILS=elf2ea5 elf2cart ea5split
all: $(UTILS)

$(UTILS): %: %.c
	gcc $^ -o $@

clean:
	rm $(UTILS)

