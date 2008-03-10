#include "game.h"
#include "defs.h"

struct game {
	int **field;
	int size_x;		/* This would be the field size */
	int size_y;
	int x;			/* This is the actual position */
	int y;
};

struct game g;
int area_count = 1;

/*** PRIVATE SETTER ***/
inline void reset_area(x, y)		{ g.field[x][y] = 0; }

/*** PUBLIC GETTERS ***/
inline int prev_area()			{ return area_count - 2; }

inline int area_now()			{ return g.field[g.x][g.y]; }
inline int area_right()			{ return g.field[g.x+1][g.y]; }
inline int area_down()			{ return g.field[g.x][g.y+1]; }
inline int area_left()			{ return g.field[g.x-1][g.y]; }
inline int area_up()			{ return g.field[g.x][g.y-1]; }
inline int area_in(int x, int y)	{ return g.field[x][y]; }

inline int game_size_x()		{ return g.size_x; }
inline int game_size_y()		{ return g.size_y; }

/* These do tests on the status of the game (return a boolean value) */
inline int check_bounds_right()		{ return g.x+1 >= g.size_x; }
inline int check_bounds_down()		{ return g.y+1 >= g.size_y; }
inline int check_bounds_left()		{ return g.x-1 < 0; }
inline int check_bounds_up()		{ return g.y-1 < 0; }

/*** PUBLIC SETTERS ***/
/* next_area_set_"direction"():
 * Move to direction, set current area to area_count and increase it afterwards 
 */
inline void next_area_set_right()	{ g.x++; g.field[g.x][g.y] = area_count; area_count++; }
inline void next_area_set_left()	{ g.x--; g.field[g.x][g.y] = area_count; area_count++; }
inline void next_area_set_down()	{ g.y++; g.field[g.x][g.y] = area_count; area_count++; }
inline void next_area_set_up()		{ g.y--; g.field[g.x][g.y] = area_count; area_count++; }

/* next_area_cl_"direction"():
 * Set current area to AREA_CLEAR, move to direction and increase area_count
 */
inline void prev_area_cl_right()	{ g.field[g.x][g.y] = AREA_CLEAR; g.x++; --area_count; }
inline void prev_area_cl_left()		{ g.field[g.x][g.y] = AREA_CLEAR; g.x--; --area_count; }
inline void prev_area_cl_down()		{ g.field[g.x][g.y] = AREA_CLEAR; g.y++; --area_count; }
inline void prev_area_cl_up()		{ g.field[g.x][g.y] = AREA_CLEAR; g.y--; --area_count; }

/* High-level setters */
int init_game(int **f, int sx, int sy) {
	g.field = f;
	
	g.size_x = sx;
	g.size_y = sy;

	return TRUE;
}

void reset_game(int x, int y) {
	int i, j;
	
	g.x = x;
	g.y = y;

	for(i=0; i<game_size_x(); i++) {
		for(j=0; j<game_size_y(); j++) {
			if(area_in(i,j) > 0) reset_area(i, j);
		}
	}
	
	g.field[x][y] = 1;
	area_count = 2;
}
