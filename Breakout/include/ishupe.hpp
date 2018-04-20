#include "Breakout.hpp"
#include "objects.hpp"

/*
Returns true if two objects are colliding

Variable Requirements:
	x <numeric value>
	y <numeric value>
	width <numeric value>
	height <numeric value>
*/
template <class type_1, class type_2>
bool test_collision(type_1 object_1, type_2 object_2) {
	if (
		(object_1.x <= (object_2.x + object_2.width)) &&
		((object_1.x + object_1.width) >= object_2.x) &&
		(object_1.y <= (object_2.y + object_2.height)) &&
		((object_1.y + object_1.height) >= object_2.y)
		)
		return true;
	return false;
}

/*
Returns true if two objects are colliding

bool:
	true: horizontal collision
	false: vertical collision

Variable Requirements:
	x <numeric value>
	y <numeric value>
	mCircle:
		rad <numeric value>
	type_1
		width <numeric value>
		height <numeric value>
*/
template <class type_1>
bool test_collision(mCircle circle_object, type_1 object_2, bool test_horizontal) {
	double line1a2 = circle_object.x, line3a4 = circle_object.y, line5a6 = circle_object.x, line7a8 = circle_object.y;
	if (test_horizontal) {
		line1a2 -= circle_object.rad;
		line5a6 += circle_object.rad;
	}
	if (!test_horizontal) {
		line3a4 -= circle_object.rad;
		line7a8 += circle_object.rad;
	}
	if ((
		line1a2 >= object_2.x &&
		line1a2 <= object_2.x + object_2.width &&
		line3a4 >= object_2.y &&
		line3a4 <= object_2.y + object_2.height
		) || (
			line5a6 >= object_2.x &&
			line5a6 <= object_2.x + object_2.width &&
			line7a8 >= object_2.y &&
			line7a8 <= object_2.y + object_2.height
			))
		return true;
	return false;
}

/*
Returns true if object is offscreen

Variable Requirements:
	x <numeric value>
	y <numeric value>
	width <numeric value>
	height <numeric value>
*/
template <class obj>
bool off_screen(obj object) {
	if ((object.x + object.width <= 0) ||
		(object.x >= 400) ||
		(object.y + object.height <= 0) ||
		(object.y >= 240))
		return true;
	return false;
}

/*
Returns true if object is offscreen

Variable Requirements:
	x <numeric value>
	y <numeric value>
	rad <numeric value>
*/
extern bool off_screen(mCircle object);

#define paddleMax(the_paddle) (399 - the_paddle.paddle_mrect.width)
extern void movePaddle(bool right, paddle &the_paddle, bool ball_is_attached, ball &the_ball);
