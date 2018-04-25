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
bool test_collision(type_1 &object_1, type_2 &object_2) {
	return ((object_1.x <= (object_2.x + object_2.width)) &&
		((object_1.x + object_1.width) >= object_2.x) &&
		(object_1.y <= (object_2.y + object_2.height)) &&
		((object_1.y + object_1.height) >= object_2.y));
}

template <class type_1>
bool test_collision(ball &tBall, type_1 &obj) {
	return ((tBall.mask_x <= (obj.x + obj.width)) &&
		((tBall.mask_x + tBall.mask_width) >= obj.x) &&
		(tBall.mask_y <= (obj.y + obj.height)) &&
		((tBall.mask_y + tBall.mask_height) >= obj.y));
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
bool test_collision(ball &object_1, type_1 &object_2, bool test_horizontal) {
	double ball_x = object_1.getLeft(true) + ((object_1.width - object_1.mask_width) / 2);
	double ball_y = object_1.getTop(false) + ((object_1.height - object_1.mask_height) / 2);
	double ball_width = object_1.mask_width;
	double ball_height = object_1.mask_height;
	return ((ball_x <= (object_2.x + object_2.width)) &&
		((ball_x + ball_width) >= object_2.x) &&
		(ball_y <= (object_2.y + object_2.height)) &&
		((ball_y + ball_height) >= object_2.y));
	/*return ((object_1.x <= (object_2.x + object_2.width)) &&
		((object_1.x + object_1.width) >= object_2.x) &&
		(object_1.y <= (object_2.y + object_2.height)) &&
		((object_1.y + object_1.height) >= object_2.y));
	double line1a2 = object_1.x, line3a4 = object_1.y, line5a6 = object_1.x, line7a8 = object_1.y;
	if (test_horizontal) {
		line1a2 -= object_1.rad;
		line5a6 += object_1.rad;
	}
	else {
		line3a4 -= object_1.rad;
		line7a8 += object_1.rad;
	}
	return ((
		line1a2 >= object_2.x &&
		line1a2 <= object_2.x + object_2.width &&
		line3a4 >= object_2.y &&
		line3a4 <= object_2.y + object_2.height
		) || (
			line5a6 >= object_2.x &&
			line5a6 <= object_2.x + object_2.width &&
			line7a8 >= object_2.y &&
			line7a8 <= object_2.y + object_2.height
			));*/
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
bool off_screen(obj &object) {
	return ((object.x + object.width <= 0) ||
		(object.x >= 400) ||
		(object.y + object.height <= 0) ||
		(object.y >= 240));
}

/*
Returns true if object is offscreen

Variable Requirements:
	x <numeric value>
	y <numeric value>
	rad <numeric value>
*/
extern bool off_screen(ball &object);

#define paddleMax(the_paddle) (399 - the_paddle.width)
extern void movePaddle(bool right, paddle &the_paddle, std::vector<ball> &ball_vec);
extern void setBallDirection(ball &tBall, double speed);
extern void moveBall(ball &tBall, bool &cMode);
extern void setNewBallAngle(double &angle);
extern void setAngleGood(double &angle);
