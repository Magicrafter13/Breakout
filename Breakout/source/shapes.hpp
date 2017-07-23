class mRectangle{
public:
	double x;
	double y;
	double width;
	double height;
	u32 color;
	double default_x;
	double default_y;
	double default_width;
	double default_height;
	u32 default_color;
	void setDefaults(double i_x, double i_y, double i_width, double i_height, int R, int G, int B, int A)
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
	void setPosition(double sx, double sy)
	{
		x = sx;
		y = sy;
	}
	void setSize(double swidth, double sheight)
	{
		width = swidth;
		height = sheight;
	}
};

class brick{
public:
	bool exists;
	mRectangle brick_mrect;
	void setDefaults(double bx, double by, double bwidth, double bheight, int R, int G, int B, int A)
	{
		brick_mrect.setDefaults(bx, by, bwidth, bheight, R, G, B, A);
		exists = true;
	}
};

class mCircle{
public:
	double x;
	double y;
	double rad;
	u32 color;
	double default_x;
	double default_y;
	double default_rad;
	u32 default_color;
	void setDefaults(double i_x, double i_y, double i_rad, int R, int G, int B, int A)
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
	void setPosition(double sx, double sy)
	{
		x = sx;
		y = sy;
	}
	void setRadius(double srad)
	{
		rad = srad;
	}
};

class mTriangle {
public:
	double x1;
	double y1;
	double x2;
	double y2;
	double x3;
	double y3;
	u32 color;
	double default_x1;
	double default_y1;
	double default_x2;
	double default_y2;
	double default_x3;
	double default_y3;
	u32 default_color;
	void setDefaults(double i_x1, double i_y1, double i_x2, double i_y2, double i_x3, double i_y3, int R, int G, int B, int A)
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
	void setPosition(double sx1, double sy1, double sx2, double sy2, double sx3, double sy3)
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
	void setDefaults(double bx, double by, double brad, int R, int G, int B, int A, double tx1, double ty1, double tx2, double ty2, double tx3, double ty3, int TR, int TG, int TB, int TA)
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
	double getLeft(bool X)
	{
		if (X)
			return ball_mcirc.x - ball_mcirc.rad;
		else
			return ball_mcirc.y;
	}
	double getRight(bool X)
	{
		if (X)
			return ball_mcirc.x + ball_mcirc.rad;
		else
			return ball_mcirc.y;
	}
	double getTop(bool X)
	{
		if (X)
			return ball_mcirc.x;
		else
			return ball_mcirc.y - ball_mcirc.rad;
	}
	double getBottom(bool X) //true for X false for Y
	{
		if (X)
			return ball_mcirc.x;
		else
			return ball_mcirc.y + ball_mcirc.rad;
	}
	void move(double dx, double dy)
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
	void setDefaults(double bx, double by, double bwidth, double bheight, int R, int G, int B, int A)
	{
		paddle_mrect.setDefaults(bx, by, bwidth, bheight, R, G, B, A);
	}
	void reset()
	{
		paddle_mrect.reset();
	}
	double getLeft(bool X)
	{
		if (X)
			return paddle_mrect.x;
		else
			return paddle_mrect.y + (paddle_mrect.height / 2.0);
	}
	double getRight(bool X)
	{
		if (X)
			return paddle_mrect.x + paddle_mrect.width;
		else
			return paddle_mrect.y + (paddle_mrect.height / 2.0);
	}
	double getTop(bool X)
	{
		if (X)
			return paddle_mrect.x + (paddle_mrect.width / 2.0);
		else
			return paddle_mrect.y;
	}
	double getBottom(bool X)
	{
		if (X)
			return paddle_mrect.x + (paddle_mrect.width / 2.0);
		else
			return paddle_mrect.y + paddle_mrect.height;
	}
};
