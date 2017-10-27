#pragma once
#include "Breakout.hpp"

/*
Creates Save Files
[Dynamic, based upon header definition SAVE_FILES]
int setup_type:
0 if old save level file (breakout_level.bsl)
1 if no save files ever (0 renames breakout_level to breakout_level_0)
*/
void create_save_files(int setup_type);

/*
Loads save files into memory
[save level array/vector]
*/
void load_save_files();
