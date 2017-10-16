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

#include <sf2d.h>
#include <sfil.h>
#include <sftd.h>

#include <vector>

extern PrintConsole topScreen, bottomScreen, versionWin, killBox, debugBox;

#include "Title_png.h"
#include "thanksbeta_png.h"
#include "waveform_png.h"

#include "paddle_png.h"
#include "brick00_png.h"
#include "brick01_png.h"
#include "brick02_png.h"
#include "brick03_png.h"
#include "brick04_png.h"
#include "brick05_png.h"
#include "brick06_png.h"
#include "brick07_png.h"
#include "brick08_png.h"
#include "brick09_png.h"
#include "ball00_png.h"

#include "ethnocen_ttf.h"

extern char versiontxtt[8];
extern char versiontxtn[9];
extern char buildnumber[14];
extern char ishupeversion[9];
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
