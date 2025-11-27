#include "../includes_bonus/cub_bonus.h"

static int	mouse_hook(int x, int y, t_game *game)
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

void	start(t_game *game)
{
	game->player.speed = 3;
	game->player.angle_speed = 0.003;
	game->canvas.mlx = mlx_init();
	init_textures(game);
	init_collectables(game);
	init_doors(game);
	game->canvas.win = mlx_new_window(game->canvas.mlx, WIDTH, HEIGHT, "cub3d");
	game->canvas.img = mlx_new_image(game->canvas.mlx, WIDTH, HEIGHT);
	if (!game->canvas.img)
		close_game(game, INVALID_TEXTURE);
	game->data = mlx_get_data_addr(game->canvas.img, &game->bpp,
			&game->size_line, &game->endian);
	if (!game->data)
		close_game(game, INVALID_TEXTURE);
	game->zbuffer = (double *)malloc(sizeof(double) * WIDTH);
	if (!game->zbuffer)
		close_game(game, MISSING_ASSETS);
	mlx_hook(game->canvas.win, 2, 1L << 0, key_press, game);
	mlx_hook(game->canvas.win, 3, 1L << 1, key_release, &game->player);
	mlx_loop_hook(game->canvas.mlx, game_loop, game);
	mlx_hook(game->canvas.win, DestroyNotify, 0, close_and_printf, game);
	mlx_hook(game->canvas.win, MotionNotify, 1L << 6, mouse_hook, game);
	mlx_loop(game->canvas.mlx);
}

int	game_loop(t_game *game)
{
	t_plane	plane;
	int		i;

	ft_bzero(&plane, sizeof(t_plane));
	move_player(game);
	fill_background(game);
	i = 0;
	while (i < WIDTH)
	{
		game->zbuffer[i] = 1e9;
		i++;
	}
	setup_plane(&plane, game);
	render_collectables(game);
	update_collectables(game);
	draw_minimap(game);
	mlx_put_image_to_window(game->canvas.mlx, game->canvas.win,
		game->canvas.img, 0, 0);
	auto_open_near_doors(game, 2, -1);
	update_doors_proximity(game, 2, -1);
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

void	init_textures(t_game *g)
{
	g->north.img = mlx_xpm_file_to_image(g->canvas.mlx, g->north.path,
			&g->north.width, &g->north.height);
	if (!g->north.img)
		close_game(g, "Error\nMlx failed to load north\n");
	g->north.data = mlx_get_data_addr(g->north.img, &g->north.bpp,
			&g->north.size_line, &g->north.endian);
	g->south.img = mlx_xpm_file_to_image(g->canvas.mlx, g->south.path,
			&g->south.width, &g->south.height);
	if (!g->south.img)
		close_game(g, "Error\nMlx failed to load south\n");
	g->south.data = mlx_get_data_addr(g->south.img, &g->south.bpp,
			&g->south.size_line, &g->south.endian);
	g->west.img = mlx_xpm_file_to_image(g->canvas.mlx, g->west.path,
			&g->west.width, &g->west.height);
	if (!g->west.img)
		close_game(g, "Error\nMlx failed to load west\n");
	g->west.data = mlx_get_data_addr(g->west.img, &g->west.bpp,
			&g->west.size_line, &g->west.endian);
	g->east.img = mlx_xpm_file_to_image(g->canvas.mlx, g->east.path,
			&g->east.width, &g->east.height);
	if (!g->east.img)
		close_game(g, "Error\nMlx failed to load east\n");
	g->east.data = mlx_get_data_addr(g->east.img, &g->east.bpp,
			&g->east.size_line, &g->east.endian);
	verify_bonus_init(g);
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
