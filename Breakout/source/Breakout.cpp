#include "Breakout.hpp"
#include "shapes.hpp"
#include "draw.hpp"

//init
int debugTF = 1;
char versiontxtt[8] = "  Beta ";
char versiontxtn[9] = "01.01.00";
int vernumqik = 0;
u32 kDown, kHeld;

int breakout();
int lives = 3;
double ball_dx, ball_dy;
double ball_angle;
bool crushBall = false;
paddle the_paddle;
int level_count = 2;
int level = 0;
brick brick_array[2][50];
ball the_ball;
double trail_new_frame_x[8];
double trail_new_frame_y[8];
mCircle trail_new_frame_circle[8];
int points;

void trail_new_frame(ball ball_object)
{
	double current_x = ball_object.ball_mcirc.x;
	double current_y = ball_object.ball_mcirc.y;
	for (int i = 7; i > 0; i--)
	{
		trail_new_frame_x[i] = trail_new_frame_x[i - 1];
		trail_new_frame_y[i] = trail_new_frame_y[i - 1];
	}
	trail_new_frame_x[0] = current_x;
	trail_new_frame_y[0] = current_y;
	for (int i = 0; i < 8; i++)
		trail_new_frame_circle[i].setPosition(trail_new_frame_x[i], trail_new_frame_y[i]);
}

bool touchInBox(touchPosition touch, int x, int y, int w, int h)
{
	int tx = touch.px;
	int ty = touch.py;
	u32 kDown = hidKeysDown();
	if (kDown & KEY_TOUCH && tx > x && tx < x + w && ty > y && ty < y + h) {
		return true;
	}
	else {
		return false;
	}
}

touchPosition touch;
bool gameRunning = true;

PrintConsole topScreen, bottomScreen, versionWin, killBox, debugBox;

FS_Archive sdmcArchive;

void openSD()
{
	FSUSER_OpenArchive(&sdmcArchive, ARCHIVE_SDMC, fsMakePath(PATH_EMPTY, ""));
}

void closeSD()
{
	FSUSER_CloseArchive(sdmcArchive);
}

int main(int argc, char **argv)
{
	srand(time(NULL));
	//hidInit();
	//gfxInitDefault();
	sf2d_init();
	sf2d_set_3D(0);
	//fsInit();
	//sdmcInit();
	consoleInit(GFX_BOTTOM, &bottomScreen);
	consoleInit(GFX_BOTTOM, &versionWin);
	consoleInit(GFX_BOTTOM, &killBox);
	consoleInit(GFX_BOTTOM, &debugBox);

	consoleSetWindow(&versionWin, 6, 26, 34, 3);
	consoleSetWindow(&killBox, 0, 28, 40, 2);
	consoleSetWindow(&debugBox, 18, 4, 9, 12);

	consoleSelect(&killBox);
	std::cout << ANSI B_RED CEND;
	for (int i = 0; i < 80; i++)
		std::cout << " ";

	consoleSelect(&versionWin);
	std::cout << "     Tap red area any time to exit";
	std::cout << "Breakout Version: " ANSI RED CEND << versiontxtt << CRESET " " ANSI YELLOW CEND << versiontxtn;
	std::cout << ANSI B_RED ASEP GREEN CEND "              Build: 17.10.04.1824";

	/*consoleSelect(&topScreen);
	std::cout << ANSI "29;07" PEND "by Matthew Rease\n";*/

	consoleSelect(&bottomScreen);

	hidTouchRead(&touch);

	sf2d_set_clear_color(RGBA8(0x95, 0x95, 0x95, 0xFF));
	sf2d_set_vblank_wait(1);
	sf2d_texture *img_title = sfil_load_PNG_buffer(Title_png, SF2D_PLACE_RAM);

	for (int i = 0; i < 8; i++)
		trail_new_frame_circle[7 - i].setDefaults(200.0, 120.0, (0.875 * (i + 1)), 0xC3, 0xC3, 0xC3, (32 * (i + 1)));

	the_paddle.setDefaults(175, 215, 50, 10, 0xC0, 0x61, 0x0A, 0xFF);
	the_ball.setDefaults(200.0, 120.0, 7.0, 0xC3, 0xC3, 0xC3, 0xFF, 200.3, 115.2, 202.5, 119.8, 204.9, 117.1, 0xFF, 0xFF, 0xFF, 0xFF);
	int brick_R[5] = { 0xFF, 0xFF, 0xFF, 0x00, 0x00 };
	int brick_G[5] = { 0x00, 0x80, 0xFF, 0xFF, 0x00 };
	int brick_B[5] = { 0x00, 0x00, 0x00, 0x00, 0xFF };
	int brick_A[5] = { 0xFA, 0xFA, 0xFA, 0xFA, 0xFA }; //250 instead of 255. I want to the bricks to fade out by 25 each frame (lasting 10 frames)
	int array_step = 0;
	for (int a = 0; a < 5; a++)
	{
		for (int b = 0; b < 10; b++)
		{
			brick_array[0][array_step].setDefaults((40 * b) + 2, ((20 * a) + 2), 36, 16, a, true, (4 - a));
			brick_array[0][array_step].exists = true;
			array_step++;
		}
	}
	array_step = 0;
	for (int a = 0; a < 5; a++)
	{
		for (int b = 0; b < 10; b++)
		{
			if ((array_step > 1 && array_step < 10) || (array_step > 13 && array_step < 20) || (array_step > 25 && array_step < 30) || (array_step > 37 && array_step < 40) || (array_step > 49 && array_step < 50))
			{
				brick_array[1][array_step].setDefaults((40 * b) + 2, ((20 * a) + 2), 36, 16, a, false, (4 - a));
				brick_array[1][array_step].exists = false;
			} else {
				brick_array[1][array_step].setDefaults((40 * b) + 2, ((20 * a) + 2), 36, 16, a, true, (4 - a));
				brick_array[1][array_step].exists = true;
			}
			array_step++;
		}
	}
	std::cout << ANSI "20;0" PEND "Array Step : " << array_step << "\n";
	std::cout << "Brick 11 Y: " << brick_array[0][10].brick_mrect.y << " Brick 22 X: " << brick_array[0][21].brick_mrect.x << "\n";
	std::cout << "RGB Data Brick 50: " << brick_array[0][49].brick_mrect.color << "\n";
	//BRICK01.setDefaults(2, 2, 36, 16, 0xFF, 0xFF, 0x00, 0xFF);

	std::cout << ANSI "2;0" PEND "Press Select to begin.";

	// Main loop
	while (aptMainLoop())
	{
		hidScanInput();
		u32 kDown = hidKeysDown();
		if (kDown & KEY_START) break; // break in order to return to hbmenu
		if (kDown & KEY_SELECT)
		{
			lives = 3;
			int result = 3;
			the_ball.reset();
			double angle = 0.0;
			while (angle < 30.0 || angle > 150.0 || (angle > 80 && angle < 100))
				angle = rand() % 360;
			ball_angle = angle;
			sf2d_swapbuffers();
			for (int i = 0; i < level_count; i++)
			{
				for (int j = 0; j < 50; j++)
				{
					brick_array[i][j].reset();
				}
			}
			level = 0;
			points = 0;
			while (true)
			{
				result = breakout();
				if (result != 0)
					break;
			}
			if (result == 3)
				break;
		}

		sf2d_start_frame(GFX_TOP, GFX_LEFT);
		sf2d_draw_rectangle(the_paddle.paddle_mrect.x, the_paddle.paddle_mrect.y, the_paddle.paddle_mrect.width, the_paddle.paddle_mrect.height, RGBA8(0xC0, 0x61, 0x0A, 0xFF)); //Brown rectangle to be paddle
		sf2d_draw_texture(img_title, 80, 20);
		sf2d_end_frame();

		std::cout << ANSI "0;0" PEND "                                                            ";
		std::cout << ANSI "1;0" PEND "                                                            ";
		std::cout << ANSI "0;0" PEND "X = " << the_paddle.paddle_mrect.x << " Y = " << the_paddle.paddle_mrect.y << " Width = " << the_paddle.paddle_mrect.width << " Height = " << the_paddle.paddle_mrect.height;
		std::cout << ANSI "1;0" PEND "H-Mid = " << (the_paddle.paddle_mrect.x + (the_paddle.paddle_mrect.width / 2)) << "   H-Mid-Length = " << (the_paddle.paddle_mrect.width / 2);
		std::cout << ANSI "2;0" PEND;

		sf2d_swapbuffers();

		hidTouchRead(&touch);

		if (touchInBox(touch, 0, 224, 320, 16)) {
			std::cout << "Exiting...\n";
			break;
		}
	}

	// Exit services
	sf2d_fini();
	sf2d_free_texture(img_title);

	return 0;
}

bool hitWallH = false;
bool hitWallV = false;
bool hasHitPadd = false;
bool hasHitWall = false;
bool isMovingRight = false;
bool isInPaddle = false;
bool isInWall = false;
bool brickHitV = false;
bool brickHitH = false;
int angle;

int breakout()
{
	hidScanInput();
	kDown = hidKeysDown();
	kHeld = hidKeysHeld();
	if (kDown & KEY_A)
		level++;
	if (kDown & KEY_B)
		level--;
	if (kDown & KEY_SELECT || lives == 0)
		return 2;
	if (kHeld & KEY_START)
		return 3;
	if (kHeld & KEY_LEFT && the_paddle.paddle_mrect.x > 1)
		the_paddle.paddle_mrect.x -= 2;
	if (kHeld & KEY_RIGHT && the_paddle.paddle_mrect.x < 399 - the_paddle.paddle_mrect.width)
		the_paddle.paddle_mrect.x += 2;

	if (the_ball.getTop(false) > 240)
	{
		lives--;
		the_ball.reset();
		while (ball_angle < 30.0 || ball_angle > 150.0 || (ball_angle > 80 && ball_angle < 100))
			ball_angle = rand() % 360;
		return 0;
	}

	bool hasInteracted = false;
	for (int i = 0; i < 300; i++)
	{
		hasInteracted = false;
		if (the_paddle.getTop(false) <= the_ball.getBottom(false) && (the_paddle.paddle_mrect.x <= the_ball.ball_mcirc.x && the_ball.ball_mcirc.x <= the_paddle.paddle_mrect.x + the_paddle.paddle_mrect.width /*balls x coordinate is <= paddles x coordinate + it's width*/))
			hasHitPadd = true;
		else
			hasHitPadd = false;
		if (hasHitPadd && the_ball.getBottom(false) >= the_paddle.getTop(false) + 0.02)
			hasHitPadd = false;

		//Add gravity powerup (magnet but different) rotates around paddle until button pressed.
		if (the_ball.getLeft(true) <= 0.00 || the_ball.getRight(true) >= 400.00 || the_ball.getTop(false) <= 0.00)
			hasHitWall = true;
		else
			hasHitWall = false;
		if (hasHitWall && !isInWall)
		{
			hasInteracted = true;
			isInWall = true;
			if (the_ball.getTop(false) <= 0.00)
				ball_angle = -ball_angle + 360.0;
			else
				ball_angle = -ball_angle + 180.0;
		}

		if (hasHitPadd && !isInPaddle)
		{
			hasInteracted = true;
			isInPaddle = true;
			angle = 1;
			if (the_ball.getBottom(true) >= the_paddle.paddle_mrect.x + (the_paddle.paddle_mrect.width / 3.0))
				angle += 1;
			if (the_ball.getBottom(true) >= the_paddle.paddle_mrect.x + ((the_paddle.paddle_mrect.width / (2.0 / 3.0))))
				angle += 1;
			if (ball_angle < 90.0)
				isMovingRight = true;
			else
				isMovingRight = false;
			if (isMovingRight)
			{
				if (angle == 1)
					ball_angle += 180.0;
				if (angle == 2)
					ball_angle = -ball_angle + 360.0;
				if (angle == 3)
					ball_angle += 270.0;
			}
			else {
				if (angle == 3)
					ball_angle += 180.0;
				if (angle == 2)
					ball_angle = -ball_angle + 360.0;
				if (angle == 1)
				{
					ball_angle += 90.0;
				}
			}
		}
		for (int j = 0; j < 50; j++)
		{
			if (
				(
					the_ball.getTop(true) >= brick_array[level][j].brick_mrect.x &&
					the_ball.getTop(true) <= brick_array[level][j].brick_mrect.x + brick_array[level][j].brick_mrect.width &&
					the_ball.getTop(false) >= brick_array[level][j].brick_mrect.y &&
					the_ball.getTop(false) <= brick_array[level][j].brick_mrect.y + brick_array[level][j].brick_mrect.height
					) || (
						the_ball.getBottom(true) >= brick_array[level][j].brick_mrect.x &&
						the_ball.getBottom(true) <= brick_array[level][j].brick_mrect.x + brick_array[level][j].brick_mrect.width &&
						the_ball.getBottom(false) >= brick_array[level][j].brick_mrect.y &&
						the_ball.getBottom(false) <= brick_array[level][j].brick_mrect.y + brick_array[level][j].brick_mrect.height
						)
				)
				brickHitV = true;
			if (
				(
					the_ball.getLeft(true) >= brick_array[level][j].brick_mrect.x &&
					the_ball.getLeft(true) <= brick_array[level][j].brick_mrect.x + brick_array[level][j].brick_mrect.width &&
					the_ball.getLeft(false) >= brick_array[level][j].brick_mrect.y &&
					the_ball.getLeft(false) <= brick_array[level][j].brick_mrect.y + brick_array[level][j].brick_mrect.height
					) || (
						the_ball.getRight(true) >= brick_array[level][j].brick_mrect.x &&
						the_ball.getRight(true) <= brick_array[level][j].brick_mrect.x + brick_array[level][j].brick_mrect.width &&
						the_ball.getRight(false) >= brick_array[level][j].brick_mrect.y &&
						the_ball.getRight(false) <= brick_array[level][j].brick_mrect.y + brick_array[level][j].brick_mrect.height
						)
				)
				brickHitH = true;
			if (brickHitV || brickHitH)
			{
				if (brick_array[level][j].exists)
				{
					brick_array[level][j].destroy();
					points += brick_array[level][j].point_value();
					if (brickHitV && brickHitH)
					{
						ball_angle -= 180.0;
						if (ball_angle < 0.0)
							ball_angle += 360.0;
					}
					else if (brickHitV)
						ball_angle = -ball_angle + 360.0;
					else if (brickHitH)
						ball_angle = -ball_angle + 180.0;
				}
				brickHitV = false;
				brickHitH = false;
			}
		}
		ball_dx = 2.0 * cos(ball_angle * (M_PI / 180.0));
		ball_dy = 2.0 * sin(ball_angle * (M_PI / 180.0));
		if (hasHitPadd && isInPaddle)
			the_ball.move(ball_dx / 300.0, ball_dy / 300.0);
		else if (isInPaddle && !hasHitPadd)
			isInPaddle = false;
		if (hasHitWall && isInWall)
			the_ball.move(ball_dx / 300.0, ball_dy / 300.0);
		else if (isInWall && !hasHitWall)
			isInWall = false;
		if (!hasInteracted && !hasHitPadd && !hasHitWall && !isInPaddle && !isInWall)
			the_ball.move(ball_dx / 300.0, ball_dy / 300.0);
		if (hasHitPadd && hasHitWall)
		{
			lives--;
			the_ball.reset();
			ball_angle = 0.0;
			while (ball_angle < 30.0 || ball_angle > 150.0 || (ball_angle > 80 && ball_angle < 100))
				ball_angle = rand() % 360;
			return 0;
		}
	}

	trail_new_frame(the_ball);

	sf2d_start_frame(GFX_TOP, GFX_LEFT);
	draw_paddle(the_paddle);
	for (int i = 0; i < 50; i++)
		if (brick_array[level][i].exists)
			draw_brick(brick_array[level][i]);
	for (int i = 7; i >= 0; i--)
		draw_circ(trail_new_frame_circle[i]);
	draw_ball(the_ball);
	std::cout << ANSI "13;0" PEND;
	for (int i = 0; i < 3; i++)
		std::cout << "                                        ";
	std::cout << ANSI "13;0" PEND;
	std::cout << "Score: " << points << "\n";
	std::cout << "Point value for brick 1: " << brick_array[level][0].point_value() << "\n";
	std::cout << "Point Array 1 2 and 3: " << brick_point_value[0] << ", " << brick_point_value[1] << ", and" << brick_point_value[2] << "\n";
	/*std::cout << ANSI "13;0" PEND;
	for (int i = 0; i < 8; i++)
	std::cout << "                                        ";
	std::cout << ANSI "13;0" PEND;
	std::cout << "Ball hit: Top: " << hasHitPadd << " Bottom: -\n";
	std::cout << "Ball hit: Left: - Right: -\n";
	std::cout << "Ball Position: " << the_ball.ball_mcirc.x << "," << the_ball.ball_mcirc.y << "\n";
	std::cout << "Ball Direction: " << ball_dx << "," << ball_dy << "\n";
	std::cout << "Paddle Position: " << the_paddle.paddle_mrect.x << "," << the_paddle.paddle_mrect.y << "\n";
	std::cout << "Paddle Direction: " << "Left: " << mov_left << " Right: " << mov_right << "\n";*/
	sf2d_end_frame();
	sf2d_swapbuffers();
	return 0;
}
