#include "../../include/fdf.h"

// TODO TODO TODO
bool    parse_file(char *filename, t_point_list **list)
{
    int     fd;
    char    *line;
    char    **tokens;
    int     y = 0;
    int     x;
    int     z;

    fd = open(filename, O_RDONLY);
    if (fd < 0)
        return (false);
    while ((line = get_next_line(fd)) != NULL)
    {
		if (!is_line_valid(line))
		{
        ft_putendl_fd("Invalid line format in file.", 2);
        free(line);
		//TODO TODO TODO (free partial created list)
        close(fd);
        return (false);
    	}
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
	return (true);
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

bool is_line_valid(const char *line) {
	char **tokens = ft_split(line, ' ');
	if (tokens == NULL)
	return false;
	int i = 0;
	bool valid = true;
	while (tokens[i] != NULL) {
		if (!is_valid_integer(tokens[i])) {
			valid = false;
			break;
		}
		i++;
	}
	// Free memory allocated by ft_split
	for (int j = 0; tokens[j] != NULL; j++) {
		free(tokens[j]);
	}
	free(tokens);

	return valid;
}

// Function to check if a string is a valid integer, considering newline characters
bool is_valid_integer(const char *str) {
    if (*str == '-' || *str == '+')  // Handle negative and positive signs
        str++;
    if (*str == '\0')  // String is only a sign
        return false;
    while (*str != '\0' && *str != '\n') {
        if (!ft_isdigit(*str))  // Check if each character is a digit
            return false;
        str++;
    }
    return true;
}
