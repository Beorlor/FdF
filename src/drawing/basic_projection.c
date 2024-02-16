#include "../../include/fdf.h"

t_point orthogonal_project_point(t_point point3D, float scale, t_point translate) {
    t_point point2D;

    point2D.x = point3D.x * scale + translate.x;
    point2D.y = point3D.y * scale + translate.y;
	point2D.z = point3D.z;
	point2D.color = point3D.color;

    return point2D;
}

void render_grid(t_map *map, t_img *img, float scale, t_point translate) {
    for (int y = 0; y < map->num_rows; y++) {
        for (int x = 0; x < map->num_cols; x++) {
            t_point current = get_point_at(map, x, y);
            t_point projected_current = orthogonal_project_point(current, scale, translate);

            if (x < map->num_cols - 1) {
                t_point right = get_point_at(map, x + 1, y);
                t_point projected_right = orthogonal_project_point(right, scale, translate);
                bresenham_draw_line(img, projected_current, projected_right);
            }

            if (y < map->num_rows - 1) {
                t_point bottom = get_point_at(map, x, y + 1);
                t_point projected_bottom = orthogonal_project_point(bottom, scale, translate);
                bresenham_draw_line(img, projected_current, projected_bottom);
            }
        }
    }
}

t_point get_point_at(t_map *map, float x, float y) {
    t_point_list *current = map->points;
    int index = y * map->num_cols + x;

    for (int i = 0; i < index && current != NULL; i++) {
        current = current->next;
    }

    return current ? current->point : (t_point){0, 0, 0, 0};
}

// Helper functions to find minimum and maximum of two integers
float min(float a, float b) {
    return (a < b) ? a : b;
}

float max(float a, float b) {
    return (a > b) ? a : b;
}

void bresenham_draw_line(t_img *img, t_point p0, t_point p1) {
    int x0 = (int)round(p0.x);
    int y0 = (int)round(p0.y);
    int x1 = (int)round(p1.x);
    int y1 = (int)round(p1.y);

    int dx = abs(x1 - x0), sx = x0 < x1 ? 1 : -1;
    int dy = -abs(y1 - y0), sy = y0 < y1 ? 1 : -1;
    int err = dx + dy, e2;

    float totalDistance = sqrt(dx * dx + dy * dy);
    float heightDiff = p1.z - p0.z;
    float heightStep = heightDiff / totalDistance;
    float currentHeight = p0.z;

    while (true) {
        int color;
        if (p0.color != -1 && p0.color != 0) {
            color = p0.color;
        } else {
            color = determine_color((int)currentHeight);
        }
        put_pixel_to_img(img, (float)x0, (float)y0, color);

        if (x0 == x1 && y0 == y1) break;
        e2 = 2 * err;
        if (e2 >= dy) { err += dy; x0 += sx; currentHeight += heightStep; }
        if (e2 <= dx) { err += dx; y0 += sy; currentHeight += heightStep; }
    }
}

int determine_color(int height)
{
	if (height < 0)
		height *= -1;

	int normalizedHeight = height;
    //int normalizedHeight = height % 11;

    if (normalizedHeight > 0 && normalizedHeight <= 1) return 0x0000FF; // Blue
    else if (normalizedHeight <= 2) return 0x1E90FF; // Dodger Blue
    else if (normalizedHeight <= 3) return 0x00FFFF; // Cyan
    else if (normalizedHeight <= 4) return 0x00FA9A; // Medium Spring Green
    else if (normalizedHeight <= 5) return 0x32CD32; // Lime Green
    else if (normalizedHeight <= 6) return 0xFFFF00; // Yellow
    else if (normalizedHeight <= 7) return 0xFFD700; // Gold
    else if (normalizedHeight <= 8) return 0xFFA500; // Orange
    else if (normalizedHeight <= 9) return 0xFF4500; // Orange Red
    else if (normalizedHeight > 9) return 0xFF0000; // Red
    else return 0xFFFFFF; // Default to blue for any unexpected value
}

