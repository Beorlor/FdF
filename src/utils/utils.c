/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jedurand <jedurand@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/20 02:09:40 by jedurand          #+#    #+#             */
/*   Updated: 2024/02/20 02:48:08 by jedurand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/fdf.h"

float	min(float a, float b)
{
	if (a < b)
		return (a);
	else
		return (b);
}

float	max(float a, float b)
{
	if (a > b)
		return (a);
	else
		return (b);
}

int	calculate_scale_factor(int size, int max_size)
{
	if (size > max_size)
		return ((max_size * 100) / size);
	return (100);
}

int	calculate_initial_scale(int num_cols, int num_rows)
{
	int	scale_factor_col;
	int	scale_factor_row;
	int	scale_factor;

	scale_factor_col = calculate_scale_factor(num_cols,
			MAX_COLS_FOR_DEFAULT_SCALE);
	scale_factor_row = calculate_scale_factor(num_rows,
			MAX_ROWS_FOR_DEFAULT_SCALE);
	if (scale_factor_col < scale_factor_row)
		scale_factor = scale_factor_col;
	else
		scale_factor = scale_factor_row;
	return ((DEFAULT_SCALE * scale_factor) / 100);
}

void	reinit_image(t_fdf *fdf)
{
	if (fdf->img != NULL)
	{
		mlx_destroy_image(fdf->mlx->mlx_ptr, fdf->img->img_ptr);
		free(fdf->img);
	}
	fdf->img = init_img(fdf->mlx, 1280, 720);
	render_setup(fdf);
}
