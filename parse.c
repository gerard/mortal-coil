#include <stdio.h>
#include "mcoil.h"

int mc_parsefile(const char *file, struct game *g) {
	FILE *f;
	int i, j;
	char c;
	
	f = fopen(file, "r");
	fscanf(f, "%dx%d\n", &(g->x), &(g->y));
	
	/* 
	 * FIXME: This is a bit screwed up: The matrix is not filled in the
	 * same order that we would need to allocate it.
	 */
	g->fields = (int **)malloc(sizeof(int *)*(g->x));
	for(i=0; i < (g->x); i++) g->fields = (int *)malloc(sizeof(int)*(g->y))
	
	for(i=0; i < (g->y); i++) {
		for(j=0; j < (g->x); j++) {
			c = getc(f);
			switch(c) {
			case 'x':
				g->field[j][i] = AREA_BLOCKED;
				break;
			
			case '\n': /* Parsing error */
				return FALSE;
				
			default: /* By default we assume a free area */
				g->field[j][i] = AREA_CLEAR;
			}
		}
		
		/* We expect '\n' here */
		if(getc() != '\n') return FALSE;
	}
	
	/* Succesfully parsed! */
	return TRUE;
}