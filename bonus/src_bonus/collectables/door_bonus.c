#include "../../includes_bonus/cub_bonus.h"

double	dist2(double ax, double ay, double bx, double by)
{
	double	dx;
	double	dy;

	dx = ax - bx;
	dy = ay - by;
	return (dx * dx + dy * dy);
}

void	init_doors(t_game *game)
{
	int		x;
	int		y;
	int		idx;
	int		count;
	size_t	rowlen;

	if (!game || !game->grid.map || game->grid.height <= 0)
		return ;
	count = 0;
	for (y = 0; y < game->grid.height; y++)
	{
		rowlen = ft_strlen(game->grid.map[y]);
		for (x = 0; (size_t)x < rowlen; x++)
		{
			if (game->grid.map[y][x] == 'D')
				count++;
		}
	}
	if (count == 0)
	{
		game->doors = NULL;
		game->n_doors = 0;
		return ;
	}
	game->doors = ft_calloc(count, sizeof(t_door));
	if (!game->doors)
		close_game(game, MISSING_ASSETS);
	game->n_doors = count;
	idx = 0;
	for (y = 0; y < game->grid.height; y++)
	{
		rowlen = ft_strlen(game->grid.map[y]);
		for (x = 0; (size_t)x < rowlen; x++)
		{
			if (game->grid.map[y][x] == 'D')
			{
				game->doors[idx].x = x;
				game->doors[idx].y = y;
				game->doors[idx].open = 0;
				idx++;
				if (idx >= count)
					break ;
			}
		}
		if (idx >= count)
			break ;
	}
}

//MIPINHEI -- o abir e fechar a porta sao parecidos, depois podemos juntar tudo

void	auto_open_near_doors(t_game *game, int tiles)
{
	int		i;
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
	for (i = 0; i < game->n_doors; i++)
	{
		if (game->doors[i].open)
			continue ;
		dcx = (game->doors[i].x + 0.5) * BLOCK;
		dcy = (game->doors[i].y + 0.5) * BLOCK;
		if (dist2(px, py, dcx, dcy) <= thr2)
		{
			if (in_bounds_tile(game, game->doors[i].x, game->doors[i].y)
				&& game->grid.map[game->doors[i].y][game->doors[i].x] == 'D')
			{
				game->grid.map[game->doors[i].y][game->doors[i].x] = '0';
				game->doors[i].open = 1;
			}
		}
	}
}

void	update_doors_proximity(t_game *game, int tiles)
{
	int		i;
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
	for (i = 0; i < game->n_doors; i++)
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
