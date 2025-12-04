#include "../../includes_bonus/cub_bonus.h"

int	get_number_lines_map(t_mapi *map)
{
	int		num_lines;
	int		fd;
	char	*line;

	num_lines = 0;
	fd = open(map->file_name, O_RDONLY);
	if (fd < 0)
		return (0);
	line = get_next_line(fd);
	while (line != NULL)
	{
		num_lines++;
		free(line);
		line = get_next_line(fd);
	}
	free(line);
	close(fd);
	return (num_lines);
}

void	skip_file_lines(t_mapi *map)
{
	char	*line;
	int		i;

	line = NULL;
	i = 0;
	while (i <= map->skip_lines)
	{
		line = get_next_line(map->fd);
		free(line);
		i++;
	}
}

void	set_player(t_game *game, int column_pos, int row_pos, int *flag)
{
	char	**map;

	map = game->grid.map;
	if (*flag)
		close_game(game, "Error\nDuplicate player position\n");
	game->player.x = row_pos * BLOCK + BLOCK / 2;
	game->player.y = column_pos * BLOCK + BLOCK / 2;
	if (map[column_pos][row_pos] == 'S')
		game->player.angle = PI / 2;
	if (map[column_pos][row_pos] == 'N')
		game->player.angle = PI * 1.5;
	if (map[column_pos][row_pos] == 'E')
		game->player.angle = PI * 2;
	if (map[column_pos][row_pos] == 'W')
		game->player.angle = PI;
	*flag = 1;
	return ;
}

int	find_valid_line(char *line)
{
	int	i;

	if (!line)
		return (0);
	i = 0;
	while (ft_iswhite_space(line[i]))
		i++;
	if (line[i] == '\0')
		return (0);
	if (!ft_isascii(line[i]) && !ft_iswhite_space(line[i]))
		return (0);
	return (1);
}
