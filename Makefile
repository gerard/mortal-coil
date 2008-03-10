CFLAGS = -g -Wall
objects = parse.o mcoil.o game.o

mcoil: $(objects)
	$(CC) $(CFLAGS) $^ -o $@

clean:
	rm -f mcoil $(objects)

$(objects): defs.h
mcoil.o: game.h parse.h
parse.o: game.h

.PHONY: clean
