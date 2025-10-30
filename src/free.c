#include "../includes/cub.h"

int	close_and_printf(t_game *game)
{
	(void)game;
	close_game(game, CLOSE_GAME);
	return (0);
}
/*
int	close_game(t_game *game, char *msg)
{
	if (game->north.path)
		free(game->north.path);
	if (game->south.path)
		free(game->south.path);
	if (game->west.path)
		free(game->west.path);
	if (game->east.path)
		free(game->east.path);
	if (game->player)
		free(game->player);
	if (game->canvas)
	{
		if (game->north.img)
			mlx_destroy_image(game->canvas->mlx, game->north.img);
		if (game->south.img)
			mlx_destroy_image(game->canvas->mlx, game->south.img);
		if (game->west.img)
			mlx_destroy_image(game->canvas->mlx, game->west.img);
		if (game->east.img)
			mlx_destroy_image(game->canvas->mlx, game->east.img);
		if (game->canvas->img)
			mlx_destroy_image(game->canvas->mlx, game->canvas->img);
		if (game->canvas->win)
			mlx_destroy_window(game->canvas->mlx, game->canvas->win);
		if (game->canvas->mlx) 
		{
			mlx_destroy_display(game->canvas->mlx);
			free(game->canvas->mlx);
		}
		free(game->canvas);
	}
	if (game->map)
	{
		int i = 0;
		while (game->map[i])
			free(game->map[i++]);
		free(game->map);
	}
	if (!msg)
		msg = CLOSE_GAME;
	ft_printf(msg);
	exit(0);
	return (0);
}*/
