#include "Breakout.hpp"
#include "shapes.hpp"

int brick_texture_by_type[brick_types] = {
	0, 1, 2, 3, 4, 5, 10, 11, 12, 13, 14
};

int brick_second_texture_by_type[brick_types] = {
	0, 0, 0, 0, 0, 0, 0, 6, 7, 8, 9
};
int brick_point_value[brick_types] = {
	0, 0, 10, 15, 20, 25, 0, 20, 30, 40, 50
};
int ball_texture_id[2] = {
	0, 18
};

std::vector<std::vector<int>> powerup_texture_id = {
	{41, 42, 43, 44, 45},
	{46, 47, 48, 49, 50},
	{51, 52, 53, 54, 55},
	{36, 37, 38, 39, 40}
};
