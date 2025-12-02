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
	char		*path;
	int			width;
	int			height;
	int			bpp;
	int			size_line;
	int			endian;
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
	int			map_x;
	int			map_y;
	int			step_x;
	int			step_y;
	int			side;
	int			iter;
	int			rows;
	int			tex_x;
	int			draw_start;
	int			draw_end;
	int			draw_pixels;
	int			tex_x_flipped;
	int			tex_y;
	int			t_idx;
	int			color;
	char		cell;
	double		cos_angle;
	double		sin_angle;
	double		eps;
	double		angle;
	double		delta_dist_x;
	double		delta_dist_y;
	double		side_dist_x;
	double		side_dist_y;
	double		perp_dist;
	double		uncorredted_dist;
	double		hit_x;
	double		hit_y;
	double		angle_diff;
	double		proj_plane;
	double		wall_hit;
	double		orig_height;
	double		orig_start_f;
	double		orig_end_f;
	double		tex_step;
	double		tex_pos;
	size_t		row_len;

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
	int			x;
	int			y;
	bool		is_collected;
	t_tex		*frames;
}	t_collect;

typedef struct s_bunny
{
	int			x;
	int			y;
}	t_bunny;

typedef struct s_door
{
	int			x;
	int			y;
	bool		open;
}	t_door;

typedef struct s_sprite
{
	int			mx;
	int			my;
	int			screen_x;
	int			start_x;
	int			end_x;
	int			start_y;
	int			end_y;
	int			spr_w;
	int			spr_h;
	double		trans_x;
	double		trans_y;
}	t_sprite;

typedef struct t_minimap
{
	int	mx;
	int	my;
	int	cx;
	int	cy;
	int	dx;
	int	dy;
	int	screen_x;
	int	screen_y;
	int	x;
	int	y;
}	t_mini;

typedef struct s_game
{
	int			bpp;
	int			size_line;
	int			endian;
	int			assets_ready;
	int			floor_set;
	int			ceiling_set;
	int			n_collectables;
	int			collect_frame_count;
	int			n_doors;
	char		*data;
	char		**map;
	bool		has_door;
	bool		end_game;
	double		*zbuffer;
	t_tex		north;
	t_tex		south;
	t_tex		east;
	t_tex		west;
	t_tex		door;
	t_tex		collect;
	t_tex		*collect_frames;
	t_ray		ray;
	t_door		*doors;
	t_mapi		grid;
	t_bunny		exit;
	t_color		floor;
	t_color		cealing;
	t_plane		plane;
	t_canvas	canvas;
	t_player	player;
	t_collect	*collectables;
}	t_game;

#endif
