#include "../../includes_bonus/cub_bonus.h"

void	mm_circle(t_game *game)
{
	int	cx;
	int	cy;
	int	r;

	cx = MM_ORG_X + MM_W / 2;
	cy = MM_ORG_Y + MM_H / 2;
	r = (MM_H / 2) - 1;
	if (MM_W < MM_H)
		r = (MM_W / 2) - 1;
	fill_mm_background(game, cx, cy, r);
}

void	fill_mm_background(t_game *game, int cx, int cy, int r)
{
	int	i;
	int	w;
	int	start_x;
	int	end_x;

	i = -r -1;
	while (++i <= r)
	{
		w = (int)sqrt((double)(r * r - i * i));
		start_x = cx - w;
		end_x = cx + w;
		while (start_x <= end_x)
		{
			put_pixel_minimap(start_x, cy + i, 0x000000, game);
			start_x++;
		}
	}
}

int	is_inside_square(int x, int y)
{
	int	cx;
	int	cy;
	int	r;

	cx = x - (MM_ORG_X + MM_W / 2);
	cy = y - (MM_ORG_Y + MM_H / 2);
	r = (MM_H / 2) - 1;
	if (MM_W < MM_H)
		r = (MM_W / 2) - 1;
	return (cx * cx + cy * cy <= r * r);
}

void	put_pixel_minimap(int x, int y, int color, t_game *game)
{
	int	index;

	if (!game || !game->data)
		return ;
	if (y < 0 || x >= WIDTH || y >= HEIGHT || x < 0)
		return ;
	index = y * game->size_line + x * (game->bpp / 8);
	game->data[index] = color & 0xFF;
	game->data[index + 1] = (color >> 8) & 0xFF;
	game->data[index + 2] = (color >> 16) & 0xFF;
}

void	draw_square(int x, int y, int size, t_game *game)
{
	int	column;
	int	row;

	column = 0;
	while (column < size)
	{
		row = 0;
		while (row < size)
		{
			if (is_inside_square(x + row, y + column))
				put_pixel_minimap(x + row, y + column, BLUE, game);
			row++;
		}
		column++;
	}
}
