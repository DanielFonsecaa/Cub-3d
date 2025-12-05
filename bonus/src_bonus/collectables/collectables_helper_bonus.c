/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   collectables_helper_bonus.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mipinhei <mipinhei@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/05 15:59:47 by mipinhei          #+#    #+#             */
/*   Updated: 2025/12/05 15:59:48 by mipinhei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes_bonus/cub_bonus.h"

int	in_bounds_tile(t_game *g, int tx, int ty)
{
	size_t	len;

	if (ty < 0 || ty >= g->grid.height)
		return (0);
	len = ft_strlen(g->grid.map[ty]);
	if (tx < 0 || (size_t)tx >= len)
		return (0);
	return (1);
}

int	load_texture(t_game *game, t_tex *tex, const char *path)
{
	int	w;
	int	h;

	if (!game || !tex || !path)
		return (0);
	tex->img = mlx_xpm_file_to_image(game->canvas.mlx, (char *)path, &w, &h);
	if (!tex->img)
		return (0);
	tex->data = mlx_get_data_addr(tex->img, &tex->bpp,
			&tex->size_line, &tex->endian);
	tex->width = w;
	tex->height = h;
	tex->path = ft_strdup(path);
	if (!tex->path)
		return (0);
	return (1);
}

int	load_textures(t_game *game, t_tex *arr, int n, const char **paths)
{
	int	i;

	if (!game || !arr || !paths || n <= 0)
		return (0);
	i = 0;
	while (i < n)
	{
		if (!load_texture(game, &arr[i], paths[i]))
		{
			while (--i >= 0)
			{
				if (arr[i].img)
					mlx_destroy_image(game->canvas.mlx, arr[i].img);
				free(arr[i].path);
				arr[i].img = NULL;
				arr[i].path = NULL;
			}
			return (0);
		}
		i++;
	}
	return (1);
}

int	sprite_cmp(const void *a, const void *b)
{
	const t_sprite	*sa = (const t_sprite *)a;
	const t_sprite	*sb = (const t_sprite *)b;

	if (sa->trans_y < sb->trans_y)
		return (1);
	if (sa->trans_y > sb->trans_y)
		return (-1);
	return (0);
}

int	tex_read_color(t_tex *tt, int tx, int ty)
{
	int				idx;
	unsigned char	*d;
	int				color;

	if (!tt || !tt->data)
		return (0x000000);
	if (tx < 0)
		tx = 0;
	if (tx >= tt->width)
		tx = tt->width - 1;
	if (ty < 0)
		ty = 0;
	if (ty >= tt->height)
		ty = tt->height - 1;
	idx = ty * tt->size_line + tx * (tt->bpp / 8);
	d = (unsigned char *)tt->data + idx;
	color = 0x000000;
	if (tt->bpp == 32)
		color = *(int *)d;
	else if (tt->bpp == 24)
		color = d[0] | (d[1] << 8) | (d[2] << 16);
	return (color);
}
