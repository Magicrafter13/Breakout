#include "Breakout.hpp"
#include "shapes.hpp"
#include "draw.hpp"
#include "physics.hpp"

int frame = 0;
std::vector<double> frame_x;
std::vector<double> frame_y;

//init
int debugTF = 1;
char versiontxtt[8] = " Alpha ";
char versiontxtn[9] = " 1.02.0 ";
int vernumqik = 0;

int breakout();
int lives = 3;
double ball_dx, ball_dy;
bool crushBall = false;
paddle the_paddle;
brick brick_array[50];
ball the_ball;

bool touchInBox(touchPosition touch, int x, int y, int w, int h)
{
	int tx=touch.px;
	int ty=touch.py;
	u32 kDown = hidKeysDown();
	if (kDown & KEY_TOUCH && tx > x && tx < x+w && ty > y && ty < y+h) {
		return true;
	} else {
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
	
	consoleSetWindow(&versionWin, 6, 26, 34, 2);
	consoleSetWindow(&killBox, 0, 28, 40, 2);
	consoleSetWindow(&debugBox, 18, 4, 9, 12);

	consoleSelect(&versionWin);
	std::cout << "     Tap red area any time to exit";
	std::cout << "Breakout Version: " ANSI RED CEND << versiontxtt << CRESET " " ANSI YELLOW CEND << versiontxtn;

	consoleSelect(&killBox);
	std::cout << ANSI B_RED CEND;
	for (int i = 0; i < 80; i++)
		std::cout << " ";

	/*consoleSelect(&topScreen);
	std::cout << ANSI "29;07" PEND "by Matthew Rease\n";*/

	consoleSelect(&bottomScreen);
	
	hidTouchRead(&touch);

	sf2d_set_clear_color(RGBA8(0x95, 0x95, 0x95, 0xFF));
	sf2d_set_vblank_wait(1);
	sf2d_texture *img_title = sfil_load_PNG_buffer(Title_png, SF2D_PLACE_RAM);

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
			brick_array[array_step].setDefaults((40 * b) + 2, ((20 * a) + 2), 36, 16, brick_R[a], brick_G[a], brick_B[a], brick_A[a]);
			brick_array[array_step].exists = true;
			array_step++;
		}
	}
	std::cout << ANSI "20;0" PEND "Array Step : " << array_step << "\n";
	std::cout << "Brick 11 Y: " << brick_array[10].brick_mrect.y << " Brick 22 X: " << brick_array[21].brick_mrect.x << "\n";
	std::cout << "RGB Data Brick 50: " << brick_array[49].brick_mrect.color << "\n";
	//BRICK01.setDefaults(2, 2, 36, 16, 0xFF, 0xFF, 0x00, 0xFF);
	
	std::cout << ANSI "2;0" PEND "Press Select to begin.";

	// Main loop
	while (aptMainLoop())
	{
		hidScanInput();
		u32 kDown = hidKeysDown();
		u32 kHeld = hidKeysHeld();
		if (kDown & KEY_START) break; // break in order to return to hbmenu
		if (kDown & KEY_SELECT)
		{
			lives = 3;
			int result = 3;
			the_ball.reset();
			double angle = 0.0;
			while (angle < 30.0 || angle > 150.0 || (angle > 80 && angle < 100))
			{
				angle = rand() % 360;
			}
			ball_dx = 3.0 * cos(angle * (M_PI / 180.0));
			ball_dy = 3.0 * sin(angle * (M_PI / 180.0));
			sf2d_swapbuffers();
			frame = 0;
			frame_x.clear();
			frame_y.clear();
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

		if(touchInBox(touch, 0, 224, 320, 16)) {
			std::cout << "Exiting...\n";
			break;
		}
	}

	// Exit services
	sf2d_fini();
	sf2d_free_texture(img_title);
	
	return 0;
}

bool mov_left = false;
bool mov_right = false;
bool hitWallH = false;
bool hitWallV = false;
bool hasHit = false;
bool isMovingRight = false;

int breakout()
{
	if (lives == 0)
		return 2;
	hidScanInput();
	u32 kDown = hidKeysDown();
	u32 kHeld = hidKeysHeld();
	if (kDown & KEY_SELECT)
		return 2;
	if (kHeld & KEY_START)
		return 3;
	if (kHeld & KEY_LEFT)
		if (the_paddle.paddle_mrect.x > 1)
			the_paddle.paddle_mrect.x -= 2;
	if (kHeld & KEY_RIGHT)
		if (the_paddle.paddle_mrect.x < 399 - the_paddle.paddle_mrect.width)
			the_paddle.paddle_mrect.x += 2;
	for (int i = 0; i < 100; i++)
	{
		if (i == 101)
			std::cout << "Yeah sorry about that breakdown (no pun intended) in the last commit... let's never discuss that...";
	}
	if ((the_ball.getLeft(true) <= 0) || (the_ball.getRight(true) >= 400))
		hitWallH = true;
	if (the_ball.getTop(false) <= 0)
		hitWallV = true;
	if (the_paddle.getTop(false) <= the_ball.getBottom(false) && the_ball.getBottom(false) <= the_paddle.getBottom(false) && (the_paddle.paddle_mrect.x <= the_ball.ball_mcirc.x && the_ball.ball_mcirc.x <= the_paddle.paddle_mrect.x + the_paddle.paddle_mrect.width))
	{
		hasHit = true;
	}
	if (hasHit)
	{
		if (the_ball.getBottom(false) >= the_paddle.getTop(false) + (the_paddle.paddle_mrect.height / 2))
		{
			hasHit = false;
		}
	}
	if (the_ball.getTop(false) > 240)
	{
		lives--;
		the_ball.reset();
		double angle = 0.0;
		while (angle < 30.0 || angle > 150.0 || (angle > 80 && angle < 100))
		{
			angle = rand() % 360;
		}
		ball_dx = 3.0 * cos(angle * (M_PI / 180.0));
		ball_dy = 3.0 * sin(angle * (M_PI / 180.0));
	}
	//Add gravity powerup (magnet but different) rotates around paddle until button pressed.
	if (hitWallH)
	{
		double amount = 0.0;
		amount = the_ball.getLeft(true);
		if (amount <= -1.0)
			the_ball.move(-amount, 0.0);
		amount = the_ball.getRight(true);
		if (amount >= 401.0)
			the_ball.move((400.0 - amount), 0.0);
		ball_dx = -ball_dx;
	}
	if (hitWallV)
	{
		double amount = 0.0;
		amount = the_ball.getTop(false);
		if (amount <= -1.0)
			the_ball.move(0.0, -amount);
		amount = the_ball.getBottom(false);
		if (amount >= 241.0)
			the_ball.move(0.0, (240.0 - amount));
		ball_dy = -ball_dy;
	}
	if (hasHit)
	{
		int angle = 1;
		if (the_ball.getBottom(true) >= the_paddle.paddle_mrect.x + (the_paddle.paddle_mrect.width / 3.0))
			angle = 2;
		if (the_ball.getBottom(true) >= the_paddle.paddle_mrect.x + ((the_paddle.paddle_mrect.width / 3.0) * 2.0))
			angle = 3;
		if (ball_dx == abs(ball_dx))
			isMovingRight = true;
		else
			isMovingRight = false;
		if (isMovingRight)
		{
			if (angle == 1)
			{
				ball_dx = -ball_dx;
				ball_dy = -ball_dy;
			}
			if (angle == 2)
				ball_dy = -ball_dy;
			if (angle == 3)
			{
				double direction_angle = atan2(ball_dy, ball_dx) * (180.0 / M_PI);
				std::cout << ANSI "5;0" PEND;
				for (int i = 0; i < 2; i++)
					std::cout << "                                        ";
				std::cout << "isMovingRight | true :: d_angle = " << direction_angle << "\n";
				direction_angle -= 90.0;
				if (direction_angle < 0.0)
					direction_angle += 360.0;
				ball_dx = 3.0 * cos(direction_angle * (M_PI / 180.0));
				ball_dy = 3.0 * sin(direction_angle * (M_PI / 180.0));
				std::cout << "dx = " << ball_dx << " dy = " << ball_dy << "\n";
			}
		}
		else {
			if (angle == 3)
			{
				ball_dx = -ball_dx;
				ball_dy = -ball_dy;
			}
			if (angle == 2)
				ball_dy = -ball_dy;
			if (angle == 1)
			{
				double direction_angle = atan2(ball_dy, ball_dx) * (180.0 / M_PI);
				std::cout << ANSI "3;0" PEND;
				for (int i = 0; i < 2; i++)
					std::cout << "                                        ";
				std::cout << "isMovingRight | false :: d_angle = " << direction_angle << "\n";
				direction_angle += 90.0;
				if (direction_angle > 360.0)
					direction_angle -= 360.0;
				ball_dx = 3.0 * cos(direction_angle * (M_PI / 180.0));
				ball_dy = 3.0 * sin(direction_angle * (M_PI / 180.0));
				std::cout << "dx = " << ball_dx << " dy = " << ball_dy << "\n";
			}
		}
	}

	the_ball.move(ball_dx, ball_dy);

	/*-new collision detector, will move accordingly if works*/

	//-end-
	frame_x.push_back(the_ball.ball_mcirc.x);
	frame_y.push_back(the_ball.ball_mcirc.y);
	sf2d_start_frame(GFX_TOP, GFX_LEFT);
	draw_paddle(the_paddle);
	for (int i = 0; i < 50; i++)
		draw_brick(brick_array[i]);
	for (int i = 0; i < frame; i++)
		sf2d_draw_fill_circle(frame_x[i], frame_y[i], 3, RGBA8(0xFF, 0xFF, 0x00, 0xFF));
	draw_ball(the_ball);
	std::cout << ANSI "13;0" PEND;
	for (int i = 0; i < 8; i++)
		std::cout << "                                        ";
	std::cout << ANSI "13;0" PEND;
	std::cout << "Ball hit: Top: " << hasHit << " Bottom: -\n";
	std::cout << "Ball hit: Left: - Right: -\n";
	std::cout << "Ball Position: " << the_ball.ball_mcirc.x << "," << the_ball.ball_mcirc.y << "\n";
	std::cout << "Ball Direction: " << ball_dx << "," << ball_dy << "\n";
	std::cout << "Paddle Position: " << the_paddle.paddle_mrect.x << "," << the_paddle.paddle_mrect.y << "\n";
	std::cout << "Paddle Direction: " << "Left: " << mov_left << " Right: " << mov_right << "\n";
	
	mov_left = false;
	mov_right = false;
	hasHit = false;
	hitWallH = false;
	hitWallV = false;
	mov_left = false;
	mov_right = false;
	sf2d_end_frame();
	sf2d_swapbuffers();
	frame++;
	return 0;
}
