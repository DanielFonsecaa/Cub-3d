#include "../../includes/cub.h"
/*
	char	**map;
	char	*file_name;
	int		height;
	int		fd;*/

void	parsing(t_game *game, char *map_name)
{
	get_map(game, map_name);
}

void	get_map(t_game *game, char *map_name)
{
	t_mapi	map;
	int		map_lines;
	int		flag;

	flag = 0;
	valid_name(game, &map, map_name);
	map_lines = get_number_lines_map(&map);
	if (map_lines <= 0)
		return ;
	map.fd = open(map_name, O_RDONLY);
	if (map.fd < 0)
	{
		free(map);
		return ;
	}
	handle_assets(game, &map);
	map = ft_calloc((map_lines -map.skip_lines + 1), sizeof(char *));
	if (!map)
		return ;
	map.fd = open(map_name, O_RDONLY);
	fill_map(game, &map, &flag);
	close(map.fd);
	validate(game, map);
	game->grid = map;
}

void	validate(t_game *game, t_mapi *map, int *invalid_map)
{
	if (*invalid_map)
		close_game(game, MAP_INVALID); //todo
	if (!game->north.path)
		close_game(game, NORTH_TEXT_ERR);
	if (!game->south.path)
		close_game(game, SOUTH_TEXT_ERR);
	if (!game->east.path)
		close_game(game, EAST_TEXT_ERR);
	if (!game->west.path)
		close_game(game, WEST_TEXT_ERR);
	validate_map(game, map);
}

void	fill_map(t_game *game, t_mapi *map, int *flag)
{
	char	*line;
	int		heigth;

	skip_file_lines(map); //check if skiped the 1 line later
	heigth = 0;
	while ((line = get_next_line(map->fd)) != NULL)
	{
		if (!get_lines_of_map(line)) // todo
		{
			free(line);
			break ;
		}
		map[heigth++] = line;
	}
	while ((line = get_next_line(map->fd)) != NULL)
	{
		if (get_lines_of_map(line))
		{
			free(line);
			*flag = 1;
			break ;
		}
		free(line);
	}
	map->height = heigth;
}

void	handle_assets(t_game *game, t_mapi *map)
{
	char	*line;

	line = NULL;
	while (game->assets_ready < 6)
	{
		line = get_next_line(map->fd);
		i = 0;
		if (!line)
			close_game(game, MISSING_ASSETS);
		while (ft_iswhite_space(line[i]))
			i++;
		get_textures(game, line); //todo
		free(line);
		map->skip_lines++;
	}
	while ((line = get_next_line(map->fd)) != NULL)
	{
		if (get_lines_of_map(line))
			break ;
		free(line);
		map->skip_lines++;
	}
	map->skip_lines--;
	close(map->fd)
}
