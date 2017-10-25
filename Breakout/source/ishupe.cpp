#include "Breakout.hpp"
#include "ishupe.hpp"
#include "shapes.hpp"

bool off_screen(mCircle object) {
	if ((object.x + object.rad <= 0) ||
		(object.x - object.rad >= 400) ||
		(object.y + object.rad <= 0) ||
		(object.y - object.rad >= 240))
		return true;
	return false;
}