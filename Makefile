all: mcoil

mcoil: mcoil.c
	gcc mcoil.c -o mcoil

clean: 
	rm -f mcoil
