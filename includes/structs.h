#ifndef STRUCTS_H
# define STRUCTS_H

typedef struct	s_player
{
	double		x;
	double		y;
	double		angle;

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


typedef struct	s_game
{
	char		*data;
	int			bpp;
	int			size_line;
	int			endian;
	char		**map;
	t_player	player;
	t_canvas	canvas;
}	t_game;

#endif