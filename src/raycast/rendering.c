#include "../../includes/cub.h"

void	draw_line(t_player *player, t_game *game, double start_x, int i)
{
	init_starting_values(game, player, start_x);
	dda_loop(game, game->ray);
	compute_perpendicular(game, game->ray, game->player);
}

void	compute_perpendicular(t_game *game, t_ray *ray, t_player *player)
{
	ray->perp_dist = ray->side_dist_y - ray->delta_dist_y;
	if (ray->side == 0)
	
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

void	init_starting_values(t_player *player, t_game *game, double start_x)
{
	(game->ray){.cos_angle = cos(start_x), .sin_angle = sin(start_x)};
	(game->ray){.iter = 0, .rows = 0, .EPS = 1e-9, .step_x = 1, .step_y = 1};
	game->ray.delta_dist_x = fabs((double)BLOCK / game->ray.cos_angle);
	if (fabs(game->ray.cos_angle) < game->ray.EPS)
		game->ray.delta_dist_x = 1e30;
	game->ray.delta_dist_y = fabs((double)BLOCK / game->ray.sin_angle);
	if (fabs(game->ray.sin_angle) < game->ray.EPS)
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
		game->ray.side_dist_x = (player.x - game->ray.map_x * BLOCK)
			* game->ray.delta_dist_x / (double)BLOCK;
	game->ray.side_dist_y = ((game->ray.map_y + 1) * BLOCK - player->y)
		* game->ray.delta_dist_y / (double)BLOCK;
	if (game->ray.step_y < 0)
		game->ray.side_dist_y = (player->y - game->ray.map_y * BLOCK)
			* game->ray.delta_dist_y / (double)BLOCK;
	game->ray.side = -1;
}
