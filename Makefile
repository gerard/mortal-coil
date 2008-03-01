CFLAGS = -g -Wall
objects = parse.o mcoil.o 

mcoil: $(objects)
	$(CC) $(CFLAGS) $^ -o $@

clean:
	rm -f mcoil $(objects)

$(objects): defs.h
mcoil.o: parse.h

.PHONY: clean
