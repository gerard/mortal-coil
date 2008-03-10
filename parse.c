#include <stdio.h>
#include <stdlib.h>

#include "defs.h"
#include "game.h"

int mc_parsefile(const char *file) {
	FILE *f;
	int i, j;
	char c;
	
	/* These are to be passed to init_game() */
	int **field;
	int size_x, size_y;
	
	f = fopen(file, "r");
	if(!f) return FALSE;
	
	if(fscanf(f, "%dx%d\n", &size_x, &size_y) != 2) return FALSE;
	
	/* 
	 * FIXME: This is a bit screwed up: The matrix is not filled in the
	 * same order that we would need to allocate it.
	 */
	field = (int **)malloc(sizeof(int *)*size_x);
	for(i=0; i < size_x; i++)
		field[i] = (int *)malloc(sizeof(int)*size_y);
	
	for(i=0; i < size_y; i++) {
		for(j=0; j < size_x; j++) {
			c = getc(f);
			switch(c) {
			case '#':
				field[j][i] = AREA_BLOCKED;
				break;
			
			case '\n': /* Parsing error */
				return FALSE;
				
			default: /* By default we assume a free area */
				field[j][i] = AREA_CLEAR;
			}
		}
		
		/* We expect '\n' here */
		if(getc(f) != '\n') return FALSE;
	}
	
	/* Succesfully parsed! */
	return init_game(field, size_x, size_y);
}
