#include "Breakout.hpp"
#include "shapes.hpp"
#include "draw.hpp"

void draw_object(mRectangle Rectangle_to_draw)
{
	sf2d_draw_rectangle(Rectangle_to_draw.x, Rectangle_to_draw.y, Rectangle_to_draw.width, Rectangle_to_draw.height, Rectangle_to_draw.color);
}

void draw_object(brick brick_to_draw)
{
	if (brick_to_draw.uses_texture)
		sf2d_draw_texture(brick_color_texture[brick_to_draw.texture_id], brick_to_draw.brick_mrect.x, brick_to_draw.brick_mrect.y);
	else
		draw_object(brick_to_draw.brick_mrect);
}

void draw_object(mCircle circle_to_draw)
{
	sf2d_draw_fill_circle(circle_to_draw.x, circle_to_draw.y, circle_to_draw.rad, circle_to_draw.color);
}

void draw_object(mTriangle tri_to_draw)
{
	sf2d_draw_triangle(tri_to_draw.x1, tri_to_draw.y1, tri_to_draw.x2, tri_to_draw.y2, tri_to_draw.x3, tri_to_draw.y3, tri_to_draw.color);
}

void draw_object(ball ball_to_draw)
{
	draw_object(ball_to_draw.ball_mcirc);
	draw_object(ball_to_draw.ball_mtri);
}

void draw_object(paddle paddle_to_draw)
{
	sf2d_draw_texture(paddle_to_draw.paddle_texture, paddle_to_draw.paddle_mrect.x, paddle_to_draw.paddle_mrect.y);
}
