#include "../../includes_bonus/cub_bonus.h"

void	find_collectables_and_doors(t_game *game)
{
	int		y;
	int		x;
	int		n_collect;
	char	**map;

	find_n_collect(game);
	game->collectables = ft_calloc(game->n_collectables, sizeof(t_collect));
	y = 0;
	n_collect = 0;
	map = game->grid.map;
	while (map[y])
	{
		x = 0;
		while (map[y][x])
		{
			if (map[y][x] == 'C' || map[y][x] == 'D')
				add_collect_or_door(game, x, y, &n_collect);
			x++;
		}
		y++;
	}
}

void	add_collect_or_door(t_game *game, int x, int y, int *n_collect)
{
	if (game->grid.map[y][x] == 'C')
	{
		game->collectables[*n_collect].x = x;
		game->collectables[*n_collect].y = y;
		game->collectables[*n_collect].is_collected = false;
		*n_collect += 1;
		return ;
	}
	if (y == 0 || !game->grid.map[y - 1]
		|| x >= (int)ft_strlen(game->grid.map[y - 1])
		|| ft_strchr(WS, game->grid.map[y - 1][x]))
		close_game(game, MAP_INVALID);
	if (x >= (int)ft_strlen(game->grid.map[y + 1])
		|| !game->grid.map[y + 1] || ft_strchr(WS, game->grid.map[y + 1][x]))
		close_game(game, MAP_INVALID);
	if (x == 0 || !game->grid.map[y]
		|| ft_strchr(WS, game->grid.map[y][x - 1]))
		close_game(game, MAP_INVALID);
	if (x + 1 >= (int)ft_strlen(game->grid.map[y])
		|| !game->grid.map[y] || ft_strchr(WS, game->grid.map[y][x + 1]))
		close_game(game, MAP_INVALID);
	validade_door(game, x, y);
	game->exit.y = y;
	game->exit.x = x;
}

void	find_n_collect(t_game *game)
{
	int		y;
	int		x;
	int		n_collect;
	char	**map;

	n_collect = 0;
	map = game->grid.map;
	y = 0;
	while (map[y])
	{
		x = 0;
		while (map[y][x])
		{
			if (map[y][x] == 'C')
				n_collect++;
			if (!game->has_door && map[y][x] == 'D')
				game->has_door = true;
			x++;
		}
		y++;
	}
	game->n_collectables = n_collect;
}

void	validade_door(t_game *game, int x, int y)
{
	char	**map;

	map = game->grid.map;
	if (!(((map[y][x + 1] == '1' && map[y][x - 1] == '1')
		&& (map[y + 1][x] == '0' && map[y - 1][x] == '0'))
		|| ((map[y + 1][x] == '1' && map[y - 1][x] == '1')
		&& (map[y][x + 1] == '0' && map[y][x - 1] == '0'))))
		close_game(game, INVALID_DOOR);
}
