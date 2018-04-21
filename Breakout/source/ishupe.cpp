#include "Breakout.hpp"
#include "ishupe.hpp"
#include "objects.hpp"

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

void setBallDirection(double &dirX, double &dirY, double angle, double speed) {
	dirX = speed * cos(angle * (M_PI / 180.0));
	dirY = speed * sin(angle * (M_PI / 180.0));
}

inline void moveBall(ball &tBall, double dirX, double dirY, bool cMode) {
	tBall.move(dirX / (cMode ? 100.0 : 300.0), dirY / (cMode ? 100.0 : 300.0));
}

inline void setNewBallAngle(double &angle) {
	do angle = (rand() % 120) + 30.0;
	while (angle > 80 && angle < 100);
}
