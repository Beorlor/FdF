#include "../include/fdf.h"

int main(int argc, char **argv)
{
    t_mlx   *mlx;
    t_point_list  *point_list;

    if (argc != 2)
    {
        ft_putendl_fd("Usage: ./fdf filename", 2);
        return (1);
    }

    point_list = NULL;
    if (!parse_file(argv[1], &point_list))
	{
		return (EXIT_FAILURE);
	}

	print_point_list(point_list);
    // Initialize MiniLibX
    mlx = init_mlx(800, 600, "FdF");
    if (!mlx)
        return (EXIT_FAILURE);

    // Add your rendering code here

    // Start the MiniLibX loop
    mlx_loop(mlx->mlx_ptr);

    // Clean up
    free(mlx);
    // Don't forget to free the linked list
    return (EXIT_SUCCESS);
}

void print_point_list(t_point_list *list) {
    while (list != NULL) {
        printf("Point (x: %d, y: %d, z: %d)\n", list->point.x, list->point.y, list->point.z);
        list = list->next;
    }
}

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
