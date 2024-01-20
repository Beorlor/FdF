#include "../../include/fdf.h"

void free_point_list(t_point_list **list) {
    t_point_list *temp;
    while (*list) {
        temp = (*list)->next;
        free(*list);
        *list = temp;
    }
}

bool parse_file(char *filename, t_map *map) {
    int fd;
    char *line;
    char **tokens;
    int y = 0;
    int x, z;
    int color;

    fd = open(filename, O_RDONLY);
    if (fd < 0) {
        ft_putendl_fd("File not found", 2);
        return (false);
    }

    map->num_rows = 0;
    map->num_cols = -1; // Will be updated with the number of columns on the first line

    while ((line = get_next_line(fd)) != NULL) {
        if (!is_line_valid(line)) {
            ft_putendl_fd("Invalid line format in file.", 2);
            free(line);
            free_point_list(&map->points);
            close(fd);
            return (false);
        }

        tokens = ft_split(line, ' ');
        x = 0;
        while (tokens[x] != NULL) {
            char **point_data = ft_split(tokens[x], ',');
            z = ft_atoi(point_data[0]); // Convert the string to an int for the z value

            // Check if color information is present
            color = (point_data[1] != NULL) ? strtol(point_data[1], NULL, 16) : -1;

            add_point_to_list(&map->points, x, y, z, color);

            // Free the allocated memory for point_data
            for (int j = 0; point_data[j] != NULL; j++) {
                free(point_data[j]);
            }
            free(point_data);

            free(tokens[x]); // Free each token after use
            x++;
        }

        if (map->num_cols == -1)
            map->num_cols = x;

        free(tokens); // Free the array of tokens
        free(line);   // Free the line from get_next_line
        y++;
    }

    map->num_rows = y;
    close(fd);
    return (true);
}

void add_point_to_list(t_point_list **list, int x, int y, int z, int color)
{
    t_point_list *new_node;
    t_point_list *temp;

    new_node = (t_point_list *)malloc(sizeof(t_point_list));
    if (!new_node)
        return ;
    new_node->point.x = x;
    new_node->point.y = y;
    new_node->point.z = z;
	new_node->point.color = color;
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
    if (tokens == NULL) return false;

    bool valid = true;
    for (int i = 0; tokens[i] != NULL; i++) {
        char **point_tokens = ft_split(tokens[i], ',');
        if (!point_tokens || !point_tokens[0] || !is_valid_integer(point_tokens[0])) {
            valid = false;
        } else if (point_tokens[1] && !is_valid_hex_color(point_tokens[1])) {
            valid = false;
        }
        // Free point_tokens and its contents
        for (int j = 0; point_tokens && point_tokens[j] != NULL; j++) {
            free(point_tokens[j]);
        }
        free(point_tokens);

        if (!valid) break;
    }

    // Free memory allocated by ft_split
    for (int j = 0; tokens[j] != NULL; j++) {
        free(tokens[j]);
    }
    free(tokens);

    return valid;
}

bool is_valid_hex_color(const char *str) {
    // Check if the string starts with "0x" or "0X"
    if (str[0] == '0' && (str[1] == 'x' || str[1] == 'X')) {
        str += 2; // Skip the "0x" part
    } else {
        return false; // Invalid if it doesn't start with "0x" or "0X"
    }

    // The color part should be exactly 6 characters long
    if (ft_strlen(str) != 6) return false;

    // Check if each remaining character is a valid hexadecimal digit
    while (*str) {
        if (!ft_isxdigit(*str)) return false;
        str++;
    }

    return true;
}

int ft_isxdigit(int c) {
    // Check if the character is a digit (0-9)
    if (c >= '0' && c <= '9') return 1;

    // Check if the character is a letter that's valid in hexadecimal (A-F or a-f)
    if ((c >= 'A' && c <= 'F') || (c >= 'a' && c <= 'f')) return 1;

    // If it's neither, return 0
    return 0;
}


// Function to check if a string is a valid integer, considering newline characters
bool is_valid_integer (const char *str) {
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
