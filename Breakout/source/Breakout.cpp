#include "Breakout.h"

//init
int debugTF = 1;
char versiontxtt[8] = " Alpha ";
char versiontxtn[9] = " 1.00.0 ";
int vernumqik = 0;

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

class mRectangle {
public:
	int x;
	int y;
	int width;
	int height;
	int default_x;
	int default_y;
	int default_width;
	int default_height;
	int setDefaults(int i_x, int i_y, int i_width, int i_height)
	{
		x = i_x;
		default_x = i_x;
		y = i_y;
		default_y = i_y;
		width = i_width;
		default_width = i_width;
		height = i_height;
		default_height = i_height;
		return 0;
	}
	int reset()
	{
		x = default_x;
		y = default_y;
		width = default_width;
		height = default_height;
		return 0;
	}
};

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

	mRectangle paddle;
	paddle.setDefaults(175, 215, 50, 10);

	// Main loop
	while (aptMainLoop())
	{
		hidScanInput();
		u32 kDown = hidKeysDown();
		u32 kHeld = hidKeysHeld();
		if (kDown & KEY_START) break; // break in order to return to hbmenu
		if (kHeld & KEY_LEFT) paddle.width--;
		if (kHeld & KEY_RIGHT) paddle.width++;
		if (kHeld & KEY_UP) paddle.height--;
		if (kHeld & KEY_DOWN) paddle.height++;
		if (kHeld & KEY_Y) paddle.x--;
		if (kHeld & KEY_A) paddle.x++;
		if (kHeld & KEY_X) paddle.y--;
		if (kHeld & KEY_B) paddle.y++;

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