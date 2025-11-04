#include "../../includes/cub.h"

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
