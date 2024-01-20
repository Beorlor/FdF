#include "../../include/fdf.h"

t_point orthogonal_project_point(t_point point3D, float scale, t_point translate) {
    t_point point2D;

    point2D.x = point3D.x * scale + translate.x;
    point2D.y = point3D.y * scale + translate.y;

    return point2D;
}

void render_grid(t_map *map, t_img *img, float scale, t_point translate, int height_threshold) {
    for (int y = 0; y < map->num_rows; y++) {
        for (int x = 0; x < map->num_cols; x++) {
            t_point current = get_point_at(map, x, y);
            t_point projected_current = orthogonal_project_point(current, scale, translate);
            int color = (current.z > height_threshold) ? 0xFF0000 : 0x0000FF;

            if (x < map->num_cols - 1) {
                t_point right = get_point_at(map, x + 1, y);
                t_point projected_right = orthogonal_project_point(right, scale, translate);
                draw_line(img, projected_current, projected_right, color);
            }

            if (y < map->num_rows - 1) {
                t_point bottom = get_point_at(map, x, y + 1);
                t_point projected_bottom = orthogonal_project_point(bottom, scale, translate);
                draw_line(img, projected_current, projected_bottom, color);
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

    return current ? current->point : (t_point){0, 0, 0};
}

// Helper functions to find minimum and maximum of two integers
int min(int a, int b) {
    return (a < b) ? a : b;
}

int max(int a, int b) {
    return (a > b) ? a : b;
}

void draw_line(t_img *img, t_point p0, t_point p1, int color) {
    // Check for horizontal line (y-coordinates are the same)
    if (p0.y == p1.y) {
        for (int x = min(p0.x, p1.x); x <= max(p0.x, p1.x); x++) {
            put_pixel_to_img(img, x, p0.y, color);
        }
    }
    // Check for vertical line (x-coordinates are the same)
    else if (p0.x == p1.x) {
        for (int y = min(p0.y, p1.y); y <= max(p0.y, p1.y); y++) {
            put_pixel_to_img(img, p0.x, y, color);
        }
    }
}
