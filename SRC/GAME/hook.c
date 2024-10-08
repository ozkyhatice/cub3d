#include "../../INC/cub3d.h"

void print_info(t_game *game) // Debugging
{
	printf("player       x %f\n", game->player->x);
	printf("player       y %f\n", game->player->y);
	printf("player   dir_x %f\n", game->player->dir_x);
	printf("player   dir_y %f\n", game->player->dir_y);
	printf("player plane_x %f\n", game->player->plane_x);
	printf("player plane_y %f\n", game->player->plane_y);
	printf("player   angle %f\n", game->player->angle);
	printf("\n");
	printf("raycast camera_x %f\n", game->raycast->camera_x);
	printf("perp_dist %f\n", game->raycast->perp_dist);
	printf("start_y   %d\n", game->raycast->start_y);
	printf("end_y     %d\n", game->raycast->end_y);
	printf("height    %d\n", game->raycast->height);
	printf("tex_x     %d\n", game->raycast->tex_x);
	printf("tex_y     %d\n", game->raycast->tex_y);
	printf("\n\n");
	printf("--------------------------\n");
}

int	exit_game(t_game *game)
{
	if (game->win)
		mlx_destroy_window(game->mlx, game->win);
	exit(0);
	return (0);
}

void move_ws(t_game *game, int direction)
{
	double	des_x;
	double	des_y;
  
	des_x = game->player->x + game->player->dir_x * (double)2 / 10 * direction;
	des_y = game->player->y + game->player->dir_y * (double)2 / 10 * direction;
	if (game->map[(int)game->player->y][(int)des_x] == '0')
		game->player->x = des_x;
	if (game->map[(int)des_y][(int)game->player->x] == '0')
		game->player->y = des_y;
}

void move_ad(t_game *game, int direction)
{
	double	des_x;
	double	des_y;

	des_x = game->player->x + game->player->plane_x * (double)2 / 10 * direction;
	des_y = game->player->y + game->player->plane_y * (double)2 / 10 * direction;
	if (game->map[(int)game->player->y][(int)des_x] == '0')
		game->player->x = des_x;
	if (game->map[(int)des_y][(int)game->player->x] == '0')
		game->player->y = des_y;
}

void rotate_player(t_game *game, int direction)
{
	double old_dirx;
	double old_planex;
	double rotate_angle;

	rotate_angle = 0.05 * direction;

	old_dirx = game->player->dir_x;
	game->player->dir_x = game->player->dir_x * cos(rotate_angle) - game->player->dir_y * sin(rotate_angle);
	game->player->dir_y = old_dirx * sin(rotate_angle) + game->player->dir_y * cos(rotate_angle);

	old_planex = game->player->plane_x;
	game->player->plane_x = game->player->plane_x * cos(rotate_angle) - game->player->plane_y * sin(rotate_angle);
	game->player->plane_y = old_planex * sin(rotate_angle) + game->player->plane_y * cos(rotate_angle);
}


void player_game(t_game *game)
{
	if (game->key->w)
		move_ws(game, 1);
	if (game->key->s)
		move_ws(game, -1);
	if (game->key->a)
		move_ad(game, -1);
	if (game->key->d)
		move_ad(game, 1);
	if (game->key->right)
		rotate_player(game, 1);
	if (game->key->left)
		rotate_player(game, -1);
}

int game_hook(void *param)
{
	int	x;
	t_game *game = (t_game *)param;

	x = 0;
	draw_floor_ceiling(game);
	player_game(game);
	
		print_info(game); // Debugging
	
	while (x < SCREENWIDTH)
	{
		calc_ray(game, x);
		dda(game);
		calc_wall(game);
		map_line(game, x);
		x++;
	}
	mlx_put_image_to_window(game->mlx, game->win, game->img_ptr, 0, 0);
	return (0);
}