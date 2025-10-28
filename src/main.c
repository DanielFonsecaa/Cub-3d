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
	if (game->north.img) mlx_destroy_image(game->canvas->mlx, game->north.img);
	if (game->south.img) mlx_destroy_image(game->canvas->mlx, game->south.img);
	if (game->west.img)  mlx_destroy_image(game->canvas->mlx, game->west.img);
	if (game->east.img)  mlx_destroy_image(game->canvas->mlx, game->east.img);
	if (game->player)
		free(game->player);
	mlx_destroy_image(game->canvas->mlx, game->canvas->img);
	mlx_destroy_window(game->canvas->mlx, game->canvas->win);
	mlx_destroy_display(game->canvas->mlx);
	if (game->canvas)
	{
		free(game->canvas->mlx);
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


