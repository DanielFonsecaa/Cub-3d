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
		map[count++] = line;
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

void put_pixel(int x, int y, int color, t_game *game)
{
	if (x < 0 || y < 0 || x >= WIDTH || y >= HEIGHT || !game->data)
		return;

	int bpp_bytes = game->bpp / 8;
	int idx = y * game->size_line + x * bpp_bytes;

	if (game->bpp == 32)
	{
		// Fast 32-bit write. Works regardless of endian if you build the 'color'
		// to match the expected int layout. On little-endian machines this is fine:
		*(unsigned int *)(game->data + idx) = (unsigned int)color;
	}
	else if (game->bpp == 24)
	{
		if (game->endian == 0) // little-endian: LSB first (B G R)
		{
			game->data[idx]     = color & 0xFF;        // blue
			game->data[idx + 1] = (color >> 8) & 0xFF; // green
			game->data[idx + 2] = (color >> 16) & 0xFF; // red
		}
		else // big-endian: MSB first (R G B)
		{
			game->data[idx]     = (color >> 16) & 0xFF; // red
			game->data[idx + 1] = (color >> 8) & 0xFF;  // green
			game->data[idx + 2] = color & 0xFF;         // blue
		}
	}
	else
	{
		// fallback: write per-byte up to bpp_bytes, using little-endian mapping
		for (int i = 0; i < bpp_bytes; ++i)
			game->data[idx + i] = (color >> (8 * i)) & 0xFF;
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
	double angle = atan2(delta_y, delta_x) - game->player.angle;
	double fix_dist = distance(delta_x, delta_y) * cos(angle);
	return (fix_dist);
}

// touch function 
bool	touch(double px, double py, t_game *game)
{
	int x = px / BLOCK;
	int y = py / BLOCK;
	if(game->map[y][x] == '1')
		return true;
	return false;
}


// raycasting functions
void	draw_line(t_player *player, t_game *game, double start_x, int i)
{
	double cos_angle = cos(start_x);
	double sin_angle = sin(start_x);
	double ray_x = player->x;
	double ray_y = player->y;

	while(!touch(ray_x, ray_y, game))
	{
		if(DEBUG)
			put_pixel(ray_x, ray_y, RED, game);
		ray_x += cos_angle;
		ray_y += sin_angle;
	}
	if(!DEBUG)
	{
		double dist = fixed_dist(player->x, player->y, ray_x, ray_y, game);
		double height = (BLOCK / dist) * (WIDTH / 2);
		int start_y = (HEIGHT - height) / 2;
		int end = start_y + height;
		while(start_y < end)
		{
			put_pixel(i, start_y, 255, game);
			start_y++;
		}
	}
}
