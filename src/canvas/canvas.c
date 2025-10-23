#include "../../includes/cub.h"

void	init_canvas(t_canvas *canvas)
{
	canvas->mlx = mlx_init();
	canvas->win = mlx_new_window(canvas->mlx, WIDTH, HEIGHT, "canvas");
	canvas->img = mlx_new_image(canvas->mlx, WIDTH, HEIGHT);
}

void	draw_square(int x, int y, int size, int color, t_game *game)
{
	int	i;

	i = -1;
	while (++i < size)
		put_pixel(x + i, y, color, game);
	i = -1;
	while (++i < size)
		put_pixel(x + i, y + size - 1, color, game);
	i = -1;
	while (++i < size)
		put_pixel(x, y + i, color, game);
	i = -1;
	while (++i < size)
		put_pixel(x + size - 1, y + i, color, game);
}

void	draw_map(t_game *game)
{
	char	**map;
	int	x;
	int	y;

	y = -1;
	map = game->map;
	while (map[++y])
	{
		x = -1;
		while (map[y][++x])
		{
			if (map[y][x] == '1')
				draw_square(x * BLOCK, y * BLOCK, BLOCK, BLUE, game);
		}
	}
}

// distance calculation functions
double	distance(double x, double y){
	return sqrt(x * x + y * y);
}

double	fixed_dist(double x1, double y1, double x2, double y2, t_game *game)
{
	double delta_x = x2 - x1;
	double delta_y = y2 - y1;
	double angle = atan2(delta_y, delta_x) - game->player->angle;
	double fix_dist = distance(delta_x, delta_y) * cos(angle);
	return (fix_dist);
}

// touch function 
bool	touch(double px, double py, t_game *game)
{
	int x;
	int y;
	int rows;
	size_t row_len;

	if (!game || !game->map)
		return (true); /* treat missing map as wall */

	x = (int)(px / BLOCK);
	y = (int)(py / BLOCK);

	/* count map rows */
	rows = 0;
	while (game->map[rows])
		rows++;

	/* out of vertical bounds -> treat as wall */
	if (y < 0 || y >= rows)
		return (true);

	/* get length of this row and check horizontal bounds */
	row_len = ft_strlen(game->map[y]);
	if (x < 0 || (size_t)x >= row_len)
		return (true);

	return (game->map[y][x] == '1');
}

static double	compute_perp_dist(double hit_x, double hit_y, t_player *player)
{
	double dx = hit_x - player->x;
	double dy = hit_y - player->y;
	double forward_x = cos(player->angle);
	double forward_y = sin(player->angle);

	double perp = dx * forward_x + dy * forward_y;
	if (perp < 0) perp = -perp;
	if (perp < 1e-6) perp = 1e-6;
	return (perp);
}

// raycasting functions
void	draw_line(t_player *player, t_game *game, double start_x, int i)
{
	double cos_angle = cos(start_x);
	double sin_angle = sin(start_x);

	/* guard against exact zeros */
	const double EPS = 1e-9;
	/* delta_dist = distance along the ray to cross one grid cell (in pixels) */
	double delta_dist_x = (fabs(cos_angle) < EPS) ? 1e30 : fabs((double)BLOCK / cos_angle);
	double delta_dist_y = (fabs(sin_angle) < EPS) ? 1e30 : fabs((double)BLOCK / sin_angle);

	int map_x = (int)(player->x / BLOCK);
	int map_y = (int)(player->y / BLOCK);

	int step_x = (cos_angle < 0) ? -1 : 1;
	int step_y = (sin_angle < 0) ? -1 : 1;

	double side_dist_x;
	double side_dist_y;

	/* distance from player to first vertical/horizontal gridline along the ray (same units as delta_dist) */
	if (step_x < 0)
		side_dist_x = (player->x - map_x * BLOCK) / (fabs(cos_angle) < EPS ? 1.0 : fabs(cos_angle));
	else
		side_dist_x = ((map_x + 1) * BLOCK - player->x) / (fabs(cos_angle) < EPS ? 1.0 : fabs(cos_angle));

	if (step_y < 0)
		side_dist_y = (player->y - map_y * BLOCK) / (fabs(sin_angle) < EPS ? 1.0 : fabs(sin_angle));
	else
		side_dist_y = ((map_y + 1) * BLOCK - player->y) / (fabs(sin_angle) < EPS ? 1.0 : fabs(sin_angle));

	int side = -1; /* 0 = vertical (x-side), 1 = horizontal (y-side) */

	/* DDA loop with bounds checks and iteration limit */
	int iter = 0;
	int rows = 0;
	while (game->map[rows]) rows++;
	while (1)
	{
		if (++iter > 10000) /* fail-safe */
			break;

		if (side_dist_x < side_dist_y)
		{
			side_dist_x += delta_dist_x;
			map_x += step_x;
			side = 0;
		}
		else
		{
			side_dist_y += delta_dist_y;
			map_y += step_y;
			side = 1;
		}

		/* bounds check: treat out-of-bounds as wall hit and stop */
		if (map_y < 0 || map_y >= rows)
			break;
		size_t row_len = ft_strlen(game->map[map_y]);
		if (map_x < 0 || (size_t)map_x >= row_len)
			break;

		/* actual map hit */
		if (game->map[map_y][map_x] == '1')
			break;
		//printf("map_x=%d, map_y=%d\n", map_x,map_y);
	}

	/* compute perpendicular distance (use the DDA distances) */
	double perp_dist = (side == 0) ? (side_dist_x - delta_dist_x) : (side_dist_y - delta_dist_y);
	if (perp_dist < 1e-6) perp_dist = 1e-6; /* avoid div by zero */

	/* recompute exact hit point */
	double ray_x = player->x + perp_dist * cos_angle;
	double ray_y = player->y + perp_dist * sin_angle;

	perp_dist = compute_perp_dist(ray_x, ray_y, player);

	double proj_plane = (WIDTH / 2.0) / tan(FOV / 2.0);
	
	/* choose texture using side & step */
	t_texture *texture;
	if (side == 0)
		texture = (step_x > 0) ? &game->east : &game->west;
	else
		texture = (step_y > 0) ? &game->south : &game->north;

	/* compute tex X */
	double wall_hit = (side == 0) ? fmod(ray_y, BLOCK) : fmod(ray_x, BLOCK);
	if (wall_hit < 0) wall_hit += BLOCK;
	int tex_x = (int)((wall_hit / (double)BLOCK) * texture->width);
	if (tex_x < 0) tex_x = 0;
	if (tex_x >= texture->width) tex_x = texture->width - 1;

	if (!DEBUG)
	{
		double orig_height = (BLOCK / perp_dist) * proj_plane;
		if (orig_height > HEIGHT) orig_height = HEIGHT;
		if (orig_height < 1.0) orig_height = 1.0;

		double orig_start_f = (HEIGHT - orig_height) / 2.0;
		double orig_end_f = (orig_start_f + orig_height);
		int draw_start = (int)orig_start_f;
		int draw_end = (int)orig_end_f;
		if (draw_start < 0) draw_start = 0;
		if (draw_end > HEIGHT) draw_end = HEIGHT;

		int draw_pixels = draw_end - draw_start;
		if (draw_pixels <= 0)
			return ;

		double tex_step = (double)texture->height / orig_height;
		double tex_pos = 0.0;
		if (orig_start_f < 0.0) tex_pos = (-orig_start_f * tex_step);
		else
			tex_pos = 0.0;
		tex_pos = (double)(draw_start) - orig_start_f;
		tex_pos *= tex_step;

		int tex_x_flipped = tex_x;
		if ((side == 0 && cos_angle > 0.0) || (side == 1 && sin_angle < 0.0))
			tex_x_flipped = texture->width - tex_x - 1;

		for (int y = draw_start; y < draw_end; ++y)
		{
			int tex_y = (int)(tex_pos);
			if (tex_y < 0) tex_y = 0;
			if (tex_y >= texture->height) tex_y = texture->height - 1;

			int t_idx = tex_y * texture->size_line + tex_x_flipped * (texture->bpp / 8);

			int color = 0xDADADA;
			if (texture->bpp == 32)
				color = *(int *)(texture->data + t_idx);
			else if (texture->bpp == 24)
			{
				unsigned char *d = (unsigned char *)(texture->data + t_idx);
				color = d[0] | (d[1] << 8) | (d[2] << 16);
			}
			put_pixel(i, y, color, game);
			tex_pos += tex_step;
		}
	}
}

void put_pixel(int x, int y, int color, t_game *game)
{
	if (!game || !game->data)
		return;
	if (y < 0 || x >= WIDTH || y >= HEIGHT || x < 0)
		return ;
	int index = y * game->size_line + x * (game->bpp / 8);
	game->data[index] = color & 0xFF;
	game->data[index + 1] = (color >> 8) & 0xFF;
	game->data[index + 2] = (color >> 16) & 0xFF;
}

void put_pixel_safe(int x, int y, int color, t_game *game)
{
    /* defensive: require valid game and image buffer */
    if (!game || !game->data)
        return;
    if (x < 0 || x >= WIDTH || y < 0 || y >= HEIGHT)
        return;

    int bytes_per_pixel = game->bpp / 8;
    unsigned char *buf = (unsigned char *)game->data;
    int index = y * game->size_line + x * bytes_per_pixel;

    /* write safely according to bytes_per_pixel */
    buf[index] = color & 0xFF;
    if (bytes_per_pixel > 1) buf[index + 1] = (color >> 8) & 0xFF;
    if (bytes_per_pixel > 2) buf[index + 2] = (color >> 16) & 0xFF;
    if (bytes_per_pixel > 3) buf[index + 3] = (color >> 24) & 0xFF;
}
