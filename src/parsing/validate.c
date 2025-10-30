#include "../../includes/cub.h"

void	validate_cell(t_game *game, int column_pos, int row_pos, int *flag)
{
	char **map;
	(void)flag;
	map = game->grid.map;
	/*if (ft_strchr("NWES", map[column_pos][row_pos]))
	{
		if (*flag)
			close_game(game, "Error\nDuplicate player position\n");
		game->player->x = row_pos * BLOCK + BLOCK / 2;
		game->player->y = column_pos * BLOCK + BLOCK / 2;
		if (map[column_pos][row_pos] == 'S')
			game->player->angle = PI / 2;
		if (map[column_pos][row_pos] == 'N')
			game->player->angle = PI * 1.5;
		if (map[column_pos][row_pos] == 'E')
			game->player->angle = PI * 2;
		if (map[column_pos][row_pos] == 'W')
			game->player->angle = PI;
		*flag = 1;
		return ;
	}*/
	if (column_pos == 0 || !map[column_pos - 1] || ft_strchr(WS, map[column_pos - 1][row_pos]) || row_pos >= (int)ft_strlen(map[column_pos - 1]))
		close_game(game, MAP_INVALID);
	if (row_pos >= (int)ft_strlen(map[column_pos + 1]) || !map[column_pos + 1] || ft_strchr(WS, map[column_pos + 1][row_pos]))
		close_game(game, MAP_INVALID);
	if (row_pos == 0 || !map[column_pos] || ft_strchr(WS, map[column_pos][row_pos - 1]))
		close_game(game, MAP_INVALID);
	if (row_pos + 1 >= (int)ft_strlen(map[column_pos]) || !map[column_pos] || ft_strchr(WS, map[column_pos][row_pos + 1]))
		close_game(game, MAP_INVALID);
}