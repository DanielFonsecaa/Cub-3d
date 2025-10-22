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
		game->north.path = ft_strtrim(line + i + 2, WS);
	if (ft_strncmp("SO", line + i, 2) == 0)
		game->south.path = ft_strtrim(line + i + 2, WS);
	if (ft_strncmp("WE", line + i, 2) == 0)
		game->west.path = ft_strtrim(line + i + 2, WS);
	if (ft_strncmp("EA", line + i, 2) == 0)
		game->east.path = ft_strtrim(line + i + 2, WS);
}

void	validade_texture(t_game *game)
{
	if (!game->canvas || !game->canvas->mlx)
	{
		ft_printf("error: canvas or mlx not initialized\n");
		close_game(game, INVALID_TEXTURE);
	}

	if (!game->north.path) close_game(game, NORTH_TEXT_ERR);
	if (!game->south.path) close_game(game, SOUTH_TEXT_ERR);
	if (!game->east.path)  close_game(game, EAST_TEXT_ERR);
	if (!game->west.path)  close_game(game, WEST_TEXT_ERR);

	game->north.img = mlx_xpm_file_to_image(game->canvas->mlx, game->north.path, &game->north.width, &game->north.height);
	if (!game->north.img) { ft_printf("mlx failed to load north: %s (errno=%s)\n", game->north.path, strerror(errno)); close_game(game, INVALID_TEXTURE); }
	game->north.data = mlx_get_data_addr(game->north.img, &game->north.bpp, &game->north.size_line, &game->north.endian);

	game->south.img = mlx_xpm_file_to_image(game->canvas->mlx, game->south.path, &game->south.width, &game->south.height);
	if (!game->south.img) { ft_printf("mlx failed to load south: %s (errno=%s)\n", game->south.path, strerror(errno)); close_game(game, INVALID_TEXTURE); }
	game->south.data = mlx_get_data_addr(game->south.img, &game->south.bpp, &game->south.size_line, &game->south.endian);

	game->west.img = mlx_xpm_file_to_image(game->canvas->mlx, game->west.path, &game->west.width, &game->west.height);
	if (!game->west.img) { ft_printf("mlx failed to load west: %s (errno=%s)\n", game->west.path, strerror(errno)); close_game(game, INVALID_TEXTURE); }
	game->west.data = mlx_get_data_addr(game->west.img, &game->west.bpp, &game->west.size_line, &game->west.endian);

	game->east.img = mlx_xpm_file_to_image(game->canvas->mlx, game->east.path, &game->east.width, &game->east.height);
	if (!game->east.img) { ft_printf("mlx failed to load east: %s (errno=%s)\n", game->east.path, strerror(errno)); close_game(game, INVALID_TEXTURE); }
	game->east.data = mlx_get_data_addr(game->east.img, &game->east.bpp, &game->east.size_line, &game->east.endian);

	if (!game->canvas->img)
		close_game(game, INVALID_TEXTURE);
	game->data = mlx_get_data_addr(game->canvas->img, &game->bpp, &game->size_line, &game->endian);
	if (!game->data)
		close_game(game, INVALID_TEXTURE);
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
		set_background_color(game, &game->floor, i + 1, line);
	else if (ft_strncmp("C", line + i, 1) == 0)
		set_background_color(game, &game->cealing, i + 1, line);
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

int get_lines_of_map(char *line, int *flag)
{
	int i;

	if (!line)
		return (0);
	i = 0;
	while (ft_iswhite_space(line[i]))
		i++;

	/* empty/whitespace-only line */
	if (line[i] == '\0')
	{
		/* if map not started yet, skip; if started, treat as end-of-map -> stop adding */
		return (*flag ? 0 : 0);
	}

	/* first non-space char must be '1' or '0' for a valid map row;
	   require map to start with a '1' row (outer wall) */
	if (!*flag)
	{
		if (line[i] != '1')
			return (0); /* skip header/texture lines until map start */
		*flag = 1;
		return (1);
	}

	/* after map started accept lines that look like map rows ('0' or '1' or other map tokens) */
	if (line[i] != '1' && line[i] != '0' && line[i] != ' ')
		return (0); /* ignore malformed/extra lines */

	return (1);
}

char	**get_map(t_game *game, char *map_name)
{
	int		fd;
	char	*line;
	char	**map;
	size_t	count;
	int     map_lines;
	int		flag;

	count = 0;
	flag = 0;
	map_lines = get_map_lines(map_name);
	if (map_lines <= 0)
		return (NULL);
	/* allocate map_lines + 1 for the NULL terminator */
	map = ft_calloc(map_lines + 1, sizeof(char *));
	if (!map)
		return (NULL);

	fd = open(map_name, O_RDONLY);
	if (fd < 0)
	{
		free(map);
		return (NULL);
	}
	while ((line = get_next_line(fd)) != NULL)
	{
		get_textures(game, line);
		free(line);
	}
	close(fd);

	fd = open(map_name, O_RDONLY);
	if (fd < 0)
	{
		/* cleanup any allocated strings if necessary */
		free(map);
		return (NULL);
	}
	while ((line = get_next_line(fd)) != NULL)
	{
		if (!get_lines_of_map(line, &flag))
		{
			free(line);
			continue ;
		}
		map[count++] = line;
	}
	close(fd);
	/* null-terminate the array */
	map[count] = NULL;
	validade_texture(game);
	return (map);
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

