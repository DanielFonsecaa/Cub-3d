#include "../includes_bonus/cub_bonus.h"

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
