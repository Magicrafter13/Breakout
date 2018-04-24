#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <string>
//using std::string;
#include <sys/stat.h>
#include <sys/types.h>
#include <iostream>
#include <fstream>
#define _USE_MATH_DEFINES
#include <cmath>
#include <3ds.h>
#include <citro3d.h>
#include "gfx/pp2d.h"
#include <limits>
#include <vector>

extern PrintConsole bottomScreen, versionWin, killBox, debugBox;

#define texture_count 62
#define brick_types 12
#define powerup_types 4
#define ball_sprites 7
#define def_level_count 6
#define SAVE_FILES 3
#define AMOUNT_OF_POWERUPS 5

#define O3DS_CHECKS 250
#define N3DS_CHECKS 300
#define BRICK_FILES 16

extern std::string versiontxtt, versiontxtn;
extern std::string buildnumber, ishupeversion;
extern std::vector<FILE*> saved_level;
extern int designed_level[SAVE_FILES][50];
extern std::string saved_level_filename[SAVE_FILES];
extern int vernumqik;
extern size_t ball00ID, paddleBigID, paddleSmallID;
extern std::vector<size_t> pLaserID, pLifeID, pMultiBallID, pPaddleBigID, pPaddleSmallID;

#define ANSI "\x1b["
#define RED "31"
#define B_RED "41"
#define GREEN "32"
#define B_GREEN "42"
#define YELLOW "33"
#define B_YELLOW "43"
#define BLUE "34"
#define B_BLUE "44"
#define MAGENTA "35"
#define B_MAGENTA "45"
#define CYAN "36"
#define B_CYAN "46"
#define WHITE "37"
#define B_WHITE "47"
#define BRIGHT "2"
#define CRESET "\x1b[0m"
#define PRESET "\x1b[0;0H"
#define ASEP ";"
#define CEND "m"
#define PEND "H"

// For level creation

#define lvlFullLine(a) a, a, a, a, a, a, a, a, a, a
