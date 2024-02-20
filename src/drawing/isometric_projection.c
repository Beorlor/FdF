/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   isometric_projection.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jedurand <jedurand@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/20 02:11:58 by jedurand          #+#    #+#             */
/*   Updated: 2024/02/20 02:22:40 by jedurand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/fdf.h"

// Helper function to rotate a point around the X axis
t_point	rotate_x(t_point p, float angle)
{
	t_point	rotated;

	rotated.x = p.x;
	rotated.y = p.y * cos(angle) - p.z * sin(angle);
	rotated.z = p.y * sin(angle) + p.z * cos(angle);
	rotated.color = p.color;
	return (rotated);
}

// Helper function to rotate a point around the Y axis
t_point	rotate_y(t_point p, float angle)
{
	t_point	rotated;

	rotated.x = p.x * cos(angle) + p.z * sin(angle);
	rotated.y = p.y;
	rotated.z = -p.x * sin(angle) + p.z * cos(angle);
	rotated.color = p.color;
	return (rotated);
}

// Helper function to rotate a point around the Z axis
t_point	rotate_z(t_point p, float angle)
{
	t_point	rotated;

	rotated.x = p.x * cos(angle) - p.y * sin(angle);
	rotated.y = p.x * sin(angle) + p.y * cos(angle);
	rotated.z = p.z;
	rotated.color = p.color;
	return (rotated);
}

// Use 'next' variable for both
// 'right' and 'bottom' points to reduce variable count
void	render_iso_lines(t_fdf *fdf, int x, int y)
{
	t_point	current;
	t_point	projected;
	t_point	next;

	current = get_point_at(fdf->map, x, y);
	projected = isometric_project_point(current, fdf->scale, fdf->translate,
			fdf->rotation);
	if (x < fdf->map->num_cols - 1)
	{
		next = get_point_at(fdf->map, x + 1, y);
		next = isometric_project_point(next, fdf->scale, fdf->translate,
				fdf->rotation);
		bresenham_draw_line(fdf->img, projected, next);
	}
	if (y < fdf->map->num_rows - 1)
	{
		next = get_point_at(fdf->map, x, y + 1);
		next = isometric_project_point(next, fdf->scale, fdf->translate,
				fdf->rotation);
		bresenham_draw_line(fdf->img, projected, next);
	}
}
