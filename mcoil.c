#include <stdio.h>

#include "defs.h"
#include "parse.h"

#define MAX_LEN_PATH 		256
#define INC_AREA 		(++area_count)
#define DEC_AREA		(--area_count)
#define PREV_AREA 		(area_count-1)
#define NEXT_AREA		(area_count+1)

#define AREA_NOW(t)		(t.field[t.x][t.y])
#define AREA_RIGHT(t)		(t.field[t.x+1][t.y])
#define AREA_DOWN(t)		(t.field[t.x][t.y+1])
#define AREA_LEFT(t)		(t.field[t.x-1][t.y])
#define AREA_UP(t)		(t.field[t.x][t.y-1])

/* Check whether the position is *outside* the field */
#define CHECK_BOUNDS_RIGHT(t) 	(t.x+1 >= t.size_x)
#define CHECK_BOUNDS_DOWN(t) 	(t.y+1 >= t.size_y)
#define CHECK_BOUNDS_LEFT(t) 	(t.x-1 < 0)
#define CHECK_BOUNDS_UP(t)	(t.y-1 < 0)

#define MOVE_RIGHT(t)		(t.x++)
#define MOVE_DOWN(t)		(t.y++)
#define MOVE_LEFT(t)		(t.x--)
#define MOVE_UP(t)		(t.y--)

enum moves {
	PLAY_LEFT = 1,
	PLAY_UP,
	PLAY_RIGHT,
	PLAY_DOWN,
	UNWIND_LEFT,
	UNWIND_UP,
	UNWIND_RIGHT,
	UNWIND_DOWN
};

struct game g;
int area_count = 0;
int start_x, start_y;

/* Test whether we win or not */

int is_done() {
	int i, j;
	
	for(i=0; i<g.size_x; i++) {
		for(j=0; j<g.size_y; j++) {
			if(g.field[i][j] == 0) return FALSE;
		}
	}
	
	return TRUE;
}

int print_sol() {
	int i, j;
	
	for(i=0; i<g.size_y; i++) {
		for(j=0; j<g.size_x; j++) printf("%2d", g.field[j][i]);
		printf("\n");
	}
	printf("\n");
	
	return TRUE;
}

int print_sol_v(int v[], int x, int y) {
	int i;
	printf("(%d,%d) ", x, y);
	for(i=0; v[i] != 0; i++) {
		switch(v[i]) {
		case PLAY_LEFT:         printf("L"); break;
		case PLAY_DOWN:         printf("D"); break;
		case PLAY_RIGHT:        printf("R"); break;
		case PLAY_UP:           printf("U"); break;
		default:
			fprintf(stderr, "ERROR: Unreachable condition\n");
			return FALSE;
		}
	}
	
	printf("\n");
	return TRUE;
}

/* 
 * Takes the direction to move and return either TRUE (success) or false.
 * Note that it only moves ONE area.
 * PS: I hope you don't mind if I shout too much :D
 */
int move(int play_to) {
	switch(play_to) {
	case PLAY_RIGHT:
	case UNWIND_LEFT:
		if(CHECK_BOUNDS_RIGHT(g)) return FALSE;
		if(play_to == PLAY_RIGHT) {
			if(AREA_RIGHT(g) != AREA_CLEAR) return FALSE;
			AREA_RIGHT(g) = NEXT_AREA;
			INC_AREA;
		} else { /* UNWIND_LEFT */
			if(AREA_NOW(g) == 1 || AREA_RIGHT(g) != PREV_AREA) return FALSE;
			AREA_NOW(g) = AREA_CLEAR;
			DEC_AREA;
		}
		MOVE_RIGHT(g);
		return TRUE;
	
	case PLAY_DOWN:
	case UNWIND_UP:
		if(CHECK_BOUNDS_DOWN(g)) return FALSE;
		if(play_to == PLAY_DOWN) {
			if(AREA_DOWN(g) != AREA_CLEAR) return FALSE;
			AREA_DOWN(g) = NEXT_AREA;
			INC_AREA;
		} else { /* UNWIND_UP */
			if(AREA_NOW(g) == 1 || AREA_DOWN(g) != PREV_AREA) return FALSE;
			AREA_NOW(g) = AREA_CLEAR;
			DEC_AREA;
		}
		MOVE_DOWN(g);
		return TRUE;
	
	case PLAY_LEFT:
	case UNWIND_RIGHT:
		if(CHECK_BOUNDS_LEFT(g)) return FALSE;
		if(play_to == PLAY_LEFT) {
			if(AREA_LEFT(g) != AREA_CLEAR) return FALSE;
			AREA_LEFT(g) = NEXT_AREA;
			INC_AREA;
		} else { /* UNWIND_RIGHT */
			if(AREA_NOW(g) == 1 || AREA_LEFT(g) != PREV_AREA) return FALSE;
			AREA_NOW(g) = AREA_CLEAR;
			DEC_AREA;
		}
		MOVE_LEFT(g);
		return TRUE;
	
	case PLAY_UP:
	case UNWIND_DOWN:
		if(CHECK_BOUNDS_UP(g)) return FALSE;
		if(play_to == PLAY_UP) {
			if(AREA_UP(g) != AREA_CLEAR) return FALSE;
			AREA_UP(g) = NEXT_AREA;
			INC_AREA;
		} else { /* UNWIND_DOWN */
			if(AREA_NOW(g) == 1 || AREA_UP(g) != PREV_AREA) return FALSE;
			AREA_NOW(g) = AREA_CLEAR;
			DEC_AREA;
		}
		MOVE_UP(g);
		return TRUE;
	
	default:
		fprintf(stderr, "ERROR:Unreachable condition\n");
	}
	
	return FALSE;
}

/* Move in a fixed direction until we reach a filled area */
int play(int play_to) {
	while(move(play_to));
	
	return TRUE;
}

/*
 * Returns whether it found the path or not (stored in v).
 * This is simply backtracking. For each direction we:
 * o Try to move one step, and then the whole way
 * o Mark the direction in the vector
 * o Recursively look in that direction
 * o Unwind our movements
 * o Remove the direction mark in the vector
 * 
 * Note that unless we return TRUE at the end, this function will never return
 * TRUE, as all the other cases are conditional to a recursive call.
 */
int look_for_path(int *v, int depth) {
	if(move(PLAY_RIGHT) && play(PLAY_RIGHT)) {
	 	v[depth] = PLAY_RIGHT;
		if(look_for_path(v, depth+1)) return TRUE;
		else play(UNWIND_RIGHT);
		v[depth] = 0;
	}
	
	if(move(PLAY_DOWN) && play(PLAY_DOWN)) {
		v[depth] = PLAY_DOWN;
		if(look_for_path(v, depth+1)) return TRUE;
		else play(UNWIND_DOWN);
		v[depth] = 0;
	}
	
	if(move(PLAY_LEFT) && play(PLAY_LEFT)) {
		v[depth] = PLAY_LEFT;
		if(look_for_path(v, depth+1)) return TRUE;
		else play(UNWIND_LEFT);
		v[depth] = 0;
	}
	
	if(move(PLAY_UP) && play(PLAY_UP)) {
		v[depth] = PLAY_UP;
		if(look_for_path(v, depth+1)) return TRUE;
		else play(UNWIND_UP);
		v[depth] = 0;
	}

	/* No more movements. Lets see if we win or not... */
	if(is_done()) print_sol_v(v, start_x, start_y);
	
	/* 
	 * We return FALSE always when we reach this point to obtain all the
	 * possible solutions.
	 */
	return FALSE;
}

void reset_game() {
	int i, j;

	area_count = 0;
	for(i=0; i<g.size_x; i++) {
		for(j=0; j<g.size_y; j++) {
			if(g.field[i][j] > 0) g.field[i][j] = 0;
		}
	}
}

int main(int argc, char *argv[]) {
	int v[MAX_LEN_PATH];
	int i, j;	

	/* Switch to getopt as soon as we have more choices!! */
	if(argc != 2) {
		fprintf(stderr, "Usage: %s filename\n", argv[0]);
		return 1;
	}

	if(!mc_parsefile(argv[1], &g)) return 2;

	for(i=0; i<g.size_x; i++) {
		for(j=0; j<g.size_y; j++) {
			if(g.field[i][j] == AREA_CLEAR) {
				reset_game();
				g.x = start_x = i;
				g.y = start_y = j;
				area_count = 0;
				AREA_NOW(g) = NEXT_AREA;
				INC_AREA;
				look_for_path(v, 0);
			}
		}
	}

	return 0;
}
