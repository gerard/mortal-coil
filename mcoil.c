#include <stdio.h>
#define MAX_LEN_PATH 	256
#define INC_AREA 		(++area_count)
#define DEC_AREA		(--area_count)
#define PREV_AREA 		(area_count-1)
#define NEXT_AREA		(area_count+1)
#define AREA_BLOCKED 	(-1)
#define AREA_CLEAR 		0

#define AREA_NOW(t)				(t.field[t.x][t.y])
#define AREA_RIGHT(t)			(t.field[t.x+1][t.y])
#define AREA_DOWN(t)			(t.field[t.x][t.y+1])
#define AREA_LEFT(t)			(t.field[t.x-1][t.y])
#define AREA_UP(t)				(t.field[t.x][t.y-1])

#define CHECK_BOUNDS_RIGHT(t) 	(t.x+1 >= t.size_x)
#define CHECK_BOUNDS_DOWN(t) 	(t.y+1 >= t.size_y)
#define CHECK_BOUNDS_LEFT(t) 	(t.x-1 >= t.size_x)
#define CHECK_BOUNDS_UP(t)	 	(t.y-1 >= t.size_y)

#define MOVE_RIGHT(t)			(t.x++)
#define MOVE_DOWN(t)			(t.y++)
#define MOVE_LEFT(t)			(t.x--)
#define MOVE_UP(t)				(t.y--)

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

enum boolean {
	FALSE = 0,
	TRUE
};

struct game {
	int **field;
	int size_x;		/* This would be the field size */
	int size_y;
	int x;			/* This is the actual position */
	int y;
};

struct game g;
int area_count = 0;

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
			if(AREA_RIGHT(g) != PREV_AREA) return FALSE;
			AREA_RIGHT(g) = AREA_CLEAR;
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
			if(AREA_DOWN(g) != PREV_AREA) return FALSE;
			AREA_DOWN(g) = AREA_CLEAR;
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
			if(AREA_LEFT(g) != PREV_AREA) return FALSE;
			AREA_LEFT(g) = AREA_CLEAR;
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
			if(AREA_DOWN(g) != PREV_AREA) return FALSE;
			AREA_DOWN(g) = AREA_CLEAR;
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
 * This is simply backtracking.
 */
int look_for_path(int *v, int depth) {
	if(move(PLAY_RIGHT) && play(PLAY_RIGHT)) {
	 	if(look_for_path(v, depth+1)) {
			v[depth] = PLAY_RIGHT;
			return TRUE;
		} else play(UNWIND_RIGHT);
	}
	
	if(move(PLAY_DOWN) && play(PLAY_DOWN)) {
		if(look_for_path(v, depth+1)) {
			v[depth] = PLAY_DOWN;
			return TRUE;
		} else play(UNWIND_DOWN);
	}
	
	if(move(PLAY_LEFT) && play(PLAY_LEFT)) {
		if(look_for_path(v, depth+1)) {
			v[depth] = PLAY_LEFT;
			return TRUE;
		} else play(UNWIND_LEFT);
	}
	
	if(move(PLAY_UP) && play(PLAY_UP)) {
		if(look_for_path(v, depth+1)) {
			v[depth] = PLAY_UP;
			return TRUE;
		} else play(UNWIND_UP);
	}

	/* No more movements. Lets see if we win or not... */
	if(is_done()) {
		print_sol();
	}
}

int main() {
	int v[MAX_LEN_PATH];
	
	AREA_NOW(g) = NEXT_AREA;
	INC_AREA;
	return look_for_path(v, 0);
}