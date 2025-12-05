/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dda-fons <dda-fons@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/05 15:51:28 by dda-fons          #+#    #+#             */
/*   Updated: 2025/12/05 15:51:29 by dda-fons         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub.h"

int	close_and_printf(t_game *game)
{
	(void)game;
	close_game(game, CLOSE_GAME);
	return (0);
}

int	close_game(t_game *game, char *msg)
{
	int	i;

	if (!game)
		return (1);
	close_textures(game);
	close_canvas(game);
	if (game->grid.map)
	{
		i = 0;
		while (game->grid.map[i])
			free(game->grid.map[i++]);
		free(game->grid.map);
	}
	if (!msg)
		msg = CLOSE_GAME;
	ft_printf(msg);
	exit(0);
	return (0);
}

void	close_canvas(t_game *game)
{
	if (game->north.img)
		mlx_destroy_image(game->canvas.mlx, game->north.img);
	if (game->south.img)
		mlx_destroy_image(game->canvas.mlx, game->south.img);
	if (game->west.img)
		mlx_destroy_image(game->canvas.mlx, game->west.img);
	if (game->east.img)
		mlx_destroy_image(game->canvas.mlx, game->east.img);
	if (game->canvas.img)
		mlx_destroy_image(game->canvas.mlx, game->canvas.img);
	if (game->canvas.win)
		mlx_destroy_window(game->canvas.mlx, game->canvas.win);
	if (game->canvas.mlx)
	{
		mlx_destroy_display(game->canvas.mlx);
		free(game->canvas.mlx);
	}
}

void	close_textures(t_game *game)
{
	if (game->north.path)
	{
		free(game->north.path);
		game->north.path = NULL;
	}
	if (game->south.path)
	{
		free(game->south.path);
		game->south.path = NULL;
	}
	if (game->west.path)
	{
		free(game->west.path);
		game->west.path = NULL;
	}
	if (game->east.path)
	{
		free(game->east.path);
		game->east.path = NULL;
	}
}
