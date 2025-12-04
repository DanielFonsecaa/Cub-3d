#include "../../includes_bonus/cub_bonus.h"

void	set_configure(t_game *g, t_ray *r, int i, t_tex *t)
{
	int	draw_pixels;

	if (r->proj_plane == 0.0)
		r->proj_plane = (WIDTH / 2.0) / tan(FOV / 2.0);
	r->orig_height = (BLOCK / r->perp_dist) * r->proj_plane;
	if (r->orig_height < 1.0)
		r->orig_height = 1.0;
	r->orig_start_f = (HEIGHT - r->orig_height) / 2.0;
	r->orig_end_f = r->orig_start_f + r->orig_height;
	r->draw_start = (int)r->orig_start_f;
	r->draw_end = (int)r->orig_end_f;
	if (r->draw_start < 0)
		r->draw_start = 0;
	if (r->draw_end > HEIGHT)
		r->draw_end = HEIGHT;
	draw_pixels = r->draw_end - r->draw_start;
	if (draw_pixels <= 0)
		return ;
	r->tex_step = (double)t->height / r->orig_height;
	r->tex_pos = (r->draw_start - r->orig_start_f) * r->tex_step;
	r->tex_x_flipped = r->tex_x;
	flip_and_draw(g, r, i, t);
}

void	flip_and_draw(t_game *g, t_ray *r, int i, t_tex *t)
{
	if (r->side == 0)
	{
		if (r->step_x < 0)
			r->tex_x_flipped = t->width - r->tex_x - 1;
	}
	else
	{
		if (r->step_y > 0)
			r->tex_x_flipped = t->width - r->tex_x - 1;
	}
	draw_lines(g, r, i, t);
}

void	draw_lines(t_game *g, t_ray *r, int i, t_tex *t)
{
	int				y;
	int				color;
	unsigned char	*d;

	y = r->draw_start;
	while (y < r->draw_end)
	{
		r->tex_y = (int)(r->tex_pos);
		if (r->tex_y < 0)
			r->tex_y = 0;
		if (r->tex_y >= t->height)
			r->tex_y = t->height - 1;
		r->t_idx = r->tex_y * t->size_line + r->tex_x_flipped * (t->bpp / 8);
		color = 0xFFFFFF;
		if (t->bpp == 32)
			color = *(int *)(t->data + r->t_idx);
		else if (t->bpp == 24)
		{
			d = (unsigned char *)(t->data + r->t_idx);
			color = d[0] | (d[1] << 8) | (d[2] << 16);
		}
		put_pixel(i, y, color, g);
		r->tex_pos += r->tex_step;
		y++;
	}
}

void	put_pixel(int x, int y, int color, t_game *game)
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

void	put_pixel_safe(int x, int y, int color, t_game *game)
{
	int				index;
	int				bytes_per_pixel;
	unsigned char	*buf;

	if (!game || !game->data)
		return ;
	if (x < 0 || x >= WIDTH || y < 0 || y >= HEIGHT)
		return ;
	bytes_per_pixel = game->bpp / 8;
	buf = (unsigned char *)game->data;
	index = y * game->size_line + x * bytes_per_pixel;
	buf[index] = color & 0xFF;
	if (bytes_per_pixel > 1)
		buf[index + 1] = (color >> 8) & 0xFF;
	if (bytes_per_pixel > 2)
		buf[index + 2] = (color >> 16) & 0xFF;
	if (bytes_per_pixel > 3)
		buf[index + 3] = (color >> 24) & 0xFF;
}
