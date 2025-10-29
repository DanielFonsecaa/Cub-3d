#ifndef CUB_H
# define CUB_H

//	C libraries
# include <../X11/keysym.h>
# include <../X11/X.h>
# include <stdio.h>
# include <math.h>
# include <stdbool.h>
# include <stdlib.h>
#include <strings.h>
//	Custom libs
# include "../lib/mlx/mlx.h"
# include "../lib/libft/libft/libft.h"

//	Custom headers
# include "structs.h"
# include "declarations.h"

void	init_map(t_game *game, char *map_name);
void	parsing(t_game *game, char *map_name);

#endif