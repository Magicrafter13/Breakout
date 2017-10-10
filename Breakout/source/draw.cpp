#include "Breakout.hpp"
#include "shapes.hpp"
#include "draw.hpp"

void draw_rect(mRectangle Rectangle_to_draw)
{
	sf2d_draw_rectangle(Rectangle_to_draw.x, Rectangle_to_draw.y, Rectangle_to_draw.width, Rectangle_to_draw.height, Rectangle_to_draw.color);
}

void draw_brick(brick brick_to_draw)
{
	draw_rect(brick_to_draw.brick_mrect);
}

void draw_circ(mCircle circle_to_draw)
{
	sf2d_draw_fill_circle(circle_to_draw.x, circle_to_draw.y, circle_to_draw.rad, circle_to_draw.color);
}

void draw_triangle(mTriangle tri_to_draw)
{
	sf2d_draw_triangle(tri_to_draw.x1, tri_to_draw.y1, tri_to_draw.x2, tri_to_draw.y2, tri_to_draw.x3, tri_to_draw.y3, tri_to_draw.color);
}

void draw_ball(ball ball_to_draw)
{
	draw_circ(ball_to_draw.ball_mcirc);
	draw_triangle(ball_to_draw.ball_mtri);
}

void draw_paddle(paddle paddle_to_draw)
{
	sf2d_draw_texture(paddle_to_draw.paddle_texture, paddle_to_draw.paddle_mrect.x, paddle_to_draw.paddle_mrect.y);
}
