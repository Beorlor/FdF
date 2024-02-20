/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jedurand <jedurand@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/20 01:45:36 by jedurand          #+#    #+#             */
/*   Updated: 2024/02/20 01:54:15 by jedurand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/fdf.h"

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

void	init_map(t_map *map)
{
	map->num_rows = 0;
	map->num_cols = -1;
}

bool	read_and_process_line(int fd, t_map *map, int *y, int *num_cols)
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

void	add_point_to_list(t_point_list **list, t_point point)
{
	t_point_list	*new_node;
	t_point_list	*temp;

	new_node = (t_point_list *)malloc(sizeof(t_point_list));
	if (!new_node)
		return ;
	new_node->point.x = point.x;
	new_node->point.y = point.y;
	new_node->point.z = point.z;
	new_node->point.color = point.color;
	new_node->next = NULL;
	if (!*list)
	{
		*list = new_node;
	}
	else
	{
		temp = *list;
		while (temp->next != NULL)
			temp = temp->next;
		temp->next = new_node;
	}
}
