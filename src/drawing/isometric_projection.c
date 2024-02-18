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
t_point	isometric_project_point(t_point point3D, float scale, t_point translate,
		t_point rotation)
{
	float	temp_z;
	float	rad_x;
	float	rad_y;
	float	rad_z;
	t_point	point2D;

	temp_z = point3D.z;
	rad_x = rotation.x * M_PI / 180.0;
	rad_y = rotation.y * M_PI / 180.0;
	rad_z = rotation.z * M_PI / 180.0;
	point3D = rotate_x(point3D, rad_x);
	point3D = rotate_y(point3D, rad_y);
	point3D = rotate_z(point3D, rad_z);
	point3D.x *= scale;
	point3D.y *= scale;
	point3D.z *= scale;
	point3D.x += translate.x;
	point3D.y += translate.y;
	point3D.z += translate.z;
	point2D.x = (point3D.x - point3D.y) * cos(0.523599);
	point2D.y = (point3D.x + point3D.y) * sin(0.523599) - point3D.z;
	point2D.color = point3D.color;
	point2D.z = temp_z;
	return (point2D);
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
