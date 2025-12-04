#include "../includes_bonus/cub_bonus.h"

int	main(int argc, char **argv)
{
	t_game	game;

	if (argc != 2)
	{
		ft_printf("Usage: %s <map_file>\n", argv[0]);
		return (1);
	}
	ft_bzero(&game, sizeof(t_game));
	parsing(&game, argv[1]);
	if (!game.grid.map)
	{
		ft_printf("Error\nEmpty map\n");
		exit(1);
	}
	start(&game);
	return (0);
}
