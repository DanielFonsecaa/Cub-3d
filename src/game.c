#include "../includes/cub.h"

void	init_game(t_game *game)
{
    game->player = malloc(sizeof(*game->player));
    if (!game->player) { perror("malloc player"); exit(1); }

    game->canvas = malloc(sizeof(*game->canvas));
    if (!game->canvas) { perror("malloc canvas"); exit(1); }

    init_player(game->player);
    init_canvas(game->canvas);
	fill_background(game);
}

int	game_loop(t_game *game)
{
	t_player	*player;
	double		fraction;
	double		start_x;
	int			i;

	player = game->player;
	move_player(game);
	//clear_canvas(game);
	fill_background(game);
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
