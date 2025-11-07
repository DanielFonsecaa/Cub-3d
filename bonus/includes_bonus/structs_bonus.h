#ifndef STRUCTS_BONUS_H
# define STRUCTS_BONUS_H

# include "cub_bonus.h"
# include <stdbool.h>

typedef struct s_player
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

typedef struct s_canvas
{
	void		*mlx;
	void		*win;
	void		*img;
}	t_canvas;

typedef struct s_tex
{
	void		*img;
	char		*data;
	int			width;
	int			height;
	int			bpp;
	int			size_line;
	int			endian;
	char		*path;
}	t_tex;

typedef struct s_color
{
	int			red;
	int			blue;
	int			green;
}	t_color;

typedef struct s_mapi
{
	char	**map;
	char	*file_name;
	int		height;
	int		skip_lines;
	int		fd;
}	t_mapi;

typedef struct s_ray
{
	double		cos_angle;
	double		sin_angle;
	double		eps;
	double		angle;
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
	double		orig_start_f;
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

typedef struct s_plane
{
	double		dirx;
	double		diry;
	double		planex;
	double		planey;
	double		plane_len;
	double		camera_x;
	double		raydx;
	double		raydy;
}	t_plane;

typedef struct s_collect
{
	int		x;
	int		y;
	bool	is_collected;
//	t_tex	*frames;
}	t_collect;

typedef struct s_bunny
{
	int		x;
	int		y;
}	t_bunny;

typedef struct s_game
{
	char		*data;
	int			bpp;
	int			size_line;
	int			endian;
	char		**map;
	int			assets_ready;
	int			floor_set;
	int			ceiling_set;
	int			n_collectables;
	bool		has_door;
	bool		end_game;
	t_bunny		exit;
	t_collect	*collectables;
	t_color		floor;
	t_color		cealing;
	t_player	player;
	t_canvas	canvas;
	t_plane		plane;
	t_tex		north;
	t_tex		south;
	t_tex		east;
	t_tex		west;
	t_mapi		grid;
	t_ray		ray;
//	t_tex		*collect_frames;
//	int			collect_frame_count;
}	t_game;

#endif
