#include "../includes_bonus/cub_bonus.h"

int	key_press(int keycode, t_game *game)
{
	if (keycode == XK_Escape)
		close_game(game, CLOSE_GAME);
	if (keycode == W)
		game->player.key_up = true;
	if (keycode == S)
		game->player.key_down = true;
	if (keycode == A)
		game->player.key_left = true;
	if (keycode == D)
		game->player.key_right = true;
	if (keycode == LEFT)
		game->player.left_rotate = true;
	if (keycode == RIGHT)
		game->player.right_rotate = true;
	return (0);
}

int	key_release(int keycode, t_player *player)
{
	if (keycode == W)
		player->key_up = false;
	if (keycode == S)
		player->key_down = false;
	if (keycode == A)
		player->key_left = false;
	if (keycode == D)
		player->key_right = false;
	if (keycode == LEFT)
		player->left_rotate = false;
	if (keycode == RIGHT)
		player->right_rotate = false;
	return (0);
}

void	move_player(t_game *game)
{
	double	cos_angle;
	double	sin_angle;

	cos_angle = cos(game->player.angle);
	sin_angle = sin(game->player.angle);
	if (game->player.left_rotate)
		game->player.angle -= game->player.angle_speed;
	if (game->player.right_rotate)
		game->player.angle += game->player.angle_speed;
	if (game->player.angle > 2 * PI)
		game->player.angle = 0;
	if (game->player.angle < 0)
		game->player.angle = 2 * PI;
	move_wasd(game, sin_angle, cos_angle);
}

void	move_wasd(t_game *game, double sin, double cos)
{
	double	new_cos;
	double	new_sin;

	new_cos = cos * game->player.speed;
	new_sin = sin * game->player.speed;
	if (game->player.key_up)
		go_up(game, new_sin, new_cos);
	if (game->player.key_down)
		go_down(game, (new_sin * -1), (new_cos * -1));
	if (game->player.key_left)
		go_left(game, sin, (new_cos * -1));
	if (game->player.key_right)
		go_right(game, (new_sin * -1), new_cos);
}
