#include "../../include/fdf.h"

void	free_point_list(t_point_list **list)
{
	t_point_list	*temp;

	while (*list)
	{
		temp = (*list)->next;
		free(*list);
		*list = temp;
	}
}

bool	parse_file(char *filename, t_map *map)
{
	int		fd;
	char	*line;
	char	**tokens;
	int		y;
	int		color;
	char	**point_data;

	y = 0;
	int x, z;
	fd = open(filename, O_RDONLY);
	if (fd < 0)
	{
		ft_putendl_fd("File not found", 2);
		return (false);
	}
	map->num_rows = 0;
	map->num_cols = -1;
	while ((line = get_next_line(fd)) != NULL)
	{
		if (!is_line_valid(line))
		{
			ft_putendl_fd("Invalid line format in file.", 2);
			free(line);
			free_point_list(&map->points);
			close(fd);
			return (false);
		}
		tokens = ft_split(line, ' ');
		x = 0;
		while (tokens[x] != NULL)
		{
			point_data = ft_split(tokens[x], ',');
			z = ft_atoi(point_data[0]);
			color = (point_data[1] != NULL) ? strtol(point_data[1], NULL, 16) : -1;
			add_point_to_list(&map->points, x, y, z, color);
			for (int j = 0; point_data[j] != NULL; j++)
			{
				free(point_data[j]);
			}
			free(point_data);
			free(tokens[x]);
			x++;
		}
		if (map->num_cols == -1)
			map->num_cols = x;
		free(tokens);
		free(line);
		y++;
	}
	map->num_rows = y;
	close(fd);
	return (true);
}

void	add_point_to_list(t_point_list **list, int x, int y, int z, int color)
{
	t_point_list	*new_node;
	t_point_list	*temp;

	new_node = (t_point_list *)malloc(sizeof(t_point_list));
	if (!new_node)
		return ;
	new_node->point.x = (float)x;
	new_node->point.y = (float)y;
	new_node->point.z = (float)z;
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

bool	is_line_valid(const char *line)
{
	char	*mutableLine;
	size_t	len;
	char	**tokens;
	bool	valid;
	char	**point_tokens;

	mutableLine = ft_strdup(line);
	if (!mutableLine)
		return (false);
	len = ft_strlen(mutableLine);
	if (len > 0 && mutableLine[len - 1] == '\n')
	{
		mutableLine[len - 1] = '\0';
	}
	tokens = ft_split(mutableLine, ' ');
	free(mutableLine);
	if (tokens == NULL)
		return (false);
	valid = true;
	for (int i = 0; tokens[i] != NULL; i++)
	{
		point_tokens = ft_split(tokens[i], ',');
		if (!point_tokens || !point_tokens[0]
			|| !is_valid_integer(point_tokens[0]))
		{
			valid = false;
		}
		else if (point_tokens[1] && !is_valid_hex_color(point_tokens[1]))
		{
			valid = false;
		}
		for (int j = 0; point_tokens && point_tokens[j] != NULL; j++)
		{
			free(point_tokens[j]);
		}
		free(point_tokens);
		if (!valid)
			break ;
	}
	for (int j = 0; tokens[j] != NULL; j++)
	{
		free(tokens[j]);
	}
	free(tokens);
	return (valid);
}

bool	is_valid_hex_color(const char *str)
{
	if (str[0] == '0' && (str[1] == 'x' || str[1] == 'X'))
	{
		str += 2;
	}
	else
	{
		return (false);
	}
	if (ft_strlen(str) != 6)
		return (false);
	while (*str)
	{
		if (!ft_isxdigit(*str))
			return (false);
		str++;
	}
	return (true);
}

int	ft_isxdigit(int c)
{
	if (c >= '0' && c <= '9')
		return (1);
	if ((c >= 'A' && c <= 'F') || (c >= 'a' && c <= 'f'))
		return (1);
	return (0);
}

// Function to check if a string is a valid integer,
// considering newline characters
bool	is_valid_integer(const char *str)
{
	if (*str == '-' || *str == '+')
		str++;
	if (*str == '\0')
		return (false);
	while (*str != '\0' && *str != '\n')
	{
		if (!ft_isdigit(*str))
			return (false);
		str++;
	}
	return (true);
}
