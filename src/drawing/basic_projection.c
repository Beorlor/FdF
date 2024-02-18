#include "../../include/fdf.h"

t_point	orthogonal_project_point(t_point point3D, float scale,
		t_point translate)
{
	t_point	point2D;

	point2D.x = point3D.x * scale + translate.x;
	point2D.y = point3D.y * scale + translate.y;
	point2D.z = point3D.z;
	point2D.color = point3D.color;
	return (point2D);
}

void	render_grid(t_map *map, t_img *img, float scale, t_point translate)
{
	int		y;
	int		x;
	t_point	current;
	t_point	projected_current;
	t_point	right;
	t_point	projected_right;
	t_point	bottom;
	t_point	projected_bottom;

	y = 0;
	while (y < map->num_rows)
	{
		x = 0;
		while (x < map->num_cols)
		{
			current = get_point_at(map, x, y);
			projected_current = orthogonal_project_point(current, scale,
					translate);
			if (x < map->num_cols - 1)
			{
				right = get_point_at(map, x + 1, y);
				projected_right = orthogonal_project_point(right, scale,
						translate);
				bresenham_draw_line(img, projected_current, projected_right);
			}
			if (y < map->num_rows - 1)
			{
				bottom = get_point_at(map, x, y + 1);
				projected_bottom = orthogonal_project_point(bottom, scale,
						translate);
				bresenham_draw_line(img, projected_current, projected_bottom);
			}
			x++;
		}
		y++;
	}
}

t_point	get_point_at(t_map *map, float x, float y)
{
	t_point_list	*current;
	int				index;

	current = map->points;
	index = y * map->num_cols + x;
	for (int i = 0; i < index && current != NULL; i++)
	{
		current = current->next;
	}
	return (current ? current->point : (t_point){0, 0, 0, 0});
}

// Helper functions to find minimum and maximum of two integers
float	min(float a, float b)
{
	return ((a < b) ? a : b);
}

float	max(float a, float b)
{
	return ((a > b) ? a : b);
}

void	bresenham_draw_line(t_img *img, t_point p0, t_point p1)
{
	int		x0;
	int		y0;
	int		x1;
	int		y1;
	int		dx;
	int		dy;
	int		err;
	float	totalDistance;
	float	heightDiff;
	float	heightStep;
	float	currentHeight;
	int color;
	int e2;
	int sx;
	int sy;

	x0 = (int)round(p0.x);
	y0 = (int)round(p0.y);
	x1 = (int)round(p1.x);
	y1 = (int)round(p1.y);
	dx = abs(x1 - x0);
	dy = -abs(y1 - y0);
	sx = x0 < x1 ? 1 : -1;
	sy = y0 < y1 ? 1 : -1;
	err = dx + dy;
	totalDistance = sqrt(dx * dx + dy * dy);
	heightDiff = p1.z - p0.z;
	heightStep = heightDiff / totalDistance;
	currentHeight = p0.z;
	while (true)
	{
		if (p0.color != -1 && p0.color != 0)
		{
			color = p0.color;
		}
		else
		{
			color = determine_color((int)currentHeight);
		}
		put_pixel_to_img(img, (float)x0, (float)y0, color);
		if (x0 == x1 && y0 == y1)
			break ;
		e2 = 2 * err;
		if (e2 >= dy)
		{
			err += dy;
			x0 += sx;
			currentHeight += heightStep;
		}
		if (e2 <= dx)
		{
			err += dx;
			y0 += sy;
			currentHeight += heightStep;
		}
	}
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
