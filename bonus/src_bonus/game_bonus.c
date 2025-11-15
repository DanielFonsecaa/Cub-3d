#include "../includes_bonus/cub_bonus.h"

static int	mouse_hook(int x, int y, t_game *game)
{
	int		dx;
	double	delta;

	dx = x - (WIDTH / 2);
	if (dx != 0)
	{
		delta = dx * game->player.angle_speed;
		game->player.angle = game->player.angle + delta;
	}
	mlx_mouse_move(game->canvas.mlx, game->canvas.win, WIDTH / 2, y);
	return (0);
}

void	start(t_game *game)
{
	game->player.speed = 10;
	game->player.angle_speed = 0.003;
	game->canvas.mlx = mlx_init();
	init_textures(game);
	game->canvas.win = mlx_new_window(game->canvas.mlx, WIDTH, HEIGHT, "cub3d");
	game->canvas.img = mlx_new_image(game->canvas.mlx, WIDTH, HEIGHT);
	if (!game->canvas.img)
		close_game(game, INVALID_TEXTURE);
	game->data = mlx_get_data_addr(game->canvas.img, &game->bpp,
			&game->size_line, &game->endian);
	if (!game->data)
		close_game(game, INVALID_TEXTURE);
	game->zbuffer = (double *)malloc(sizeof(double) * WIDTH);
	if (!game->zbuffer)
		close_game(game, MISSING_ASSETS);
	mlx_hook(game->canvas.win, 2, 1L << 0, key_press, game);
	mlx_hook(game->canvas.win, 3, 1L << 1, key_release, &game->player);
	mlx_loop_hook(game->canvas.mlx, game_loop, game);
	mlx_hook(game->canvas.win, DestroyNotify, 0, close_and_printf, game);
	mlx_hook(game->canvas.win, MotionNotify, 1L << 6, mouse_hook, game);
	mlx_loop(game->canvas.mlx);
}

int	game_loop(t_game *game)
{
	t_plane	plane;
	int		i;

	ft_bzero(&plane, sizeof(t_plane));
	move_player(game);
	fill_background(game);
	plane.dirx = cos(game->player.angle);
	plane.diry = sin(game->player.angle);
	plane.plane_len = tan(FOV / 2.0);
	plane.planex = -plane.diry * plane.plane_len;
	plane.planey = plane.dirx * plane.plane_len;
	i = 0;
	while (i < WIDTH)
	{
		plane.camera_x = (2.0 * i) / (double)WIDTH - 1.0;
		plane.raydx = plane.dirx + plane.planex * plane.camera_x;
		plane.raydy = plane.diry + plane.planey * plane.camera_x;
		draw_line(&game->player, game, &plane, i);
		i++;
	}
	game->plane = plane;
	render_collectables(game);
	update_collectables(game);
	draw_minimap(game);
	mlx_put_image_to_window(game->canvas.mlx, game->canvas.win,
		game->canvas.img, 0, 0);
	return (0);
}

void	fill_background(t_game *game)
{
	int	x;
	int	y;
	int	half;
	int	roof_color;
	int	floor_color;

	roof_color = (game->cealing.blue) | (game->cealing.green << 8)
		| (game->cealing.red << 16);
	floor_color = (game->floor.blue) | (game->floor.green << 8)
		| (game->floor.red << 16);
	y = -1;
	half = HEIGHT / 2;
	while (++y < half)
	{
		x = -1;
		while (++x < WIDTH)
			put_pixel_safe(x, y, roof_color, game);
	}
	y = half;
	while (++y < HEIGHT)
	{
		x = -1;
		while (++x < WIDTH)
			put_pixel_safe(x, y, floor_color, game);
	}
}

static int in_bounds_tile(t_game *g, int tx, int ty)
{
	size_t len;

	if (ty < 0 || ty >= g->grid.height)
		return (0);
	len = ft_strlen(g->grid.map[ty]);
	if (tx < 0 || (size_t)tx >= len)
		return (0);
	return (1);
}

void    update_collectables(t_game *game)
{
	int px;
	int py;
	int i;

	if (!game || !game->grid.map)
		return ;
	px = (int)(game->player.x / BLOCK);
	py = (int)(game->player.y / BLOCK);
	if (in_bounds_tile(game, px, py) && game->grid.map[py][px] == 'C')
	{
		i = 0;
		while (i < game->n_collectables)
		{
			if (!game->collectables[i].is_collected
				&& game->collectables[i].x == px
				&& game->collectables[i].y == py)
			{
				game->collectables[i].is_collected = true;
				if (game->n_collectables > 0)
					game->n_collectables--;
				game->grid.map[py][px] = '0';
				break ;
			}
			i++;
		}
	}
	if (game->n_collectables == 0)
	{
		if (in_bounds_tile(game, game->exit.x, game->exit.y)
			&& game->grid.map[game->exit.y][game->exit.x] == 'D')
			game->grid.map[game->exit.y][game->exit.x] = '0';
		if (px == game->exit.x && py == game->exit.y)
			close_game(game, "You win!\n");
	}
}

void	init_textures(t_game *g)
{
	g->north.img = mlx_xpm_file_to_image(g->canvas.mlx, g->north.path,
			&g->north.width, &g->north.height);
	if (!g->north.img)
		close_game(g, "Error\nMlx failed to load north\n");
	g->north.data = mlx_get_data_addr(g->north.img, &g->north.bpp,
			&g->north.size_line, &g->north.endian);
	g->south.img = mlx_xpm_file_to_image(g->canvas.mlx, g->south.path,
			&g->south.width, &g->south.height);
	if (!g->south.img)
		close_game(g, "Error\nMlx failed to load south\n");
	g->south.data = mlx_get_data_addr(g->south.img, &g->south.bpp,
			&g->south.size_line, &g->south.endian);
	g->west.img = mlx_xpm_file_to_image(g->canvas.mlx, g->west.path,
			&g->west.width, &g->west.height);
	if (!g->west.img)
		close_game(g, "Error\nMlx failed to load west\n");
	g->west.data = mlx_get_data_addr(g->west.img, &g->west.bpp,
			&g->west.size_line, &g->west.endian);
	g->east.img = mlx_xpm_file_to_image(g->canvas.mlx, g->east.path,
			&g->east.width, &g->east.height);
	if (!g->east.img)
		close_game(g, "Error\nMlx failed to load east\n");
	g->east.data = mlx_get_data_addr(g->east.img, &g->east.bpp,
			&g->east.size_line, &g->east.endian);
	if (g->door.path)
	{
		g->door.img = mlx_xpm_file_to_image(g->canvas.mlx, g->door.path,
				&g->door.width, &g->door.height);
		if (g->door.img)
			g->door.data = mlx_get_data_addr(g->door.img, &g->door.bpp,
					&g->door.size_line, &g->door.endian);
	}
	if (g->collect.path)
	{
		g->collect.img = mlx_xpm_file_to_image(g->canvas.mlx, g->collect.path,
				&g->collect.width, &g->collect.height);
		if (g->collect.img)
			g->collect.data = mlx_get_data_addr(g->collect.img, &g->collect.bpp,
					&g->collect.size_line, &g->collect.endian);
	}
}

 /* void	init_collectables(t_game *game)
{
	const int frame_count = 6;
	const char *paths[6] = {
		"textures/carrot/carrot_1.xpm", "textures/carrot/carrot_2.xpm",
		"textures/carrot/carrot_3.xpm", "textures/carrot/carrot_4.xpm",
		"textures/carrot/carrot_5.xpm", "textures/carrot/carrot_6.xpm"
	};
	game->collect_frame_count = frame_count;
	game->collect_frames = ft_calloc(frame_count, sizeof(t_tex));
	if (!game->collect_frames)
		close_game(game, MISSING_ASSETS);
	if (!load_textures(game, game->collect_frames, frame_count, paths))
		close_game(game, MISSING_ASSETS);
	if (game->collectables && game->n_collectables > 0)
	{
		int i = 0;
		while (i < game->n_collectables)
		{
			game->collectables[i].frames = game->collect_frames;
			i++;
		}
	}
}



int	load_texture(t_game *game, t_tex *tex, const char *path)
{
	int w;
	int h;

	if (!game || !tex || !path)
		return (0);
	tex->img = mlx_xpm_file_to_image(game->canvas.mlx, (char *)path, &w, &h);
	if (!tex->img)
		return (0);
	tex->data = mlx_get_data_addr(tex->img, &tex->bpp, &tex->size_line, &tex->endian);
	tex->width = w;
	tex->height = h;
	tex->path = ft_strdup(path);
	if (!tex->path)
		return (0);
	return (1);
}

int	load_textures(t_game *game, t_tex *arr, int n, const char **paths)
{
	int i;

	if (!game || !arr || !paths || n <= 0)
		return (0);
	i = 0;
	while (i < n)
	{
		if (!load_texture(game, &arr[i], paths[i]))
		{
			while (--i >= 0)
			{
				if (arr[i].img)
					mlx_destroy_image(game->canvas.mlx, arr[i].img);
				free(arr[i].path);
				arr[i].img = NULL;
				arr[i].path = NULL;
			}
			return (0);
		}
		i++;
	}
	return (1);
}
  */