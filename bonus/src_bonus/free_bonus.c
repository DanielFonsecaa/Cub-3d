#include "../includes_bonus/cub_bonus.h"

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
	if (game->doors)
		free(game->doors);
	close_textures(game);
	close_canvas(game);
	if (game->grid.map)
	{
		i = 0;
		while (game->grid.map[i])
			free(game->grid.map[i++]);
		free(game->grid.map);
	}
	if (game->zbuffer)
		free(game->zbuffer);
	if (game->collectables)
		free(game->collectables);
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
	if (game->door.img)
		mlx_destroy_image(game->canvas.mlx, game->door.img);
	if (game->collect.img)
		mlx_destroy_image(game->canvas.mlx, game->collect.img);
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
		free(game->north.path);
	if (game->south.path)
		free(game->south.path);
	if (game->west.path)
		free(game->west.path);
	if (game->east.path)
		free(game->east.path);
	if (game->door.path)
		free(game->door.path);
	if (game->collect.path)
		free(game->collect.path);
	close_collectables(game);
}

void	close_collectables(t_game *game)
{
	int	i;

	i = 0;
	while (i < game->collect_frame_count)
	{
		if (game->collect_frames[i].img && game->canvas.mlx)
			mlx_destroy_image(game->canvas.mlx, game->collect_frames[i].img);
		if (game->collect_frames[i].path)
			free(game->collect_frames[i].path);
		game->collect_frames[i].img = NULL;
		game->collect_frames[i].path = NULL;
		i++;
	}
	free(game->collect_frames);
	game->collect_frames = NULL;
}
