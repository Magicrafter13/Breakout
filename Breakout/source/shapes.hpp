#pragma once

class mRectangle {
	double default_x;
	double default_y;
	double default_width;
	double default_height;
	u32 default_color;
public:
	/*current x coordinate*/
	double x;
	/*current y coordinate*/
	double y;
	/*current width*/
	double width;
	/*current height*/
	double height;
	/*32-bit unsigned integer representing color (in RGBA8 Format)*/
	u32 color;
	/*
	i_x: X
	i_y: Y
	i_width: Width
	i_height: Height
	R: [R]GB Value
	G: R[G]B Value
	B: RG[B] value
	A: RGB Alpha Value (Transparency)

	Sets the Rectangles default X,Y position, Width, Height, and RGBA8 color
	*/
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
	/*
	Resets the rectangles position, size, and color to their defaults
	*/
	void reset()
	{
		x = default_x;
		y = default_y;
		width = default_width;
		height = default_height;
		color = default_color;
	}
	/*
	R: [R]GB Value
	G: R[G]B Value
	B: RG[B] Value
	A: RGB Alpha Value (Transparency)

	Sets the rectangles color using RGBA8
	*/
	void setColor(int R, int G, int B, int A)
	{
		color = RGBA8(R, G, B, A);
	}
	/*
	sx: New X value
	sy: New Y value
	
	Sets the rectangles position
	*/
	void setPosition(double sx, double sy)
	{
		x = sx;
		y = sy;
	}
	/*
	swidth: New Width value
	sheight: New Height value
	
	Sets the rectangles size
	*/
	void setSize(double swidth, double sheight)
	{
		width = swidth;
		height = sheight;
	}
};

extern sf2d_texture *game_textures[19];
extern int brick_texture_by_type[11];
extern int brick_second_texture_by_type[11];
extern int brick_palette_by_type[11];
/*RGB Red channel by brick type*/
extern int brick_color_R[5];
/*RGB Green channel by brick type*/
extern int brick_color_G[5];
/*RGB Blue channel by brick type*/
extern int brick_color_B[5];
/*RGB Alpha channel by brick type*/
extern int brick_color_A[5];
/*point value by brick type*/
extern int brick_point_value[11];
/*for arithmetic*/
#define max_textures 6

class brick {
private:
	int internal_brick_type;
	void set_hits() {
		switch (internal_brick_type) {
		case 0: hits_left = 0;
			break;
		case 1:
		case 2:
		case 3:
		case 4:
		case 5: hits_left = 1;
			break;
		case 6:
		case 7:
		case 8:
		case 9:
		case 10: hits_left = 2;
			break;
		}
	}
	bool internal_is_used;
	int get_powerup()
	{
		switch (internal_brick_type)
		{
			case 0: return 0;
			case 1: {
				int temp_number = rand() % 20;
				if (temp_number <= 3)
				{
					temp_number = rand() % 3;
					return (temp_number + 1);
				}
				return 0;
			}
			case 2: {
				int temp_number = rand() % 20;
				if (temp_number <= 3)
				{
					temp_number = rand() % 3;
					return (temp_number + 1);
				}
				return 0;
			}
			case 3: {
				int temp_number = rand() % 4;
				if (temp_number == 1)
				{
					temp_number = rand() % 3;
					return (temp_number + 1);
				}
				return 0;
			}
			case 4: {
				int temp_number = rand() % 4;
				if (temp_number == 1)
				{
					temp_number = rand() % 3;
					return (temp_number + 1);
				}
				return 0;
			}
		}
		return 0;
	}
public:
	sf2d_texture *public_texture;
	int hits_left;
	bool uses_texture;
	/*wether or not the brick is currently in play*/
	bool exists;
	/*Rectangle to be used for brick (ie hit detection, think of it as a mask)*/
	mRectangle brick_mrect;
	/*
	bx: Default X
	by: Default Y
	bwidth: Default Width
	bheight: Default Height
	palette: Default palette
	is_used: If false, brick.exist will be false by default
	brick_type: value to be used to determine point value and powerup chance
	
	Sets the default X,Y Coordinate, Width, Height, Palette, Existance, and Brick Type of the Brick
	*/
	void setDefaults(double bx, double by, double bwidth, double bheight, bool is_used, int brick_type)
	{
		brick_mrect.setDefaults(bx, by, bwidth, bheight, brick_color_R[brick_palette_by_type[brick_type]], brick_color_G[brick_palette_by_type[brick_type]], brick_color_B[brick_palette_by_type[brick_type]], brick_color_A[brick_palette_by_type[brick_type]]);
		internal_is_used = is_used;
		if (internal_is_used)
			exists = true;
		else
			exists = false;
		internal_brick_type = brick_type;
		if (brick_type == 0)
			uses_texture = false;
		else
		{
			uses_texture = true;
			public_texture = game_textures[brick_texture_by_type[brick_type]];
		}
		set_hits();
	}
	/*
	Sets the exists value to false thus removing it from play
	*/
	void destroy()
	{
		if (hits_left <= 1)
			exists = false;
		else {
			hits_left--;
			if (internal_brick_type >= 6 && internal_brick_type <= 10)
				public_texture = game_textures[brick_second_texture_by_type[internal_brick_type]];
		}
	}
	/*
	Resets the brick values to their defaults
	*/
	void reset()
	{
		if (internal_is_used)
			exists = true;
		else
			exists = false;
		brick_mrect.reset();
		public_texture = game_textures[brick_texture_by_type[internal_brick_type]];
		set_hits();
	}
	/*
	Integer: Returns the point value given by the brick
	*/
	int point_value()
	{
		return brick_point_value[internal_brick_type];
	}
	/*
	Integer: Returns 0 for no powerup or higher number for powerup drop
	*/
	int random_powerup()
	{
		return get_powerup();
		/* returns 1 for laser powerup
		   returns 2 for bigger paddle
		   returns 3 for smaller paddle
		*/
	}
};

class mCircle {
	double default_x;
	double default_y;
	double default_rad;
	u32 default_color;
public:
	double x;
	double y;
	double rad;
	u32 color;
	/*
	Sets the circles default values for:
	i_x: X coord
	i_y: Y coord
	i_rad: radius
	R: Red Channel
	G: Green Channel
	B: Blue Channel
	A: Alpha Channel
	*/
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
	double default_x1;
	double default_y1;
	double default_x2;
	double default_y2;
	double default_x3;
	double default_y3;
	u32 default_color;
public:
	double x1;
	double y1;
	double x2;
	double y2;
	double x3;
	double y3;
	u32 color;
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

/*RGB Red channel by ball type*/
extern int ball_cir_color_R[2];
/*RGB Red channel by ball type*/
extern int ball_tri_color_R[2];
/*RGB Green channel by ball type*/
extern int ball_cir_color_G[2];
/*RGB Green channel by ball type*/
extern int ball_tri_color_G[2];
/*RGB Blue channel by ball type*/
extern int ball_cir_color_B[2];
/*RGB Blue channel by ball type*/
extern int ball_tri_color_B[2];
/*RGB Alpha channel by ball type*/
extern int ball_cir_color_A[2];
/*RGB Alpha channel by ball type*/
extern int ball_tri_color_A[2];
/*texture db*/
extern sf2d_texture *ball_color_texture[2];
/*id for textures (or null)*/
extern int ball_texture_id[2];

class ball {
	int internal_ball_type;
public:
	bool exists;
	mCircle ball_mcirc;
	mTriangle ball_mtri;
	bool uses_texture;
	int texture_id;
	void setDefaults(double bx, double by, double brad, int ball_type, double tx1, double ty1, double tx2, double ty2, double tx3, double ty3)
	{
		ball_mcirc.setDefaults(bx, by, brad, ball_cir_color_R[ball_type], ball_cir_color_G[ball_type], ball_cir_color_B[ball_type], ball_cir_color_A[ball_type]);
		ball_mtri.setDefaults(tx1, ty1, tx2, ty2, tx3, ty3, ball_tri_color_R[ball_type], ball_tri_color_G[ball_type], ball_tri_color_B[ball_type], ball_tri_color_A[ball_type]);
		internal_ball_type = ball_type;
		if (ball_texture_id[ball_type] == 0)
			uses_texture = false;
		else
		{
			uses_texture = true;
			texture_id = ball_texture_id[ball_type];
		}
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
	sf2d_texture *paddle_texture;
	void setDefaults(double bx, double by, double bwidth, double bheight, int R, int G, int B, int A, sf2d_texture *paddle_texture_set)
	{
		paddle_mrect.setDefaults(bx, by, bwidth, bheight, R, G, B, A);
		paddle_texture = paddle_texture_set;
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
