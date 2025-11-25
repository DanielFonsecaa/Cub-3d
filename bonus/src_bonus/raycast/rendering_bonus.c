#include "../../includes_bonus/cub_bonus.h"

void	draw_line(t_player *player, t_game *game, t_plane *p, int i)
{
	t_tex	*texture;
	double	inv_det;
	double	relx;
	double	rely;
	double	trans_y_wall;

	init_r_val(game, player, p->raydx, p->raydy);
	dda_loop(game, &game->ray);
	texture = compute_per(game, &game->ray, &game->player);
	if (i >= 0 && i < WIDTH)
	{
		inv_det = 1.0 / (p->planex * p->diry - p->dirx * p->planey);
		relx = game->ray.hit_x - player->x;
		rely = game->ray.hit_y - player->y;
		trans_y_wall = inv_det * (-p->planey * relx + p->planex * rely);
		trans_y_wall /= (double)BLOCK;
		if (trans_y_wall < 1e-6)
			trans_y_wall = 1e-6;
		game->zbuffer[i] = trans_y_wall;
	}
	if (!texture || !texture->data)
		return ;
	compute_tex(game, &game->ray, i, texture);
}

void	compute_tex(t_game *g, t_ray *r, int i, t_tex *t)
{
	r->proj_plane = (WIDTH / 2.0) / tan(FOV / 2.0);
	if (r->side == 0)
		r->wall_hit = r->hit_y;
	else
		r->wall_hit = r->hit_x;
	r->wall_hit = fmod(r->wall_hit, BLOCK);
	if (r->wall_hit < 0)
		r->wall_hit += BLOCK;
	r->tex_x = (int)((r->wall_hit / (double)BLOCK) * t->width);
	if (r->tex_x < 0)
		r->tex_x = 0;
	if (r->tex_x >= t->width)
		r->tex_x = t->width - 1;
	set_configure(g, r, i, t);
}

t_tex	*compute_per(t_game *g, t_ray *r, t_player *p)
{
	t_tex	*texture;
	double	boundary;

	if (r->side == 0)
		texture = side_gt_zero(g, r, p);
	else
	{
		if (r->step_y > 0)
			texture = &g->south;
		else
			texture = &g->north;
		if (r->step_y < 0)
			boundary = (r->map_y + 1) * BLOCK;
		else
			boundary = r->map_y * BLOCK;
		if (r->sin_angle == 0)
			r->perp_dist = (boundary - p->y) / (1e-9);
		else
			r->perp_dist = (boundary - p->y) / (r->sin_angle);
	}
	if (r->cell == 'D' && g->door.img)
		texture = &g->door;
	if (r->perp_dist < 1e-6)
		r->perp_dist = 1e-6;
	r->hit_x = p->x + r->perp_dist * r->cos_angle;
	r->hit_y = p->y + r->perp_dist * r->sin_angle;
	return (texture);
}

void	dda_loop(t_game *game, t_ray *ray)
{
	while (game->grid.map[ray->rows])
		ray->rows++;
	while (1)
	{
		if (++ray->iter > 10000)
			break ;
		if (ray->side_dist_x < ray->side_dist_y)
		{
			ray->side_dist_x += ray->delta_dist_x;
			ray->map_x += ray->step_x;
			ray->side = 0;
		}
		else
		{
			ray->side_dist_y += ray->delta_dist_y;
			ray->map_y += ray->step_y;
			ray->side = 1;
		}
		if (ray->map_y < 0 || ray->map_y >= ray->rows)
		{
			ray->cell = '1';
			break ;
		}
		ray->row_len = ft_strlen(game->grid.map[ray->map_y]);
		if ((ray->map_x < 0 || (size_t)ray->map_x >= ray->row_len))
		{
			ray->cell = '1';
			break ;
		}
		ray->cell = game->grid.map[ray->map_y][ray->map_x];
		if (ray->cell == '1' || ray->cell == 'D')
			break ;
	}
}

void	init_r_val(t_game *game, t_player *player, double raydx, double raydy)
{
	(game->ray) = (t_ray){.cos_angle = raydx, .sin_angle = raydy,
		.iter = 0, .rows = 0, .eps = 1e-9, .step_x = 1, .step_y = 1};
	game->ray.delta_dist_x = fabs((double)BLOCK / game->ray.cos_angle);
	if (fabs(game->ray.cos_angle) < game->ray.eps)
		game->ray.delta_dist_x = 1e30;
	game->ray.delta_dist_y = fabs((double)BLOCK / game->ray.sin_angle);
	if (fabs(game->ray.sin_angle) < game->ray.eps)
		game->ray.delta_dist_y = 1e30;
	game->ray.map_x = (int)(player->x / BLOCK);
	game->ray.map_y = (int)(player->y / BLOCK);
	if (game->ray.cos_angle < 0)
		game->ray.step_x = -1;
	if (game->ray.sin_angle < 0)
		game->ray.step_y = -1;
	game->ray.side_dist_x = ((game->ray.map_x + 1) * BLOCK - player->x)
		* game->ray.delta_dist_x / (double)BLOCK;
	if (game->ray.step_x < 0)
		game->ray.side_dist_x = (player->x - game->ray.map_x * BLOCK)
			* game->ray.delta_dist_x / (double)BLOCK;
	game->ray.side_dist_y = ((game->ray.map_y + 1) * BLOCK - player->y)
		* game->ray.delta_dist_y / (double)BLOCK;
	if (game->ray.step_y < 0)
		game->ray.side_dist_y = (player->y - game->ray.map_y * BLOCK)
			* game->ray.delta_dist_y / (double)BLOCK;
	game->ray.side = -1;
}
