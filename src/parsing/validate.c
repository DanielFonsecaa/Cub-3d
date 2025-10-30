#include "../../includes/cub.h"

void	validate(t_game *game, int *invalid_map)
{
	if (*invalid_map)
		close_game(game, MAP_INVALID);
	if (!game->north.path)
		close_game(game, NORTH_TEXT_ERR);
	if (!game->south.path)
		close_game(game, SOUTH_TEXT_ERR);
	if (!game->east.path)
		close_game(game, EAST_TEXT_ERR);
	if (!game->west.path)
		close_game(game, WEST_TEXT_ERR);
	validate_map(game);
}

void	validate_map(t_game *game)
{
	int	x;
	int	y;
	int	flag;

	y = -1;
	flag = 0;
	while (++y < game->grid.height)
	{
		x = -1;
		while (++x < (int)ft_strlen(game->grid.map[y]))
		{
			if (game->grid.map[y][x] == '1' || ft_iswhite_space(game->grid.map[y][x]))
				continue ;
			if (game->grid.map[y][x] == '0' || ft_strchr("NWES", game->grid.map[y][x]))
				validate_cell(game, y, x, &flag);
			else
				close_game(game, INVALID_CHAR_AT_MAP);
		}
	}
	if (game->grid.height == 0)
		close_game(game, MAP_NOT_SET);
	if (!flag)
		close_game(game, NO_PLAYER);
}

void	validate_cell(t_game *game, int column_pos, int row_pos, int *flag)
{
	char	**map;

	map = game->grid.map;
	if (ft_strchr("NWES", map[column_pos][row_pos]))
	{
		set_player(game, column_pos, row_pos, flag);
		return ;
	}
	if (column_pos == 0 || !map[column_pos - 1]
		|| ft_strchr(WS, map[column_pos - 1][row_pos])
		|| row_pos >= (int)ft_strlen(map[column_pos - 1]))
		close_game(game, MAP_INVALID);
	if (row_pos >= (int)ft_strlen(map[column_pos + 1])
		|| !map[column_pos + 1] || ft_strchr(WS, map[column_pos + 1][row_pos]))
		close_game(game, MAP_INVALID);
	if (row_pos == 0 || !map[column_pos]
		|| ft_strchr(WS, map[column_pos][row_pos - 1]))
		close_game(game, MAP_INVALID);
	if (row_pos + 1 >= (int)ft_strlen(map[column_pos])
		|| !map[column_pos] || ft_strchr(WS, map[column_pos][row_pos + 1]))
		close_game(game, MAP_INVALID);
}

void	valid_name(t_game *game, t_mapi *map, char *map_name)
{
	char	*str;
	char	*file_name;
	int		len;

	map->file_name = map_name;
	str = ft_strrchr(map_name, '.');
	file_name = ft_strrchr(map_name, '/');
	if (file_name)
		len = ft_strlen(ft_strrchr(map_name, '/')) - 1;
	else
		len = ft_strlen(map_name);
	if (!str || len < 5 || ft_strncmp(str, EXT, ft_strlen(EXT) + 1))
		close_game(game, INVALID_MAP_NAME);
}
