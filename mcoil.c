#include <stdio.h>
#include <strings.h>

#include "defs.h"
#include "parse.h"
#include "game.h"

#define MAX_LEN_PATH 		256

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

int start_x, start_y;

/* Test whether we win or not */

int is_done() {
	int i, j;
	
	for(i=0; i<game_size_x(); i++) {
		for(j=0; j<game_size_y(); j++) {
			if(area_in(i,j) == AREA_CLEAR) return FALSE;
		}
	}
	
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
			fprintf(stderr, "ERROR: Unreachable condition: %x\n", v[i]);
			return FALSE;
		}
	}
	
	printf("\n");
	return TRUE;
}

/* 
 * Takes the direction to move and return either TRUE (success) or false.
 * Note that it only moves ONE area.
 */
int move(int play_to) {
	switch(play_to) {
	case PLAY_RIGHT:
	case UNWIND_LEFT:
		if(check_bounds_right()) return FALSE;
		if(play_to == PLAY_RIGHT) {
			if(area_right() != AREA_CLEAR) return FALSE;
			next_area_set_right();
		} else { /* UNWIND_LEFT */
			if(area_now() == 1 || area_right() != prev_area()) return FALSE;
			prev_area_cl_right();
		}
		return TRUE;
	
	case PLAY_DOWN:
	case UNWIND_UP:
		if(check_bounds_down()) return FALSE;
		if(play_to == PLAY_DOWN) {
			if(area_down() != AREA_CLEAR) return FALSE;
			next_area_set_down();
		} else { /* UNWIND_UP */
			if(area_now() == 1 || area_down() != prev_area()) return FALSE;
			prev_area_cl_down();
		}
		return TRUE;
	
	case PLAY_LEFT:
	case UNWIND_RIGHT:
		if(check_bounds_left()) return FALSE;
		if(play_to == PLAY_LEFT) {
			if(area_left() != AREA_CLEAR) return FALSE;
			next_area_set_left();
		} else { /* UNWIND_RIGHT */
			if(area_now() == 1 || area_left() != prev_area()) return FALSE;
			prev_area_cl_left();
		}
		return TRUE;
	
	case PLAY_UP:
	case UNWIND_DOWN:
		if(check_bounds_up()) return FALSE;
		if(play_to == PLAY_UP) {
			if(area_up() != AREA_CLEAR) return FALSE;
			next_area_set_up();
		} else { /* UNWIND_DOWN */
			if(area_now() == 1 || area_up() != prev_area()) return FALSE;
			prev_area_cl_up();
		}
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

int main(int argc, char *argv[]) {
	int v[MAX_LEN_PATH];
	int i, j;	

	/* Switch to getopt as soon as we have more choices!! */
	if(argc != 2) {
		fprintf(stderr, "Usage: %s filename\n", argv[0]);
		return 1;
	}

	bzero(v, MAX_LEN_PATH);
	if(!mc_parsefile(argv[1])) return 2;

	for(i=0; i<game_size_x(); i++) {
		for(j=0; j<game_size_y(); j++) {
			if(area_in(i, j) == AREA_CLEAR) {
				start_x = i;
				start_y = j;
				reset_game(i, j);
				look_for_path(v, 0);
			}
		}
	}
	
	return 0;
}
