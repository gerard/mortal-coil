#include <stdio.h>
#include <stdlib.h>

#include "defs.h"

int mc_parsefile(const char *file, struct game *g) {
	FILE *f;
	int i, j;
	char c;
	
	f = fopen(file, "r");
	if(!f) return FALSE;
	
	if(fscanf(f, "%dx%d\n", &(g->size_x), &(g->size_y)) != 2) return FALSE;
	
	/* 
	 * FIXME: This is a bit screwed up: The matrix is not filled in the
	 * same order that we would need to allocate it.
	 */
	g->field = (int **)malloc(sizeof(int *)*(g->size_x));
	for(i=0; i < (g->size_x); i++)
		g->field[i] = (int *)malloc(sizeof(int)*(g->size_y));
	
	for(i=0; i < (g->size_y); i++) {
		for(j=0; j < (g->size_x); j++) {
			c = getc(f);
			switch(c) {
			case '#':
				g->field[j][i] = AREA_BLOCKED;
				break;
			
			case '\n': /* Parsing error */
				return FALSE;
				
			default: /* By default we assume a free area */
				g->field[j][i] = AREA_CLEAR;
			}
		}
		
		/* We expect '\n' here */
		if(getc(f) != '\n') return FALSE;
	}
	
	/* Succesfully parsed! */
	return TRUE;
}
