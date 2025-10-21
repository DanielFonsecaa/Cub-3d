#include "../includes/cub.h"

void	init_game(t_game *game)
{
    game->player = malloc(sizeof(*game->player));
    if (!game->player) { perror("malloc player"); exit(1); }

    game->canvas = malloc(sizeof(*game->canvas));
    if (!game->canvas) { perror("malloc canvas"); exit(1); }

    init_player(game->player);
    init_canvas(game->canvas);
    game->data = mlx_get_data_addr(game->canvas->img, &game->bpp, &game->size_line, &game->endian);
	game->north.img = mlx_xpm_file_to_image(game->canvas->mlx, "textures/north.xpm", &game->north.width, &game->north.height);
	game->north.data = mlx_get_data_addr(game->north.img, &game->north.bpp, &game->north.size_line, &game->north.endian);
	game->south.img = mlx_xpm_file_to_image(game->canvas->mlx, "textures/south.xpm", &game->south.width, &game->south.height);
	game->south.data = mlx_get_data_addr(game->south.img, &game->south.bpp, &game->south.size_line, &game->south.endian);
	game->west.img = mlx_xpm_file_to_image(game->canvas->mlx, "textures/west.xpm", &game->west.width, &game->west.height);
	game->west.data = mlx_get_data_addr(game->west.img, &game->west.bpp, &game->west.size_line, &game->west.endian);
	game->east.img = mlx_xpm_file_to_image(game->canvas->mlx, "textures/east.xpm", &game->east.width, &game->east.height);
	game->east.data = mlx_get_data_addr(game->east.img, &game->east.bpp, &game->east.size_line, &game->east.endian);
}

int	game_loop(t_game *game)
{
	t_player	*player;
	double		fraction;
	double		start_x;
	int			i;

	player = game->player;
	move_player(game);
	clear_canvas(game);
	if (DEBUG)
	{
		draw_square((int)player->x, (int)player->y, 10, GREEN, game); // small debug square
		draw_map(game); //todo
	}
	fraction = PI / 3 / WIDTH; // angle step between rays (60 degress)
	start_x = player->angle - PI / 6; // angle of the first ray (leftmost)
	i = 0;
	while (i < WIDTH)
	{
		draw_line(player, game, start_x, i); // cast a ray at angle start_x
		start_x += fraction; // move to the next angle for the next column
		i++;
	}
	mlx_put_image_to_window(game->canvas->mlx, game->canvas->win, game->canvas->img, 0 , 0);
	return (0);
}
