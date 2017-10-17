#include "Breakout.hpp"
#include "shapes.hpp"
#include "draw.hpp"
#include "sfx.h"
#include "filesystem.h"

//init
char versiontxtt[8] = "  Beta ", versiontxtn[9] = "01.06.00";
char buildnumber[14] = "17.10.16.2033", ishupeversion[9] = "00.04.00";
int vernumqik = 0;
u32 kDown, kHeld;

int breakout();
int thanks_for_playing_the_beta();
int extras_10_13_2017();
int level_designer();

int lives = 3, points, level = 0;
int level_count = def_level_count;
double ball_dx, ball_dy, ball_angle, trail_new_frame_x[8], trail_new_frame_y[8]; bool crushBall = false; bool ball_is_attached;
int last_power, times_power_1, times_power_2, times_power_3;
int press_select_frame = 0; bool press_select_visible = true;

paddle the_paddle; ball the_ball; mCircle trail_new_frame_circle[8]; brick brick_array[def_level_count][50];
sf2d_texture *img_thanksbeta, *img_paddle, *img_brick00, *img_brick01, *img_brick02, *img_brick03, *img_brick04, *img_brick05, *img_waveform;
SFX_s *testsound[1], *ball_bounce[8];

/*integer mask for levels*/
int level_mask[def_level_count][50] = {
	{
		5, 5, 5, 5, 5, 5, 5, 5, 5, 5,
		4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
		3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
		2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1
	},
	{
		5, 5, 0, 0, 0, 0, 0, 0, 0, 0,
		4, 4, 4, 4, 0, 0, 0, 0, 0, 0,
		3, 3, 3, 3, 3, 3, 0, 0, 0, 0,
		2, 2, 2, 2, 2, 2, 2, 2, 0, 0,
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1
	},
	{
		0, 0, 1, 1, 2, 2, 1, 1, 0, 0,
		1, 1, 2, 2, 3, 3, 2, 2, 1, 1,
		2, 2, 3, 3, 9, 9, 3, 3, 2, 2,
		1, 1, 2, 2, 3, 3, 2, 2, 1, 1,
		0, 0, 1, 1, 2, 2, 1, 1, 0, 0
	},
	{
		 5,  0,  3,  2,  0,  0,  2,  3,  0,  5,
		 4,  8,  2,  1, 10, 10,  1,  2,  8,  4,
		 0,  2,  0,  0,  9,  9,  0,  0,  2,  0,
		 4,  8,  2,  1, 10, 10,  1,  2,  8,  4,
		 5,  0,  3,  2,  0,  0,  2,  3,  0,  5
	}
};

/*set ball trail*/
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

/*returns if area is being touched by stylus*/
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

PrintConsole topScreen, bottomScreen, versionWin, killBox, debugBox;

FS_Archive sdmcArchive;

/*open SD card filesystem*/
void openSD()
{
	FSUSER_OpenArchive(&sdmcArchive, ARCHIVE_SDMC, fsMakePath(PATH_EMPTY, ""));
}

/*close SD card filesystem*/
void closeSD()
{
	FSUSER_CloseArchive(sdmcArchive);
}

/*begin application*/
int main(int argc, char **argv)
{
	romfsInit();

	csndInit();
	initSound();
	
	srand(time(NULL));
	
	sf2d_init();
	sf2d_set_3D(0);
	sftd_init();
	
	consoleInit(GFX_BOTTOM, &bottomScreen);
	consoleInit(GFX_BOTTOM, &versionWin);
	consoleInit(GFX_BOTTOM, &killBox);
	consoleInit(GFX_BOTTOM, &debugBox);

	consoleSetWindow(&versionWin, 6, 26, 34, 4);
	consoleSetWindow(&killBox, 0, 28, 40, 2);
	consoleSetWindow(&debugBox, 18, 4, 9, 12);

	hidTouchRead(&touch);

	sf2d_set_clear_color(RGBA8(0x95, 0x95, 0x95, 0xFF));
	sf2d_set_vblank_wait(1);
	//sftd_font *fnt_main;
	sftd_font *game_fonts[1] = {
		sftd_load_font_mem(ethnocen_ttf, ethnocen_ttf_size)
	};
	sf2d_texture *img_title = game_textures[15];
	img_thanksbeta = game_textures[16];

	img_paddle = game_textures[17];
	img_brick00 = game_textures[0];

	img_waveform = game_textures[18];

	//fnt_main = game_fonts[0];

	testsound[0] = createSFX("romfs:/testfile.raw", SOUND_FORMAT_16BIT);
	ball_bounce[0] = createSFX("romfs:/bounce0.raw", SOUND_FORMAT_16BIT);
	ball_bounce[1] = createSFX("romfs:/bounce1.raw", SOUND_FORMAT_16BIT);
	ball_bounce[2] = createSFX("romfs:/bounce2.raw", SOUND_FORMAT_16BIT);
	ball_bounce[3] = createSFX("romfs:/bounce3.raw", SOUND_FORMAT_16BIT);
	ball_bounce[4] = createSFX("romfs:/bounce4.raw", SOUND_FORMAT_16BIT);
	ball_bounce[5] = createSFX("romfs:/bounce5.raw", SOUND_FORMAT_16BIT);
	ball_bounce[6] = createSFX("romfs:/bounce6.raw", SOUND_FORMAT_16BIT);
	ball_bounce[7] = createSFX("romfs:/bounce7.raw", SOUND_FORMAT_16BIT);

	for (int i = 0; i < 8; i++)
		trail_new_frame_circle[7 - i].setDefaults(200.0, 120.0, (0.875 * (i + 1)), 0xC3, 0xC3, 0xC3, (32 * (i + 1)));

	the_paddle.setDefaults(175, 215, 50, 10, 0xC0, 0x61, 0x0A, 0xFF, img_paddle);
	the_ball.setDefaults(200.0, 200.0, 7.0, 1, 200.3, 195.2, 202.5, 199.8, 204.9, 197.1);
	for (int q = 0; q < level_count; q++)
	{
		int array_step = 0;
		for (int a = 0; a < 5; a++)
		{
			for (int b = 0; b < 10; b++)
			{
				if (level_mask[q][array_step] == 0)
					brick_array[q][array_step].setDefaults((40 * b) + 2, ((20 * a) + 2), 36, 16, false, 0);
				else
					brick_array[q][array_step].setDefaults((40 * b) + 2, ((20 * a) + 2), 36, 16, true, level_mask[q][array_step]);
				array_step++;
			}
		}
	}

	int bottom_screen_text = 0;

	playSFX(testsound[0]);

	// Main loop
	while (aptMainLoop())
	{
		hidScanInput();
		u32 kDown = hidKeysDown();
		if (kDown & KEY_START) break; // break in order to return to hbmenu
		/*go to extras when X pressed*/
		if (kDown & KEY_X) {
			int ext_return = extras_10_13_2017();
			if (ext_return == 3)
				break;
		}
		/*level designer*/
		if (kDown & KEY_Y) {
			level_designer();
		}
		/*begin game*/
		if (kDown & KEY_SELECT)
		{
			lives = 3;
			int result = 3;
			the_ball.reset();
			the_paddle.reset();
			double angle = 0.0;
			while (angle < 225.0 || angle > 315.0 || (angle > 265 && angle < 275))
				angle = rand() % 360;
			ball_angle = angle;
			sf2d_swapbuffers();
			for (int i = 0; i < level_count; i++)
				for (int j = 0; j < 50; j++)
					brick_array[i][j].reset();
			level = 0; points = 0; last_power = 0;
			times_power_1 = 0; times_power_2 = 0; times_power_3 = 0;
			ball_is_attached = true;
			/*main breakout loop*/
			while (true)
			{
				result = breakout();
				if (result != 0)
					break;
			}
			bottom_screen_text = 0;
			if (result == 3) break;
		}
		/*text display (run once to avoid screen tearing)*/
		if (bottom_screen_text == 0)
		{
			consoleSelect(&killBox); consoleClear();
			consoleSelect(&versionWin); consoleClear();
			consoleSelect(&bottomScreen); consoleClear();

			consoleSelect(&killBox);
			std::cout << ANSI B_RED CEND;
			for (int i = 0; i < 80; i++)
				std::cout << " ";

			consoleSelect(&versionWin);
			std::cout << CRESET "     Tap red area any time to exit";
			std::cout << "Breakout Version: " ANSI RED CEND << versiontxtt << CRESET " " ANSI YELLOW CEND << versiontxtn;
			std::cout << ANSI B_RED ASEP GREEN CEND "              Build: " << buildnumber;
			std::cout << ANSI B_RED ASEP GREEN CEND "   ISHUPE Engine Version: " << ishupeversion;

			consoleSelect(&bottomScreen);
			std::cout << ANSI "20;0" PEND;

			std::cout << ANSI "2;0" PEND "Press Select to begin.\n";
			std::cout << "Press X to see what I'm working on or have planned.\n";

			bottom_screen_text = 1;
		}
		press_select_frame++;
		sf2d_start_frame(GFX_TOP, GFX_LEFT);
		draw_object(the_paddle);
		sf2d_draw_texture(img_title, 80, 20);
		sf2d_draw_texture(img_paddle, 122, 92);
		if (press_select_visible)
			sftd_draw_textf(game_fonts[0], 100, 180, RGBA8(0x00, 0x00, 0x00, 0xFF), 11, "Press Select to play!");
		sf2d_end_frame();
		/*after half a second, Press Select to play! is toggled*/
		if (press_select_frame == 30)
		{
			press_select_frame = 0;
			if (press_select_visible)
				press_select_visible = false;
			else
				press_select_visible = true;
		}

		sf2d_swapbuffers();

		hidTouchRead(&touch);

		/*exit game if red box touched*/
		if (touchInBox(touch, 0, 224, 320, 16)) {
			std::cout << "Exiting...\n";
			break;
		}
	}

	// Exit services
	sftd_fini();

	for (int i = 0; i < texture_count; i++)
		sf2d_free_texture(game_textures[i]);

	sf2d_fini();

	exitSound();

	return 0;
}

bool hitWallH = false, hitWallV = false;
bool hasHitPadd = false, hasHitWall = false;
bool isMovingRight = false;
bool isInPaddle = false, isInWall = false;
bool brickHitV = false, brickHitH = false;
int angle;
int bricks_hit_this_frame;
bool change_level;
int thanks_text_display;
bool has_hit_paddle, has_hit_wall;

/*main game*/
int breakout()
{
	hidScanInput();
	kDown = hidKeysDown();
	kHeld = hidKeysHeld();
	/*if (kDown & KEY_R)
		level++;
	if (kDown & KEY_L)
		level--;*/
	if (kDown & KEY_SELECT || lives == 0) return 2;
	if (kHeld & KEY_START) return 3;
	/*move paddle left (if applicable)*/
	if (kHeld & KEY_LEFT && the_paddle.paddle_mrect.x > 1)
	{
		the_paddle.paddle_mrect.x -= 3;
		if (ball_is_attached) the_ball.move(-3.0, 0.0);
	}
	/*move paddle right (if applicable)*/
	if (kHeld & KEY_RIGHT && the_paddle.paddle_mrect.x < 399 - the_paddle.paddle_mrect.width)
	{
		the_paddle.paddle_mrect.x += 3;
		if (ball_is_attached) the_ball.move(3.0, 0.0);
	}

	/*lose life if outside of game field*/
	if (the_ball.getTop(false) > 240)
	{
		lives--;
		the_ball.reset(); the_paddle.reset();
		ball_is_attached = true;
		while (ball_angle < 225.0 || ball_angle > 315.0 || (ball_angle > 265 && ball_angle < 275)) ball_angle = rand() % 360;
		return 0;
	}

	bool hasInteracted = false;
	has_hit_paddle = false; has_hit_wall = false;
	if (kDown & KEY_A && ball_is_attached == true) ball_is_attached = false;
	/*run main engine code if the ball is not attached to the paddle*/
	if (!ball_is_attached)
	{
		bricks_hit_this_frame = 0;
		change_level = false;
		//if (kDown & KEY_X) change_level = true;
		/*main hit detection engine (runs 300 times per frame)*/
		for (int i = 0; i < 300; i++)
		{
			hasInteracted = false;
			if (the_paddle.getTop(false) <= the_ball.getBottom(false) && (the_paddle.paddle_mrect.x <= the_ball.ball_mcirc.x && the_ball.ball_mcirc.x <= the_paddle.paddle_mrect.x + the_paddle.paddle_mrect.width /*balls x coordinate is <= paddles x coordinate + it's width*/))
				hasHitPadd = true;
			else
				hasHitPadd = false;
			if (hasHitPadd && the_ball.getBottom(false) >= the_paddle.getTop(false) + 0.02) hasHitPadd = false;
			if (hasHitPadd) has_hit_paddle = true;
			//Add gravity powerup (magnet but different) rotates around paddle until button pressed.
			if (the_ball.getLeft(true) <= 0.00 || the_ball.getRight(true) >= 400.00 || the_ball.getTop(false) <= 0.00)
				hasHitWall = true;
			else
				hasHitWall = false;
			if (hasHitWall) has_hit_wall = true;
			/*first time ball is detected touching wall*/
			if (hasHitWall && !isInWall)
			{
				hasInteracted = true;
				isInWall = true;
				if (the_ball.getTop(false) <= 0.00)
					ball_angle = 360.0 - ball_angle;
				else
					ball_angle = 180.0 - ball_angle;
			}

			while (ball_angle < 0.0) ball_angle += 360.0;
			while (ball_angle > 360.0) ball_angle -= 360.0;

			/*first time ball is detected touching paddle*/
			if (hasHitPadd && !isInPaddle)
			{
				hasInteracted = true;
				isInPaddle = true;
				double paddle_width_ninth = the_paddle.paddle_mrect.width / 9.0;
				angle = 1;
				for (double z = 1.0; z < 9.0; z += 1.0)
					if (the_ball.getBottom(true) >= the_paddle.paddle_mrect.x + (paddle_width_ninth * z))
						angle += 1;
				/*change ball angle according to the area of paddle hit*/
				switch (angle) {
				case 1: ball_angle = (360.0 - ball_angle) - 40.0;
					break;
				case 2: ball_angle = (360.0 - ball_angle) - 30.0;
					break;
				case 3: ball_angle = (360.0 - ball_angle) - 20.0;
					break;
				case 4: ball_angle = (360.0 - ball_angle) - 10.0;
					break;
				case 5: ball_angle = (360.0 - ball_angle);
					break;
				case 6: ball_angle = (360.0 - ball_angle) + 10.0;
					break;
				case 7: ball_angle = (360.0 - ball_angle) + 20.0;
					break;
				case 8: ball_angle = (360.0 - ball_angle) + 30.0;
					break;
				case 9: ball_angle = (360.0 - ball_angle) + 40.0;
					break;
				}
			}
			/*large if statement to determine if a brick has been hit (run once per brick)*/
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
				/*code if brick(s) hit*/
				if (brickHitV || brickHitH)
				{
					/*check if brick is actually in play*/
					if (brick_array[level][j].exists)
					{
						brick_array[level][j].destroy();
						if (!brick_array[level][j].exists) {
							points += brick_array[level][j].point_value();
							last_power = brick_array[level][j].random_powerup();
							if (last_power == 1)
								times_power_1++;
							if (last_power == 2)
								times_power_2++;
							if (last_power == 3)
								times_power_3++;
						}
						bricks_hit_this_frame++;
						/*if brick hit V and H reverse direction*/
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
			ball_dx = 2.0 * cos(ball_angle * (M_PI / 180.0)); ball_dy = 2.0 * sin(ball_angle * (M_PI / 180.0));
			if (hasHitPadd && isInPaddle)
				the_ball.move(ball_dx / 300.0, ball_dy / 300.0);
			else if (isInPaddle && !hasHitPadd)
				isInPaddle = false;
			if (hasHitWall && isInWall)
				the_ball.move(ball_dx / 300.0, ball_dy / 300.0);
			else if (isInWall && !hasHitWall)
				isInWall = false;
			if (!hasInteracted && !hasHitPadd && !hasHitWall && !isInPaddle && !isInWall) the_ball.move(ball_dx / 300.0, ball_dy / 300.0);
			/*if paddle and wall hit in same frame, ball is "crushed" (this could cause problems later*/
			if (hasHitPadd && hasHitWall)
			{
				lives--;
				the_ball.reset();
				ball_angle = 0.0;
				while (ball_angle < 30.0 || ball_angle > 150.0 || (ball_angle > 80 && ball_angle < 100)) ball_angle = rand() % 360;
				return 0;
			}
			int bricks_available = 0;
			for (int brick_array_pos = 0; brick_array_pos < 50; brick_array_pos++)
				if (brick_array[level][brick_array_pos].exists)
					bricks_available++;
			if (bricks_available == 0)
				change_level = true;
		}
		/*either increase level, or go to win screen*/
		if (change_level == true)
		{
			if (level == level_count - 1)
			{
				int thanks_return = 0;
				thanks_text_display = 0;
				while (thanks_return == 0) thanks_return = thanks_for_playing_the_beta();
				thanks_text_display = 0;
				return thanks_return;
			}
			else {
				level++;
				ball_is_attached = true;
				the_ball.reset();
				the_paddle.reset();
				while (ball_angle < 225.0 || ball_angle > 315.0 || (ball_angle > 265 && ball_angle < 275)) ball_angle = rand() % 360;
			}
		}
		/*to avoid a glitch, if more than one brick is hit on the same frame the direction is reversed*/
		if (bricks_hit_this_frame > 1)
		{
			ball_angle += 180.0;
			if (ball_angle > 360.0)
				ball_angle -= 360.0;
		}
	}

	/*plays random SFX if a brick has been hit*/
	if (bricks_hit_this_frame > 0)
	{
		int which_bounce = rand() % 7;
		playSFX(ball_bounce[which_bounce]);
	}
	if (has_hit_paddle || has_hit_wall) playSFX(ball_bounce[7]);

	trail_new_frame(the_ball);

	sf2d_start_frame(GFX_TOP, GFX_LEFT);
	draw_object(the_paddle);
	for (int i = 0; i < 50; i++)
		if (brick_array[level][i].exists)
			draw_object(brick_array[level][i]);
	if (the_ball.uses_texture)
		for (int i = 7; i >= 0; i--)
			sf2d_draw_texture_scale_blend(game_textures[the_ball.texture_id], (trail_new_frame_circle[i].x - trail_new_frame_circle[i].rad) + 1.0, (trail_new_frame_circle[i].y - trail_new_frame_circle[i].rad) + 2.0, (7 - i) / 8.0, (7 - i) / 8.0, RGBA8(0xFF, 0xFF, 0xFF, 32 * (7 - i)));
	else
		for (int i = 7; i >= 0; i--)
			draw_object(trail_new_frame_circle[i]);
	draw_object(the_ball);
	std::cout << ANSI "13;0" PEND;
	for (int i = 0; i < 2; i++)
		std::cout << "                                        ";
	std::cout << ANSI "13;0" PEND;
	std::cout << "Score: " << points << "\n";
	std::cout << "Lives: " << lives << "\n";
	sf2d_end_frame();
	sf2d_swapbuffers();
	return 0;
}

/*win screen for the beta*/
int thanks_for_playing_the_beta()
{
	hidScanInput();
	kDown = hidKeysDown(); kHeld = hidKeysHeld();
	if (kDown & (KEY_SELECT | KEY_A | KEY_B | KEY_X | KEY_Y | KEY_L | KEY_R | KEY_ZL | KEY_ZR)) return 2;
	if (kHeld & KEY_START) return 3;
	sf2d_start_frame(GFX_TOP, GFX_LEFT);
	sf2d_draw_texture(img_thanksbeta, 80, 20);
	/*run text display once (to avoid screen tear)*/
	if (thanks_text_display == 0)
	{
		std::cout << ANSI "0;0" PEND;
		for (int i = 0; i < 30; i++)
			std::cout << "                                        ";
		std::cout << ANSI "0;0" PEND;
		std::cout << "[Press any key to return to the title.]\n";
		std::cout << "Thanks to:\n\n";
		std::cout << "Jared for helping me " ANSI BRIGHT ASEP YELLOW CEND "Beta" CRESET " test.\n\n";
		std::cout << ANSI YELLOW CEND "StackOverflow" CRESET " for helping me with " ANSI BRIGHT ASEP GREEN CEND "c++" CRESET "\n\n";
		std::cout << "The awesome " ANSI RED CEND "3ds" CRESET " " ANSI BLUE CEND "homebrew" CRESET " community for\n";
		std::cout << "helping me with many things relating to\n";
		std::cout << ANSI BRIGHT ASEP BLUE CEND "libctru" CRESET " and " ANSI GREEN CEND "sf2dlib." CRESET "\n\n";
		std::cout << "Bryan for helping me with programming\n\n";
		std::cout << ANSI BRIGHT ASEP MAGENTA CEND "GBAtemp" CRESET " community for helping me with so";
		std::cout << "much! You guys are so helpful.\n\n";
		std::cout << "\n";
		std::cout << "     And " ANSI MAGENTA CEND "YOU" CRESET " for playing my game!\n";
		std::cout << "    And remember, all feedback and\n";
		std::cout << "       suggestions are welcome!\n";
		thanks_text_display = 1;
	}
	sf2d_end_frame();
	sf2d_swapbuffers();
	return 0;
}

/*extras: created 10/13/2017*/
int extras_10_13_2017()
{
	consoleSelect(&bottomScreen);
	consoleClear();
	std::cout << ANSI "0;0" PEND;
	for (int i = 0; i < 30; i++)
		std::cout << "                                        ";
	std::cout << ANSI "0;0" PEND;
	std::cout << "The blue cracked brick was orginally\n";
	std::cout << "going to be the design for the normal\n";
	std::cout << "brick but I decided to instead save that";
	std::cout << "for the future for bricks that take more";
	std::cout << "than one hit.\n\n";
	std::cout << "Future Gamemode: This idea came about\n";
	std::cout << "when I'd thought about all the people I\n";
	std::cout << "saw playing some new game on their\n";
	std::cout << "phones. It's this game where you aim\n";
	std::cout << "your ball, and it shoots out like 100\n";
	std::cout << "balls. And all the bricks in the game\n";
	std::cout << "many hits, which is determined via a\n";
	std::cout << "number displayed on the brick.\n";
	std::cout << "So I figure I'll add this sometime.\n\n";
	std::cout << "These new graphics and SFX aren't\n";
	std::cout << "necessarily permanent. This update is\n";
	std::cout << "more to show off that actual graphics\n";
	std::cout << "and SFX can and have been added, and\n";
	std::cout << "they may very well be changed and\n";
	std::cout << "improved in the future, so if you don't\n";
	std::cout << "like what's presented to you now, then\n";
	std::cout << "don't worry because it can easily be\n";
	std::cout << "changed and I'm always open to\n";
	std::cout << "suggestions!\n";
	/*main loop with sf2d drawing*/
	while (true)
	{
		sf2d_start_frame(GFX_TOP, GFX_LEFT);
		sf2d_draw_texture_scale(game_textures[1], 20, 20, 2.0, 2.0);
		sf2d_draw_texture(img_paddle, 350, 110);
		sf2d_draw_texture(img_waveform, 40, 97);
		hidScanInput();
		kDown = hidKeysDown();
		if (kDown & (KEY_SELECT | KEY_A | KEY_B | KEY_X | KEY_Y | KEY_L | KEY_R | KEY_ZL | KEY_ZR | KEY_START)) break;
		sf2d_end_frame();
		sf2d_swapbuffers();
	}
	if (kDown & (KEY_SELECT | KEY_A | KEY_B | KEY_X | KEY_Y | KEY_L | KEY_R | KEY_ZL | KEY_ZR)) return 2;
	if (kDown & KEY_START) return 3;
	return 4;
}

int level_designer() {
	int designed_level[50] = {
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	};
	int current_spot = 0;
	while (true) {
		sf2d_start_frame(GFX_TOP, GFX_LEFT);
		int array_step = 0;
		for (int a = 0; a < 5; a++)
		{
			for (int b = 0; b < 10; b++)
			{
				if (!designed_level[array_step] == 0)
					sf2d_draw_texture(game_textures[brick_texture_by_type[designed_level[array_step]]], (40 * b) + 2, (20 * a) + 2);
				if (array_step == current_spot)
					sf2d_draw_texture(game_textures[19], (40 * b) + 20, (20 * a) + 8);
				array_step++;
			}
		}
		sf2d_end_frame();
		hidScanInput();
		kDown = hidKeysDown();
		if (kDown & KEY_UP && current_spot >= 10)
			current_spot -= 10;
		if (kDown & KEY_DOWN && current_spot <= 39)
			current_spot += 10;
		if (kDown & KEY_LEFT && current_spot > 0)
			current_spot--;
		if (kDown & KEY_RIGHT && current_spot < 49)
			current_spot++;
		if (kDown & KEY_R) {
			if (designed_level[current_spot] == 10)
				designed_level[current_spot] = 0;
			else
				designed_level[current_spot]++;
		}
		if (kDown & KEY_L) {
			if (designed_level[current_spot] == 0)
				designed_level[current_spot] = 10;
			else
				designed_level[current_spot]--;
		}
		if (kDown & (KEY_START | KEY_B))
			break;
		std::cout << ANSI "0;0" PEND;
		for (int i = 0; i < 30; i++)
			std::cout << "                              ";
		std::cout << ANSI "0;0" PEND;
		std::cout << "Levellayout:\n";
		std::cout << designed_level[0] << ", " << designed_level[1] << ", " << designed_level[2] << ", " << designed_level[3] << ", " << designed_level[4] << ", " << designed_level[5] << ", " << designed_level[6] << ", " << designed_level[7] << ", " << designed_level[8] << ", " << designed_level[9] << "\n";
		std::cout << designed_level[10] << ", " << designed_level[11] << ", " << designed_level[12] << ", " << designed_level[13] << ", " << designed_level[14] << ", " << designed_level[15] << ", " << designed_level[16] << ", " << designed_level[17] << ", " << designed_level[18] << ", " << designed_level[19] << "\n";
		std::cout << designed_level[20] << ", " << designed_level[21] << ", " << designed_level[22] << ", " << designed_level[23] << ", " << designed_level[24] << ", " << designed_level[25] << ", " << designed_level[26] << ", " << designed_level[27] << ", " << designed_level[28] << ", " << designed_level[29] << "\n";
		std::cout << designed_level[30] << ", " << designed_level[31] << ", " << designed_level[32] << ", " << designed_level[33] << ", " << designed_level[34] << ", " << designed_level[35] << ", " << designed_level[36] << ", " << designed_level[37] << ", " << designed_level[38] << ", " << designed_level[39] << "\n";
		std::cout << designed_level[40] << ", " << designed_level[41] << ", " << designed_level[42] << ", " << designed_level[43] << ", " << designed_level[44] << ", " << designed_level[45] << ", " << designed_level[46] << ", " << designed_level[47] << ", " << designed_level[48] << ", " << designed_level[49] << "\n";
		sf2d_swapbuffers();
	}
	return 0;
}
