#include "mlx.h"
#include <X11/keysym.h>
#include <X11/X.h>
#include <unistd.h>
#include <stdio.h>

1L >> 0

typedef struct s_data {
    void *mlx;
    void *win;
    int width;
    int height;
    int red;
    int is_drawn; // State variable to track the drawing
} t_data;

void draw_cross_and_rectangles(t_data *data, int color)
{
	int cross_length = 300;  // Longueur de la croix
	int cross_height = 50;   // Hauteur de la croix
	int rect_width = 125;    // Largeur du rectangle supplémentaire
	int rect_height = 50;    // Hauteur du rectangle supplémentaire
	data->red = color;

	// Dessiner le rectangle horizontal de la croix
	int x_start_horizontal = (data->width - cross_length) / 2;
	int y_start_horizontal = (data->height - cross_height) / 2;
	for (int x = x_start_horizontal; x < x_start_horizontal + cross_length; x++)
	{
		for (int y = y_start_horizontal; y < y_start_horizontal + cross_height; y++)
		{
			mlx_pixel_put(data->mlx, data->win, x, y, data->red);
		}
	}

	// Dessiner le rectangle vertical de la croix
	int x_start_vertical = (data->width - cross_height) / 2;
	int y_start_vertical = (data->height - cross_length) / 2;
	for (int x = x_start_vertical; x < x_start_vertical + cross_height; x++)
	{
		for (int y = y_start_vertical; y < y_start_vertical + cross_length; y++)
		{
			mlx_pixel_put(data->mlx, data->win, x, y, data->red);
		}
	}

	// Dessiner le rectangle supplémentaire en haut de la croix
	int x_start_extra_rect = x_start_vertical + cross_height;
	int y_start_extra_rect = y_start_vertical;
	for (int x = x_start_extra_rect; x < x_start_extra_rect + rect_width; x++)
	{
		for (int y = y_start_extra_rect; y < y_start_extra_rect + rect_height; y++)
		{
			mlx_pixel_put(data->mlx, data->win, x, y, data->red);
		}
	}

	// Dessiner le rectangle supplémentaire en bas de la croix
	x_start_extra_rect = x_start_vertical;
	y_start_extra_rect = y_start_vertical + cross_length - rect_height;
	for (int x = x_start_extra_rect; x > x_start_extra_rect - rect_width; x--)
	{
		for (int y = y_start_extra_rect; y < y_start_extra_rect + rect_height; y++)
		{
			mlx_pixel_put(data->mlx, data->win, x, y, data->red);
		}
	}

	// Dessiner le rectangle supplémentaire à droite de la croix
	x_start_extra_rect = x_start_horizontal + cross_length - rect_height;
	y_start_extra_rect = y_start_horizontal + rect_height;
	for (int x = x_start_extra_rect; x < x_start_extra_rect + rect_height; x++)
	{
		for (int y = y_start_extra_rect; y < y_start_extra_rect + rect_width; y++)
		{
			mlx_pixel_put(data->mlx, data->win, x, y, data->red);
		}
	}

	// Dessiner le rectangle supplémentaire à gauche de la croix
	x_start_extra_rect = x_start_horizontal;
	y_start_extra_rect = y_start_horizontal - rect_width;
	for (int x = x_start_extra_rect; x < x_start_extra_rect + rect_height; x++)
	{
		for (int y = y_start_extra_rect; y < y_start_extra_rect + rect_width; y++)
		{
			mlx_pixel_put(data->mlx, data->win, x, y, data->red);
		}
	}
}
// Fonction de callback pour les événements clavier
int key_hook(int keycode, t_data *data)
{
    if (keycode == XK_h)
    {
        if (data->is_drawn)
        {
            // Erase the drawing by redrawing in black
            draw_cross_and_rectangles(data, 0x000000);
            data->is_drawn = 0;
        }
        else
        {
            // Draw in red
            draw_cross_and_rectangles(data, 0xF06292);
            data->is_drawn = 1;
        }
    }
    return (0);
}


int main()
{
    t_data data;

    data.mlx = mlx_init(); // Initialize the library
    data.width = 800;
    data.height = 600;
    data.red = 0xF06292;
	data.is_drawn = 0;
    data.win = mlx_new_window(data.mlx, data.width, data.height, "MiniLibX Window"); // Create a window

    mlx_key_hook(data.win, key_hook, &data);

    mlx_loop(data.mlx); // Start the event loop

    return (0); // Properly close the main function
}

