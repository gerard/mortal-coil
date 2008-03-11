#ifndef _GAME_H_
#define _GAME_H_

#define AREA_CLEAR	 0
#define AREA_BLOCKED	-1

inline int prev_area();

inline void next_area_set_right();
inline void next_area_set_left();
inline void next_area_set_down();
inline void next_area_set_up();

inline void prev_area_cl_right();
inline void prev_area_cl_left();
inline void prev_area_cl_down();
inline void prev_area_cl_up();

inline int area_now();
inline int area_right();
inline int area_down();
inline int area_left();
inline int area_up();
inline int area_in(int x, int y);

inline int game_size_x();
inline int game_size_y();

inline int check_bounds_right();
inline int check_bounds_down();
inline int check_bounds_left();
inline int check_bounds_up();

/* Memory is allocated by the caller */
int init_game(int **f, int sx, int sy);
void reset_game(int x, int y);

#endif

