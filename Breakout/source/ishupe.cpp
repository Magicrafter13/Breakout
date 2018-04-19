#include "Breakout.hpp"
#include "ishupe.hpp"
#include "shapes.hpp"

#define paddleMax(the_paddle) (399 - the_paddle.paddle_mrect.width)

bool off_screen(mCircle object) {
	if ((object.x + object.rad <= 0) ||
		(object.x - object.rad >= 400) ||
		(object.y + object.rad <= 0) ||
		(object.y - object.rad >= 240))
		return true;
	return false;
}

void movePaddle(bool right, paddle &the_paddle, bool ball_is_attached, ball &the_ball) {
	int x = (3 * (right ? (the_paddle.paddle_mrect.x < paddleMax(the_paddle) ? 1 : 0) : (the_paddle.paddle_mrect.x > 1 ? -1 : 0)));
	if (x != 0 && ball_is_attached) the_ball.move(x, 0.0);
	the_paddle.paddle_mrect.x += x;
}
