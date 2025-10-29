#include "../../includes/cub.h"
#include <unistd.h>
#include <limits.h>
#include <stdio.h>
#include <errno.h>

int get_map_lines(char *map_name)
{
	int     num_lines;
	int     fd;
	char    *line;

	num_lines = 0;
	fd = open(map_name, O_RDONLY);
	if (fd < 0)
		return (0);
	while ((line = get_next_line(fd)) != NULL)
	{
		num_lines++;
		free(line);
	}
	close(fd);
	return (num_lines);
}

void fill_background(t_game *game)
{
    int x, y;
    int half = HEIGHT / 2;
    int ceil_color = (game->cealing.blue) | (game->cealing.green << 8) | (game->cealing.red << 16);
    int floor_color = (game->floor.blue)   | (game->floor.green   << 8) | (game->floor.red   << 16);

    for (y = 0; y < half; ++y)
        for (x = 0; x < WIDTH; ++x)
            put_pixel_safe(x, y, ceil_color, game);

    for (y = half; y < HEIGHT; ++y)
        for (x = 0; x < WIDTH; ++x)
            put_pixel_safe(x, y, floor_color, game);
}

void	set_direction_texture(t_game *game, char *line, int i)
{
	if (!line[i + 2] || !line)
		return ;
	if (ft_strncmp("NO", line + i, 2) == 0)
	{
		if (game->north.path)
			close_game(game, "Error\nDuplicate north texture definition\n");
		game->north.path = ft_strtrim(line + i + 2, WS);
		game->assets_ready++;
	}
	if (ft_strncmp("SO", line + i, 2) == 0)
	{
		if (game->south.path)
			close_game(game, "Error\nDuplicate south texture definition\n");
		game->south.path = ft_strtrim(line + i + 2, WS);
		game->assets_ready++;
	}
	if (ft_strncmp("WE", line + i, 2) == 0)
	{
		if (game->west.path)
			close_game(game, "Error\nDuplicate west texture definition\n");
		game->west.path = ft_strtrim(line + i + 2, WS);
		game->assets_ready++;
	}
	if (ft_strncmp("EA", line + i, 2) == 0)
	{
		if (game->east.path)
			close_game(game, "Error\nDuplicate east texture definition\n");
		game->east.path = ft_strtrim(line + i + 2, WS);
		game->assets_ready++;
	}
}

void	validade_texture(t_game *game)
{
	if (!game->north.path) close_game(game, NORTH_TEXT_ERR);
	if (!game->south.path) close_game(game, SOUTH_TEXT_ERR);
	if (!game->east.path)  close_game(game, EAST_TEXT_ERR);
	if (!game->west.path)  close_game(game, WEST_TEXT_ERR);
}

int	validate_map_name(char *file)
{
	char	*str;
	char	*file_name;
	int		len;

	str = ft_strrchr(file, '.');
	file_name = ft_strrchr(file, '/');
	if (file_name)
		len = ft_strlen(ft_strrchr(file, '/')) - 1;
	else
		len = ft_strlen(file);
	if (!str || len < 5 || ft_strncmp(str, EXTENSION, ft_strlen(EXTENSION) + 1))
		return (0);
	return (1);
}

void    get_textures(t_game *game, char *line)
{
	int		i;
	
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
	int	start;
	int	j;
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

int get_lines_of_map(char *line)
{
	int i;

	if (!line)
		return (0);
	i = 0;
	while (ft_iswhite_space(line[i]))
		i++;

	/* empty/whitespace-only line */
	if (line[i] == '\0')
		return (0);

	/* after map started accept lines that look like map rows ('0' or '1' or other map tokens) */
	if (!ft_isascii(line[i]) && !ft_iswhite_space(line[i]))
		return (0); /* ignore malformed/extra lines */
	return (1);
}

void	validate_cell(t_game *game, int column_pos, int row_pos, int *flag)
{
	char **map;

	map = game->map;
	if (ft_strchr("NWES", map[column_pos][row_pos]))
	{
		if (*flag)
			close_game(game, "Error\nDuplicate player position\n");
		game->player->x = row_pos * BLOCK + BLOCK / 2;
		game->player->y = column_pos * BLOCK + BLOCK / 2;
		if (map[column_pos][row_pos] == 'S')
			game->player->angle = PI / 2;
		if (map[column_pos][row_pos] == 'N')
			game->player->angle = PI * 1.5;
		if (map[column_pos][row_pos] == 'E')
			game->player->angle = PI * 2;
		if (map[column_pos][row_pos] == 'W')
			game->player->angle = PI;
		*flag = 1;
		return ;
	}
	if (column_pos == 0 || !map[column_pos - 1] || ft_strchr(WS, map[column_pos - 1][row_pos]) || row_pos >= (int)ft_strlen(map[column_pos - 1]))
		close_game(game, MAP_INVALID);
	if (row_pos >= (int)ft_strlen(map[column_pos + 1]) || !map[column_pos + 1] || ft_strchr(WS, map[column_pos + 1][row_pos]))
		close_game(game, MAP_INVALID);
	if (row_pos == 0 || !map[column_pos] || ft_strchr(WS, map[column_pos][row_pos - 1]))
		close_game(game, MAP_INVALID);
	if (row_pos + 1 >= (int)ft_strlen(map[column_pos]) || !map[column_pos] || ft_strchr(WS, map[column_pos][row_pos + 1]))
		close_game(game, MAP_INVALID);
}

int	validate_map(t_game *game, int height)
{
	int	x;
	int	y;
	int	flag;

	y = -1;
	flag = 0;
	while (++y < height)
	{
		x = -1;
		while (++x < (int)ft_strlen(game->map[y]))
		{
			if (game->map[y][x] == '1' || ft_iswhite_space(game->map[y][x]))
				continue ;
			if (game->map[y][x] == '0' || ft_strchr("NWES", game->map[y][x]))
				validate_cell(game, y, x, &flag);
			else
				close_game(game, INVALID_CHAR_AT_MAP);
		}
	}
	if (height == 0)
		close_game(game, MAP_NOT_SET);
	if (!flag)
		close_game(game, NO_PLAYER);
	return (0);
}

void	get_map(t_game *game, char *map_name)
{
	int		fd;
	char	*line;
	char	**map;
	size_t	count;
	int     map_lines;
	int		flag;
	int 	i;
	if (!validate_map_name(map_name))
		close_game(game, INVALID_MAP_NAME);
	map_lines = get_map_lines(map_name);
	if (map_lines <= 0)
		return ;
	/* allocate map_lines + 1 for the NULL terminator */
	map = ft_calloc(map_lines + 1, sizeof(char *));
	if (!map)
		return ;

	fd = open(map_name, O_RDONLY);
	if (fd < 0)
	{
		free(map);
		return ;
	}
	count = 0;
	while (game->assets_ready < 6)
	{
		line = get_next_line(fd);
		i = 0;
		if (!line)
			close_game(game, MISSING_ASSETS);
		while (ft_iswhite_space(line[i]))
			i++;
		// Check if we have at least 3 characters starting from position i
		/*if (game->assets_ready == 2 && line[i] && line[i + 1] && line[i + 2] && 
		    line[i] == '1' && line[i + 1] == '1' && line[i + 2] == '1')
		{
			map[count++] = line;
			break ;
		}*/
		get_textures(game, line);
		free(line);
	}
	flag = 0;
	while ((line = get_next_line(fd)) != NULL)
	{
		if (get_lines_of_map(line))
		{
			map[count++] = line;
			break ;
		}
		free(line);
	}
	while ((line = get_next_line(fd)) != NULL)
	{
		if (!get_lines_of_map(line))
		{
			free(line);
			break ;
		}
		map[count++] = line;
	}
	while ((line = get_next_line(fd)) != NULL)
	{
		if (get_lines_of_map(line))
		{
			free(line);
			flag = 1;
			break ;
		}
		free(line);
	}
	close(fd);
	if (flag)
		close_game(game, MAP_INVALID);
		/* null-terminate the array */
	/*its ft_calloc. Do not need I guess*/
	//map[count] = NULL;
	validade_texture(game);
//	validate_map(game);
	game->map = map;
	validate_map(game, count);
	//return ;
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

