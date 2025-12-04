#include "../../includes_bonus/cub_bonus.h"

void	pickup_check(t_game *g)
{
	int	px;
	int	py;
	int	i;

	px = (int)(g->player.x / BLOCK);
	py = (int)(g->player.y / BLOCK);
	if (in_bounds_tile(g, px, py) && g->grid.map[py][px] == 'C')
	{
		i = 0;
		while (i < g->n_collectables)
		{
			if (!g->collectables[i].is_collected
				&& g->collectables[i].x == px
				&& g->collectables[i].y == py)
			{
				g->collectables[i].is_collected = true;
				g->grid.map[py][px] = '0';
				break ;
			}
			i++;
		}
	}
}

void	check_collect_grip(t_game *game, bool *any_left)
{
	int	ry;
	int	rx;

	ry = 0;
	while (game->grid.map[ry] && !*any_left)
	{
		rx = 0;
		while (game->grid.map[ry][rx])
		{
			if (game->grid.map[ry][rx] == 'C')
			{
				*any_left = true;
				return ;
			}
			rx++;
		}
		ry++;
	}
}

void	update_collectables(t_game *game)
{
	bool	any_left;

	if (!game || !game->grid.map)
		return ;
	pickup_check(game);
	any_left = false;
	check_collect_grip(game, &any_left);
	if (!any_left)
		close_game(game, "You win!\n");
}

void	init_collectables(t_game *game)
{
	int			i;
	const int	frame_count = 6;
	const char	*paths[6] = {
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
		i = 0;
		while (i < game->n_collectables)
		{
			game->collectables[i].frames = game->collect_frames;
			i++;
		}
	}
}

void	init_anim_state(t_game *g, t_r_anim *ra)
{
	static int	anim_tick;
	static int	anim_frame;
	const int	anim_speed = 8;

	ra->proj_plane = (WIDTH / 2.0) / tan(FOV / 2.0);
	ra->inv_det = (g->plane.planex * g->plane.diry
			- g->plane.dirx * g->plane.planey);
	{
		if (g->collect_frame_count > 0 && g->collect_frames)
		{
			anim_tick++;
			if (anim_tick >= anim_speed)
			{
				anim_tick = 0;
				anim_frame = (anim_frame + 1) % g->collect_frame_count;
			}
		}
	}
	ra->anim_frame = anim_frame;
}
