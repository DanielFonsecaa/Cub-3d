#include "../../includes_bonus/cub_bonus.h"

void	draw_player(t_game *game, int cx, int cy, int color)
{
	int	dy;
	int	dx;

	dy = -PLAYER_R -1;
	while (++dy <= PLAYER_R)
	{
		dx = -PLAYER_R;
		while (++dx <= PLAYER_R)
		{
			if (dx * dx + dy * dy <= PLAYER_R * PLAYER_R)
				if (is_inside_square(cx + dx, cy + dy))
					put_pixel_minimap(cx + dx, cy + dy, color, game);
		}
	}
}

void	draw_minimap(t_game *game)
{
	int	size_x;
	int	size_y;
	int	px;
	int	py;

	size_x = (int)(game->player.x / BLOCK);
	size_y = (int)(game->player.y / BLOCK);
	mm_circle(game);
	draw_map_window(game, size_x, size_y);
	px = MM_ORG_X + (MM_RAD * MM_TILE) + (MM_TILE - 4) / 2;
	py = MM_ORG_Y + (MM_RAD * MM_TILE) + (MM_TILE - 4) / 2;
	draw_player(game, px, py, GREEN);
}

static inline int	in_bounds(t_game *g, int mx, int my)
{
	size_t	len;

	if (my < 0 || my >= g->grid.height)
		return (0);
	len = ft_strlen(g->grid.map[my]);
	if (mx < 0 || (size_t)mx >= len)
		return (0);
	return (1);
}

void	draw_map_window(t_game *game, int px, int py)
{
	int	mx;
	int	my;
	int	cx;
	int	cy;
	int	dx;
	int	dy;
	int	screen_x;
	int	screen_y;
	int	x;
	int	y;

	my = py - MM_RAD -1;
	while (++my <= py + MM_RAD)
	{
		mx = px - MM_RAD -1;
		while (++mx <= px + MM_RAD)
		{
			if (!in_bounds(game, mx, my))
				continue ;
			if (game->grid.map[my][mx] == '1')
			{
				screen_x = MM_ORG_X + (mx - (px - MM_RAD)) * MM_TILE;
				screen_y = MM_ORG_Y + (my - (py - MM_RAD)) * MM_TILE;
				draw_square(screen_x, screen_y, MM_TILE, game);
			}
			else if (game->grid.map[my][mx] == 'C')
			{
				cx = MM_ORG_X + (mx - (px - MM_RAD)) * MM_TILE + MM_TILE / 2;
				cy = MM_ORG_Y + (my - (py - MM_RAD)) * MM_TILE + MM_TILE / 2;
				dx = -2;
				while (dx <= 2)
				{
					dy = -2;
					while (dy <= 2)
					{
						if (is_inside_square(cx + dx, cy + dy))
							put_pixel_minimap(cx + dx, cy + dy, RED, game);
						dy++;
					}
					dx++;
				}
			}
			else if (game->grid.map[my][mx] == 'D')
			{
				screen_x = MM_ORG_X + (mx - (px - MM_RAD)) * MM_TILE;
				screen_y = MM_ORG_Y + (my - (py - MM_RAD)) * MM_TILE;
				y = 0;
				while (y < MM_TILE)
				{
					x = 0;
					while (x < MM_TILE)
					{
						if (y == 0 || y == MM_TILE - 1 || x == 0 || x == MM_TILE - 1)
						{
							if (is_inside_square(screen_x + x, screen_y + y))
								put_pixel_minimap(screen_x + x, screen_y + y, GREEN, game);
						}
						x++;
					}
					y++;
				}
			}
		}
	}
}

void	draw_map(t_game *game)
{
	char	**map;
	int		x;
	int		y;

	y = -1;
	map = game->grid.map;
	while (map[++y])
	{
		x = -1;
		while (map[y][++x])
		{
			if (map[y][x] == '1')
				draw_square(x * BLOCK, y * BLOCK, BLOCK, game);
		}
	}
}
