#include "Breakout.hpp"
#include "shapes.hpp"
#include "draw.hpp"

void draw_object(brick brick_to_draw)
{
	if (brick_to_draw.uses_texture)
		pp2d_draw_texture(brick_to_draw.texture_id, brick_to_draw.brick_mrect.x, brick_to_draw.brick_mrect.y);
}

void draw_object(ball ball_to_draw)
{
	if (ball_to_draw.uses_texture)
		pp2d_draw_texture(ball_to_draw.texture_id, ball_to_draw.getLeft(true), ball_to_draw.getTop(false));
}

void draw_object(paddle paddle_to_draw)
{
	pp2d_draw_texture(paddle_to_draw.texture_id, paddle_to_draw.paddle_mrect.x, paddle_to_draw.paddle_mrect.y);
}
