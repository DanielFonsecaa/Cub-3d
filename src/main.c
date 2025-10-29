#include "../includes/cub.h"

int	close_and_printf(t_game *game)
{
	close_game(game, CLOSE_GAME);
	return (0);
}

int	close_game(t_game *game, char *msg)
{
	if (game->north.path) free(game->north.path);
    if (game->south.path) free(game->south.path);
    if (game->west.path)  free(game->west.path);
    if (game->east.path)  free(game->east.path);
	if (game->player)
		free(game->player);
	if (game->canvas)
	{
		if (game->north.img) mlx_destroy_image(game->canvas->mlx, game->north.img);
		if (game->south.img) mlx_destroy_image(game->canvas->mlx, game->south.img);
		if (game->west.img)  mlx_destroy_image(game->canvas->mlx, game->west.img);
		if (game->east.img)  mlx_destroy_image(game->canvas->mlx, game->east.img);
		if (game->canvas->img) mlx_destroy_image(game->canvas->mlx, game->canvas->img);
		if (game->canvas->win) mlx_destroy_window(game->canvas->mlx, game->canvas->win);
		if (game->canvas->mlx) 
		{
			mlx_destroy_display(game->canvas->mlx);
			free(game->canvas->mlx);
		}
		free(game->canvas);
	}
	if (game->map)
	{
		int i = 0;
		while (game->map[i])
			free(game->map[i++]);
		free(game->map);
	}
	if (!msg)
		msg = CLOSE_GAME;
	ft_printf(msg);
	exit(0);
	return (0);
}

void	loop(t_game *game, t_player *player)
{
	init_canvas(game->canvas);
	game->north.img = mlx_xpm_file_to_image(game->canvas->mlx, game->north.path, &game->north.width, &game->north.height);
	if (!game->north.img) { ft_printf("mlx failed to load north: %s\n", game->north.path); close_game(game, INVALID_TEXTURE); }
	game->north.data = mlx_get_data_addr(game->north.img, &game->north.bpp, &game->north.size_line, &game->north.endian);

	game->south.img = mlx_xpm_file_to_image(game->canvas->mlx, game->south.path, &game->south.width, &game->south.height);
	if (!game->south.img) { ft_printf("mlx failed to load south: %s\n", game->south.path); close_game(game, INVALID_TEXTURE); }
	game->south.data = mlx_get_data_addr(game->south.img, &game->south.bpp, &game->south.size_line, &game->south.endian);

	game->west.img = mlx_xpm_file_to_image(game->canvas->mlx, game->west.path, &game->west.width, &game->west.height);
	if (!game->west.img) { ft_printf("mlx failed to load west: %s\n", game->west.path); close_game(game, INVALID_TEXTURE); }
	game->west.data = mlx_get_data_addr(game->west.img, &game->west.bpp, &game->west.size_line, &game->west.endian);

	game->east.img = mlx_xpm_file_to_image(game->canvas->mlx, game->east.path, &game->east.width, &game->east.height);
	if (!game->east.img) { ft_printf("mlx failed to load east: %s\n", game->east.path); close_game(game, INVALID_TEXTURE); }
	game->east.data = mlx_get_data_addr(game->east.img, &game->east.bpp, &game->east.size_line, &game->east.endian);

	if (!game->canvas->img)
		close_game(game, INVALID_TEXTURE);
	game->data = mlx_get_data_addr(game->canvas->img, &game->bpp, &game->size_line, &game->endian);
	if (!game->data)
		close_game(game, INVALID_TEXTURE);
	mlx_hook(game->canvas->win, 2, 1L << 0, key_press, game);
	mlx_hook(game->canvas->win, 3, 1L << 1, key_release, player);
	mlx_loop_hook(game->canvas->mlx, game_loop, game);
	mlx_hook(game->canvas->win, DestroyNotify, 0, close_and_printf, game);
	mlx_loop(game->canvas->mlx);
}

int	main(int argc, char **argv)
{
	t_game	game;

	if (argc < 2)
	{
		ft_printf("Usage: %s <map_file>\n", argv[0]);
		return (1);
	}
	ft_bzero(&game, sizeof(t_game));
	init_game(&game);
	get_map(&game, argv[1]);
	if (!game.map) { perror("get_map"); exit(1); }
	loop(&game, game.player);
	return (0);
}
