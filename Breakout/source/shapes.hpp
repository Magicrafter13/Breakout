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
	void setSize(int swidth, int sheight)
	{
		width = swidth;
		height = sheight;
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

class mCircle {
public:
	int x;
	int y;
	int rad;
	u32 color;
	int default_x;
	int default_y;
	int default_rad;
	u32 default_color;
	void setDefaults(int i_x, int i_y, int i_rad, int R, int G, int B, int A)
	{
		x = i_x;
		default_x = i_x;
		y = i_y;
		default_y = i_y;
		rad = i_rad;
		default_rad = i_rad;
		color = RGBA8(R, G, B, A);
		default_color = RGBA8(R, G, B, A);
	}
	void reset()
	{
		x = default_x;
		y = default_y;
		rad = default_rad;
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
	void setRadius(int srad)
	{
		rad = srad;
	}
};

class mTriangle {
public:
	float x1;
	float y1;
	float x2;
	float y2;
	float x3;
	float y3;
	u32 color;
	float default_x1;
	float default_y1;
	float default_x2;
	float default_y2;
	float default_x3;
	float default_y3;
	u32 default_color;
	void setDefaults(float i_x1, float i_y1, float i_x2, float i_y2, float i_x3, float i_y3, int R, int G, int B, int A)
	{
		x1 = i_x1;
		default_x1 = i_x1;
		y1 = i_y1;
		default_y1 = i_y1;
		x2 = i_x2;
		default_x2 = i_x2;
		y2 = i_y2;
		default_y2 = i_y2;
		x3 = i_x3;
		default_x3 = i_x3;
		y3 = i_y3;
		default_y3 = i_y3;
		color = RGBA8(R, G, B, A);
		default_color = RGBA8(R, G, B, A);
	}
	void reset()
	{
		x1 = default_x1;
		y1 = default_y1;
		x2 = default_x2;
		y2 = default_y2;
		x3 = default_x3;
		y3 = default_y3;
		color = default_color;
	}
	void setColor(int R, int G, int B, int A)
	{
		color = RGBA8(R, G, B, A);
	}
	void setPosition(float sx1, float sy1, float sx2, float sy2, float sx3, float sy3)
	{
		x1 = sx1;
		y1 = sy1;
		x2 = sx2;
		y2 = sy2;
		x3 = sx3;
		y3 = sy3;
	}
};

class ball {
public:
	bool exists;
	mCircle ball_mcirc;
	mTriangle ball_mtri;
	void setDefaults(int bx, int by, int brad, int R, int G, int B, int A, float tx1, float ty1, float tx2, float ty2, float tx3, float ty3, int TR, int TG, int TB, int TA)
	{
		ball_mcirc.setDefaults(bx, by, brad, R, G, B, A);
		ball_mtri.setDefaults(tx1, ty1, tx2, ty2, tx3, ty3, TR, TG, TB, TA);
		exists = true;
	}
	int getLeft()
	{
		return ball_mcirc.x;
	}
	int getRight()
	{
		return ball_mcirc.x + (ball_mcirc.rad * 2);
	}
	int getTop()
	{
		return ball_mcirc.y;
	}
	int getBottom()
	{
		return ball_mcirc.y + (ball_mcirc.rad * 2);
	}
};
