#include "../../includes_bonus/cub_bonus.h"

void	pickup_check(t_game *g)
{
	int px;
	int py;
	int i;

	px = (int)(g->player.x / BLOCK);
	py = (int)(g->player.y / BLOCK);
	if (in_bounds_tile(g, px, py) && g->grid.map[py][px] == 'C')
	{
		i = 0;
		while (i < g->n_collectables)
		{
			if (!g->collectables[i].is_collected
				&& g->collectables[i].x == px
				&& g->collectables[i].y == py)
			{
				g->collectables[i].is_collected = true;
				g->grid.map[py][px] = '0';
				break ;
			}
			i++;
		}
	}
}

void	check_collect_grip(t_game *game, bool *any_left)
{
	int ry;
	int rx;

	ry = 0;
	while (game->grid.map[ry] && !*any_left)
	{
		rx = 0;
		while (game->grid.map[ry][rx])
		{
			if (game->grid.map[ry][rx] == 'C')
			{
				*any_left = true;
				return ;
			}
			rx++;
		}
		ry++;
	}
}

void	update_collectables(t_game *game)
{
	bool	any_left;

	if (!game || !game->grid.map)
		return ;
	pickup_check(game);
	any_left = false;
	check_collect_grip(game, &any_left);
	if (!any_left)
		close_game(game, "You win!\n");
}

void	init_collectables(t_game *game)
{
	int			i;
	const int	frame_count = 6;
	const char	*paths[6] = {
		"textures/carrot/carrot_1.xpm", "textures/carrot/carrot_2.xpm",
		"textures/carrot/carrot_3.xpm", "textures/carrot/carrot_4.xpm",
		"textures/carrot/carrot_5.xpm", "textures/carrot/carrot_6.xpm"
	};

	game->collect_frame_count = frame_count;
	game->collect_frames = ft_calloc(frame_count, sizeof(t_tex));
	if (!game->collect_frames)
		close_game(game, MISSING_ASSETS);
	if (!load_textures(game, game->collect_frames, frame_count, paths))
		close_game(game, MISSING_ASSETS);
	if (game->collectables && game->n_collectables > 0)
	{
		i = 0;
		while (i < game->n_collectables)
		{
			game->collectables[i].frames = game->collect_frames;
			i++;
		}
	}
}

void	init_anim_state(t_game *g, t_render_anim *ra)
{
	static int	anim_tick;
	static int	anim_frame;

	ra->proj_plane = (WIDTH / 2.0) / tan(FOV / 2.0);
	ra->inv_det = (g->plane.planex * g->plane.diry - g->plane.dirx * g->plane.planey);
	{
		const int anim_speed = 8;
		if (g->collect_frame_count > 0 && g->collect_frames)
		{
			anim_tick++;
			if (anim_tick >= anim_speed)
			{
				anim_tick = 0;
				anim_frame = (anim_frame + 1) % g->collect_frame_count;
			}
		}
	}
	ra->anim_frame = anim_frame;
}

int	count_collect_sprites(t_game *g)
{
	int		count;
	int		y;
	size_t	rowlen;
	int		x;

	count = 0;
	if (!g->grid.map || g->grid.height <= 0)
		return (0);
	y = 0;
	while (y < g->grid.height)
	{
		rowlen = ft_strlen(g->grid.map[y]);
		x = 0;
		while ((size_t)x < rowlen)
		{
			if (g->grid.map[y][x] == 'C')
				count++;
			x++;
		}
		y++;
	}
	return (count);
}

void	push_sprite_if_visible(t_game *g, t_render_anim *ra, t_sprite *sp, int x, int y)
{
	double	sx;
	double	sy;
	double	relx;
	double	rely;

	sx = (x + 0.5) * BLOCK;
	sy = (y + 0.5) * BLOCK;
	relx = sx - g->player.x;
	rely = sy - g->player.y;
	ra->trans_x = ra->inv_det * (g->plane.diry * relx - g->plane.dirx * rely);
	ra->trans_y = ra->inv_det * (-g->plane.planey * relx + g->plane.planex * rely);
	ra->trans_x /= (double)BLOCK;
	ra->trans_y /= (double)BLOCK;
	if (ra->trans_y <= 1e-6)
		return ;
	ra->screen_x = (int)((WIDTH / 2.0) * (1.0 + ra->trans_x / ra->trans_y));
	ra->spr_h = (int)fabs(ra->proj_plane / ra->trans_y);
	if (ra->spr_h <= 0)
		ra->spr_h = 1;
	if (ra->spr_h > HEIGHT)
		ra->spr_h = HEIGHT;
	ra->spr_w = ra->spr_h;
	if (ra->spr_w > WIDTH)
		ra->spr_w = WIDTH;
	push_sprite_visible_inits(ra, sp, x, y);
}

void	push_sprite_visible_inits(t_render_anim *ra, t_sprite *sp, int x, int y)
{
	ra->start_y = -ra->spr_h / 2 + HEIGHT / 2;
	ra->end_y = ra->spr_h / 2 + HEIGHT / 2;
	if (ra->start_y < 0)
		ra->start_y = 0;
	if (ra->end_y >= HEIGHT)
		ra->end_y = HEIGHT - 1;
	ra->start_x = -ra->spr_w / 2 + ra->screen_x;
	ra->end_x = ra->spr_w / 2 + ra->screen_x;
	if (ra->start_x < 0)
		ra->start_x = 0;
	if (ra->end_x >= WIDTH)
		ra->end_x = WIDTH - 1;
	sp->mx = x;
	sp->my = y;
	sp->trans_x = ra->trans_x;
	sp->trans_y = ra->trans_y;
	sp->screen_x = ra->screen_x;
	sp->spr_w = ra->spr_w;
	sp->spr_h = ra->spr_h;
	sp->start_x = ra->start_x;
	sp->end_x = ra->end_x;
	sp->start_y = ra->start_y;
	sp->end_y = ra->end_y;
}

int	build_sprite_list(t_game *g, t_render_anim *ra, t_sprite *sprites)
{
	int		sidx;
	int		y;
	int		x;
	size_t	rowlen;

	sidx = 0;
	y = 0;
	while (y < g->grid.height)
	{
		rowlen = ft_strlen(g->grid.map[y]);
		x = 0;
		while ((size_t)x < rowlen)
		{
			if (g->grid.map[y][x] == 'C')
				push_sprite_if_visible(g, ra, &sprites[sidx++], x, y);
			x++;
		}
		y++;
	}
	return (sidx);
}

void	draw_sprite(t_game *g, t_render_anim *ra, t_sprite *sp)
{
	int		stripe;
	int		texX;
	int		ypix;
	int		d;
	t_tex		*t;

	if (g->collect_frames && g->collect_frame_count > 0)
		t = &g->collect_frames[ra->anim_frame];
	else
		t = &g->collect;
	if (!t || !t->data)
		return ;
	stripe = sp->start_x;
	while (stripe <= sp->end_x)
	{
		if (stripe >= 0 && stripe < WIDTH && sp->trans_y < g->zbuffer[stripe])
		{
			texX = (int)((double)(stripe - (-sp->spr_w / 2 + sp->screen_x)) * t->width / sp->spr_w);
			if (texX < 0)
				texX = 0;
			if (texX >= t->width)
				texX = t->width - 1;
			ypix = sp->start_y;
			while (ypix <= sp->end_y)
			{
				int texY;
				int col;

				d = ypix - (-sp->spr_h / 2 + HEIGHT / 2);
				texY = (int)((double)d * t->height / sp->spr_h);
				if (texY < 0)
					texY = 0;
				if (texY >= t->height)
					texY = t->height - 1;
				col = tex_read_color(t, texX, texY);
				if ((col & 0x00FFFFFF) != 0)
					put_pixel_safe(stripe, ypix, col, g);
				ypix++;
			}
		}
		stripe++;
	}
}

void	render_collectables(t_game *g)
{
	t_render_anim	ra;
	int			count;
	t_sprite		*sprites;
	int			idx;

	if (!g || !g->grid.map)
		return ;
	init_anim_state(g, &ra);
	if (fabs(ra.inv_det) < 1e-9)
		return ;
	ra.inv_det = 1.0 / ra.inv_det;
	count = count_collect_sprites(g);
	if (count <= 0)
		return ;
	sprites = ft_calloc(count, sizeof(t_sprite));
	if (!sprites)
		return ;
	idx = build_sprite_list(g, &ra, sprites);
	if (idx <= 0)
	{
		free(sprites);
		return ;
	}
	qsort(sprites, idx, sizeof(t_sprite), sprite_cmp);
	count = idx;
	idx = -1;
	while (++idx < count)
	{
		draw_sprite(g, &ra, &sprites[idx]);
		idx++;
	}
	free(sprites);
}
