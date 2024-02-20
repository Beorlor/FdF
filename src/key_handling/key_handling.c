/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   key_handling.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jedurand <jedurand@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/20 02:00:08 by jedurand          #+#    #+#             */
/*   Updated: 2024/02/20 02:00:53 by jedurand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/fdf.h"

int	handle_key(int keycode, t_fdf *fdf)
{
	handle_movement(keycode, fdf);
	handle_scale(keycode, fdf);
	handle_rotation(keycode, fdf);
	handle_projection_mode(keycode, fdf);
	if (keycode == KEY_ESC)
	{
		exit_cleanup(fdf);
		return (0);
	}
	reinit_image(fdf);
	return (0);
}

void	handle_movement(int keycode, t_fdf *fdf)
{
	if (keycode == KEY_W)
		fdf->translate.y -= 10;
	else if (keycode == KEY_A)
		fdf->translate.x -= 10;
	else if (keycode == KEY_S)
		fdf->translate.y += 10;
	else if (keycode == KEY_D)
		fdf->translate.x += 10;
}

void	handle_scale(int keycode, t_fdf *fdf)
{
	if (keycode == KEY_Q)
		fdf->scale *= 0.9;
	else if (keycode == KEY_E)
		fdf->scale *= 1.1;
}

void	handle_rotation(int keycode, t_fdf *fdf)
{
	if (keycode == KEY_Y)
		fdf->rotation.x += 1;
	else if (keycode == KEY_H)
		fdf->rotation.x -= 1;
	else if (keycode == KEY_U)
		fdf->rotation.y += 1;
	else if (keycode == KEY_J)
		fdf->rotation.y -= 1;
	else if (keycode == KEY_I)
		fdf->rotation.z += 1;
	else if (keycode == KEY_K)
		fdf->rotation.z -= 1;
}

void	handle_projection_mode(int keycode, t_fdf *fdf)
{
	if (keycode == KEY_O)
		fdf->is_isometric = !fdf->is_isometric;
}
