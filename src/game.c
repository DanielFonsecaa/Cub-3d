#include "../includes/cub.h"

void	init_game(t_game *game, char *map_name)
{
	init_player(&game->player);
	init_canvas(&game->canvas);
	game->data = mlx_get_data_addr(game->canvas.img, &game->bpp, &game->size_line, &game->endian);
	game->map = get_map(map_name);
}

int	game_loop(t_game *game)
{
	t_player	*player;
	double		fraction;
	double		start_x;
	int			i;

	player = &game->player;
	move_player(player);
	clear_canvas(game);
	if(DEBUG)
	{
		draw_square((int)player->x, (int)player->y, 10, GREEN, game); // small debug square
		draw_map(game); //todo
	}
	fraction = PI / 3 / WIDTH; //FOV field of view (60º)
	start_x = player->angle - PI / 6;
	i = 0;
	while (i < WIDTH)
	{
		draw_line(player, game, start_x, i); // cast order: player, game, angle, column
		start_x += fraction;
		i++;
	}
	mlx_put_image_to_window(game->canvas.mlx, game->canvas.win, game->canvas.img, 0 , 0);
	return (0);
}
