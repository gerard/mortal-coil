objects = mcoil.o 

mcoil: $(objects)
	$(CC) $< -o $@

clean:
	rm -f mcoil $(objects)

.PHONY: clean
