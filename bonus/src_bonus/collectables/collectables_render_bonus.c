#include "../../includes_bonus/cub_bonus.h"

void	render_collectables(t_game *g)
{
	int			count;
	int			idx;
	t_r_anim	ra;
	t_sprite	*sprites;

	count = 0;
	if (!verify_render_collect(g, &ra, &count))
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
		draw_sprite(g, &ra, &sprites[idx]);
	free(sprites);
}

int	verify_render_collect(t_game *g, t_r_anim *ra, int *count)
{
	if (!g || !g->grid.map)
		return (0);
	init_anim_state(g, ra);
	if (fabs(ra->inv_det) < 1e-9)
		return (0);
	ra->inv_det = 1.0 / ra->inv_det;
	*count = count_collect_sprites(g);
	if (*count <= 0)
		return (0);
	return (1);
}

void	draw_sprite(t_game *g, t_r_anim *ra, t_sprite *sp)
{
	t_tex	*t;

	t = &g->collect;
	if (g->collect_frames && g->collect_frame_count > 0)
		t = &g->collect_frames[ra->anim_frame];
	if (!t || !t->data)
		return ;
	ra->stripe = sp->start_x;
	while (ra->stripe <= sp->end_x)
	{
		if (ra->stripe >= 0 && ra->stripe < WIDTH
			&& sp->trans_y < g->zbuffer[ra->stripe])
		{
			ra->tex_x = (int)((double)(ra->stripe - (-sp->spr_w / 2
							+ sp->screen_x)) * t->width / sp->spr_w);
			if (ra->tex_x < 0)
				ra->tex_x = 0;
			if (ra->tex_x >= t->width)
				ra->tex_x = t->width - 1;
			ra->ypix = sp->start_y;
			while (ra->ypix <= sp->end_y)
				helper_draw_sprite(g, ra, sp, t);
		}
		ra->stripe++;
	}
}

void	helper_draw_sprite(t_game *g, t_r_anim *ra, t_sprite *sp, t_tex *t)
{
	ra->d = ra->ypix - (-sp->spr_h / 2 + HEIGHT / 2);
	ra->tex_y = (int)((double)ra->d * t->height / sp->spr_h);
	if (ra->tex_y < 0)
		ra->tex_y = 0;
	if (ra->tex_y >= t->height)
		ra->tex_y = t->height - 1;
	ra->col = tex_read_color(t, ra->tex_x, ra->tex_y);
	if ((ra->col & 0x00FFFFFF) != 0)
		put_pixel_safe(ra->stripe, ra->ypix, ra->col, g);
	ra->ypix++;
}
