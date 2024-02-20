/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jedurand <jedurand@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/20 01:45:41 by jedurand          #+#    #+#             */
/*   Updated: 2024/02/20 01:52:36 by jedurand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/fdf.h"

bool	is_hex_digit(char c)
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
