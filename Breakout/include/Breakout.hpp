#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <string>
using std::string;
#include <sys/stat.h>
#include <sys/types.h>
#include <iostream>
#include <fstream>
#define _USE_MATH_DEFINES
#include <cmath>
#include <3ds.h>
#include <citro3d.h>
#include "gfx/pp2d.h"

#include <vector>

extern PrintConsole bottomScreen, versionWin, killBox, debugBox;

#define texture_count 56
#define brick_types 11
#define brick_sprites 15
#define powerup_types 4
#define ball_sprites 7
#define def_level_count 5
#define SAVE_FILES 3

extern std::string versiontxtt, versiontxtn;
extern std::string buildnumber, ishupeversion;
extern std::vector<FILE*> saved_level;
extern int designed_level[SAVE_FILES][50];
extern std::string saved_level_filename[SAVE_FILES];
extern int vernumqik;

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
