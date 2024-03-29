#ifndef GRAPHICS_H
#define GRAPHICS_H


/**
 * Draws the player. This depends on the player state, so it is not a DrawFunc.
 */
void draw_player(int u, int v, int key);

/**
 * Takes a string image and draws it to the screen. The string is 121 characters
 * long, and represents an 11x11 tile in row-major ordering (across, then down,
 * like a regular multi-dimensional array). The available colors are:
 *      R = Red
 *      Y = Yellow
 *      G = Green
 *      D = Brown ("dirt")
 *      5 = Light grey (50%)
 *      3 = Dark grey (30%)
 *      Any other character is black
 * More colors can be easily added by following the pattern already given.
 */
void draw_img(int u, int v, const char* img);

/**
 * DrawFunc functions. 
 * These can be used as the MapItem draw functions.
 */
void draw_nothing(int u, int v);
void draw_wall(int u, int v);
void draw_plant(int u, int v);
void draw_npc(int u, int v);
void draw_gbMan(int u, int v);
void draw_stairs(int u, int v);
void draw_milk(int u, int v);
void draw_cookie(int u, int v);
void draw_door(int u, int v);
void draw_good_fly(int u, int v);
void draw_bad_fly(int u, int v);

/**
 * Draw the upper status bar.
 */
void draw_upper_status(int x, int y, int time_reamining, int time_spent);

/**
 * Draw the lower status bar.
 */ 
void draw_lower_status(int hasKey, int defeatMonster);

/**
 * Draw the border for the map.
 */
void draw_border();

#endif // GRAPHICS_H