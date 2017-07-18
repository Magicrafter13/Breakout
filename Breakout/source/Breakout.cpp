#include "Breakout.hpp"
#include "shapes.hpp"
#include "draw.hpp"

//init
int debugTF = 1;
char versiontxtt[8] = " Alpha ";
char versiontxtn[9] = " 1.02.0 ";
int vernumqik = 0;

int breakout();
int lives = 3;
mRectangle paddle;
brick brick_array[50];

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

PrintConsole topScreen, bottomScreen, versionWin, killBox;

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
	//hidInit();
	//gfxInitDefault();
	sf2d_init();
	sf2d_set_3D(0);
	//fsInit();
	//sdmcInit();
	consoleInit(GFX_BOTTOM, &bottomScreen);
	consoleInit(GFX_BOTTOM, &versionWin);
	consoleInit(GFX_BOTTOM, &killBox);
	
	consoleSetWindow(&versionWin, 6, 26, 34, 2);
	consoleSetWindow(&killBox, 0, 28, 40, 2);

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

	paddle.setDefaults(175, 215, 50, 10, 0xC0, 0x61, 0x0A, 0xFF);
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
			int result = 3;
			sf2d_swapbuffers();
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
		sf2d_draw_rectangle(paddle.x, paddle.y, paddle.width, paddle.height, RGBA8(0xC0, 0x61, 0x0A, 0xFF)); //Brown rectangle to be paddle
		sf2d_draw_texture(img_title, 80, 20);
		sf2d_end_frame();

		std::cout << ANSI "0;0" PEND "                                                            ";
		std::cout << ANSI "1;0" PEND "                                                            ";
		std::cout << ANSI "0;0" PEND "X = " << paddle.x << " Y = " << paddle.y << " Width = " << paddle.width << " Height = " << paddle.height;
		std::cout << ANSI "1;0" PEND "H-Mid = " << (paddle.x + (paddle.width / 2)) << "   H-Mid-Length = " << (paddle.width / 2);
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

int breakout()
{
	if (lives == 0)
		return 2;
	hidScanInput();
	u32 kDown = hidKeysDown();
	if (kDown & KEY_SELECT)
		return 2;
	u32 kHeld = hidKeysHeld();
	if (kHeld & KEY_START)
		return 3;
	if (kHeld & KEY_LEFT)
		if (paddle.x > 1)
			paddle.x -= 2;
	if (kHeld & KEY_RIGHT)
		if (paddle.x < 399 - paddle.width)
			paddle.x += 2;
	if (kDown & KEY_A)
		std::cout << "what was I gonna add here... oh yeah color change, not important :p\n";
	sf2d_start_frame(GFX_TOP, GFX_LEFT);
	draw_rect(paddle);
	for (int i = 0; i < 50; i++)
		draw_brick(brick_array[i]);
	sf2d_end_frame();
	sf2d_swapbuffers();
	return 0;
}
