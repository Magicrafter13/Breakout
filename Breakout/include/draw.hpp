#pragma once

/*
Draws an object

Must have member function getCoords(bool X), and member texture_id
*/
template <class obj>
void draw_object(obj &object) {
	pp2d_draw_texture(object.texture_id, object.x, object.y);
}
/*
Draws a ball
*/
extern void draw_object(ball &ball_to_draw);
