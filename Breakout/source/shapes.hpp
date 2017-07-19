//#include "Breakout.hpp"
class mRectangle {
public:
	float x;
	float y;
	float width;
	float height;
	u32 color;
	float default_x;
	float default_y;
	float default_width;
	float default_height;
	u32 default_color;
	void setDefaults(float i_x, float i_y, float i_width, float i_height, int R, int G, int B, int A)
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
	void setPosition(float sx, float sy)
	{
		x = sx;
		y = sy;
	}
	void setSize(float swidth, float sheight)
	{
		width = swidth;
		height = sheight;
	}
};

class brick {
public:
	bool exists;
	mRectangle brick_mrect;
	void setDefaults(float bx, float by, float bwidth, float bheight, int R, int G, int B, int A)
	{
		brick_mrect.setDefaults(bx, by, bwidth, bheight, R, G, B, A);
		exists = true;
	}
};

class mCircle {
public:
	float x;
	float y;
	float rad;
	u32 color;
	float default_x;
	float default_y;
	float default_rad;
	u32 default_color;
	void setDefaults(float i_x, float i_y, float i_rad, int R, int G, int B, int A)
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
	void setPosition(float sx, float sy)
	{
		x = sx;
		y = sy;
	}
	void setRadius(float srad)
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
	void setDefaults(float bx, float by, float brad, int R, int G, int B, int A, float tx1, float ty1, float tx2, float ty2, float tx3, float ty3, int TR, int TG, int TB, int TA)
	{
		ball_mcirc.setDefaults(bx, by, brad, R, G, B, A);
		ball_mtri.setDefaults(tx1, ty1, tx2, ty2, tx3, ty3, TR, TG, TB, TA);
		exists = true;
	}
	void reset()
	{
		ball_mcirc.reset();
		ball_mtri.reset();
	}
	int getLeft(bool X)
	{
		if (X)
			return ball_mcirc.x - ball_mcirc.rad;
		else
			return ball_mcirc.y;
	}
	int getRight(bool X)
	{
		if (X)
			return ball_mcirc.x + ball_mcirc.rad;
		else
			return ball_mcirc.y;
	}
	int getTop(bool X)
	{
		if (X)
			return ball_mcirc.x;
		else
			return ball_mcirc.y - ball_mcirc.rad;
	}
	int getBottom(bool X) //true for X false for Y
	{
		if (X)
			return ball_mcirc.x;
		else
			return ball_mcirc.y + ball_mcirc.rad;
	}
	void move(float dx, float dy)
	{
		ball_mcirc.x += dx;
		ball_mtri.x1 += dx;
		ball_mtri.x2 += dx;
		ball_mtri.x3 += dx;
		ball_mcirc.y += dy;
		ball_mtri.y1 += dy;
		ball_mtri.y2 += dy;
		ball_mtri.y3 += dy;
	}
};

class paddle {
public:
	mRectangle paddle_mrect;
	void setDefaults(float bx, float by, float bwidth, float bheight, int R, int G, int B, int A)
	{
		paddle_mrect.setDefaults(bx, by, bwidth, bheight, R, G, B, A);
	}
	void reset()
	{
		paddle_mrect.reset();
	}
	int getLeft(bool X)
	{
		if (X)
			return paddle_mrect.x;
		else
			return paddle_mrect.y + (paddle_mrect.height / 2);
	}
	int getRight(bool X)
	{
		if (X)
			return paddle_mrect.x + paddle_mrect.width;
		else
			return paddle_mrect.y + (paddle_mrect.height / 2);
	}
	int getTop(bool X)
	{
		if (X)
			return paddle_mrect.x + (paddle_mrect.width / 2);
		else
			return paddle_mrect.y;
	}
	int getBottom(bool X)
	{
		if (X)
			return paddle_mrect.x + (paddle_mrect.width / 2);
		else
			return paddle_mrect.y + paddle_mrect.height;
	}
};
