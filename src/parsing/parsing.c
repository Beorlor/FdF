#include "../../include/fdf.h"

static void	free_single_point_list(t_point_list **list)
{
	free(*list);
	*list = NULL;
}

void	free_point_list(t_point_list **list)
{
	t_point_list	*temp;

	while (*list)
	{
		temp = (*list)->next;
		free_single_point_list(list);
		*list = temp;
	}
}

static void	free_token(char **token)
{
	free(*token);
	*token = NULL;
}

static void	free_tokens(char **tokens)
{
	int	i;

	i = 0;
	while (tokens[i] != NULL)
	{
		free_token(&tokens[i]);
		i++;
	}
	free(tokens);
}

static int	open_file(char *filename)
{
	int	fd;

	fd = open(filename, O_RDONLY);
	if (fd < 0)
		ft_putendl_fd("File not found", 2);
	return (fd);
}

static bool	check_file_descriptor(int fd)
{
	return (fd >= 0);
}

static void	init_map(t_map *map)
{
	map->num_rows = 0;
	map->num_cols = -1;
}

static bool	validate_num_cols(int *num_cols, int x)
{
	if (*num_cols == -1)
		*num_cols = x;
	else if (*num_cols != x)
	{
		ft_putendl_fd("Error: Inconsistent number of columns.", 2);
		return (false);
	}
	return (true);
}

static bool	add_point(char **point_data, t_map *map, int x, int y)
{
	int	z;
	int	color;

	z = ft_atoi(point_data[0]);
	if (point_data[1])
		color = strtol(point_data[1], NULL, 16);
	else
		color = -1;
	add_point_to_list(&map->points, x, y, z, color);
	return (true);
}

static bool	process_tokens(char **tokens, t_map *map, int y, int *num_cols)
{
	int		x;
	char	**point_data;

	x = 0;
	while (tokens[x] != NULL)
	{
		point_data = ft_split(tokens[x], ',');
		if (!point_data)
			return (false);
		if (!add_point(point_data, map, x, y))
			return (false);
		free_tokens(point_data);
		x++;
	}
	return (validate_num_cols(num_cols, x));
}

static bool	read_and_process_line(int fd, t_map *map, int *y, int *num_cols)
{
	char	*line;
	char	**tokens;
	bool	success;

	line = get_next_line(fd);
	while (line != NULL)
	{
		tokens = ft_split(line, ' ');
		if (!tokens)
			return (false);
		success = process_tokens(tokens, map, *y, num_cols);
		free_tokens(tokens);
		free(line);
		if (!success)
			return (false);
		(*y)++;
		line = get_next_line(fd);
	}
	return (true);
}

bool	parse_file(char *filename, t_map *map)
{
	int		fd;
	int		y;
	bool	success;

	y = 0;
	fd = open_file(filename);
	if (!check_file_descriptor(fd))
		return (false);
	init_map(map);
	success = read_and_process_line(fd, map, &y, &map->num_cols);
	close(fd);
	if (!success)
	{
		free_point_list(&map->points);
		return (false);
	}
	map->num_rows = y;
	return (true);
}

void	add_point_to_list(t_point_list **list, int x, int y, int z, int color)
{
	t_point_list	*new_node;
	t_point_list	*temp;

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
	while (temp->next != NULL)
		temp = temp->next;
	temp->next = new_node;
}

static bool	is_hex_digit(char c)
{
	return ((c >= '0' && c <= '9') || (c >= 'A' && c <= 'F') || (c >= 'a'
			&& c <= 'f'));
}

bool	is_valid_hex_color(const char *str)
{
	if (str[0] != '0' || (str[1] != 'x' && str[1] != 'X')
		|| ft_strlen(str) != 8)
		return (false);
	str += 2;
	while (*str)
	{
		if (!is_hex_digit(*str))
			return (false);
		str++;
	}
	return (true);
}

bool	is_valid_integer(const char *str)
{
	if (*str == '-' || *str == '+')
		str++;
	if (*str == '\0')
		return (false);
	while (*str)
	{
		if (!ft_isdigit(*str))
			return (false);
		str++;
	}
	return (true);
}

bool	is_line_valid(const char *line)
{
	char	*mutable_line;
	char	**tokens;
	bool	valid;
	int		i;
	char	**point_tokens;

	valid = true;
	i = -1;
	mutable_line = ft_strdup(line);
	if (!mutable_line)
		return (false);
	tokens = ft_split(mutable_line, ' ');
	free(mutable_line);
	if (!tokens)
		return (false);
	while (valid && tokens[++i])
	{
		point_tokens = ft_split(tokens[i], ',');
		valid = point_tokens && point_tokens[0]
			&& is_valid_integer(point_tokens[0]) && (!point_tokens[1]
				|| is_valid_hex_color(point_tokens[1]));
		free_tokens(point_tokens);
	}
	free_tokens(tokens);
	return (valid);
}
