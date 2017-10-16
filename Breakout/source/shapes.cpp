#include "Breakout.hpp"
#include "shapes.hpp"

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
int brick_point_value[5] = {
	0, 10, 15, 20, 25
};
sf2d_texture *brick_color_texture[max_textures] = {
	sfil_load_PNG_buffer(brick01_png, SF2D_PLACE_RAM),
	sfil_load_PNG_buffer(brick02_png, SF2D_PLACE_RAM),
	sfil_load_PNG_buffer(brick03_png, SF2D_PLACE_RAM),
	sfil_load_PNG_buffer(brick04_png, SF2D_PLACE_RAM),
	sfil_load_PNG_buffer(brick05_png, SF2D_PLACE_RAM),
	sfil_load_PNG_buffer(brick00_png, SF2D_PLACE_RAM)
};
int brick_texture_id[5] = {
	0, 1, 2, 3, 4
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
sf2d_texture *ball_color_texture[2] = {
	0, sfil_load_PNG_buffer(ball00_png, SF2D_PLACE_RAM)
};
int ball_texture_id[2] = {
	0, 1
};
