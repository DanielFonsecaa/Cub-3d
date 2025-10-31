#include "../../includes/cub.h"

void	draw_line(t_player *player, t_game *game, double start_x, int i)
{
	t_texture	*texture;

	init_starting_values(game, player, start_x);
	dda_loop(game, &game->ray);
	texture = compute_per(game, &game->ray, &game->player, start_x);
	compute_tex_with_uncorrect_dist(game, &game->ray, i, texture);
}

void	compute_tex_with_uncorrect_dist(t_game *g, t_ray *r,
		int i, t_texture *t)
{
	r->proj_plane = (WIDTH / 2.0) / tan(FOV / 2.0);
	r->wall_hit = r->hit_x;
	if (r->side == 0)
		r->wall_hit = r->hit_y;
	r->wall_hit = fmod(r->wall_hit, BLOCK);
	if (r->wall_hit < 0)
		r->wall_hit += BLOCK;
	r->tex_x = (int)((r->wall_hit / (double)BLOCK) * t->width);
	if (r->tex_x < 0)
		r->tex_x = 0;
	if (r->tex_x >= t->width)
		r->tex_x = t->width - 1;
	if (!DEBUG)
		set_configure(g, r, i, t);
}

t_texture	*compute_per(t_game *g, t_ray *r, t_player *p, double start_x)
{
	t_texture	*texture;

	if (r->side == 0)
	{
		texture = &g->west;
		if (r->step_x > 0)
			texture = &g->east;
		r->perp_dist = r->side_dist_x - r->delta_dist_x;
		r->uncorredted_dist = r->side_dist_x - r->delta_dist_x;
	}
	else
	{
		texture = &g->north;
		if (r->step_y > 0)
			texture = &g->south;
		r->perp_dist = r->side_dist_y - r->delta_dist_y;
		r->uncorredted_dist = r->side_dist_y - r->delta_dist_y;
	}
	if (r->perp_dist < 1e-6)
		r->perp_dist = 1e-6;
	r->hit_x = p->x + r->uncorredted_dist * r->cos_angle;
	r->hit_y = p->y + r->uncorredted_dist * r->sin_angle;
	r->angle_diff = start_x - p->angle;
	r->perp_dist = r->perp_dist * cos(start_x);
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
			break ;
		ray->row_len = ft_strlen(game->grid.map[ray->map_y]);
		if ((ray->map_x < 0 || (size_t)ray->map_x >= ray->row_len)
			|| game->grid.map[ray->map_y][ray->map_x] == '1')
			break ;
	}
}

void	init_starting_values(t_game *game, t_player *player, double start_x)
{
	(game->ray) = (t_ray){.cos_angle = cos(start_x), .sin_angle = sin(start_x),
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
