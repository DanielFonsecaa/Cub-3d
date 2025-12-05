/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   assets.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dda-fons <dda-fons@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/05 15:51:32 by dda-fons          #+#    #+#             */
/*   Updated: 2025/12/05 17:43:34 by dda-fons         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub.h"

void	get_textures(t_game *game, char *line)
{
	int	i;

	i = 0;
	if (line[i])
	{
		while (ft_iswhite_space(line[i]))
			i++;
		if (!line[i])
			return ;
		set_direction_texture(game, line, i);
		set_floor_cealing(game, line, i);
	}
}

void	set_direction_texture(t_game *game, char *line, int i)
{
	if (!line[i + 2] || !line)
		return ;
	if (ft_strncmp("NO", line + i, 2) == 0)
	{
		set_texture_path(game, &game->north, line, i);
		return ;
	}
	if (ft_strncmp("SO", line + i, 2) == 0)
	{
		set_texture_path(game, &game->south, line, i);
		return ;
	}
	if (ft_strncmp("WE", line + i, 2) == 0)
	{
		set_texture_path(game, &game->west, line, i);
		return ;
	}
	if (ft_strncmp("EA", line + i, 2) == 0)
	{
		set_texture_path(game, &game->east, line, i);
		return ;
	}
}

void	set_texture_path(t_game *g, t_tex *texture, char *line, int i)
{
	if (texture->path)
	{
		free(line);
		close_game(g, MAP_INVALID);
	}
	texture->path = ft_strtrim(line + i + 2, WS);
	if (!texture->path || !is_xpm(texture->path))
	{
		free(line);
		close_game(g, MAP_INVALID);
	}
	g->assets_ready++;
	return ;
}

void	set_floor_cealing(t_game *game, char *line, int i)
{
	if (!line[i + 1])
		return ;
	if (ft_strncmp("F", line + i, 1) == 0)
	{
		if (game->floor_set)
		{
			free(line);
			close_game(game, "Error\nDuplicate floor color definition\n");
		}
		set_background_color(game, &game->floor, i + 1, line);
		game->floor_set = 1;
		game->assets_ready++;
	}
	else if (ft_strncmp("C", line + i, 1) == 0)
	{
		if (game->ceiling_set)
		{
			free(line);
			close_game(game, "Error\nDuplicate ceiling color definition\n");
		}
		set_background_color(game, &game->cealing, i + 1, line);
		game->ceiling_set = 1;
		game->assets_ready++;
	}
}

void	set_background_color(t_game *game, t_color *bg, int i, char *line)
{
	t_con	counter;

	counter.x = i;
	counter.y = -1;
	while (++counter.y < 3)
		help_background_setter(game, bg, line, &counter);
	if (line[counter.x])
	{
		while (ft_iswhite_space(line[counter.x]))
			counter.x++;
		if (line[counter.x])
		{
			free(line);
			close_game(game, "Error\nInvalid Color\n");
		}
	}
}
