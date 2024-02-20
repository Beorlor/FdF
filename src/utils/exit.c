/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jedurand <jedurand@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/20 01:58:15 by jedurand          #+#    #+#             */
/*   Updated: 2024/02/20 23:55:57 by jedurand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../../include/fdf.h"

void	cleanup(t_fdf *fdf)
{
	if (fdf->map)
		free_point_list(&fdf->map->points);
	if (fdf->img)
	{
		mlx_destroy_image(fdf->mlx->mlx_ptr, fdf->img->img_ptr);
		free(fdf->img);
	}
	if (fdf->mlx)
	{
		mlx_destroy_display(fdf->mlx->mlx_ptr);
		free(fdf->mlx->mlx_ptr);
		free(fdf->mlx);
	}
}

int	exit_cleanup(void *param)
{
	t_fdf	*fdf;

	fdf = (t_fdf *)param;
	if (fdf->map != NULL)
	{
		free_point_list(&(fdf->map->points));
	}
	if (fdf->img != NULL)
	{
		mlx_destroy_image(fdf->mlx->mlx_ptr, fdf->img->img_ptr);
		free(fdf->img);
	}
	if (fdf->mlx->win_ptr != NULL)
	{
		mlx_destroy_window(fdf->mlx->mlx_ptr, fdf->mlx->win_ptr);
	}
	if (fdf->mlx != NULL)
	{
		mlx_destroy_display(fdf->mlx->mlx_ptr);
		free(fdf->mlx->mlx_ptr);
		free(fdf->mlx);
	}
	exit(EXIT_SUCCESS);
}

int	ft_error(char *message)
{
	write(2, message, ft_strlen(message));
	write(2, "\n", 1);
	return (EXIT_FAILURE);
}
