#include "../includes_bonus/cub_bonus.h"

int	mouse_hook(int x, int y, t_game *game)
{
	int		dx;
	double	delta;

	dx = x - (WIDTH / 2);
	if (dx != 0)
	{
		delta = dx * game->player.angle_speed;
		game->player.angle = game->player.angle + delta;
	}
	mlx_mouse_move(game->canvas.mlx, game->canvas.win, WIDTH / 2, y);
	return (0);
}

void	setup_plane(t_plane *p, t_game *game)
{
	int	i;

	p->dirx = cos(game->player.angle);
	p->diry = sin(game->player.angle);
	p->plane_len = tan(FOV / 2.0);
	p->planex = -p->diry * p->plane_len;
	p->planey = p->dirx * p->plane_len;
	i = 0;
	while (i < WIDTH)
	{
		p->camera_x = (2.0 * i) / (double)WIDTH - 1.0;
		p->raydx = p->dirx + p->planex * p->camera_x;
		p->raydy = p->diry + p->planey * p->camera_x;
		draw_line(&game->player, game, p, i);
		i++;
	}
	game->plane = *p;
}

void	fill_background(t_game *game)
{
	int	x;
	int	y;
	int	half;
	int	roof_color;
	int	floor_color;

	roof_color = (game->cealing.blue) | (game->cealing.green << 8)
		| (game->cealing.red << 16);
	floor_color = (game->floor.blue) | (game->floor.green << 8)
		| (game->floor.red << 16);
	y = -1;
	half = HEIGHT / 2;
	while (++y < half)
	{
		x = -1;
		while (++x < WIDTH)
			put_pixel_safe(x, y, roof_color, game);
	}
	y = half;
	while (++y < HEIGHT)
	{
		x = -1;
		while (++x < WIDTH)
			put_pixel_safe(x, y, floor_color, game);
	}
}

void	verify_bonus_init(t_game *g)
{
	if (g->door.path)
	{
		g->door.img = mlx_xpm_file_to_image(g->canvas.mlx, g->door.path,
				&g->door.width, &g->door.height);
		if (g->door.img)
			g->door.data = mlx_get_data_addr(g->door.img, &g->door.bpp,
					&g->door.size_line, &g->door.endian);
	}
	if (g->collect.path)
	{
		g->collect.img = mlx_xpm_file_to_image(g->canvas.mlx, g->collect.path,
				&g->collect.width, &g->collect.height);
		if (g->collect.img)
			g->collect.data = mlx_get_data_addr(g->collect.img, &g->collect.bpp,
					&g->collect.size_line, &g->collect.endian);
	}
}
