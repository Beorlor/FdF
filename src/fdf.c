#include "fdf.h"

typedef struct s_mlx {
    void    *mlx_ptr;
    void    *win_ptr;
}               t_mlx;

typedef struct  s_point {
    int     x;
    int     y;
    int     z;
}               t_point;

typedef struct  s_point_list {
    t_point         point;
    struct s_point_list   *next;
}               t_point_list;

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

void    add_point_to_list(t_point_list **list, int x, int y, int z)
{
    t_point_list *new_node;
    t_point_list *temp;

    new_node = (t_point_list *)malloc(sizeof(t_point_list));
    if (!new_node)
        return ;
    new_node->point.x = x;
    new_node->point.y = y;
    new_node->point.z = z;
    new_node->next = NULL;

    if (!*list)
    {
        *list = new_node;
        return ;
    }
    temp = *list;
    while (temp->next)
        temp = temp->next;
    temp->next = new_node;
}

void    parse_file(char *filename, t_point_list **list)
{
    int     fd;
    char    *line;
    char    **tokens;
    int     y = 0;
    int     x;
    int     z;

    fd = open(filename, O_RDONLY);
    if (fd < 0)
        return ;

    while ((line = get_next_line(fd)) != NULL)
    {
        tokens = ft_split(line, ' ');
        x = 0;
        while (tokens[x] != NULL)
        {
            z = ft_atoi(tokens[x]);  // Convert the string to an int for the z value
            add_point_to_list(list, x, y, z);
            free(tokens[x]);     // Free each token after use
            x++;
        }
        free(tokens);           // Free the array of tokens
        free(line);             // Free the line from get_next_line
        y++;
    }
    close(fd);
}

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
    parse_file(argv[1], &point_list);

	print_point_list(point_list);
    // Initialize MiniLibX
    mlx = init_mlx(800, 600, "FdF");
    if (!mlx)
        return (1);

    // Add your rendering code here

    // Start the MiniLibX loop
    mlx_loop(mlx->mlx_ptr);

    // Clean up
    free(mlx);
    // Don't forget to free the linked list
    return (0);
}
