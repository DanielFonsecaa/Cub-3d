#ifndef DECLARATIONS_H
# define DECLARATIONS_H

# include "cub.h"

# define WIDTH 1280
# define HEIGHT 720
# define FOV 1.0471975511965977461542144610931676280657231331250
# define BLOCK 64
# define DEBUG 0
# define WS " \t\n\r\v\f"
# define VC "10NSWE"

# define W 119
# define A 97
# define S 115
# define D 100
# define LEFT 65361
# define RIGHT 65363
# define PI 3.14159265358979323846264338327950288419716939937510

# define EXT ".cub"

# define RED 0xFF0000
# define GREEN 0x00FF00
# define BLUE 0x0000FF

# define CLOSE_GAME "game has ended\n"
# define NORTH_TEXT_ERR "Error\nNot founded North texture\n"
# define SOUTH_TEXT_ERR "Error\nNot founded South texture\n"
# define WEST_TEXT_ERR "Error\nNot founded West texture\n"
# define EAST_TEXT_ERR "Error\nNot founded East texture\n"
# define MISSING_ASSETS "Error\nMissing assets\n"
# define DUPLICATE_ASSETS "Error\nDuplicate assets\n"
# define INVALID_TEXTURE "Error\nInvalid texture\n"
# define INVALID_MAP_NAME "Error\nInvalid map name\n"
# define INVALID_CHAR_AT_MAP "Error\nInvalid character on the map\n"
# define MAP_INVALID "Error\nMap is invalid\n"
# define MAP_NOT_SET "Error\nMap inexistence\n"
# define NO_BG_COLOR "Error\nMap has no floor or cealing colors\n"
# define NO_PLAYER "Error\nPlayer position not initialized\n"
# define TESTE "TESTE\n"
#endif