#include "../includes/cub.h"

void	move_wasd(t_game *game, double sin, double cos)
{
	double new_cos;
	double new_sin;

	new_cos = cos * game->player.speed;
	new_sin = sin * game->player.speed;
	if (game->player.key_up)
		go_up(game, new_sin, new_cos);
	if (game->player.key_down)
		go_down(game, (new_sin * - 1), (new_cos * - 1));
	if (game->player.key_left)
		go_left(game, sin, (new_cos * - 1));
	if (game->player.key_right)
		go_right(game, (new_sin * - 1), new_cos);
}

void	go_up(t_game *game, double sin, double cos)
{
	double next_x;
	double next_y;
	double radius;

	next_x = game->player.x + cos;
	if (cos > 0)
		radius = 8;
	else
		radius = -8;
	if (!touch(next_x + radius, game->player.y, game))
		game->player.x = next_x;
	next_y = game->player.y + sin;
	if (sin > 0)
		radius = 8;
	else
		radius = -8;
	if (!touch(game->player.x, next_y + radius, game))
		game->player.y = next_y;
}

void	go_down(t_game *game, double sin, double cos)
{
	double next_x;
	double next_y;
	double radius;

	next_x = game->player.x + cos;
	if (cos > 0)
		radius = 8;
	else
		radius = -8;
	if (!touch(next_x + radius, game->player.y, game))
		game->player.x = next_x;
	next_y = game->player.y + sin;
	if (sin > 0)
		radius = 8;
	else
		radius = -8;
	if (!touch(game->player.x, next_y + radius, game))
		game->player.y = next_y;
}

void	go_left(t_game *game, double sin, double cos)
{
	double next_x;
	double next_y;
	double radius;

	next_x = game->player.x + sin;
	if (sin > 0)
		radius = 8;
	else
		radius = -8;
	if (!touch(next_x + radius, game->player.y, game))
		game->player.x = next_x;
	next_y = game->player.y + cos;
	if (cos > 0)
		radius = 8;
	else
		radius = -8;
	if (!touch(game->player.x, next_y + radius, game))
		game->player.y = next_y;
}

void	go_right(t_game *game, double sin, double cos)
{
	double next_x;
	double next_y;
	double radius;

	next_x = game->player.x + sin;
	if (sin > 0)
		radius = 8;
	else
		radius = -8;
	if (!touch(next_x + radius, game->player.y, game))
		game->player.x = next_x;
	next_y = game->player.y + cos;
	if (cos > 0)
		radius = 8;
	else
		radius = -8;
	if (!touch(game->player.x, next_y + radius, game))
		game->player.y = next_y;
}
