#include "Breakout.hpp"
#include "file/file_access.hpp"

void create_save_files(int setup_type) {
	for (int i = 0; i < SAVE_FILES; i++)
		saved_level_filename[i] = "sdmc:/3ds/breakout_level_" + std::to_string(i) + ".bsl";
	if (setup_type == 0) {
		rename("sdmc:/3ds/breakout_level.bsl", "sdmc:/3ds/breakout_level_0.bsl");
		for (int i = 1; i < SAVE_FILES; i++) {
			saved_level[i] = fopen(saved_level_filename[i].c_str(), "w");
			fprintf(saved_level[i], "0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 ");
			fclose(saved_level[i]);
		}
	}
	if (setup_type == 1) {
		for (int i = 0; i < SAVE_FILES; i++) {
			saved_level[i] = fopen(saved_level_filename[i].c_str(), "w");
			fprintf(saved_level[i], "0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 ");
			fclose(saved_level[i]);
		}
	}
}

void load_save_files() {
	for (int i = 0; i < SAVE_FILES; i++)
		saved_level_filename[i] = "sdmc:/3ds/breakout_level_" + std::to_string(i) + ".bsl";
	for (int i = 0; i < SAVE_FILES; i++) {
		saved_level[i] = fopen(saved_level_filename[i].c_str(), "r");
		for (int j = 0; j < 50; j++)
			fscanf(saved_level[i], "%d ", &designed_level[i][j]);
	}
}