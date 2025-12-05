/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   door_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mipinhei <mipinhei@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/05 15:59:54 by mipinhei          #+#    #+#             */
/*   Updated: 2025/12/05 15:59:55 by mipinhei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes_bonus/cub_bonus.h"

void	init_doors(t_game *game)
{
	int		x;
	int		y;
	int		idx;
	int		count;
	size_t	rowlen;

	if (!setup_door_init(game, &idx, &count, &y))
		return ;
	while (++y < game->grid.height)
	{
		rowlen = ft_strlen(game->grid.map[y]);
		x = -1;
		while ((size_t)++x < rowlen)
		{
			if (game->grid.map[y][x] == 'D')
			{
				dumb_refactor_function(game, x, y, &idx);
				if (idx >= count)
					break ;
			}
		}
		if (idx >= count)
			break ;
	}
}

int	setup_door_init(t_game *game, int *idx, int *count, int *y)
{
	if (!game || !game->grid.map || game->grid.height <= 0)
		return (0);
	if (!count_doors(game, count))
		return (0);
	if (*count == 0 && game->n_doors == 0)
	{
		if (game->doors != NULL)
			game->doors = NULL;
		return (0);
	}
	else
	{
		game->doors = ft_calloc(*count, sizeof(t_door));
		if (!game->doors)
			close_game(game, MISSING_ASSETS);
	}
	game->n_doors = *count;
	*idx = 0;
	*y = -1;
	return (1);
}

void	auto_open_verify_if(t_game *game, int i)
{
	if (in_bounds_tile(game, game->doors[i].x, game->doors[i].y)
		&& game->grid.map[game->doors[i].y][game->doors[i].x] == 'D')
	{
		game->grid.map[game->doors[i].y][game->doors[i].x] = '0';
		game->doors[i].open = 1;
	}
}

void	auto_open_near_doors(t_game *game, int tiles, int i)
{
	double	thr2;
	double	px;
	double	py;
	double	dcx;
	double	dcy;

	if (!game || !game->doors)
		return ;
	px = game->player.x;
	py = game->player.y;
	thr2 = ((double)(tiles * BLOCK)) * ((double)(tiles * BLOCK));
	while (++i < game->n_doors)
	{
		if (game->doors[i].open)
			continue ;
		dcx = (game->doors[i].x + 0.5) * BLOCK;
		dcy = (game->doors[i].y + 0.5) * BLOCK;
		if (dist2(px, py, dcx, dcy) <= thr2)
			auto_open_verify_if(game, i);
	}
}

void	update_doors_proximity(t_game *game, int tiles, int i)
{
	double	px;
	double	py;
	double	thr;
	double	dcy;
	double	dcx;

	px = game->player.x;
	py = game->player.y;
	thr = (double)(tiles * BLOCK);
	if (!game || !game->doors || game->n_doors <= 0)
		return ;
	while (++i < game->n_doors)
	{
		if (!game->doors[i].open)
			continue ;
		dcx = (game->doors[i].x + 0.5) * BLOCK;
		dcy = (game->doors[i].y + 0.5) * BLOCK;
		if (dist2(px, py, dcx, dcy) > (thr * thr))
		{
			if (in_bounds_tile(game, game->doors[i].x, game->doors[i].y)
				&& game->grid.map[game->doors[i].y][game->doors[i].x] == '0')
				game->grid.map[game->doors[i].y][game->doors[i].x] = 'D';
			game->doors[i].open = 0;
		}
	}
}
