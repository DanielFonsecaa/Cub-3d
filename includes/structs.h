#ifndef STRUCTS_H
# define STRUCTS_H

# include "cub.h"
# include <stdbool.h>

typedef struct	s_player
{
	double		x;
	double		y;
	double		angle;
	double		angle_speed;
	int			speed;

	bool		key_up;
	bool		key_down;
	bool		key_left;
	bool		key_right;

	bool		left_rotate;
	bool		right_rotate;
}	t_player;

typedef struct	s_canvas
{
	void		*mlx;
	void		*win;
	void		*img;
}	t_canvas;

typedef struct s_texture {
	void		*img;
	char		*data;
	int			width;
	int			height;
	int			bpp;
	int			size_line;
	int			endian;
	char		*path;
} t_texture;

typedef struct s_color {
	int			red;
	int			blue;
	int			green;
} t_color;


typedef struct s_mapi {
	char	**map;
	char	*file_name;
	int		height;
	int		skip_lines;
	int		fd;
} t_mapi;


typedef struct s_ray
{
	double		cos_angle;
	double		sin_angle;
	double		EPS;
	double		delta_x;
	double		delta_y;
	double		angle;
	double		fix_dist;
	double		delta_dist_x;
	double		delta_dist_y;
	int			map_x;
	int			map_y;
	int			step_x;
	int			step_y;
	double		side_dist_x;
	double		side_dist_y;
	int			side;
	int			iter;
	int			rows;
	size_t		row_len;
	double		perp_dist;
	double		uncorredted_dist;
	double		hit_x;
	double		hit_y;
	double		angle_diff;
	double		proj_plane;
	double		wall_hit;
	int			tex_x;
	double		orig_height;
	double		orig_end_f;
	int			draw_start;
	int			draw_end;
	int			draw_pixels;
	double		tex_step;
	double		tex_pos;
	int			tex_x_flipped;
	int			tex_y;
	int			t_idx;
	int			color;

}	t_ray;

typedef struct	s_game
{
	char		*data;
	int			bpp;
	int			size_line;
	int			endian;
	char		**map;
	int			assets_ready;
	int			floor_set;
	int			ceiling_set;
	t_color		floor;
	t_color		cealing;
	t_player	player;
	t_canvas	canvas;
	t_texture	north;
	t_texture	south;
	t_texture	east;
	t_texture	west;
	t_mapi		grid;
	t_ray		ray;
}	t_game;

#endif
