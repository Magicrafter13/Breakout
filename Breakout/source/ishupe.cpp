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

void movePaddle(bool right, paddle &the_paddle, std::vector<ball> &ball_vec) {
	int x = (3 * (right ? (the_paddle.paddle_mrect.x < paddleMax(the_paddle) ? 1 : 0) : (the_paddle.paddle_mrect.x > 1 ? -1 : 0)));
	for (auto &tBall : ball_vec)
		if (x != 0 && tBall.is_attached) tBall.move(x, 0.0);
	the_paddle.paddle_mrect.x += x;
}

void setBallDirection(ball &tBall, double speed) {
	tBall.dx = speed * cos(tBall.angle * (M_PI / 180.0));
	tBall.dy = speed * sin(tBall.angle * (M_PI / 180.0));
}

void moveBall(ball &tBall, bool cMode) {
	tBall.move(tBall.dx / (cMode ? 100.0 : 300.0), tBall.dy / (cMode ? 100.0 : 300.0));
}

void setNewBallAngle(double &angle) {
	do angle = (rand() % 90) + 225.0;
	while (angle > 265 && angle < 275);
}

void ensureGoodAngle(double &angle) {
	while (angle < 0.0) angle += 360.0;
	while (angle > 360.0) angle -= 360.0;
}
