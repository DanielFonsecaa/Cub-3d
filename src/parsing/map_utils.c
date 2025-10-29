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

void	get_number_lines_map(t_mapi *map)
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
	while (i < map->skip_lines)
	{
		line = get_next_line(map->fd);
		free(line);
		i++;
	}
}

void	validate_map(t_game *game, t_mapi *map)
{
	int	x;
	int	y;
	int	flag;

	y = -1;
	flag = 0;
	while (++y < map->height)
	{
		x = -1;
		while (++x < (int)ft_strlen(map[y]))
		{
			if (map[y][x] == '1' || ft_iswhite_space(map[y][x]))
				continue ;
			if (map[y][x] == '0' || ft_strchr("NWES", map[y][x]))
				validate_cell(game, y, x, &flag); //todo
			else
				close_game(game, INVALID_CHAR_AT_MAP);
		}
	}
	if (map->height == 0)
		close_game(game, MAP_NOT_SET);
	if (!flag)
		close_game(game, NO_PLAYER);
}
