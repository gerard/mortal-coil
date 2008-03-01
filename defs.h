#ifndef _DEFS_H_
#define _DEFS_H_

#define AREA_BLOCKED 		(-1)
#define AREA_CLEAR 		0

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

#endif