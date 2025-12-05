/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   movement_bonus.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dda-fons <dda-fons@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/05 15:53:22 by dda-fons          #+#    #+#             */
/*   Updated: 2025/12/05 15:53:23 by dda-fons         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes_bonus/cub_bonus.h"

bool	touch(double px, double py, t_game *game)
{
	int		x;
	int		y;
	int		rows;
	size_t	row_len;

	if (!game || !game->grid.map)
		return (true);
	x = (int)(px / BLOCK);
	y = (int)(py / BLOCK);
	rows = 0;
	while (game->grid.map[rows])
		rows++;
	if (y < 0 || y >= rows)
		return (true);
	row_len = ft_strlen(game->grid.map[y]);
	if (x < 0 || (size_t)x >= row_len)
		return (true);
	return (game->grid.map[y][x] == '1' || game->grid.map[y][x] == 'D');
}

void	go_up(t_game *game, double sin, double cos)
{
	double	next_x;
	double	next_y;
	double	radius;

	next_x = game->player.x + cos;
	if (cos > 0)
		radius = 8;
	else
		radius = -8;
	if (!touch(next_x + radius, game->player.y, game))
		game->player.x = next_x;
	next_y = game->player.y + sin;
	if (sin > 0)
		radius = 8;
	else
		radius = -8;
	if (!touch(game->player.x, next_y + radius, game))
		game->player.y = next_y;
}

void	go_down(t_game *game, double sin, double cos)
{
	double	next_x;
	double	next_y;
	double	radius;

	next_x = game->player.x + cos;
	if (cos > 0)
		radius = 8;
	else
		radius = -8;
	if (!touch(next_x + radius, game->player.y, game))
		game->player.x = next_x;
	next_y = game->player.y + sin;
	if (sin > 0)
		radius = 8;
	else
		radius = -8;
	if (!touch(game->player.x, next_y + radius, game))
		game->player.y = next_y;
}

void	go_left(t_game *game, double sin, double cos)
{
	double	next_x;
	double	next_y;
	double	radius;

	next_x = game->player.x + sin;
	if (sin > 0)
		radius = 8;
	else
		radius = -8;
	if (!touch(next_x + radius, game->player.y, game))
		game->player.x = next_x;
	next_y = game->player.y + cos;
	if (cos > 0)
		radius = 8;
	else
		radius = -8;
	if (!touch(game->player.x, next_y + radius, game))
		game->player.y = next_y;
}

void	go_right(t_game *game, double sin, double cos)
{
	double	next_x;
	double	next_y;
	double	radius;

	next_x = game->player.x + sin;
	if (sin > 0)
		radius = 8;
	else
		radius = -8;
	if (!touch(next_x + radius, game->player.y, game))
		game->player.x = next_x;
	next_y = game->player.y + cos;
	if (cos > 0)
		radius = 8;
	else
		radius = -8;
	if (!touch(game->player.x, next_y + radius, game))
		game->player.y = next_y;
}
