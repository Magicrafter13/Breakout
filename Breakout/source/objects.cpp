#include "Breakout.hpp"
#include "objects.hpp"

size_t brick_texture_by_type[brick_types] = {
	0,
	1, 2, 3, 4, 5,
	10, 11, 12, 13, 14,
	0
};

size_t brick_second_texture_by_type[brick_types] = {
	0,
	0, 0, 0, 0, 0,
	0, 6, 7, 8, 9,
	0
};
int brick_point_value[brick_types] = {
	0,
	0, 10, 15, 20, 25,
	0, 20, 30, 40, 50,
	1000000
};

std::vector<size_t*> ball_texture_id = {
	0, &ball00ID
};

std::vector<std::vector<size_t>*> powerup_texture_id = {
	&pLaserID,
	&pPaddleBigID,
	&pPaddleSmallID,
	&pLifeID,
	&pMultiBallID
};

size_t stZ = 0;
