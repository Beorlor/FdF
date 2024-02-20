/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   brasenham.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jedurand <jedurand@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/20 02:11:52 by jedurand          #+#    #+#             */
/*   Updated: 2024/02/20 02:13:49 by jedurand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/fdf.h"

void	bresenham_draw_line(t_img *img, t_point p0, t_point p1)
{
	t_draw	draw;

	init_line_drawing(&draw, p0, p1);
	draw_line_loop(img, &draw, p0);
}

void	init_line_drawing(t_draw *draw, t_point p0, t_point p1)
{
	draw->startx = (int)round(p0.x);
	draw->starty = (int)round(p0.y);
	draw->endx = (int)round(p1.x);
	draw->endy = (int)round(p1.y);
	draw->deltax = abs(draw->endx - draw->startx);
	draw->deltay = -abs(draw->endy - draw->starty);
	draw->stepx = -1;
	if (draw->startx < draw->endx)
		draw->stepx = 1;
	draw->stepy = -1;
	if (draw->starty < draw->endy)
		draw->stepy = 1;
	draw->error = draw->deltax + draw->deltay;
	draw->line_length = sqrt(draw->deltax * draw->deltax + draw->deltay
			* draw->deltay);
	draw->height_diff = p1.z - p0.z;
	draw->height_step = draw->height_diff / draw->line_length;
	draw->current_height = p0.z;
}

void	draw_line_loop(t_img *img, t_draw *draw, t_point p0)
{
	int	color;

	while (true)
	{
		color = get_line_color(p0, draw->current_height);
		put_pixel_to_img(img, (float)draw->startx, (float)draw->starty, color);
		if (draw->startx == draw->endx && draw->starty == draw->endy)
			break ;
		update_line_drawing(draw);
	}
}

int	get_line_color(t_point p0, float current_height)
{
	if (p0.color != -1 && p0.color != 0)
	{
		return (p0.color);
	}
	else
	{
		return (determine_color((int)current_height));
	}
}

void	update_line_drawing(t_draw *draw)
{
	int	e2;

	e2 = 2 * draw->error;
	if (e2 >= draw->deltay)
	{
		draw->error += draw->deltay;
		draw->startx += draw->stepx;
		draw->current_height += draw->height_step;
	}
	if (e2 <= draw->deltax)
	{
		draw->error += draw->deltax;
		draw->starty += draw->stepy;
		draw->current_height += draw->height_step;
	}
}
