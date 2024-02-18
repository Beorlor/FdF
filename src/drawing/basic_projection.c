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

static void	draw_line_conditions(t_fdf *fdf, int x, int y)
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

// Helper functions to find minimum and maximum of two integers
float	min(float a, float b)
{
	if (a < b)
		return (a);
	else
		return (b);
}

float	max(float a, float b)
{
	if (a > b)
		return (a);
	else
		return (b);
}

static void	init_line_drawing(t_draw *draw, t_point p0, t_point p1)
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

static int	get_line_color(t_point p0, float current_height)
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

static void	update_line_drawing(t_draw *draw)
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

static void	draw_line_loop(t_img *img, t_draw *draw, t_point p0)
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

void	bresenham_draw_line(t_img *img, t_point p0, t_point p1)
{
	t_draw	draw;

	init_line_drawing(&draw, p0, p1);
	draw_line_loop(img, &draw, p0);
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
