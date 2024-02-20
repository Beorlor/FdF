/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   projections_and_rendering.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jedurand <jedurand@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/20 02:11:55 by jedurand          #+#    #+#             */
/*   Updated: 2024/02/20 02:13:14 by jedurand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/fdf.h"

t_point	orthogonal_project_point(t_point point3d, float scale,
		t_point translate)
{
	t_point	point2d;

	point2d.x = point3d.x * scale + translate.x;
	point2d.y = point3d.y * scale + translate.y;
	point2d.z = point3d.z;
	point2d.color = point3d.color;
	return (point2d);
}

// Main isometric projection function
// Convert rotation angles from degrees to radians
// Rotate the point
// Scale the point
// Translate the point
// Project the point isometrically
// Copy color data
t_point	isometric_project_point(t_point point3d, float scale, t_point translate,
		t_point rotation)
{
	float	temp_z;
	float	rad_x;
	float	rad_y;
	float	rad_z;
	t_point	point2d;

	temp_z = point3d.z;
	rad_x = rotation.x * M_PI / 180.0;
	rad_y = rotation.y * M_PI / 180.0;
	rad_z = rotation.z * M_PI / 180.0;
	point3d = rotate_x(point3d, rad_x);
	point3d = rotate_y(point3d, rad_y);
	point3d = rotate_z(point3d, rad_z);
	point3d.x *= scale;
	point3d.y *= scale;
	point3d.z *= scale;
	point3d.x += translate.x;
	point3d.y += translate.y;
	point3d.z += translate.z;
	point2d.x = (point3d.x - point3d.y) * cos(0.523599);
	point2d.y = (point3d.x + point3d.y) * sin(0.523599) - point3d.z;
	point2d.color = point3d.color;
	point2d.z = temp_z;
	return (point2d);
}

void	render_grid(t_fdf *fdf)
{
	int	y;
	int	x;

	y = 0;
	while (y < fdf->map->num_rows)
	{
		x = 0;
		while (x < fdf->map->num_cols)
		{
			draw_line_conditions(fdf, x, y);
			x++;
		}
		y++;
	}
}

void	render_iso(t_fdf *fdf)
{
	int	y;
	int	x;

	y = 0;
	while (y < fdf->map->num_rows)
	{
		x = 0;
		while (x < fdf->map->num_cols)
		{
			render_iso_lines(fdf, x, y);
			x++;
		}
		y++;
	}
}

void	draw_line_conditions(t_fdf *fdf, int x, int y)
{
	t_point	current;
	t_point	projected;
	t_point	next;

	current = get_point_at(fdf->map, x, y);
	projected = orthogonal_project_point(current, fdf->scale, fdf->translate);
	if (x < fdf->map->num_cols - 1)
	{
		next = get_point_at(fdf->map, x + 1, y);
		next = orthogonal_project_point(next, fdf->scale, fdf->translate);
		bresenham_draw_line(fdf->img, projected, next);
	}
	if (y < fdf->map->num_rows - 1)
	{
		next = get_point_at(fdf->map, x, y + 1);
		next = orthogonal_project_point(next, fdf->scale, fdf->translate);
		bresenham_draw_line(fdf->img, projected, next);
	}
}
