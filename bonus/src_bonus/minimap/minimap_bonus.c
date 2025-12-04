#include "../../includes_bonus/cub_bonus.h"

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

void	minimap_draw_wall(t_game *g, int px, int py, t_mini *m)
{
	m->screen_x = MM_ORG_X + (m->mx - (px - MM_RAD)) * MM_TILE;
	m->screen_y = MM_ORG_Y + (m->my - (py - MM_RAD)) * MM_TILE;
	draw_square(m->screen_x, m->screen_y, MM_TILE, g);
}

void	minimap_draw_collectable(t_game *g, int px, int py, t_mini *m)
{
	m->cx = MM_ORG_X + (m->mx - (px - MM_RAD)) * MM_TILE + MM_TILE / 2;
	m->cy = MM_ORG_Y + (m->my - (py - MM_RAD)) * MM_TILE + MM_TILE / 2;
	m->dx = -2;
	while (m->dx <= 2)
	{
		m->dy = -2;
		while (m->dy <= 2)
		{
			if (is_inside_square(m->cx + m->dx, m->cy + m->dy))
				put_pixel_minimap(m->cx + m->dx, m->cy + m->dy, RED, g);
			m->dy++;
		}
		m->dx++;
	}
}
