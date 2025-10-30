#include "../../includes/cub.h"

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

int	get_number_lines_map(t_mapi *map)
{
	int		num_lines;
	int		fd;
	char	*line;

	num_lines = 0;
	fd = open(map->file_name, O_RDONLY);
	if (fd < 0)
		return (0);
	while ((line = get_next_line(fd)) != NULL)
	{
		num_lines++;
		free(line);
	}
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

int	find_valid_line(char *line)
{
	int i;

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
