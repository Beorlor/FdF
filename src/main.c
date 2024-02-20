/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jedurand <jedurand@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/20 02:03:18 by jedurand          #+#    #+#             */
/*   Updated: 2024/02/20 23:57:54 by jedurand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/fdf.h"

int	main(int argc, char **argv)
{
	t_mlx	*mlx;
	t_img	*img;
	t_map	map;
	t_fdf	fdf;

	if (argc != 2)
		return (ft_error("Usage: ./fdf filename"));
	init_map(&map);
	if (!parse_file(argv[1], &map))
		return (EXIT_FAILURE);
	mlx = init_mlx(1280, 720, "FdF");
	if (!mlx)
		return (cleanup(&fdf), EXIT_FAILURE);
	img = init_img(mlx, 1280, 720);
	if (!img)
		return (cleanup(&fdf), EXIT_FAILURE);
	init_fdf(&fdf, &map, mlx, img);
	render_setup(&fdf);
	setup_hooks(mlx, &fdf);
	cleanup(&fdf);
	return (EXIT_SUCCESS);
}

void	init_map(t_map *map)
{
	if (!map)
		return ;
	map->points = NULL;
	map->num_rows = 0;
	map->num_cols = 0;
}

void	init_fdf(t_fdf *fdf, t_map *map, t_mlx *mlx, t_img *img)
{
	fdf->map = map;
	fdf->mlx = mlx;
	fdf->img = img;
	fdf->scale = calculate_initial_scale(map->num_cols, map->num_rows);
	fdf->translate.x = 500;
	fdf->translate.y = 150;
	fdf->translate.z = 0;
	fdf->rotation.x = 0;
	fdf->rotation.y = 0;
	fdf->rotation.z = 0;
	fdf->is_isometric = true;
}

void	render_setup(t_fdf *fdf)
{
	if (fdf->is_isometric)
		render_iso(fdf);
	else
		render_grid(fdf);
	render(fdf->mlx, fdf->img);
}

void	setup_hooks(t_mlx *mlx, t_fdf *fdf)
{
	mlx_key_hook(mlx->win_ptr, handle_key, fdf);
	mlx_hook(mlx->win_ptr, 17, 0, exit_cleanup, fdf);
	mlx_loop(mlx->mlx_ptr);
}
