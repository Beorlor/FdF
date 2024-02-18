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

// Use 'next' variable for both
// 'right' and 'bottom' points to reduce variable count
static void	render_iso_lines(t_fdf *fdf, int x, int y)
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

// Updated render_iso to pass t_fdf struct
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
