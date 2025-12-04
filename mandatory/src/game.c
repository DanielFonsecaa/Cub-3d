#include "../includes/cub.h"

void	start(t_game *game)
{
	game->player.speed = 2;
	game->player.angle_speed = 0.03;
	game->canvas.mlx = mlx_init();
	init_textures(game);
	game->canvas.win = mlx_new_window(game->canvas.mlx, WIDTH, HEIGHT, "cub3d");
	game->canvas.img = mlx_new_image(game->canvas.mlx, WIDTH, HEIGHT);
	if (!game->canvas.img)
		close_game(game, INVALID_TEXTURE);
	game->data = mlx_get_data_addr(game->canvas.img, &game->bpp,
			&game->size_line, &game->endian);
	if (!game->data)
		close_game(game, INVALID_TEXTURE);
	mlx_hook(game->canvas.win, 2, 1L << 0, key_press, game);
	mlx_hook(game->canvas.win, 3, 1L << 1, key_release, &game->player);
	mlx_loop_hook(game->canvas.mlx, game_loop, game);
	mlx_hook(game->canvas.win, DestroyNotify, 0, close_and_printf, game);
	mlx_loop(game->canvas.mlx);
}

int	game_loop(t_game *game)
{
	t_plane	plane;
	int		i;

	ft_bzero(&plane, sizeof(t_plane));
	move_player(game);
	fill_background(game);
	plane.dirx = cos(game->player.angle);
	plane.diry = sin(game->player.angle);
	plane.plane_len = tan(FOV / 2.0);
	plane.planex = -plane.diry * plane.plane_len;
	plane.planey = plane.dirx * plane.plane_len;
	i = 0;
	while (i < WIDTH)
	{
		plane.camera_x = (2.0 * i) / (double)WIDTH - 1.0;
		plane.raydx = plane.dirx + plane.planex * plane.camera_x;
		plane.raydy = plane.diry + plane.planey * plane.camera_x;
		draw_line(&game->player, game, &plane, i);
		i++;
	}
	mlx_put_image_to_window(game->canvas.mlx, game->canvas.win,
		game->canvas.img, 0, 0);
	return (0);
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
}

void	help_background_setter(t_game *g, t_color *bg, char *line, t_con *con)
{
	int	color;

	while (ft_iswhite_space(line[con->x]))
		con->x++;
	color = validate_rgb(g, line, &con->x);
	if (con->y == 0)
		bg->red = color;
	if (con->y == 1)
		bg->green = color;
	if (con->y == 2)
		bg->blue = color;
	if (con->y < 2)
	{
		while (ft_iswhite_space(line[con->x]))
			con->x++;
		if (!line[con->x])
		{
			free(line);
			close_game(g, "Error\nInvalid Color\n");
		}
	}
}
