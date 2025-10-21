#include "../../includes/cub.h"

void	init_canvas(t_canvas *canvas)
{
	canvas->mlx = mlx_init();
	canvas->win = mlx_new_window(canvas->mlx, WIDTH, HEIGHT, "canvas");
	canvas->img = mlx_new_image(canvas->mlx, WIDTH, HEIGHT);
}

//very bad, this need a huge refactor, sorry kkkkkk
char	**get_map(char *map_name)
{
	int		fd;
	char	*line;
	char	**map;
	size_t	count;

	count = 0;
	fd = open(map_name, O_RDONLY);
	map = malloc(16 * sizeof(char *));
	if (!map)
		return (NULL);
	while ((line = get_next_line(fd)) != NULL)
	{
		map[count++] = line;
		//free(line);
		//line = NULL;
	}
	close(fd);
	map[count] = NULL;
	return (map);
}

void	clear_canvas(t_game *game)
{
	int	i;
	int	j;

	i = -1;
	while (++i < HEIGHT)
	{
		j = -1;
		while (++j < WIDTH)
			put_pixel(j, i, 0, game);
	}
}

void	draw_square(int x, int y, int size, int color, t_game *game)
{
	int	i;

	i = -1;
	while (++i < size)
		put_pixel(x + i, y, color, game);
	i = -1;
	while (++i < size)
		put_pixel(x, y + 1, color, game);
	i = -1;
	while (++i < size)
		put_pixel(x + size, y + i, color, game);
	i = -1;
	while (++i < size)
		put_pixel(x + i, y + size, color, game);
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
	int x = px / BLOCK;
	int y = py / BLOCK;
	if(game->map[y][x] == '1')
		return (true);
	return (false);
}


// raycasting functions
void	draw_line(t_player *player, t_game *game, double start_x, int i)
{
    double cos_angle = cos(start_x);
    double sin_angle = sin(start_x);
    double ray_x = player->x;
    double ray_y = player->y;

    double delta_dist_x = fabs(1 / cos_angle);
    double delta_dist_y = fabs(1 / sin_angle);
    double side_dist_x, side_dist_y;
    int step_x, step_y;
    int map_x = (int)(player->x / BLOCK);
    int map_y = (int)(player->y / BLOCK);

    if (cos_angle < 0) {
        step_x = -1;
        side_dist_x = (player->x - map_x * BLOCK) * delta_dist_x;
    } else {
        step_x = 1;
        side_dist_x = ((map_x + 1) * BLOCK - player->x) * delta_dist_x;
    }
    if (sin_angle < 0) {
        step_y = -1;
        side_dist_y = (player->y - map_y * BLOCK) * delta_dist_y;
    } else {
        step_y = 1;
        side_dist_y = ((map_y + 1) * BLOCK - player->y) * delta_dist_y;
    }

    int side = -1; /* 0 = vertical (x-side), 1 = horizontal (y-side) */
    /* DDA loop: step through map cells until we hit a wall */
    while (!touch(ray_x, ray_y, game))
    {
        if (side_dist_x < side_dist_y)
        {
            side_dist_x += delta_dist_x;
            map_x += step_x;
            side = 0; /* we moved in x (vertical wall) */
        }
        else
        {
            side_dist_y += delta_dist_y;
            map_y += step_y;
            side = 1; /* we moved in y (horizontal wall) */
        }
        /* compute tentative ray end using the distance before the last step */
        /* (we will compute perp_dist below using the same values) */
        ray_x = player->x + (side == 0 ? (side_dist_x - delta_dist_x) * cos_angle : (side_dist_y - delta_dist_y) * cos_angle);
        ray_y = player->y + (side == 0 ? (side_dist_x - delta_dist_x) * sin_angle : (side_dist_y - delta_dist_y) * sin_angle);
    }

    /* perpendicular distance along the ray to the wall hit (use DDA distances) */
    double perp_dist = (side == 0) ? (side_dist_x - delta_dist_x) : (side_dist_y - delta_dist_y);
    /* recompute exact hit point (consistent) */
    ray_x = player->x + perp_dist * cos_angle;
    ray_y = player->y + perp_dist * sin_angle;

    /* choose texture based on side & step direction (robust at corners) */
    t_texture *texture;
    if (side == 0) /* vertical wall -> EAST/WEST */
        texture = (step_x > 0) ? &game->east : &game->west;
    else /* horizontal wall -> SOUTH/NORTH */
        texture = (step_y > 0) ? &game->south : &game->north;

    /* compute exact position on the wall (in world coords) and tex X */
    double wall_hit = (side == 0) ? fmod(ray_y, BLOCK) : fmod(ray_x, BLOCK);
    if (wall_hit < 0) wall_hit += BLOCK;
    int tex_x = (int)((wall_hit / (double)BLOCK) * texture->width);
    if (tex_x < 0) tex_x = 0;
    if (tex_x >= texture->width) tex_x = texture->width - 1;

    if (!DEBUG)
    {
        double dist = perp_dist; /* could also use fixed_dist if you prefer */
        double height = (BLOCK / dist) * (WIDTH / 2);
        if (height < 1) height = 1;
        int start_y = (HEIGHT - height) / 2;
        int end = start_y + height;
        int y = start_y;
        while (y < end)
        {
            int tex_y = ((y - start_y) * texture->height) / height;
            if (tex_y < 0) tex_y = 0;
            if (tex_y >= texture->height) tex_y = texture->height - 1;
            int t_idx = tex_y * texture->size_line + tex_x * (texture->bpp / 8);

            int color;
            if (texture->bpp == 32)
                color = *(int *)(texture->data + t_idx);
            else if (texture->bpp == 24) {
                unsigned char *d = (unsigned char *)(texture->data + t_idx);
                color = d[0] | (d[1] << 8) | (d[2] << 16);
            } else {
                color = 0xFFFFFF;
            }
            put_pixel(i, y, color, game);
            y++;
        }
    }
}

void put_pixel(int x, int y, int color, t_game *game)
{
    if(x >= WIDTH || y >= HEIGHT || x < 0 || y < 0)
        return;
    int index = y * game->size_line + x * game->bpp / 8;
    game->data[index] = color & 0xFF;
    game->data[index + 1] = (color >> 8) & 0xFF;
    game->data[index + 2] = (color >> 16) & 0xFF;
}
