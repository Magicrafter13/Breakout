#pragma once

/*
Draws most any object.

Requirements,
	class must have a 'double getCoord(bool X)'
	and a 'size_t texture_id
*/
template <class generic_item>
void draw_object(generic_item obj) {
	pp2d_draw_texture(obj.texture_id, obj.getCoord(true), obj.getCoord(false);
}

/*
Draws a ball
*/
void draw_object(ball ball_to_draw);
