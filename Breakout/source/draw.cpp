#include "Breakout.hpp"
#include "objects.hpp"
#include "draw.hpp"

void draw_object(ball &ball_to_draw) {
	pp2d_draw_texture(ball_to_draw.texture_id, ball_to_draw.getLeft(true), ball_to_draw.getTop(false));
}
