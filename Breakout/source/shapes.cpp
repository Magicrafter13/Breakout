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
