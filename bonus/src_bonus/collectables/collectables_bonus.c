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

//Deixa esse fixeiro pra fazermos juntos se quiser, faz os mais faceis
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

void	render_collectables(t_game *g)
{
	int			x;
	int			y;
	int			sidx;
	int			count;
	double		inv_det;
	double		proj_plane;
	t_tex		*t;
	t_sprite	*sprites;
	size_t		rowlen;
	static int	anim_tick = 0;
	static int	anim_frame = 0;
	const int	anim_speed = 8;

	sprites = NULL;
	if (!g || !g->grid.map)
		return ;
	if (g->collect_frame_count > 0 && g->collect_frames)
	{
		if (++anim_tick >= anim_speed)
		{
			anim_tick = 0;
			anim_frame = (anim_frame + 1) % g->collect_frame_count;
		}
	}
	proj_plane = (WIDTH / 2.0) / tan(FOV / 2.0);
	inv_det = (g->plane.planex * g->plane.diry - g->plane.dirx * g->plane.planey);
	if (fabs(inv_det) < 1e-9)
		return ;
	inv_det = 1.0 / inv_det;
	count = 0;
	if (!g->grid.map || g->grid.height <= 0)
	{
		free(sprites);
		return ;
	}
	for (y = 0; y < g->grid.height; y++)
	{
		rowlen = ft_strlen(g->grid.map[y]);
		for (x = 0; (size_t)x < rowlen; x++)
			if (g->grid.map[y][x] == 'C')
				count++;
	}
	if (count == 0)
	{
		free(sprites);
		return ;
	}
	sprites = ft_calloc(count, sizeof(t_sprite));
	if (!sprites)
		return ;
	sidx = 0;
	for (y = 0; y < g->grid.height; y++)
	{
		rowlen = ft_strlen(g->grid.map[y]);
		for (x = 0; (size_t)x < rowlen; x++)
		{
			if (g->grid.map[y][x] == 'C')
			{
				double sx = (x + 0.5) * BLOCK;
				double sy = (y + 0.5) * BLOCK;
				double relx = sx - g->player.x;
				double rely = sy - g->player.y;
				double trans_x = inv_det * ( g->plane.diry * relx - g->plane.dirx * rely);
				double trans_y = inv_det * (-g->plane.planey * relx + g->plane.planex * rely);
				trans_x /= (double)BLOCK;
				trans_y /= (double)BLOCK;
				if (trans_y > 1e-6)
				{
					int screen_x = (int)((WIDTH / 2.0) * (1.0 + trans_x / trans_y));
					int spr_h = (int)fabs(proj_plane / trans_y);
					int spr_w = spr_h;
					if (spr_h <= 0) spr_h = 1;
					if (spr_w <= 0) spr_w = 1;
					if (spr_h > HEIGHT) spr_h = HEIGHT;
					if (spr_w > WIDTH)  spr_w = WIDTH;
					int start_y = -spr_h / 2 + HEIGHT / 2;
					int end_y   =  spr_h / 2 + HEIGHT / 2;
					if (start_y < 0) start_y = 0;
					if (end_y >= HEIGHT) end_y = HEIGHT - 1;
					int start_x = -spr_w / 2 + screen_x;
					int end_x   =  spr_w / 2 + screen_x;
					if (start_x < 0) start_x = 0;
					if (end_x >= WIDTH) end_x = WIDTH - 1;
					sprites[sidx].mx = x;
					sprites[sidx].my = y;
					sprites[sidx].trans_x = trans_x;
					sprites[sidx].trans_y = trans_y;
					sprites[sidx].screen_x = screen_x;
					sprites[sidx].spr_w = spr_w;
					sprites[sidx].spr_h = spr_h;
					sprites[sidx].start_x = start_x;
					sprites[sidx].end_x = end_x;
					sprites[sidx].start_y = start_y;
					sprites[sidx].end_y = end_y;
					sidx++;
				}
			}
		}
	}
	count = sidx;
	if (count == 0)
	{
		free(sprites);
		return;
	}
	qsort(sprites, count, sizeof(t_sprite), sprite_cmp);
	for (sidx = 0; sidx < count; sidx++)
	{
		t_sprite *sp = &sprites[sidx];
		if (g->collect_frames && g->collect_frame_count > 0)
			t = &g->collect_frames[anim_frame];
		else
			t = &g->collect;

		if (!t || !t->data)
			continue ;
		for (int stripe = sp->start_x; stripe <= sp->end_x; stripe++)
		{
			if (stripe < 0 || stripe >= WIDTH)
				continue ;
			if (sp->trans_y < g->zbuffer[stripe])
			{
				int texX = (int)((double)(stripe - (-sp->spr_w / 2 + sp->screen_x)) * t->width / sp->spr_w);
				if (texX < 0) texX = 0;
				if (texX >= t->width) texX = t->width - 1;
				for (int ypix = sp->start_y; ypix <= sp->end_y; ypix++)
				{
					int d = ypix - (-sp->spr_h / 2 + HEIGHT / 2);
					int texY = (int)((double)d * t->height / sp->spr_h);
					if (texY < 0) texY = 0;
					if (texY >= t->height) texY = t->height - 1;
					int col = tex_read_color(t, texX, texY);
					if ((col & 0x00FFFFFF) != 0)
						put_pixel_safe(stripe, ypix, col, g);
				}
			}
		}
	}
	free(sprites);
}
