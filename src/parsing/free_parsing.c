/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_parsing.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jedurand <jedurand@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/20 01:52:40 by jedurand          #+#    #+#             */
/*   Updated: 2024/02/20 01:55:13 by jedurand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/fdf.h"

void	free_token(char **token)
{
	free(*token);
	*token = NULL;
}

void	free_tokens(char **tokens)
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

void	free_single_point_list(t_point_list **list)
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
