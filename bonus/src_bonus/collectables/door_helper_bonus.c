/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   door_helper_bonus.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mipinhei <mipinhei@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/05 15:59:56 by mipinhei          #+#    #+#             */
/*   Updated: 2025/12/05 16:00:03 by mipinhei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes_bonus/cub_bonus.h"

double	dist2(double ax, double ay, double bx, double by)
{
	double	dx;
	double	dy;

	dx = ax - bx;
	dy = ay - by;
	return (dx * dx + dy * dy);
}

int	count_doors(t_game *game, int *count)
{
	int		x;
	int		y;
	size_t	rowlen;

	*count = 0;
	y = -1;
	while (++y < game->grid.height)
	{
		rowlen = ft_strlen(game->grid.map[y]);
		x = -1;
		while ((size_t)++x < rowlen)
		{
			if (game->grid.map[y][x] == 'D')
				*count += 1;
		}
	}
	if (count == 0)
	{
		game->doors = NULL;
		game->n_doors = 0;
		return (0);
	}
	return (1);
}

void	dumb_refactor_function(t_game *game, int x, int y, int *idx)
{
	game->doors[*idx].x = x;
	game->doors[*idx].y = y;
	game->doors[*idx].open = 0;
	*idx += 1;
}
