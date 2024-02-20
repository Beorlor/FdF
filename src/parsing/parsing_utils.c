/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jedurand <jedurand@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/20 01:45:39 by jedurand          #+#    #+#             */
/*   Updated: 2024/02/20 22:08:07 by jedurand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/fdf.h"

int	open_file(char *filename)
{
	int	fd;

	fd = open(filename, O_RDONLY);
	if (fd < 0)
		ft_putendl_fd("File not found", 2);
	return (fd);
}

bool	check_file_descriptor(int fd)
{
	return (fd >= 0);
}

bool	validate_num_cols(int *num_cols, int x)
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

bool	process_tokens(char **tokens, t_map *map, int y, int *num_cols)
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
