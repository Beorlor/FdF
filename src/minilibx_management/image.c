#include "../../include/fdf.h"

t_mlx   *init_mlx(int width, int height, char *title)
{
    t_mlx *mlx;

    mlx = (t_mlx *)malloc(sizeof(t_mlx));
    if (!mlx)
        return (NULL);
    mlx->mlx_ptr = mlx_init();
    if (!mlx->mlx_ptr)
    {
        free(mlx);
        return (NULL);
    }
    mlx->win_ptr = mlx_new_window(mlx->mlx_ptr, width, height, title);
    if (!mlx->win_ptr)
    {
        free(mlx->mlx_ptr);
        free(mlx);
        return (NULL);
    }
    return (mlx);
}

t_img *init_img(t_mlx *mlx, int width, int height) {
    t_img *img;

    img = (t_img *)malloc(sizeof(t_img));
    if (!img)
        return (NULL);

    img->img_ptr = mlx_new_image(mlx->mlx_ptr, width, height);
    if (!img->img_ptr) {
        free(img);
        return (NULL);
    }

    img->addr = mlx_get_data_addr(img->img_ptr, &img->bpp, &img->line_length, &img->endian);
    return img;
}

void put_pixel_to_img(t_img *img, int x, int y, int color) {
    // Check if the pixel is within the window bounds
    if (x >= 0 && x < WINDOW_WIDTH && y >= 0 && y < WINDOW_HEIGHT) {
        char *dst = img->addr + (y * img->line_length + x * (img->bpp / 8));
        *(unsigned int*)dst = color;
    }
}

void render(t_mlx *mlx, t_img *img) {
    mlx_put_image_to_window(mlx->mlx_ptr, mlx->win_ptr, img->img_ptr, 0, 0);
}

