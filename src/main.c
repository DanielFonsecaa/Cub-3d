#include "../includes/cub.h"

int	main(int argc, char **argv)
{
	t_game	game;

	if (argc < 2)
	{
		ft_printf("Usage: %s <map_file>\n", argv[0]);
		return (1);
	}
	init_game(&game, argv[1]);
	game.map = get_map(argv[1]);
	if (!game.map) { perror("get_map"); exit(1); }
	mlx_hook(game.canvas.mlx, 2, 1L << 0, key_press, &game.player);
	mlx_hook(game.canvas.win, 3, 1L << 1, key_release, &game.player);
	mlx_loop_hook(game.canvas.mlx, game_loop, &game);
	mlx_loop(game.canvas.mlx);
	return (0);
}
