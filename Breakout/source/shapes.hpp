//#include "Breakout.hpp"
class mRectangle {
public:
	int x;
	int y;
	int width;
	int height;
	int color;
	int default_x;
	int default_y;
	int default_width;
	int default_height;
	int default_color;
	void setDefaults(int i_x, int i_y, int i_width, int i_height, int R, int G, int B, int A)
	{
		x = i_x;
		default_x = i_x;
		y = i_y;
		default_y = i_y;
		width = i_width;
		default_width = i_width;
		height = i_height;
		default_height = i_height;
		color = RGBA8(R, G, B, A);
		default_color = RGBA8(R, G, B, A);
	}
	void reset()
	{
		x = default_x;
		y = default_y;
		width = default_width;
		height = default_height;
		color = default_color;
	}
	void setColor(int R, int G, int B, int A)
	{
		color = RGBA8(R, G, B, A);
	}
	void setPosition(int sx, int sy)
	{
		x = sx;
		y = sy;
	}
};

class brick {
public:
	bool exists;
	mRectangle brick_mrect;
	void setDefaults(int bx, int by, int bwidth, int bheight, int R, int G, int B, int A)
	{
		brick_mrect.setDefaults(bx, by, bwidth, bheight, R, G, B, A);
		exists = true;
	}
};
