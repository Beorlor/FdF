#include "../../include/fdf.h"

t_point orthogonal_project_point(t_point point3D, float scale, t_point translate) {
    t_point point2D;

    point2D.x = point3D.x * scale + translate.x;
    point2D.y = point3D.y * scale + translate.y;
	point2D.z = point3D.z;

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
                draw_line(img, projected_current, projected_right);
            }

            if (y < map->num_rows - 1) {
                t_point bottom = get_point_at(map, x, y + 1);
                t_point projected_bottom = orthogonal_project_point(bottom, scale, translate);
                draw_line(img, projected_current, projected_bottom);
            }
        }
    }
}

t_point get_point_at(t_map *map, int x, int y) {
    t_point_list *current = map->points;
    int index = y * map->num_cols + x;

    for (int i = 0; i < index && current != NULL; i++) {
        current = current->next;
    }

    return current ? current->point : (t_point){0, 0, 0, 0};
}

// Helper functions to find minimum and maximum of two integers
int min(int a, int b) {
    return (a < b) ? a : b;
}

int max(int a, int b) {
    return (a > b) ? a : b;
}

void draw_line(t_img *img, t_point p0, t_point p1) {
    int dx = max(p0.x, p1.x) - min(p0.x, p1.x);
    int dy = max(p0.y, p1.y) - min(p0.y, p1.y);
    int steps = max(dx, dy);
    float heightIncrement = (float)(p1.z - p0.z) / steps;
    float currentHeight = p0.z;
    int color;

    for (int i = 0; i <= steps; i++) {
        int x = p0.x + (dx ? (i * (p1.x - p0.x) / steps) : 0);
        int y = p0.y + (dy ? (i * (p1.y - p0.y) / steps) : 0);
        currentHeight += heightIncrement;

        // Use predefined color if set, otherwise calculate based on height
        if (p0.color == 0 || p0.color == -1 || p0.color > 0xFFFFFF) {
            color = determine_color((int)currentHeight);
        } else {
            color = p0.color;
        }
        put_pixel_to_img(img, x, y, color);
    }
}

int determine_color(float height) {
    // Example: Simple color calculation based on height
    // Modify this logic as per your requirement
	if (height < 0)
		height *= -1;
	if (height > 0)
		printf("height = %f\n", height);
    if (height >= 10) return 0xFF0000; // Red for high
    if (height >= 5) return 0x00FF00;  // Green for medium
    return 0x0000FF;                 // Blue for low
}
