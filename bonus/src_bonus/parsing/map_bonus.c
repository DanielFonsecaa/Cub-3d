/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_bonus.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dda-fons <dda-fons@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/05 15:52:08 by dda-fons          #+#    #+#             */
/*   Updated: 2025/12/05 15:52:09 by dda-fons         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes_bonus/cub_bonus.h"

void	parsing(t_game *game, char *map_name)
{
	t_mapi	map;
	char	**grid;
	int		map_lines;
	int		flag;

	flag = 0;
	ft_bzero(&map, sizeof(t_mapi));
	valid_name(game, &map, map_name);
	map_lines = get_number_lines_map(&map);
	if (map_lines <= 0)
		return ;
	map.fd = open(map_name, O_RDONLY);
	if (map.fd < 0)
		return ;
	handle_assets(game, &map);
	grid = ft_calloc((map_lines - map.skip_lines + 1), sizeof(char *));
	if (!grid)
		return ;
	map.fd = open(map_name, O_RDONLY);
	map.map = grid;
	game->grid = map;
	fill_map(&game->grid, &flag);
	close(map.fd);
	find_collectables_and_doors(game);
	validate(game, &flag);
}

void	handle_assets(t_game *game, t_mapi *map)
{
	char	*line;
	int		i;

	line = NULL;
	while (game->assets_ready < 6)
	{
		line = get_next_line(map->fd);
		i = 0;
		if (!line)
			close_game(game, MISSING_ASSETS);
		while (ft_iswhite_space(line[i]))
			i++;
		get_textures(game, line);
		free(line);
		map->skip_lines++;
	}
	find_skip_lines(map);
	close(map->fd);
}

void	find_skip_lines(t_mapi *map)
{
	char	*line;

	line = NULL;
	line = get_next_line(map->fd);
	while (line != NULL)
	{
		if (find_valid_line(line))
			break ;
		free(line);
		map->skip_lines++;
		line = get_next_line(map->fd);
	}
	map->skip_lines--;
	free(line);
}

void	fill_map(t_mapi *map, int *flag)
{
	char	*line;
	int		heigth;

	skip_file_lines(map);
	heigth = 0;
	line = get_next_line(map->fd);
	while (line != NULL)
	{
		if (!find_valid_line(line))
		{
			free(line);
			break ;
		}
		map->map[heigth++] = line;
		line = get_next_line(map->fd);
	}
	skip_invalid_map(map, flag);
	map->height = heigth;
}

void	skip_invalid_map(t_mapi *map, int *flag)
{
	char	*line;

	line = NULL;
	line = get_next_line(map->fd);
	while (line != NULL)
	{
		if (find_valid_line(line))
		{
			free(line);
			line = NULL;
			*flag = 1;
			break ;
		}
		free(line);
		line = get_next_line(map->fd);
	}
	if (line)
	{
		free(line);
		line = NULL;
	}
}
