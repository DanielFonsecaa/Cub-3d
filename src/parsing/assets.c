#include "../../includes/cub.h"

void	get_textures(t_game *game, char *line)
{
	int	i;

	i = 0;
	if (line[i])
	{
		while (ft_iswhite_space(line[i]))
			i++;
		if (!line[i])
			return ;
		set_direction_texture(game, line, i);
		set_floor_cealing(game, line, i);
	}
}

void	set_direction_texture(t_game *game, char *line, int i)
{
	if (!line[i + 2] || !line)
		return ;
	if (set_texture_path(&game->north, line, "NO", i))
		game->assets_ready++;
	if (set_texture_path(&game->south, line, "SO", i))
		game->assets_ready++;
	if (set_texture_path(&game->west, line, "WE", i))
		game->assets_ready++;
	if (set_texture_path(&game->east, line, "EA", i))
		game->assets_ready++;
}

int	set_texture_path(t_texture *texture, char *line, char *dir, int i)
{
	if (ft_strncmp(dir, line + i, 2) == 0)
	{
		texture->path = ft_strtrim(line + i + 2, WS);
		return (1);
	}
	return (0);
}

void	set_floor_cealing(t_game *game, char *line, int i)
{
	if (!line[i + 1])
		return ;
	if (ft_strncmp("F", line + i, 1) == 0)
	{
		if (game->floor_set)
			close_game(game, "Error\nDuplicate floor color definition\n");
		set_background_color(game, &game->floor, i + 1, line);
		game->floor_set = 1;
		game->assets_ready++;
	}
	else if (ft_strncmp("C", line + i, 1) == 0)
	{
		if (game->ceiling_set)
			close_game(game, "Error\nDuplicate ceiling color definition\n");
		set_background_color(game, &game->cealing, i + 1, line);
		game->ceiling_set = 1;
		game->assets_ready++;
	}
}

void	set_background_color(t_game *game, t_color *bg, int i, char *line)
{
	int		start;
	int		j;
	char	color[4];

	j = -1;
	while (++j < 3)
	{
		while (ft_iswhite_space(line[i]))
			i += 1;
		start = 0;
		while (ft_isdigit(line[i]) && start < 3)
			color[start++] = line[i++];
		while (ft_isdigit(line[i]) || ft_iswhite_space(line[i]))
			i++;
		if (start == 0)
			close_game(game, NO_BG_COLOR);
		color[start] = '\0';
		if (j == 0)
			bg->red = ft_atoi(color);
		if (j == 1)
			bg->green = ft_atoi(color);
		if (j == 2)
			bg->blue = ft_atoi(color);
		i++;
	}
}
