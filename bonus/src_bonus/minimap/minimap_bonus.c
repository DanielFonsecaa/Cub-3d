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

void	minimap_draw_condition1(t_game *g, int px, int py, t_mini *m)
{
	m->screen_x = MM_ORG_X + (m->mx - (px - MM_RAD)) * MM_TILE;
	m->screen_y = MM_ORG_Y + (m->my - (py - MM_RAD)) * MM_TILE;
	draw_square(m->screen_x, m->screen_y, MM_TILE, g);
}

void	minimap_draw_condition2(t_game *g, int px, int py, t_mini *m)
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

void	minimap_draw_condition3(t_game *g, int px, int py, t_mini *m)
{
	m->screen_x = MM_ORG_X + (m->mx - (px - MM_RAD)) * MM_TILE;
	m->screen_y = MM_ORG_Y + (m->my - (py - MM_RAD)) * MM_TILE;
	m->y = 0;
	while (m->y < MM_TILE)
	{
		m->x = 0;
		while (m->x < MM_TILE)
		{
			if (m->y == 0 || m->y == MM_TILE - 1 || m->x == 0
				|| m->x == MM_TILE - 1)
			{
				if (is_inside_square(m->screen_x + m->x, m->screen_y + m->y))
					put_pixel_minimap(m->screen_x + m->x, m->screen_y + m->y,
						GREEN, g);
			}
			m->x++;
		}
		m->y++;
	}
}

void	draw_map_window(t_game *game, int px, int py)
{
	t_mini	m;

	m.my = py - MM_RAD - 1;
	while (++m.my <= py + MM_RAD)
	{
		m.mx = px - MM_RAD - 1;
		while (++m.mx <= px + MM_RAD)
		{
			if (!in_bounds(game, m.mx, m.my))
				continue ;
			if (game->grid.map[m.my][m.mx] == '1')
				minimap_draw_condition1(game, px, py, &m);
			else if (game->grid.map[m.my][m.mx] == 'C')
				minimap_draw_condition2(game, px, py, &m);
			else if (game->grid.map[m.my][m.mx] == 'D')
				minimap_draw_condition3(game, px, py, &m);
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
