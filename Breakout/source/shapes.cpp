#include "Breakout.hpp"
#include "shapes.hpp"

void init_game_textures() {
	pp2d_load_texture_png(0, "romfs:/sprites/brick/brick00.png");
	pp2d_load_texture_png(1, "romfs:/sprites/brick/brick01.png");
	pp2d_load_texture_png(2, "romfs:/sprites/brick/brick02.png");
	pp2d_load_texture_png(3, "romfs:/sprites/brick/brick03.png");
	pp2d_load_texture_png(4, "romfs:/sprites/brick/brick04.png");
	pp2d_load_texture_png(5, "romfs:/sprites/brick/brick05.png");
	pp2d_load_texture_png(6, "romfs:/sprites/brick/brick06.png");
	pp2d_load_texture_png(7, "romfs:/sprites/brick/brick07.png");
	pp2d_load_texture_png(8, "romfs:/sprites/brick/brick08.png");
	pp2d_load_texture_png(9, "romfs:/sprites/brick/brick09.png");
	pp2d_load_texture_png(10, "romfs:/sprites/brick/brick10.png");
	pp2d_load_texture_png(11, "romfs:/sprites/brick/brick11.png");
	pp2d_load_texture_png(12, "romfs:/sprites/brick/brick12.png");
	pp2d_load_texture_png(13, "romfs:/sprites/brick/brick13.png");
	pp2d_load_texture_png(14, "romfs:/sprites/brick/brick14.png");
	pp2d_load_texture_png(15, "romfs:/sprites/background/Title.png");
	pp2d_load_texture_png(16, "romfs:/sprites/background/thanksbeta.png");
	pp2d_load_texture_png(17, "romfs:/sprites/background/waveform.png");
	pp2d_load_texture_png(18, "romfs:/sprites/ball00.png");
	pp2d_load_texture_png(19, "romfs:/sprites/paddle.png");
	pp2d_load_texture_png(20, "romfs:/sprites/ball01.png");
	pp2d_load_texture_png(21, "romfs:/sprites/ball02.png");
	pp2d_load_texture_png(22, "romfs:/sprites/ball03.png");
	pp2d_load_texture_png(23, "romfs:/sprites/ball04.png");
	pp2d_load_texture_png(24, "romfs:/sprites/ball05.png");
	pp2d_load_texture_png(25, "romfs:/sprites/ball06.png");
	pp2d_load_texture_png(26, "romfs:/sprites/ball07.png");
	pp2d_load_texture_png(27, "romfs:/sprites/background/press_select.png");
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
	0, 18
};
