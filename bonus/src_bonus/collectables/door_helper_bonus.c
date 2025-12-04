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
/* 
void	init_doors(t_game *game)
{
	int		x;
	int		y;
	int		idx;
	int		count;
	size_t	rowlen;

 	if (!game || !game->grid.map || game->grid.height <= 0)
		return ;
	if (!count_doors(game, &count))
		return ;
	game->doors = ft_calloc(count, sizeof(t_door));
	if (!game->doors)
		close_game(game, MISSING_ASSETS);
	game->n_doors = count;
	idx = 0;
	y = -1; 
	while (++y < game->grid.height)
	{
		rowlen = ft_strlen(game->grid.map[y]);
		x = -1;
		while ((size_t)++x < rowlen)
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
VERSAO ANTERIOR, A TESTAR PARA VER SE ARRANJAMOS 
FORMA DE ABRIR O MAPA SEM PORTAS QUANDO NAO TIVER PORTA
E LOADAR QUANDO TIVER	*/

void	dumb_refactor_function(t_game *game, int x, int y, int *idx)
{
	game->doors[*idx].x = x;
	game->doors[*idx].y = y;
	game->doors[*idx].open = 0;
	*idx += 1;
}
