/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   drawing_utilities.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jedurand <jedurand@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/20 02:12:53 by jedurand          #+#    #+#             */
/*   Updated: 2024/02/20 02:15:09 by jedurand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/fdf.h"

// Check if the pixel is within the window bounds
void	put_pixel_to_img(t_img *img, float x, float y, int color)
{
	int		ix;
	int		iy;
	char	*dst;

	ix = (int)round(x);
	iy = (int)round(y);
	if (x >= 0 && x < WINDOW_WIDTH && y >= 0 && y < WINDOW_HEIGHT)
	{
		dst = img->addr + (iy * img->line_length + ix * (img->bpp / 8));
		*(unsigned int *)dst = color;
	}
}

t_point	get_point_at(t_map *map, float x, float y)
{
	t_point_list	*current;
	int				index;
	int				i;

	i = 0;
	current = map->points;
	index = y * map->num_cols + x;
	while (i < index && current != NULL)
	{
		current = current->next;
		i++;
	}
	if (current != NULL)
		return (current->point);
	else
		return ((t_point){0, 0, 0, 0});
}

int	determine_color(int height)
{
	if (height < 0)
		height *= -1;
	height %= 11;
	if (height > 0 && height <= 1)
		return (BLUE);
	else if (height <= 2)
		return (DOGER_BLUE);
	else if (height <= 3)
		return (CYAN);
	else if (height <= 4)
		return (GREEN);
	else if (height <= 5)
		return (LIME_GREEN);
	else if (height <= 6)
		return (YELLOW);
	else if (height <= 7)
		return (GOLD);
	else if (height <= 8)
		return (ORANGE);
	else if (height <= 9)
		return (ORANGE_RED);
	else if (height > 9)
		return (RED);
	else
		return (BLANK);
}
