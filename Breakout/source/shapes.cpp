#include "Breakout.hpp"
#include "shapes.hpp"

sf2d_texture *game_textures[texture_count] = {
	sfil_load_PNG_buffer(brick00_png, SF2D_PLACE_RAM),
	sfil_load_PNG_buffer(brick01_png, SF2D_PLACE_RAM),
	sfil_load_PNG_buffer(brick02_png, SF2D_PLACE_RAM),
	sfil_load_PNG_buffer(brick03_png, SF2D_PLACE_RAM),
	sfil_load_PNG_buffer(brick04_png, SF2D_PLACE_RAM),
	sfil_load_PNG_buffer(brick05_png, SF2D_PLACE_RAM),
	sfil_load_PNG_buffer(brick06_png, SF2D_PLACE_RAM),
	sfil_load_PNG_buffer(brick07_png, SF2D_PLACE_RAM),
	sfil_load_PNG_buffer(brick08_png, SF2D_PLACE_RAM),
	sfil_load_PNG_buffer(brick09_png, SF2D_PLACE_RAM),
	sfil_load_PNG_buffer(brick10_png, SF2D_PLACE_RAM),
	sfil_load_PNG_buffer(brick11_png, SF2D_PLACE_RAM),
	sfil_load_PNG_buffer(brick12_png, SF2D_PLACE_RAM),
	sfil_load_PNG_buffer(brick13_png, SF2D_PLACE_RAM),
	sfil_load_PNG_buffer(brick14_png, SF2D_PLACE_RAM),
	sfil_load_PNG_buffer(Title_png, SF2D_PLACE_RAM),
	sfil_load_PNG_buffer(thanksbeta_png, SF2D_PLACE_RAM),
	sfil_load_PNG_buffer(paddle_png, SF2D_PLACE_RAM),
	sfil_load_PNG_buffer(waveform_png, SF2D_PLACE_RAM),
	sfil_load_PNG_buffer(ball00_png, SF2D_PLACE_RAM)
};

int brick_texture_by_type[brick_types] = {
	0, 1, 2, 3, 4, 5, 10, 11, 12, 13, 14
};

int brick_second_texture_by_type[brick_types] = {
	0, 0, 0, 0, 0, 0, 0, 6, 7, 8, 9
};

int brick_palette_by_type[brick_types] = {
	0, 4, 3, 2, 1, 0, 4, 3, 2, 1, 0
};

int brick_color_R[5] = {
	0xFF, 0xFF, 0xFF, 0x00, 0x00
};
int brick_color_G[5] = {
	0x00, 0x80, 0xFF, 0xFF, 0x00
};
int brick_color_B[5] = {
	0x00, 0x00, 0x00, 0x00, 0xFF
};
int brick_color_A[5] = {
	0xFA, 0xFA, 0xFA, 0xFA, 0xFA
}; //250 instead of 255. I want to the bricks to fade out by 25 each frame (lasting 10 frames)
int brick_point_value[brick_types] = {
	0, 0, 10, 15, 20, 25, 0, 20, 30, 40, 50
};

int ball_cir_color_R[2] = {
	0xC3, 0x00
};
int ball_tri_color_R[2] = {
	0xFF, 0x00
};
int ball_cir_color_G[2] = {
	0xC3, 0x00
};
int ball_tri_color_G[2] = {
	0xFF, 0x00
};
int ball_cir_color_B[2] = {
	0xC3, 0x00
};
int ball_tri_color_B[2] = {
	0xFF, 0x00
};
int ball_cir_color_A[2] = {
	0xFF, 0x00
};
int ball_tri_color_A[2] = {
	0xFF, 0x00
};
int ball_texture_id[2] = {
	0, 19
};
