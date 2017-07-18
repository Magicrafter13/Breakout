#include "Breakout.hpp"
#include "shapes.hpp"
#include "draw.hpp"

void draw_rect(mRectangle Rectangle_to_draw)
{
	sf2d_draw_rectangle(Rectangle_to_draw.x, Rectangle_to_draw.y, Rectangle_to_draw.width, Rectangle_to_draw.height, Rectangle_to_draw.color);
}

void draw_brick(brick brick_to_draw)
{
	if (brick_to_draw.exists)
		sf2d_draw_rectangle(brick_to_draw.brick_mrect.x, brick_to_draw.brick_mrect.y, brick_to_draw.brick_mrect.width, brick_to_draw.brick_mrect.height, brick_to_draw.brick_mrect.color);
}
