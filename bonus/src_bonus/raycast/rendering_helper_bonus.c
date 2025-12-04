#include "../../includes_bonus/cub_bonus.h"

t_tex	*side_gt_zero(t_game *g, t_ray *r, t_player *p)
{
	t_tex	*texture;
	double	boundary;

	if (r->step_x > 0)
		texture = &g->east;
	else
		texture = &g->west;
	if (r->step_x < 0)
		boundary = (r->map_x + 1) * BLOCK;
	else
		boundary = r->map_x * BLOCK;
	if (r->cos_angle == 0)
		r->perp_dist = (boundary - p->x) / (1e-9);
	else
		r->perp_dist = (boundary - p->x) / (r->cos_angle);
	return (texture);
}

int	validate_rgb(t_game *game, char *line, int *i)
{
	char	*str;
	int		j;

	j = 0;
	if (line[*i] == ',')
	{
		free(line);
		close_game(game, INVALID_TEXTURE);
	}
	while (ft_isdigit(line[*i]))
	{
		j++;
		*i += 1;
	}
	str = ft_substr(line, *i - j, j);
	if (ft_strlen(str) > 3)
	{
		free(line);
		free(str);
		close_game(game, INVALID_TEXTURE);
	}
	return (rgb_helper(game, str, line, i));
}

int	rgb_helper(t_game *game, char *str, char *line, int *i)
{
	int	value;

	value = ft_atoi(str);
	free(str);
	if (value > 255 || value < 0)
	{
		free(line);
		close_game(game, INVALID_TEXTURE);
	}
	while (line[*i] && line[*i] != ',')
	{
		if (ft_isascii(line[*i]) && !ft_iswhite_space(line[*i]))
		{
			free(line);
			close_game(game, INVALID_TEXTURE);
		}
		*i += 1;
	}
	if (line[*i] == ',')
		*i += 1;
	return (value);
}

void	perp_ray_setup(t_ray *r, t_player *p)
{
	if (r->perp_dist < 1e-6)
		r->perp_dist = 1e-6;
	r->hit_x = p->x + r->perp_dist * r->cos_angle;
	r->hit_y = p->y + r->perp_dist * r->sin_angle;
}

int	break_dda_loop(t_game *game, t_ray *ray)
{
	if (ray->map_y < 0 || ray->map_y >= ray->rows)
	{
		ray->cell = '1';
		return (0);
	}
	ray->row_len = ft_strlen(game->grid.map[ray->map_y]);
	if ((ray->map_x < 0 || (size_t)ray->map_x >= ray->row_len))
	{
		ray->cell = '1';
		return (0);
	}
	ray->cell = game->grid.map[ray->map_y][ray->map_x];
	if (ray->cell == '1' || ray->cell == 'D')
		return (0);
	return (1);
}
