#include "Breakout.hpp"
#include "objects.hpp"
#include "draw.hpp"
#include "audio/sfx.h"
#include "audio/filesystem.h"
#include "ishupe.hpp"
#include "file/file_access.hpp"

//By Matthew Rease (马太) https://github.com/Magicrafter13/Breakout
//你好中国！我的名字是马太。我是中文一班。对不起，如果我的中文不好。(I did have to use Google translate for the last sentence :/ ).

FILE* debug_file,* settings_file;

//init
std::string versiontxtt = "  Beta ", versiontxtn = "01.07.02";
std::string buildnumber = "18.04.22.1228", ishupeversion = "01.00.00";
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
bool crushBall = false;
int last_power, times_power_1, times_power_2, times_power_3;
int press_select_frame = 0; bool press_select_visible = true;

paddle the_paddle; std::vector<ball> the_ball(1); brick brick_array[def_level_count][50];
SFX_s *ball_bounce[8];
size_t titleID, thanksBetaID, waveformID, paddleID, ball00ID, pressSelectID, laserPaddleID, laserID, paddleBigID, paddleSmallID;
std::vector<size_t> extraBallID, pLaserID(6), pLifeID(6), pMultiBallID(6), pPaddleBigID(6), pPaddleSmallID(6);

bool cMode = false, update_text;

/*integer mask for levels*/
int level_mask[def_level_count][50] = {
	{
		lvlFullLine(0),
		lvlFullLine(0),
		lvlFullLine(0),
		lvlFullLine(0),
		lvlFullLine(0)
	},
	{
		lvlFullLine(5),
		lvlFullLine(4),
		lvlFullLine(3),
		lvlFullLine(2),
		lvlFullLine(1),
	},
	{
		5, 5, 0, 0, 0, 0, 0, 0, 0, 0,
		4, 4, 4, 4, 0, 0, 0, 0, 0, 0,
		3, 3, 3, 3, 3, 3, 0, 0, 0, 0,
		2, 2, 2, 2, 2, 2, 2, 2, 0, 0,
		lvlFullLine(1)
	},
	{
		0,  1, 0, 0, 0, 0, 0, 0,  1, 0,
		0, 10, 0, 0, 0, 0, 0, 0,  6, 0,
		0,  8, 0, 0, 0, 0, 0, 0,  9, 0,
		0,  9, 3, 1, 3, 4, 3, 2,  8, 0,
		0,  6, 3, 3, 5, 3, 2, 2, 10, 0
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
void trail_new_frame(std::vector<ball> &ball_vec)
{
	for (auto &tBall : ball_vec) {
		tBall.trail_x.insert(tBall.trail_x.begin(), tBall.x);
		tBall.trail_y.insert(tBall.trail_y.begin(), tBall.y);
		tBall.trail_x.resize(8); tBall.trail_y.resize(8);
		tBall.trail_x.shrink_to_fit(); tBall.trail_y.shrink_to_fit();
		for (int i = 0; i < 8; i++)
			tBall.trail_circle[i].setPosition(tBall.trail_x[i], tBall.trail_y[i]);
	}
}

/*returns if area is being touched by stylus*/
bool touchInBox(touchPosition touch, int x, int y, int w, int h)
{
	int tx = touch.px;
	int ty = touch.py;
	kDown = hidKeysDown();
	if (kDown & KEY_TOUCH && tx > x && tx < x + w && ty > y && ty < y + h)
		return true;
	else
		return false;
}

void resetBalls(std::vector<ball> &ball_vec) {
	ball_vec.resize(1);
	ball_vec[0].reset();
	setNewBallAngle(ball_vec[0].angle);
	ball_vec[0].is_attached = true;
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
	size_t id;
	for (id = 0; id < BRICK_FILES; id++) {
		std::string temp = "romfs:/sprites/brick/brick";
		if (id < 10) temp += "0";
		temp += std::to_string(id) + ".png";
		pp2d_load_texture_png(id, temp.c_str());
	}
	std::vector<std::string> backgroundS = {
		"press_select.png",
		"thanksbeta.png",
		"Title.png",
		"waveform.png"
	};
	std::vector<size_t*> backgroundI = {
		&pressSelectID,
		&thanksBetaID,
		&titleID,
		&waveformID
	};
	size_t i = id;
	for (id = i; id < i + backgroundS.size(); id++) {
		pp2d_load_texture_png(id, ("romfs:/sprites/background/" + backgroundS[id - i]).c_str());
		*backgroundI[id - i] = id;
	}
	std::vector<std::string> miscS {
		"laser_paddle.png",
		"laser_trail.png"
	};
	std::vector<size_t*> miscI {
		&laserPaddleID,
		&laserID
	};
	i = id;
	for (id = i; id < i + miscS.size(); id++) {
		pp2d_load_texture_png(id, ("romfs:/sprites/misc/" + miscS[id - i]).c_str());
		*miscI[id - i] = id;
	}
	i = id;
	int extra_balls = 7;
	extraBallID.resize(extra_balls);
	for (id = i; id < i + extra_balls; id++) {
		pp2d_load_texture_png(id, ("romfs:/sprites/misc/extra_ball/ball" + (((id - i + 1) < 10) ? ("0" + std::to_string((id - i + 1))) : (std::to_string((id - i + 1)))) + ".png").c_str());
		extraBallID[id - i] = id;
	}
	std::vector<std::string> powerupS {
		"laser",
		"life",
		"multi_ball",
		"paddle_big",
		"paddle_small"
	};
	std::vector<std::vector<size_t>*> powerupI = {
		&pLaserID,
		&pLifeID,
		&pMultiBallID,
		&pPaddleBigID,
		&pPaddleSmallID
	};
	for (int f = 0; f < AMOUNT_OF_POWERUPS; f++) {
		i = id;
		powerupI[f]->resize(0);
		for (id = i; id < i + 6; id++) {
			pp2d_load_texture_png(id, ("romfs:/sprites/powerup/" + powerupS[f] + (((id - i) < 10) ? ("0" + std::to_string((id - i))) : (std::to_string((id - i)))) + ".png").c_str());
			powerupI[f]->push_back(id);
		}
	}
	std::vector<std::string> rootS = {
		"ball00.png",
		"paddle.png",
		"paddle_big.png",
		"paddle_small.png"
	};
	std::vector<size_t*> rootI = {
		&ball00ID,
		&paddleID,
		&paddleBigID,
		&paddleSmallID
	};
	i = id;
	for (id = i; id < i + 4; id++) {
		pp2d_load_texture_png(id, ("romfs:/sprites/" + rootS[id - i]).c_str());
		*rootI[id - i] = id;
	}
};

/*initialize audio*/
void initialize_audio() {
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
				brick_array[q][array_step].setDefaults((40 * b) + 2, ((20 * a) + 2), 36, 16, (level_mask[q][array_step] != 0), level_mask[q][array_step]);
				array_step++;
			}
	}
}

/*Writes Settings File*/
void writeSettings() {
	settings_file = fopen("sdmc:/3ds/settings.bsf", "w");
	fprintf(settings_file, "%s %d ", "old", (cMode ? 1 : 0));
	fclose(settings_file);
}

void readSettings() {
	char dummy;
	int cModeT;
	settings_file = fopen("sdmc:/3ds/settings.bsf", "r");
	fscanf(settings_file, "%s %d", &dummy, &cModeT);
	fclose(settings_file);
	cMode = (cModeT == 1 ? true : false);
}

/*begin application*/
int main(int argc, char **argv)
{
	debug_file = fopen("sdmc:/3ds/breakout_debug.txt", "w");
	if ((settings_file = fopen("sdmc:/3ds/settings.bsf", "r"))) {
		fclose(settings_file);
		readSettings();
	}
	else {
		writeSettings();
		readSettings();
	}
	settings_file = fopen("sdmc:/3ds/settings.bsf", "r");
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

	the_paddle.setDefaults(175, 215, 50, 10, paddleID);
	the_ball[0].setDefaults(193.0, 193.0, 14.0, 14.0, 1, 11.6, 11.6);
	for (auto &tBall : the_ball)
		for (int i = 0; i < 8; i++)
			tBall.trail_circle[7 - i].setDefaults(200.0, 120.0, (0.875 * (i + 1)));
	
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
		/*Toggle Compatibility Mode*/
		if (kDown & KEY_R) {
			cMode = (cMode ? false : true);
			writeSettings();
			readSettings();
			bottom_screen_text = 0;
		}
		/*begin game*/
		if (kDown & KEY_SELECT) {
			update_text = true;
			lives = 3;
			int result = 3;
			resetBalls(the_ball);
			the_paddle.reset();
			setNewBallAngle(the_ball[0].angle);
			for (int i = 0; i < def_level_count; i++)
				for (int j = 0; j < 50; j++)
					brick_array[i][j].reset();
			level = 1; points = 0; last_power = 0;
			times_power_1 = 0; times_power_2 = 0; times_power_3 = 0;
			the_ball[0].is_attached = true;
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

			std::cout << CRESET ANSI "2;0" PEND "Press Select to begin." << std::endl;
			std::cout << "Press X to see what I'm working on or have planned." << std::endl;
			std::cout << "Press Y to open level editor." << std::endl << std::endl;

			std::cout << "Compatibility Mode: " << (cMode ? "On" : "Off") << std::endl;
			std::cout << "(Turn on if not using a 'New' 3DS/2DS)" << std::endl << "Press R" << std::endl;
			std::cout << "May cause undesireable results on VERY" << std::endl << "rare occasions." << std::endl;

			bottom_screen_text = 1;
		}
		press_select_frame++;

		pp2d_begin_draw(GFX_TOP, GFX_LEFT);
		draw_object(the_paddle);
		pp2d_draw_texture(titleID, 80, 20);
		pp2d_draw_texture(paddleID, 122, 92);
		if (press_select_visible) pp2d_draw_texture(pressSelectID, 100, 180);
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

bool change_level;
int thanks_text_display;
std::string debug_string;

void run_powerup(int typef) {
	switch (typef) {
	case 1:
		the_paddle.remove_powerups();
		the_paddle.has_laser = true;
		the_paddle.texture_id = laserPaddleID;
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
	case 5:
		the_paddle.remove_powerups();
		the_paddle.has_multi = true;
		for (auto tBall : the_ball) {
			if (tBall.exists) the_paddle.spawn_multi(tBall, the_ball);
			break;
		}
		break;
	}
}

int loseLife() {
	lives--;
	the_paddle.reset();
	resetBalls(the_ball);
	update_text = true;
	return 0;
}

/*main game*/
int breakout()
{
	hidScanInput();
	kDown = hidKeysDown(); kHeld = hidKeysHeld();
	if (kDown & KEY_SELECT || lives == 0) return 2;
	if (kHeld & KEY_START) return 3;
	/*move paddle (if applicable)*/
	if ((kHeld | kDown) & (KEY_LEFT | KEY_RIGHT)) movePaddle((kHeld & KEY_LEFT ? false : true), the_paddle, the_ball);
	int balls_on_screen = 0;
	for (auto &tBall : the_ball) {
		if (tBall.getTop(false) > 240) tBall.exists = false;
		if (tBall.exists) balls_on_screen++;
	}
	if (balls_on_screen == 0) return loseLife();

	for (auto &tBall : the_ball) {
		if (tBall.exists) {
			tBall.has_interacted = false;
			tBall.has_hit_paddle = false;
			tBall.has_hit_wall = false;
		}
	}
	bool ball_was_shot = false;
	if (kDown & KEY_A) {
		for (auto &tBall : the_ball) {
			if (tBall.is_attached) {
				tBall.is_attached = false;
				ball_was_shot = true;
				update_text = true;
			}
		}
	}
	if ((kDown | kHeld) & KEY_A && !ball_was_shot) {
		if (the_paddle.has_laser && !the_paddle.laser_on_screen) {
			the_paddle.the_laser.setDefaults(the_paddle.x + (the_paddle.width / 2.0) - (the_paddle.the_laser.width / 2.0), the_paddle.y, 3, 30, laserID);
			the_paddle.laser_on_screen = true;
		}
	}

	if (the_paddle.laser_on_screen) {
		the_paddle.the_laser.move(0, -8);
		int the_number = 30;
		int other_number = the_paddle.y - the_paddle.the_laser.y;
		if (other_number < the_number) the_number = other_number;
		for (int i = 0; i < the_number; i++)
			pp2d_draw_texture(the_paddle.the_laser.texture_id, the_paddle.the_laser.x, the_paddle.the_laser.y + i);
		for (int i = 0; i < 50; i++)
			if (test_collision(the_paddle.the_laser, brick_array[level][i]) && brick_array[level][i].exists) {
				brick_array[level][i].destroy();
				the_paddle.laser_on_screen = false;
			}
		if (off_screen(the_paddle.the_laser)) the_paddle.laser_on_screen = false;
	}
	/*run main engine code if the ball is not attached to the paddle*/
	for (auto &tBall : the_ball) {
		tBall.bricks_hit = 0;
		tBall.has_rotated_this_frame = false;
	}
	change_level = false;
	//main hit detection engine (runs 300 times per frame)
	for (int i = 0; i < (cMode ? O3DS_CHECKS : N3DS_CHECKS); i++) {
		for (auto &tBall : the_ball) {
			if (tBall.exists && !tBall.is_attached) {
				tBall.has_interacted = false;
				tBall.hasHitPadd = (the_paddle.getTop(false) <= tBall.getBottom(false) && (the_paddle.x <= tBall.x && tBall.x <= the_paddle.x + the_paddle.width)); //balls x coordinate is <= paddles x coordinate + it's width
				if (tBall.hasHitPadd && tBall.getBottom(false) >= the_paddle.getTop(false) + 0.02) tBall.hasHitPadd = false;
				if (tBall.hasHitPadd) tBall.has_hit_paddle = true;
				//Add gravity powerup (magnet but different) rotates around paddle until button pressed.
				tBall.hasHitWall = (tBall.getLeft(true) <= 0.00 || tBall.getRight(true) >= 400.00 || tBall.getTop(false) <= 0.00);
				if (tBall.hasHitWall) tBall.has_hit_wall = true;
				//first time ball is detected touching wall
				if (tBall.hasHitWall && !tBall.isInWall) {
					tBall.has_interacted = true;
					tBall.isInWall = true;
					tBall.angle = (tBall.getTop(false) <= 0.00) ? (360.0 - tBall.angle) : (180.0 - tBall.angle);
				}

				setAngleGood(tBall.angle);

				//first time ball is detected touching paddle
				if (tBall.hasHitPadd && !tBall.isInPaddle) {
					tBall.has_interacted = true;
					tBall.isInPaddle = true;
					double paddle_width_ninth = the_paddle.width / 9.0;
					int angle = 1;
					for (double z = 1.0; z < 9.0; z += 1.0)
						if (tBall.getBottom(true) >= the_paddle.x + (paddle_width_ninth * z))
							angle += 1;
					//change ball angle according to the area of paddle hit
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
					tBall.angle = (360.0 - tBall.angle) + angle_of_change;
				}
				//large if statement to determine if a brick has been hit (run once per brick)
				bool hitV = false, hitH = false, hitBothSameTime = false;
				for (int j = 0; j < 50; j++) {
					setAngleGood(tBall.angle);
					if (test_collision(tBall, brick_array[level][j], false) || test_collision(tBall, brick_array[level][j], true)) {
						if (brick_array[level][j].exists) {
							brick_array[level][j].destroy();
							if (!brick_array[level][j].exists) {
								points += brick_array[level][j].point_value();
								last_power = brick_array[level][j].random_powerup();
								if (last_power != 0) brick_array[level][j].spawn_powerup(last_power);
							}
							tBall.bricks_hit++;
							//if brick hit V and H reverse direction
							if (!tBall.inside_brick[j]) {
								/*if ((tBall.mask_x + tBall.mask_width < brick_array[level][j].x + (1 / (cMode ? O3DS_CHECKS : N3DS_CHECKS))) ||
									(tBall.mask_x > brick_array[level][j].x + brick_array[level][j].width - (1 / (cMode ? O3DS_CHECKS : N3DS_CHECKS))))
									hitV = true;
								else if ((tBall.mask_y + tBall.mask_height < brick_array[level][j].y + (1 / (cMode ? O3DS_CHECKS : N3DS_CHECKS))) ||
									(tBall.mask_y > brick_array[level][j].y + brick_array[level][j].height - (1 / (cMode ? O3DS_CHECKS : N3DS_CHECKS))))
									hitH = true;*/
								if ((tBall.mask_x + tBall.mask_width) - brick_array[level][j].x == (tBall.mask_y + tBall.mask_height) - brick_array[level][j].y)
									hitBothSameTime = true;
								else if (tBall.angle >= 0 && tBall.angle < 90) {
									if ((tBall.mask_y + tBall.mask_height) - brick_array[level][j].y < (tBall.mask_x + tBall.mask_width) - brick_array[level][j].x)
										hitH = true;
									else
										hitV = true;
								}
								else if (tBall.angle >= 90 && tBall.angle < 180) {
									if ((tBall.mask_y + tBall.mask_height) - brick_array[level][j].y < (brick_array[level][j].x + brick_array[level][j].width) - tBall.mask_x)
										hitH = true;
									else
										hitV = true;
								}
								else if (tBall.angle >= 180 && tBall.angle < 270) {
									if ((brick_array[level][j].x + brick_array[level][j].width) - tBall.mask_x < (brick_array[level][j].y + brick_array[level][j].height) - tBall.mask_y)
										hitH = true;
									else
										hitV = true;
								}
								else if (tBall.angle >= 270 && tBall.angle < 360) {
									if ((brick_array[level][j].y + brick_array[level][j].height) - tBall.mask_y < (tBall.mask_x + tBall.mask_width) - brick_array[level][j].x)
										hitH = true;
									else
										hitV = true;
								}
								tBall.inside_brick[j] = true;
							}
						}
					}
					else
						tBall.inside_brick[j] = false;
				}
				if ((hitV || hitH) && tBall.has_rotated_this_frame == false) {
					tBall.angle = (hitH ? 180.0 : 360.0) - tBall.angle;
					tBall.has_rotated_this_frame = true;
				}
				setBallDirection(tBall, 2.0);
				if (tBall.hasHitPadd && tBall.isInPaddle)
					moveBall(tBall, cMode);
				else if (tBall.isInPaddle && !tBall.hasHitPadd)
					tBall.isInPaddle = false;
				if (tBall.hasHitWall && tBall.isInWall)
					moveBall(tBall, cMode);
				else if (tBall.isInWall && !tBall.hasHitWall)
					tBall.isInWall = false;
				if (!tBall.has_interacted && !tBall.hasHitPadd && !tBall.hasHitWall && !tBall.isInPaddle && !tBall.isInWall) moveBall(tBall, cMode);
				//if paddle and wall hit in same frame, ball is "crushed" (this could cause problems later)
				if (tBall.hasHitPadd && tBall.hasHitWall) tBall.exists = false;
				int bricks_available = 0;
				for (int brick_array_pos = 0; brick_array_pos < 50; brick_array_pos++)
					if (brick_array[level][brick_array_pos].exists && brick_array[level][brick_array_pos].type() != 11)
						bricks_available++;
				if (bricks_available == 0) change_level = true;
			}
		}
	}
	//either increase level, or go to win screen
	if (change_level == true) {
		if (level == def_level_count - 1) {
			int thanks_return;
			thanks_text_display = 0;
			do (thanks_return = thanks_for_playing_the_beta()); while (thanks_return == 0);
			return thanks_return;
		}
		else {
			level++;
			resetBalls(the_ball);
			the_paddle.reset();
		}
	}
	//to avoid a glitch, if more than one brick is hit on the same frame the direction is reversed
	/*for (auto &tBall : the_ball) {
		if (tBall.bricks_hit > 1) {
			tBall.angle += 180.0;
			setAngleGood(tBall.angle);
		}
	}*/

	/*plays random SFX if a brick has been hit*/
	for (auto &tBall : the_ball) {
		if (tBall.bricks_hit > 0) {
			update_text = true;
			int which_bounce = rand() % 7;
			playSFX(ball_bounce[which_bounce]);
		}
		if (tBall.has_hit_paddle || tBall.has_hit_wall) playSFX(ball_bounce[7]);
	}

	trail_new_frame(the_ball);

	pp2d_begin_draw(GFX_TOP, GFX_LEFT);
	for (int i = 0; i < 50; i++)
		if (brick_array[level][i].has_powerup_on_screen) {
			brick_array[level][i].my_powerup.y += 1;
			if (off_screen(brick_array[level][i].my_powerup)) brick_array[level][i].has_powerup_on_screen = false;
			if (test_collision<powerup,paddle> (brick_array[level][i].my_powerup, the_paddle)) {
				run_powerup(brick_array[level][i].my_powerup.type);
				brick_array[level][i].has_powerup_on_screen = false;
			}
			brick_array[level][i].draw();
		}
	draw_object(the_paddle);
	for (int i = 0; i < 50; i++)
		if (brick_array[level][i].exists)
			draw_object(brick_array[level][i]);
	for (auto tBall : the_ball)
		for (int i = 7; i > 0; i--)
			pp2d_draw_texture_scale(extraBallID[i], (tBall.trail_circle[i].x - tBall.trail_circle[i].rad) + 1.0, (tBall.trail_circle[i].y - tBall.trail_circle[i].rad) + 2.0, (7 - i) / 8.0, (7 - i) / 8.0); //RGBA8(0xFF, 0xFF, 0xFF, 32 * (7 - i))
	for (auto tBall : the_ball)
		draw_object(tBall);
	if (update_text) {
		std::cout << ANSI "13;0" PEND;
		for (int i = 0; i < 3; i++) std::cout << "                                        ";
		std::cout << ANSI "13;0" PEND;
		std::cout << "Score: " << points << "\n" << "Lives: " << lives << "\n";
		std::cout << "Collision being tested " << (cMode ? O3DS_CHECKS : N3DS_CHECKS) << "x/frame." << std::endl;
		std::cout << debug_string << std::endl;
		update_text = false;
	}
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
	pp2d_draw_texture(thanksBetaID, 80, 20);
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
	std::cout << "take many hits, which is determined via\n";
	std::cout << "a number displayed on the brick.\n";
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
		pp2d_draw_texture(paddleID, 350, 110);
		pp2d_draw_texture(waveformID, 40, 97);
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
		hidTouchRead(&touch);
		bool touched = false;
		for (int i = 0; i < SAVE_FILES; i++) {
			if (touchInBox(touch, 112, 32 + (16 * i), 96, 8)) {
				selection = i;
				touched = true;
			}
		}
		if (touched) break;
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
				if (array_step == current_spot) pp2d_draw_texture(ball00ID, (40 * b) + 20, (20 * a) + 8);
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
			if (designed_level[selection][current_spot] == (brick_types - 1))
				designed_level[selection][current_spot] = 0;
			else
				designed_level[selection][current_spot]++;
		}
		if (kDown & KEY_L) {
			if (designed_level[selection][current_spot] == 0)
				designed_level[selection][current_spot] = (brick_types - 1);
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
			for (int i = 0; i < 50; i++)
				level_mask[0][i] = designed_level[selection][i];
			initialize_brick_array();
			lives = 3;
			int result = 3;
			resetBalls(the_ball);
			the_paddle.reset();
			for (int i = 0; i < def_level_count; i++)
				for (int j = 0; j < 50; j++)
					brick_array[i][j].reset();
			level = 0; points = 0; last_power = 0;
			times_power_1 = 0; times_power_2 = 0; times_power_3 = 0;
			for (auto &tBall : the_ball)
				tBall.is_attached = true;
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