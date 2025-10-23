#ifndef DECLARATIONS_H
# define DECLARATIONS_H

#include "cub.h"

# define WIDTH 1280
# define HEIGHT 720
# define FOV (60.0 * M_PI / 180.0)
# define BLOCK 64
# define DEBUG 0
# define WS " \t\n\r\v\f"

# define W 119
# define A 97
# define S 115
# define D 100
# define LEFT 65361
# define RIGHT 65363

# define PI 3.14159265359

# define RED 0xFF0000
# define GREEN 0x00FF00
# define BLUE 0x0000FF

# define CLOSE_GAME "game has ended\n"
# define NORTH_TEXT_ERR "Error\nNot founded North texture\n"
# define SOUTH_TEXT_ERR "Error\nNot founded South texture\n"
# define WEST_TEXT_ERR "Error\nNot founded West texture\n"
# define EAST_TEXT_ERR "Error\nNot founded East texture\n"
# define INVALID_TEXTURE "Error\nInvalid texture\n"
# define MAP_INVALID "Error\nMap is invalid\n"
# define NO_BG_COLOR "Error\nMap has no floor or cealing colors\n"
#endif