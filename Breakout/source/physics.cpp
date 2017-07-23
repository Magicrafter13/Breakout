#include "Breakout.hpp"
#include "shapes.hpp"
#include "draw.hpp"
#include "physics.hpp"

bool movedHor = false;
bool movedVer = false;

void ballPhysics(ball BALL, paddle PADDLE, double paddle_dx, double paddle_dy, brick BRICKS[50], bool kLeft, bool kRight)
{
	consoleSelect(&debugBox);
	consoleClear();
	double horDir = 0.1;
	double verDir = 0.1;
	std::cout << "movH " << movedHor << " movV " << movedVer << " hD " << horDir << " vD " << verDir << " ";
	if (BALL.getLeft(true) + ball_dx <= 0.0 && ball_dx < 0.0) //The second part is to make certain the ball is going left
	{
		for (double i = 0.0; i > ball_dx; i -= 0.1)
		{
			if (BALL.getLeft(true) == 0.0)
			{
				ball_dx = -ball_dx;
				std::cout << ANSI MAGENTA CEND "-dx ";
				for (double degree = 0.0; degree < 360.0; degree += 1.0)
				{
					double cx = BALL.ball_mcirc.x + BALL.ball_mcirc.rad * cos(degree * M_PI / 180);
					double cy = BALL.ball_mcirc.y + BALL.ball_mcirc.rad * sin(degree * M_PI / 180);
					std::cout << ANSI "2;0" PEND "            ";
					std::cout << ANSI "2;0" PEND "cc " << cx << "," << cy << " ";
					if (PADDLE.getLeft(true) <= cx)
					{
						for (double i = 0.0; i < PADDLE.paddle_mrect.height; i += 0.1)
						{
							if (cy == PADDLE.getTop(false) + i)
								crushBall = true;
							std::cout << "cB " << crushBall << " ";
						}
					}
				}
				horDir = 0.1;
			}
			else
				horDir = -0.1;
			std::cout << "hD " << horDir << " ";
			BALL.move(horDir, 0.0);
			std::cout << "movB " << BALL.ball_mcirc.x << "," << BALL.ball_mcirc.y << " ";
		}
		movedHor = true;
	}
	if (BALL.getRight(true) + ball_dx >= 400 && ball_dx > 0.0) //The second part is to make certain the ball is going right
	{
		for (double i = 0.0; i < ball_dx; i += 0.1)
		{
			if (BALL.getRight(true) == 400.0)
			{
				ball_dx = -ball_dx;
				for (double degree = 0.0; degree < 360.0; degree += 1.0)
				{
					double cx = BALL.ball_mcirc.x + BALL.ball_mcirc.rad * cos(degree * M_PI / 180);
					double cy = BALL.ball_mcirc.y + BALL.ball_mcirc.rad * sin(degree * M_PI / 180);
					if (PADDLE.getRight(true) >= cx)
					{
						for (double i = 0.0; i < PADDLE.paddle_mrect.height; i += 0.1)
						{
							if (cy == PADDLE.getTop(false) + i)
								crushBall = true;
						}
					}
				}
				horDir = -0.1;
			}
			else
				horDir = 0.1;
			BALL.move(horDir, 0.0);
		}
		movedHor = true;
	}
	if (!movedHor)
	{
		BALL.move(ball_dx, 0.0);
		std::cout << "mH + movB " << BALL.ball_mcirc.x << "," << BALL.ball_mcirc.y << " ";
	}
	if (BALL.getBottom(false) + ball_dy >= 240.0 && ball_dy > 0.0)
	{
		consoleSelect(&debugBox);
		std::cout << ANSI "3;0" PEND "Should leave on next frame";
		for (double i = 0.0; i < ball_dy; i += 0.1)
		{
			if (BALL.getBottom(false) == 240.0)
			{
				ball_dy = -ball_dy;
				for (double degree = 0.0; degree < 360.0; degree += 1.0)
				{
					double cx = BALL.ball_mcirc.x + BALL.ball_mcirc.rad * cos(degree * M_PI / 180);
					double cy = BALL.ball_mcirc.y + BALL.ball_mcirc.rad * sin(degree * M_PI / 180);
					if (PADDLE.getBottom(false) >= cy)
					{
						for (double i = 0.0; i < PADDLE.paddle_mrect.width; i += 0.1)
						{
							if (cx == PADDLE.getLeft(false) + i)
								crushBall = true;
						}
					}
				}
				verDir = -0.1;
			}
			else
				verDir = 0.1;
			consoleSelect(&debugBox);
			if (BALL.getBottom(false) == 240.0)
				std::cout << ANSI "4;0" PEND "Bottom equals 240.0";
			else
			{
				std::cout << ANSI "5;0" PEND "                                        ";
				std::cout << ANSI "5;0" PEND "Bottom equals: " << BALL.getBottom(false);
			}
			BALL.move(0.0, verDir);
		}
		movedVer = true;
	}
	if (!movedVer)
		BALL.move(0.0, ball_dy);
	for (int i = 0; i < 15; i++)
		gspWaitForVBlank();
	consoleSelect(&debugBox);
	std::cout << "dx " << ball_dx << " dy " << ball_dy << " ";
	movedHor = false;
	movedVer = false;
}
