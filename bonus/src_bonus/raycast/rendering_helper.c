#include "../../includes_bonus/cub.h"

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
