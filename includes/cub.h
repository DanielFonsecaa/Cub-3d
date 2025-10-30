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

//parsing
void	parsing(t_game *game, char *map_name);
void	get_map(t_game *game, char *map_name);
void	fill_map(t_mapi *map, int *flag);

//assets
void	handle_assets(t_game *game, t_mapi *map);
void	get_textures(t_game *game, char *line);
void	set_direction_texture(t_game *game, char *line, int i);
int		set_texture_path(t_texture *texture, char *line, char *dir, int i);
void	set_floor_cealing(t_game *game, char *line, int i);
void	set_background_color(t_game *game, t_color *bg, int i, char *line);

//validade
void	validate_cell(t_game *game, int column_pos, int row_pos, int *flag);
void	valid_name(t_game *game, t_mapi *map, char *map_name);
void	validate(t_game *game, int *invalid_map);
void	validate_map(t_game *game);

//utils
void	skip_file_lines(t_mapi *map);
int		get_number_lines_map(t_mapi *map);
int		find_valid_line(char *line);
void	set_player(t_game *game, int column_pos, int row_pos, int *flag);

//close
int		close_and_printf(t_game *game);
int		close_game(t_game *game, char *msg);
void	close_canvas(t_game *game);
void	close_textures(t_game *game);

//player
int		key_press(int keycode, t_game *game);
int		key_release(int keycode, t_player *player);
void	move_player(t_game *game);
void	move_wasd(t_game *game, double sin, double cos);
void	go_up(t_game *game, double sin, double cos);
void	go_down(t_game *game, double sin, double cos);
void	go_left(t_game *game, double sin, double cos);
void	go_right(t_game *game, double sin, double cos);

//rendering
void	draw_line(t_player *player, t_game *game, double start_x, int i);
void	init_starting_values(t_player *player, t_game *game, double start_x);
void	dda_loop(t_game *game, t_ray *ray);

#endif