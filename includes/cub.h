#ifndef CUB_H
# define CUB_H

//	C libraries
# include <stdlib.h>
# include <../X11/keysym.h>
# include <../X11/X.h>
# include <stdio.h>
# include <math.h>
# include <stdbool.h>

//	Custom libs
# include "../lib/mlx/mlx.h"
# include "../lib/libft/libft/libft.h"

//	Custom headers
# include "structs.h"
# include "declarations.h"

void	init_player(t_player *player);
void	init_game(t_game *game, char *map_name);
void	init_canvas(t_canvas *canvas);
char	**get_map(char *map_name);
int		key_press(int keycode, t_player *player);
int		key_release(int keycode, t_player *player);
int	game_loop(t_game *game);
void	move_player(t_player *player);
void	clear_canvas(t_game *game);
void	put_pixel(int x, int y, int color, t_game *game);
void	draw_square(int x, int y, int size, int color, t_game *game);
void	draw_map(t_game *game);
double	distance(double x, double y);
double	fixed_dist(double x1, double y1, double x2, double y2, t_game *game);
bool	touch(double px, double py, t_game *game);
void	draw_line(t_player *player, t_game *game, double start_x, int i);

#endif