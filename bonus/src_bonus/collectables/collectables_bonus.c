#include "../../includes_bonus/cub_bonus.h"

int	count_collect_sprites(t_game *g)
{
	int		count;
	int		y;
	int		x;
	size_t	rowlen;

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

void	push_sprite_if_visible(t_game *g, t_r_anim *ra, t_sprite *sp)
{
	double	sx;
	double	sy;
	double	relx;
	double	rely;

	sx = (ra->x + 0.5) * BLOCK;
	sy = (ra->y + 0.5) * BLOCK;
	relx = sx - g->player.x;
	rely = sy - g->player.y;
	ra->trans_x = ra->inv_det * (g->plane.diry * relx - g->plane.dirx * rely);
	ra->trans_y = ra->inv_det
		* (-g->plane.planey * relx + g->plane.planex * rely);
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
	push_sprite_visible_inits(ra, sp, ra->x, ra->y);
}

void	push_sprite_visible_inits(t_r_anim *ra, t_sprite *sp, int x, int y)
{
	if (ra->spr_w > WIDTH)
		ra->spr_w = WIDTH;
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

int	build_sprite_list(t_game *g, t_r_anim *ra, t_sprite *sprites)
{
	int		sidx;
	size_t	rowlen;

	sidx = 0;
	ra->y = 0;
	while (ra->y < g->grid.height)
	{
		rowlen = ft_strlen(g->grid.map[ra->y]);
		ra->x = 0;
		while ((size_t)ra->x < rowlen)
		{
			if (g->grid.map[ra->y][ra->x] == 'C')
				push_sprite_if_visible(g, ra, &sprites[sidx++]);
			ra->x++;
		}
		ra->y++;
	}
	return (sidx);
}
