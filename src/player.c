#include "../includes/cub.h"

void	init_player(t_player *player)
{
/*	player->x = WIDTH / 6.0; //put the player in the center
	player->y = HEIGHT / 7.0;*/
/*	player->angle = PI / 3; //90 degrees facing*/
	player->key_up = false;
	player->key_down = false;
	player->key_left = false;
	player->key_right = false;
	player->left_rotate = false;
	player->right_rotate = false;
}

int	key_press(int keycode, t_game *game)
{
	if (keycode == XK_Escape)
		close_game(game, CLOSE_GAME);
	if (keycode == W)
		game->player->key_up = true;
	if (keycode == S)
		game->player->key_down = true;
	if (keycode == A)
		game->player->key_left = true;
	if (keycode == D)
		game->player->key_right = true;
	if (keycode == LEFT)
		game->player->left_rotate = true;
	if (keycode == RIGHT)
		game->player->right_rotate = true;
	return 0;
}

int	key_release(int keycode, t_player *player)
{
	if(keycode == W)
		player->key_up = false;
	if(keycode == S)
		player->key_down = false;
	if(keycode == A)
		player->key_left = false;
	if(keycode == D)
		player->key_right = false;
	if(keycode == LEFT)
		player->left_rotate = false;
	if(keycode == RIGHT)
		player->right_rotate = false;
	return 0;
}

void	move_player(t_game *game)
{
	int speed = 2;
	double angle_speed = 0.03;
	double cos_angle = cos(game->player->angle);
	double sin_angle = sin(game->player->angle);

	if (game->player->left_rotate)
		game->player->angle -= angle_speed;
	if (game->player->right_rotate)
		game->player->angle += angle_speed;
	if (game->player->angle > 2 * PI)
		game->player->angle = 0;
	if (game->player->angle < 0)
		game->player->angle = 2 * PI;
	move_wasd(game, sin_angle, cos_angle, speed);
}

void	move_wasd(t_game *game, double sin, double cos, int speed)
{
    double new_cos = cos * speed;
    double new_sin = sin * speed;
	double next_x;
	double next_y;
    double radius = 8; //radius of character

    if (game->player->key_up)
    {
        // X axis
        next_x = game->player->x + new_cos;
        if (!touch(next_x + (new_cos > 0 ? radius : -radius), game->player->y, game))
            game->player->x = next_x;
        // Y axis
        next_y = game->player->y + new_sin;
        if (!touch(game->player->x, next_y + (new_sin > 0 ? radius : -radius), game))
            game->player->y = next_y;
    }
    if (game->player->key_down)
    {
        next_x = game->player->x - new_cos;
        if (!touch(next_x + ((-new_cos) > 0 ? radius : -radius), game->player->y, game))
            game->player->x = next_x;
        next_y = game->player->y - new_sin;
        if (!touch(game->player->x, next_y + ((-new_sin) > 0 ? radius : -radius), game))
            game->player->y = next_y;
    }
    if (game->player->key_left)
    {
        next_x = game->player->x + new_sin;
        if (!touch(next_x + (new_sin > 0 ? radius : -radius), game->player->y, game))
            game->player->x = next_x;
        next_y = game->player->y - new_cos;
        if (!touch(game->player->x, next_y + ((-new_cos) > 0 ? radius : -radius), game))
            game->player->y = next_y;
    }
    if (game->player->key_right)
    {
        next_x = game->player->x - new_sin;
        if (!touch(next_x + ((-new_sin) > 0 ? radius : -radius), game->player->y, game))
            game->player->x = next_x;
        next_y = game->player->y + new_cos;
        if (!touch(game->player->x, next_y + (new_cos > 0 ? radius : -radius), game))
            game->player->y = next_y;
    }
}
