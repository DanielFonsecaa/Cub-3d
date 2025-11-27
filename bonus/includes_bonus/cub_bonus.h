#ifndef CUB_BONUS_H
# define CUB_BONUS_H

//	C libraries
# include <../X11/keysym.h>
# include <../X11/X.h>
# include <stdio.h>
# include <math.h>
# include <stdbool.h>
# include <stdlib.h>
# include <strings.h>

//	Custom libs
# include "../../lib/mlx/mlx.h"
# include "../../lib/libft/libft/libft.h"

//	Custom headers
# include "structs_bonus.h"
# include "declarations_bonus.h"

//parsing
void	parsing(t_game *game, char *map_name);
void	fill_map(t_mapi *map, int *flag);
void	skip_invalid_map(t_mapi *map, int *flag);
void	find_skip_lines(t_mapi *map);

//assets
void	handle_assets(t_game *game, t_mapi *map);
void	get_textures(t_game *game, char *line);
void	set_direction_texture(t_game *game, char *line, int i);
void	set_floor_cealing(t_game *game, char *line, int i);
void	set_background_color(t_game *game, t_color *bg, int i, char *line);
int		set_texture_path(t_tex *texture, char *line, char *dir, int i);
int		validate_rgb(t_game *game, char *line, int *i);
int		rgb_helper(t_game *game, char *str, char *line, int *i);

//arrumar o lugar pra essas
void	init_textures(t_game *g);
void	fill_background(t_game *game);
void	start(t_game *game);
void	init_doors(t_game *game);
void	update_doors_proximity(t_game *game, int tiles, int i);
void	auto_open_near_doors(t_game *game, int tiles, int i);
int		count_doors(t_game *game, int *count);
bool	touch(double px, double py, t_game *game);
double	dist2(double ax, double ay, double bx, double by);
int		in_bounds_tile(t_game *g, int tx, int ty);
int		game_loop(t_game *game);
int		tex_read_color(t_tex *tt, int tx, int ty);
int		sprite_cmp(const void *a, const void *b);
void	setup_plane(t_plane *p, t_game *game);
void	verify_bonus_init(t_game *g);
void	auto_open_verify_if(t_game *game, int i);
int		setup_door_init(t_game *game, int *idx, int *count, int *y);
void	dumb_refactor_function(t_game *game, int x, int y, int *idx);

//validate
void	validate_cell(t_game *game, int column_pos, int row_pos, int *flag);
void	valid_name(t_game *game, t_mapi *map, char *map_name);
void	validate(t_game *game, int *invalid_map);
void	validate_map(t_game *game);
void	validade_player(t_game *game, char **map, int x, int y);
void	validade_door(t_game *game, int x, int y);

//utils
int		get_number_lines_map(t_mapi *map);
int		find_valid_line(char *line);
void	skip_file_lines(t_mapi *map);
void	set_player(t_game *game, int column_pos, int row_pos, int *flag);

//close
int		close_and_printf(t_game *game);
int		close_game(t_game *game, char *msg);
void	close_canvas(t_game *game);
void	close_textures(t_game *game);
void	close_collectables(t_game *game);

//player
int		key_press(int keycode, t_game *game);
int		key_release(int keycode, t_player *player);
void	move_player(t_game *game);
void	move_wasd(t_game *game, double sin, double cos);
void	go_up(t_game *game, double sin, double cos);
void	go_down(t_game *game, double sin, double cos);
void	go_left(t_game *game, double sin, double cos);
void	go_right(t_game *game, double sin, double cos);
void	open_door(t_game *game);

//rendering
void	draw_line(t_player *player, t_game *game, t_plane *plane, int i);
void	init_r_val(t_game *game, t_player *player, double raydx, double raydy);
void	dda_loop(t_game *game, t_ray *ray);
void	compute_tex(t_game *g, t_ray *r,
			int i, t_tex *t);
void	render_collectables(t_game *g);
t_tex	*compute_per(t_game *g, t_ray *r, t_player *p);
t_tex	*side_gt_zero(t_game *g, t_ray *r, t_player *p);

//draw
void	set_configure(t_game *g, t_ray *r, int i, t_tex *t);
void	flip_and_draw(t_game *g, t_ray *r, int i, t_tex *t);
void	draw_lines(t_game *g, t_ray *r, int i, t_tex *t);
void	put_pixel(int x, int y, int color, t_game *game);
void	put_pixel_safe(int x, int y, int color, t_game *game);

//minimap
void	draw_minimap(t_game *game);
void	draw_square(int x, int y, int size, t_game *game);
void	draw_map(t_game *game);
void	put_pixel_minimap(int x, int y, int color, t_game *game);
void	draw_map_window(t_game *game, int px, int py);
void	fill_mm_background(t_game *game, int cx, int cy, int r);
void	mm_circle(t_game *game);
void	draw_player(t_game *game, int cx, int cy, int color);
int		is_inside_square(int x, int y);

//collectables
int		load_textures(t_game *game, t_tex *arr, int n, const char **paths);
int		load_texture(t_game *game, t_tex *tex, const char *path);
void	find_collectables_and_doors(t_game *game);
void	add_collect_or_door(t_game *game, int x, int y, int *n_collect);
void	find_n_collect(t_game *game);
void	update_collectables(t_game *game);
void	init_collectables(t_game *game);

#endif