#include "Breakout.hpp"
#include "shapes.hpp"
#include "draw.hpp"
#include "audio/sfx.h"
#include "audio/filesystem.h"
#include "ishupe.hpp"
#include "file/file_access.hpp"

//By Matthew Rease https://github.com/Magicrafter13/Breakout

//init
std::string versiontxtt = "  Beta ", versiontxtn = "01.07.00";
std::string buildnumber = "17.10.29.2153", ishupeversion = "00.04.01";
int vernumqik = 0;
u32 kDown, kHeld;

std::vector<FILE*> saved_level(SAVE_FILES);
int designed_level[SAVE_FILES][50];
std::string saved_level_filename[SAVE_FILES];

int breakout();
int thanks_for_playing_the_beta();
int extras_10_13_2017();
int level_designer();
int save_level(int selection);

int lives, points, level;
double ball_dx, ball_dy, ball_angle; std::vector<double> trail_new_frame_x(8), trail_new_frame_y(8), laser_trail_x(29), laser_trail_y(29); bool crushBall = false; bool ball_is_attached;
int last_power, times_power_1, times_power_2, times_power_3;
int press_select_frame = 0; bool press_select_visible = true;

paddle the_paddle; ball the_ball; std::vector<mCircle> trail_new_frame_circle(8); std::vector<laser> trail_new_frame_laser(29); brick brick_array[def_level_count][50];
SFX_s *testsound[1], *ball_bounce[8];

/*integer mask for levels*/
int level_mask[def_level_count][50] = {
	{
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	},
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
	trail_new_frame_x.insert(trail_new_frame_x.begin(), ball_object.ball_mcirc.x);
	trail_new_frame_y.insert(trail_new_frame_y.begin(), ball_object.ball_mcirc.y);
	trail_new_frame_x.resize(8); trail_new_frame_y.resize(8);
	trail_new_frame_x.shrink_to_fit(); trail_new_frame_y.shrink_to_fit();
	for (int i = 0; i < 8; i++) trail_new_frame_circle[i].setPosition(trail_new_frame_x[i], trail_new_frame_y[i]);
}

/*returns if area is being touched by stylus*/
bool touchInBox(touchPosition touch, int x, int y, int w, int h)
{
	int tx = touch.px;
	int ty = touch.py;
	kDown = hidKeysDown();
	if (kDown & KEY_TOUCH && tx > x && tx < x + w && ty > y && ty < y + h) {
		return true;
	}
	else {
		return false;
	}
}

touchPosition touch;

PrintConsole bottomScreen, versionWin, killBox, debugBox;

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

/*initialize textures*/
void init_game_textures() {
	pp2d_load_texture_png(0, "romfs:/sprites/brick/brick00.png");
	pp2d_load_texture_png(1, "romfs:/sprites/brick/brick01.png");
	pp2d_load_texture_png(2, "romfs:/sprites/brick/brick02.png");
	pp2d_load_texture_png(3, "romfs:/sprites/brick/brick03.png");
	pp2d_load_texture_png(4, "romfs:/sprites/brick/brick04.png");
	pp2d_load_texture_png(5, "romfs:/sprites/brick/brick05.png");
	pp2d_load_texture_png(6, "romfs:/sprites/brick/brick06.png");
	pp2d_load_texture_png(7, "romfs:/sprites/brick/brick07.png");
	pp2d_load_texture_png(8, "romfs:/sprites/brick/brick08.png");
	pp2d_load_texture_png(9, "romfs:/sprites/brick/brick09.png");
	pp2d_load_texture_png(10, "romfs:/sprites/brick/brick10.png");
	pp2d_load_texture_png(11, "romfs:/sprites/brick/brick11.png");
	pp2d_load_texture_png(12, "romfs:/sprites/brick/brick12.png");
	pp2d_load_texture_png(13, "romfs:/sprites/brick/brick13.png");
	pp2d_load_texture_png(14, "romfs:/sprites/brick/brick14.png");
	pp2d_load_texture_png(15, "romfs:/sprites/background/Title.png");
	pp2d_load_texture_png(16, "romfs:/sprites/background/thanksbeta.png");
	pp2d_load_texture_png(17, "romfs:/sprites/background/waveform.png");
	pp2d_load_texture_png(18, "romfs:/sprites/ball00.png");
	pp2d_load_texture_png(19, "romfs:/sprites/paddle.png");
	pp2d_load_texture_png(20, "romfs:/sprites/ball01.png");
	pp2d_load_texture_png(21, "romfs:/sprites/ball02.png");
	pp2d_load_texture_png(22, "romfs:/sprites/ball03.png");
	pp2d_load_texture_png(23, "romfs:/sprites/ball04.png");
	pp2d_load_texture_png(24, "romfs:/sprites/ball05.png");
	pp2d_load_texture_png(25, "romfs:/sprites/ball06.png");
	pp2d_load_texture_png(26, "romfs:/sprites/ball07.png");
	pp2d_load_texture_png(27, "romfs:/sprites/background/press_select.png");
	pp2d_load_texture_png(28, "romfs:/sprites/powerup/life00.png");
	pp2d_load_texture_png(29, "romfs:/sprites/powerup/laser00.png");
	pp2d_load_texture_png(30, "romfs:/sprites/misc/laser_trail.png");
	pp2d_load_texture_png(31, "romfs:/sprites/paddle_big.png");
	pp2d_load_texture_png(32, "romfs:/sprites/paddle_small.png");
	pp2d_load_texture_png(33, "romfs:/sprites/powerup/paddle_big00.png");
	pp2d_load_texture_png(34, "romfs:/sprites/powerup/paddle_small00.png");
	pp2d_load_texture_png(35, "romfs:/sprites/misc/laser_paddle.png");
};

/*initialize audio*/
void initialize_audio() {
	testsound[0] = createSFX("romfs:/testfile.raw", SOUND_FORMAT_16BIT);
	for (int i = 0; i < 8; i++) {
		std::string filename = "romfs:/bounce" + std::to_string(i) + ".raw";
		ball_bounce[i] = createSFX(filename.c_str(), SOUND_FORMAT_16BIT);
	}
}

/*create level layout or something*/
void initialize_brick_array() {
	for (int q = 0; q < def_level_count; q++) {
		int array_step = 0;
		for (int a = 0; a < 5; a++)
			for (int b = 0; b < 10; b++) {
				if (level_mask[q][array_step] == 0)
					brick_array[q][array_step].setDefaults((40 * b) + 2, ((20 * a) + 2), 36, 16, false, 0);
				else
					brick_array[q][array_step].setDefaults((40 * b) + 2, ((20 * a) + 2), 36, 16, true, level_mask[q][array_step]);
				array_step++;
			}
	}
}

/*begin application*/
int main(int argc, char **argv)
{
	pp2d_init();
	pp2d_set_screen_color(GFX_TOP, ABGR8(255, 149, 149, 149));
	romfsInit();
	csndInit();
	initSound();
	init_game_textures();

	if (FILE *file = fopen("sdmc:/3ds/breakout_level.bsl", "r")) {
		fclose(file);
		create_save_files(0);
	}
	if (FILE *file = fopen("sdmc:/3ds/breakout_level_0.bsl", "r")) fclose(file);
	else create_save_files(1);

	load_save_files();
	initialize_audio();
	initialize_brick_array();
	
	srand(time(NULL));
	
	consoleInit(GFX_BOTTOM, &bottomScreen); consoleInit(GFX_BOTTOM, &versionWin);
	consoleInit(GFX_BOTTOM, &killBox); consoleInit(GFX_BOTTOM, &debugBox);

	consoleSetWindow(&versionWin, 6, 26, 34, 4);
	consoleSetWindow(&killBox, 0, 28, 40, 2);
	consoleSetWindow(&debugBox, 18, 4, 9, 12);

	for (int i = 0; i < 8; i++) trail_new_frame_circle[7 - i].setDefaults(200.0, 120.0, (0.875 * (i + 1)));

	the_paddle.setDefaults(175, 215, 50, 10, 19);
	the_ball.setDefaults(200.0, 200.0, 7.0, 1);
	
	int bottom_screen_text = 0;

	// Main loop
	while (aptMainLoop()) {
		hidScanInput();
		kDown = hidKeysDown();
		if (kDown & KEY_START) break; // break in order to return to hbmenu
		/*go to extras when X pressed*/
		if (kDown & KEY_X) {
			int ext_return = extras_10_13_2017();
			if (ext_return == 3) break;
			bottom_screen_text = 0;
		}
		/*level designer*/
		if (kDown & KEY_Y) {
			level_designer();
			bottom_screen_text = 0;
		}
		/*begin game*/
		if (kDown & KEY_SELECT) {
			lives = 3;
			int result = 3;
			the_ball.reset();
			the_paddle.reset();
			ball_angle = 0.0;
			do ball_angle = rand() % 360; while (ball_angle < 225.0 || ball_angle > 315.0 || (ball_angle > 265 && ball_angle < 275));
			for (int i = 0; i < def_level_count; i++)
				for (int j = 0; j < 50; j++)
					brick_array[i][j].reset();
			level = 1; points = 0; last_power = 0;
			times_power_1 = 0; times_power_2 = 0; times_power_3 = 0;
			ball_is_attached = true;
			/*main breakout loop*/
			while (true) {
				result = breakout();
				if (result != 0) break;
			}
			bottom_screen_text = 0;
			if (result == 3) break;
		}
		/*text display (run once to avoid screen tearing)*/
		if (bottom_screen_text == 0) {
			consoleSelect(&killBox); consoleClear();
			consoleSelect(&versionWin); consoleClear();
			consoleSelect(&bottomScreen); consoleClear();

			consoleSelect(&killBox);
			std::cout << CRESET ANSI B_RED CEND;
			for (int i = 0; i < 80; i++)
				std::cout << " ";

			consoleSelect(&versionWin);
			std::cout << CRESET "     Tap red area any time to exit";
			std::cout << "Breakout Version: " ANSI RED CEND << versiontxtt << CRESET " " ANSI YELLOW CEND << versiontxtn;
			std::cout << ANSI B_RED ASEP GREEN CEND "              Build: " << buildnumber;
			std::cout << ANSI B_RED ASEP GREEN CEND "   ISHUPE Engine Version: " << ishupeversion;

			consoleSelect(&bottomScreen);
			std::cout << CRESET ANSI "20;0" PEND;

			std::cout << CRESET ANSI "2;0" PEND "Press Select to begin.\n";
			std::cout << "Press X to see what I'm working on or have planned.\n";
			std::cout << "Press Y to open level editor.\n";

			bottom_screen_text = 1;
		}
		press_select_frame++;

		pp2d_begin_draw(GFX_TOP, GFX_LEFT);
		draw_object(the_paddle);
		pp2d_draw_texture(15, 80, 20);
		pp2d_draw_texture(19, 122, 92);
		if (press_select_visible) pp2d_draw_texture(27, 100, 180);
		pp2d_end_draw();

		/*after half a second, Press Select to play! is toggled*/
		if (press_select_frame == 30) {
			press_select_frame = 0;
			if (press_select_visible)
				press_select_visible = false;
			else
				press_select_visible = true;
		}

		hidTouchRead(&touch);

		/*exit game if red box touched*/
		if (touchInBox(touch, 0, 224, 320, 16)) {
			std::cout << "Exiting...\n";
			break;
		}
	}

	// Exit services
	for (int i = 0; i < SAVE_FILES; i++) fclose(saved_level[i]);

	pp2d_exit();

	exitSound();

	return 0;
}

bool hitWallH = false, hitWallV = false;
bool hasHitPadd = false, hasHitWall = false;
bool isMovingRight = false;
bool isInPaddle = false, isInWall = false;
bool brickHitV = false, brickHitH = false;
//int angle;
int bricks_hit_this_frame;
bool change_level;
int thanks_text_display;
bool has_hit_paddle, has_hit_wall;

void run_powerup(int typef) {
	switch (typef) {
	case 1:
		the_paddle.remove_powerups();
		the_paddle.has_laser = true;
		the_paddle.texture_id = 35;
		break;
	case 2:
		the_paddle.remove_powerups();
		the_paddle.has_big = true;
		the_paddle.getBig();
		break;
	case 3:
		the_paddle.remove_powerups();
		the_paddle.has_small = true;
		the_paddle.getSmall();
		break;
	case 4:
		lives++;
		break;
	}
}

/*main game*/
int breakout()
{
	hidScanInput();
	kDown = hidKeysDown(); kHeld = hidKeysHeld();
	/*if (kDown & KEY_R)
		level++;
	if (kDown & KEY_L)
		level--;*/
	if (kDown & KEY_SELECT || lives == 0) return 2;
	if (kHeld & KEY_START) return 3;
	/*move paddle left (if applicable)*/
	if (kHeld & KEY_LEFT && the_paddle.paddle_mrect.x > 1) {
		the_paddle.paddle_mrect.x -= 3;
		if (ball_is_attached) the_ball.move(-3.0, 0.0);
	}
	/*move paddle right (if applicable)*/
	if (kHeld & KEY_RIGHT && the_paddle.paddle_mrect.x < 399 - the_paddle.paddle_mrect.width) {
		the_paddle.paddle_mrect.x += 3;
		if (ball_is_attached) the_ball.move(3.0, 0.0);
	}

	/*lose life if outside of game field*/
	if (the_ball.getTop(false) > 240) {
		lives--;
		the_ball.reset(); the_paddle.reset();
		ball_is_attached = true;
		do ball_angle = rand() % 360; while(ball_angle < 225.0 || ball_angle > 315.0 || (ball_angle > 265 && ball_angle < 275));
		return 0;
	}

	bool hasInteracted = false;
	has_hit_paddle = false; has_hit_wall = false;
	if (kDown & KEY_A && ball_is_attached)
		ball_is_attached = false;
	else if ((kDown | kHeld) & KEY_A && !ball_is_attached) {
		if (the_paddle.has_laser && !the_paddle.laser_on_screen) {
			the_paddle.the_laser.setDefaults(the_paddle.paddle_mrect.x + (the_paddle.paddle_mrect.width / 2.0) - (the_paddle.the_laser.width / 2.0), the_paddle.paddle_mrect.y, 3, 30, 30);
			the_paddle.laser_on_screen = true;
		}
	}
	if (the_paddle.laser_on_screen) {
		the_paddle.the_laser.setPosition(the_paddle.the_laser.x, the_paddle.the_laser.y - 8);
		//trail_new_frame(the_paddle.the_laser);
		int the_number = 30;
		int other_number = the_paddle.paddle_mrect.y - the_paddle.the_laser.y;
		if (other_number < the_number) the_number = other_number;
		for (int i = 0; i < the_number; i++)
			pp2d_draw_texture(the_paddle.the_laser.texture_id, the_paddle.the_laser.x, the_paddle.the_laser.y + i);
		for (int i = 0; i < the_paddle.the_laser.height; i++) pp2d_draw_texture(the_paddle.the_laser.texture_id, trail_new_frame_laser[i].x, trail_new_frame_laser[i].y);
		for (int i = 0; i < 50; i++)
			if (test_collision(the_paddle.the_laser, brick_array[level][i].brick_mrect) && brick_array[level][i].exists) {
				brick_array[level][i].destroy();
				the_paddle.laser_on_screen = false;
			}
		if (off_screen(the_paddle.the_laser))
			the_paddle.laser_on_screen = false;
	}
	/*run main engine code if the ball is not attached to the paddle*/
	if (!ball_is_attached) {
		bricks_hit_this_frame = 0;
		change_level = false;
		/*main hit detection engine (runs 300 times per frame)*/
		for (int i = 0; i < 300; i++) {
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
			if (hasHitWall && !isInWall) {
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
			if (hasHitPadd && !isInPaddle) {
				hasInteracted = true;
				isInPaddle = true;
				double paddle_width_ninth = the_paddle.paddle_mrect.width / 9.0;
				int angle = 1;
				for (double z = 1.0; z < 9.0; z += 1.0)
					if (the_ball.getBottom(true) >= the_paddle.paddle_mrect.x + (paddle_width_ninth * z))
						angle += 1;
				/*change ball angle according to the area of paddle hit*/
				double angle_of_change = 0.0;
				switch (angle) {
				case 4: angle_of_change -= 10.0;
				case 3: angle_of_change -= 10.0;
				case 2: angle_of_change -= 10.0;
				case 1: angle_of_change -= 10.0;
					break;
				case 9: angle_of_change += 10.0;
				case 8: angle_of_change += 10.0;
				case 7: angle_of_change += 10.0;
				case 6: angle_of_change += 10.0;
					break;
				}
				ball_angle = (360.0 - ball_angle) + angle_of_change;
			}
			/*large if statement to determine if a brick has been hit (run once per brick)*/
			for (int j = 0; j < 50; j++) {
				if (test_collision(the_ball.ball_mcirc, brick_array[level][j].brick_mrect, false)) brickHitV = true;
				if (test_collision(the_ball.ball_mcirc, brick_array[level][j].brick_mrect, true)) brickHitH = true;
				/*code if brick(s) hit*/
				if (brickHitV || brickHitH) {
					/*check if brick is actually in play*/
					if (brick_array[level][j].exists) {
						brick_array[level][j].destroy();
						if (!brick_array[level][j].exists) {
							points += brick_array[level][j].point_value();
							last_power = brick_array[level][j].random_powerup();
							if (last_power != 0) brick_array[level][j].spawn_powerup(last_power);
							if (last_power == 1) times_power_1++;
							if (last_power == 2) times_power_2++;
							if (last_power == 3) times_power_3++;
						}
						bricks_hit_this_frame++;
						/*if brick hit V and H reverse direction*/
						if (brickHitV && brickHitH) {
							ball_angle -= 180.0;
							if (ball_angle < 0.0) ball_angle += 360.0;
						}
						else if (brickHitV)
							ball_angle = -ball_angle + 360.0;
						else if (brickHitH)
							ball_angle = -ball_angle + 180.0;
					}
					brickHitV = false; brickHitH = false;
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
			if (hasHitPadd && hasHitWall) {
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
			if (bricks_available == 0) change_level = true;
		}
		/*either increase level, or go to win screen*/
		if (change_level == true) {
			if (level == def_level_count - 1) {
				int thanks_return;
				thanks_text_display = 0;
				do (thanks_return = thanks_for_playing_the_beta()); while (thanks_return == 0);
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
		if (bricks_hit_this_frame > 1) {
			ball_angle += 180.0;
			if (ball_angle > 360.0)
				ball_angle -= 360.0;
		}
	}

	/*plays random SFX if a brick has been hit*/
	if (bricks_hit_this_frame > 0 && !ball_is_attached) {
		int which_bounce = rand() % 7;
		playSFX(ball_bounce[which_bounce]);
	}
	if (has_hit_paddle || has_hit_wall) playSFX(ball_bounce[7]);

	trail_new_frame(the_ball);

	pp2d_begin_draw(GFX_TOP, GFX_LEFT);
	for (int i = 0; i < 50; i++)
		if (brick_array[level][i].has_powerup_on_screen) {
			brick_array[level][i].my_powerup.y += 1;
			if (off_screen(brick_array[level][i].my_powerup.mask)) brick_array[level][i].has_powerup_on_screen = false;
			if (test_collision<powerup,mRectangle> (brick_array[level][i].my_powerup, the_paddle.paddle_mrect)) {
				run_powerup(brick_array[level][i].my_powerup.my_type);
				brick_array[level][i].has_powerup_on_screen = false;
			}
			pp2d_draw_texture(brick_array[level][i].my_powerup.texture_id, brick_array[level][i].my_powerup.x, brick_array[level][i].my_powerup.y);
		}
	draw_object(the_paddle);
	for (int i = 0; i < 50; i++)
		if (brick_array[level][i].exists)
			draw_object(brick_array[level][i]);
	for (int i = 7; i > 0; i--)
		pp2d_draw_texture_scale(27 - i, (trail_new_frame_circle[i].x - trail_new_frame_circle[i].rad) + 1.0, (trail_new_frame_circle[i].y - trail_new_frame_circle[i].rad) + 2.0, (7 - i) / 8.0, (7 - i) / 8.0); //RGBA8(0xFF, 0xFF, 0xFF, 32 * (7 - i))
	draw_object(the_ball);
	std::cout << ANSI "13;0" PEND;
	for (int i = 0; i < 2; i++) std::cout << "                                        ";
	std::cout << ANSI "13;0" PEND;
	std::cout << "Score: " << points << "\n"; std::cout << "Lives: " << lives << "\n";
	pp2d_end_draw();
	hidTouchRead(&touch);

	/*exit game if red box touched*/
	if (touchInBox(touch, 0, 224, 320, 16)) {
		std::cout << "Exiting...\n";
		return 2;
	}
	return 0;
}

/*win screen for the beta*/
int thanks_for_playing_the_beta() {
	hidScanInput();
	kDown = hidKeysDown(); kHeld = hidKeysHeld();
	if (kDown & (KEY_SELECT | KEY_A | KEY_B | KEY_X | KEY_Y | KEY_L | KEY_R | KEY_ZL | KEY_ZR)) return 2;
	if (kHeld & KEY_START) return 3;
	pp2d_begin_draw(GFX_TOP, GFX_LEFT);
	pp2d_draw_texture(16, 80, 20);
	/*run text display once (to avoid screen tear)*/
	if (thanks_text_display == 0) {
		std::cout << ANSI "0;0" PEND;
		for (int i = 0; i < 30; i++) std::cout << "                                        ";
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
	pp2d_end_draw();
	return 0;
}

/*extras: created 10/13/2017*/
int extras_10_13_2017()
{
	consoleSelect(&bottomScreen);
	consoleClear();
	std::cout << ANSI "0;0" PEND;
	for (int i = 0; i < 30; i++) std::cout << "                                        ";
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
	/*main loop with pp2d drawing*/
	while (true)
	{
		pp2d_begin_draw(GFX_TOP, GFX_LEFT);
		pp2d_draw_texture_scale(0, 20, 20, 2.0, 2.0);
		pp2d_draw_texture(19, 350, 110);
		pp2d_draw_texture(17, 40, 97);
		hidScanInput();
		kDown = hidKeysDown();
		if (kDown & (KEY_SELECT | KEY_A | KEY_B | KEY_X | KEY_Y | KEY_L | KEY_R | KEY_ZL | KEY_ZR | KEY_START)) break;
		pp2d_end_draw();
	}
	if (kDown & (KEY_SELECT | KEY_A | KEY_B | KEY_X | KEY_Y | KEY_L | KEY_R | KEY_ZL | KEY_ZR)) return 2;
	if (kDown & KEY_START) return 3;
	return 4;
}

/*level designer*/
int level_designer() {
	int selection = 0;
	bool quit = false;
	consoleSelect(&bottomScreen);
	consoleClear();
	std::cout << CRESET;
	std::cout << ANSI "5;15" PEND ANSI WHITE CEND;
	std::cout << "Save Level 1" CRESET;
	for (int i = 1; i < SAVE_FILES; i++) {
		int value_a = (5 + (i * 2));
		int value_b = (i + 1);
		std::string part_1 = ANSI + std::to_string(value_a) + ";15" PEND;
		std::cout << part_1 << ANSI WHITE ASEP BRIGHT CEND;
		std::cout << "Save Level " << std::to_string(value_b) << CRESET;
	}
	while (true) {
		hidScanInput();
		kDown = hidKeysDown();
		if ((kDown & KEY_UP) && selection > 0) selection--;
		if ((kDown & KEY_DOWN) && selection < SAVE_FILES - 1) selection++;
		if (kDown & (KEY_UP | KEY_DOWN)) {
			std::cout << CRESET;
			for (int i = 0; i < SAVE_FILES; i++) {
				int value_a = ((5 + (i * 2)) - 1);
				std::string part_1 = ANSI + std::to_string(value_a) + ";15" PEND;
				int value_b = value_a + 1;
				std::string part_2 = ANSI + std::to_string(value_b) + ";15" PEND;
				std::cout << part_1 << CRESET << part_2;
				if (selection == i)
					std::cout << ANSI WHITE CEND;
				else
					std::cout << ANSI WHITE ASEP BRIGHT CEND;
				int value_c = i + 1;
				std::cout << "Save Level " << std::to_string(value_c) << CRESET;
			}
		}
		if (kDown & KEY_A) break;
		if (kDown & (KEY_B | KEY_START)) {
			quit = true;
			break;
		}
		gspWaitForVBlank();
	}
	if (quit) return 0;
	int current_spot = 0;
	bool refresh_screen = true;
	while (true) {
		pp2d_begin_draw(GFX_TOP, GFX_LEFT);
		int array_step = 0;
		for (int a = 0; a < 5; a++)
			for (int b = 0; b < 10; b++)
			{
				if (!designed_level[selection][array_step] == 0) pp2d_draw_texture(brick_texture_by_type[designed_level[selection][array_step]], (40 * b) + 2, (20 * a) + 2);
				if (array_step == current_spot) pp2d_draw_texture(18, (40 * b) + 20, (20 * a) + 8);
				array_step++;
			}
		pp2d_end_draw();
		hidScanInput();
		kDown = hidKeysDown();
		if (kDown & KEY_UP && current_spot >= 10) current_spot -= 10;
		if (kDown & KEY_DOWN && current_spot <= 39) current_spot += 10;
		if (kDown & KEY_LEFT && current_spot > 0) current_spot--;
		if (kDown & KEY_RIGHT && current_spot < 49) current_spot++;
		if (kDown & KEY_R) {
			if (designed_level[selection][current_spot] == 10)
				designed_level[selection][current_spot] = 0;
			else
				designed_level[selection][current_spot]++;
		}
		if (kDown & KEY_L) {
			if (designed_level[selection][current_spot] == 0)
				designed_level[selection][current_spot] = 10;
			else
				designed_level[selection][current_spot]--;
		}
		if (kDown & (KEY_START | KEY_B)) break;
		if (kDown & KEY_X) {
			save_level(selection);
			refresh_screen = true;
		}
		if (kDown & KEY_Y) {
			//randomize level
			std::cout << "Are you sure?\nPress A to randomize level\nPress B to cancel\n";
			while (true) {
				hidScanInput();
				kDown = hidKeysDown();
				if (kDown & KEY_A)
				{
					for (int i = 0; i < 50; i++)
					{
						int chance_block_type = rand() % 3;
						int chance_type_more = rand() % 5;
						if (chance_block_type == 0) designed_level[selection][i] = 0;
						if (chance_block_type == 1) designed_level[selection][i] = chance_type_more + 1;
						if (chance_block_type == 2) designed_level[selection][i] = chance_type_more + 6;
					}
					break;
				}
				if (kDown & KEY_B) break;
				gspWaitForVBlank();
			}
		}
		if (kDown & KEY_SELECT) {
			for (int i = 0; i < 50; i++) level_mask[0][i] = designed_level[selection][i];
			initialize_brick_array();
			lives = 3;
			int result = 3;
			the_ball.reset();
			the_paddle.reset();
			ball_angle = 0.0;
			do ball_angle = rand() % 360; while (ball_angle < 225.0 || ball_angle > 315.0 || (ball_angle > 265 && ball_angle < 275));
			for (int i = 0; i < def_level_count; i++)
				for (int j = 0; j < 50; j++)
					brick_array[i][j].reset();
			level = 0; points = 0; last_power = 0;
			times_power_1 = 0; times_power_2 = 0; times_power_3 = 0;
			ball_is_attached = true;
			/*main breakout loop*/
			while (true) {
				result = breakout();
				if (result != 0) break;
			}
			if (result == 3) break;
		}
		if (kDown & (KEY_L | KEY_R)) refresh_screen = true;
		if (refresh_screen) {
			consoleClear();
			std::cout << CRESET ANSI "0;0" PEND ANSI WHITE CEND;
			for (int i = 0; i < 30; i++) std::cout << "                              ";
			std::cout << ANSI "0;0" PEND;
			std::cout << "Levellayout:\n";
			for (int a = 0; a < 5; a++) {
				for (int b = 0; b < 10; b++) {
					if (designed_level[selection][b + (a * 10)] > 9)
						std::cout << designed_level[selection][b + (a * 10)];
					else
						std::cout << " " << designed_level[selection][b + (a * 10)];
					if (b < 9) std::cout << ", ";
				}
				std::cout << "\n";
			}
			std::cout << "                  Press X to save level.";
			std::cout << "    Press B or Start to return to title.";
			std::cout << "        Press Select to play your level!";
			refresh_screen = false;
		}
	}
	return 0;
}

/*save designed level*/
int save_level(int selection) {
	std::cout << "Are you sure you want to save this?\n";
	std::cout << "A to save B to return";
	while (true) {
		hidScanInput();
		kDown = hidKeysDown();
		if (kDown & KEY_B) break;
		if (kDown & KEY_A) {
			fclose(saved_level[selection]);
			remove(saved_level_filename[selection].c_str());
			saved_level[selection] = fopen(saved_level_filename[selection].c_str(), "w");
			for (int i = 0; i < 50; i++) fprintf(saved_level[selection], "%d\n", designed_level[selection][i]);
			fclose(saved_level[selection]);
			saved_level[selection] = fopen(saved_level_filename[selection].c_str(), "r");
			break;
		}
	}
	return 0;
}