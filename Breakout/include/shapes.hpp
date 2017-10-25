#pragma once

class mRectangle {
	double default_x;
	double default_y;
	double default_width;
	double default_height;
public:
	/*current x coordinate*/
	double x;
	/*current y coordinate*/
	double y;
	/*current width*/
	double width;
	/*current height*/
	double height;
	/*
	i_x: X
	i_y: Y
	i_width: Width
	i_height: Height

	Sets the Rectangles default X,Y position, Width, and Height
	*/
	void setDefaults(double i_x, double i_y, double i_width, double i_height)
	{
		x = i_x;
		default_x = i_x;
		y = i_y;
		default_y = i_y;
		width = i_width;
		default_width = i_width;
		height = i_height;
		default_height = i_height;
	}
	/*
	Resets the rectangles position, and size to their defaults
	*/
	void reset()
	{
		x = default_x;
		y = default_y;
		width = default_width;
		height = default_height;
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

extern int brick_texture_by_type[brick_types];
extern int brick_second_texture_by_type[brick_types];
/*point value by brick type*/
extern int brick_point_value[brick_types];

class powerup {
public:
	int x;
	int y;
	int width;
	int height;
	int my_type;
	mRectangle mask;
	int texture_id;
	void setDefaults(int set_x, int set_y, int set_width, int set_height, int set_texture, int type) {
		mask.setDefaults(set_x, set_y, set_width, set_height);
		x = set_x;
		y = set_y;
		width = set_width;
		height = set_height;
		my_type = type;
		texture_id = set_texture;
	}
};

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
					temp_number = rand() % 4;
					return (temp_number + 1);
				}
				return 0;
			}
			case 2: {
				int temp_number = rand() % 20;
				if (temp_number <= 3)
				{
					temp_number = rand() % 4;
					return (temp_number + 1);
				}
				return 0;
			}
			case 3: {
				int temp_number = rand() % 4;
				if (temp_number == 1)
				{
					temp_number = rand() % 4;
					return (temp_number + 1);
				}
				return 0;
			}
			case 4: {
				int temp_number = rand() % 4;
				if (temp_number == 1)
				{
					temp_number = rand() % 4;
					return (temp_number + 1);
				}
				return 0;
			}
		}
		return 0;
	}
public:
	int texture_id;
	int hits_left;
	bool has_powerup_on_screen;
	powerup my_powerup;
	int powerup_timer;
	/*wether or not the brick is currently in play*/
	bool exists;
	/*Rectangle to be used for brick (ie hit detection, think of it as a mask)*/
	mRectangle brick_mrect;
	/*
	bx: Default X
	by: Default Y
	bwidth: Default Width
	bheight: Default Height
	is_used: If false, brick.exist will be false by default
	brick_type: value to be used to determine point value and powerup chance
	
	Sets the default X,Y Coordinate, Width, Height, Palette, Existance, and Brick Type of the Brick
	*/
	void setDefaults(double bx, double by, double bwidth, double bheight, bool is_used, int brick_type)
	{
		brick_mrect.setDefaults(bx, by, bwidth, bheight);
		internal_is_used = is_used;
		if (internal_is_used)
			exists = true;
		else
			exists = false;
		internal_brick_type = brick_type;
		if (!(brick_type == 0))
			texture_id = brick_texture_by_type[brick_type];
		set_hits();
		has_powerup_on_screen = false;
		powerup_timer = 0;
	}
	/*
	Sets the exists value to false thus removing it from play
	or subtracts one from the hits value (if brick requires multiple hits)
	*/
	void destroy()
	{
		if (hits_left <= 1)
			exists = false;
		else {
			hits_left--;
			if (internal_brick_type >= 6 && internal_brick_type <= 10)
				texture_id = brick_second_texture_by_type[internal_brick_type];
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
		texture_id = brick_texture_by_type[internal_brick_type];
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
	/*spawns powerup*/
	void spawn_powerup(int type) {
		switch (type) {
		case 1:
			my_powerup.setDefaults(brick_mrect.x + ((brick_mrect.width - 28) / 2), brick_mrect.y + ((brick_mrect.height - 7) / 2), 18, 7, 29, type);
			break;
		case 4:
			my_powerup.setDefaults(brick_mrect.x + ((brick_mrect.width - 28) / 2), brick_mrect.y + ((brick_mrect.height - 7) / 2), 18, 7, 28, type);
			break;
		}
		has_powerup_on_screen = true;
	}
};

class mCircle {
	double default_x;
	double default_y;
	double default_rad;
public:
	/*X coordinate of ball (center)*/
	double x;
	/*Y coordinate of ball (center)*/
	double y;
	/*ball radius*/
	double rad;
	/*
	Sets the circles default values for:
	i_x: X coord
	i_y: Y coord
	i_rad: radius
	*/
	void setDefaults(double i_x, double i_y, double i_rad)
	{
		x = i_x;
		default_x = i_x;
		y = i_y;
		default_y = i_y;
		rad = i_rad;
		default_rad = i_rad;
	}
	/*resets ball's position and size*/
	void reset()
	{
		x = default_x;
		y = default_y;
		rad = default_rad;
	}
	/*
	set's the ball's position
	sx: new X position
	sy: new Y position
	*/
	void setPosition(double sx, double sy)
	{
		x = sx;
		y = sy;
	}
	/*
	set's the ball's radius
	srad: new radius
	*/
	void setRadius(double srad)
	{
		rad = srad;
	}
};

/*id for textures (or null)*/
extern int ball_texture_id[2];

class ball {
	int internal_ball_type;
public:
	bool exists;
	mCircle ball_mcirc;
	int texture_id;
	/*
	sets ball's default position, size, and type
	*/
	void setDefaults(double bx, double by, double brad, int ball_type)
	{
		ball_mcirc.setDefaults(bx, by, brad);
		internal_ball_type = ball_type;
		if (!(ball_texture_id[ball_type] == 0))
			texture_id = ball_texture_id[ball_type];
		exists = true;
	}
	/*resets the ball to it's default values*/
	void reset()
	{
		ball_mcirc.reset();
	}
	/*
	returns the leftmost side of the ball
	X: (boolean) if true returns X coordinate, if false returns Y coordinate
	*/
	double getLeft(bool X)
	{
		if (X)
			return ball_mcirc.x - ball_mcirc.rad;
		else
			return ball_mcirc.y;
	}
	/*
	returns the rightmost side of the ball
	X: (boolean) if true returns X coordinate, if false returns Y coordinate
	*/
	double getRight(bool X)
	{
		if (X)
			return ball_mcirc.x + ball_mcirc.rad;
		else
			return ball_mcirc.y;
	}
	/*
	returns the upmost side of the ball
	X: (boolean) if true returns X coordinate, if false returns Y coordinate
	*/
	double getTop(bool X)
	{
		if (X)
			return ball_mcirc.x;
		else
			return ball_mcirc.y - ball_mcirc.rad;
	}
	/*
	returns the downmost side of the ball
	X: (boolean) if true returns X coordinate, if false returns Y coordinate
	*/
	double getBottom(bool X) //true for X false for Y
	{
		if (X)
			return ball_mcirc.x;
		else
			return ball_mcirc.y + ball_mcirc.rad;
	}
	/*
	moves the ball to a new position
	dx: horizontal direction
	dy: vertical direction
	*/
	void move(double dx, double dy)
	{
		ball_mcirc.x += dx;
		ball_mcirc.y += dy;
	}
};

class paddle {
public:
	int texture_id;
	mRectangle paddle_mrect;
	/*
	sets the paddle's default position size and texture id
	bx: X position
	by: Y position
	bwidth: paddle width (for collision)
	bheight: paddle height (for collision)
	paddle_texture_id: texture id for paddle
	*/
	void setDefaults(double bx, double by, double bwidth, double bheight, int paddle_texture_id)
	{
		paddle_mrect.setDefaults(bx, by, bwidth, bheight);
		texture_id = paddle_texture_id;
	}
	/*resets the paddle to it's default values*/
	void reset()
	{
		paddle_mrect.reset();
	}
	/*
	returns the leftmost side of the paddle
	X: (boolean) if true returns X coordinate, if false returns Y coordinate
	*/
	double getLeft(bool X)
	{
		if (X)
			return paddle_mrect.x;
		else
			return paddle_mrect.y + (paddle_mrect.height / 2.0);
	}
	/*
	returns the rightmost side of the paddle
	X: (boolean) if true returns X coordinate, if false returns Y coordinate
	*/
	double getRight(bool X)
	{
		if (X)
			return paddle_mrect.x + paddle_mrect.width;
		else
			return paddle_mrect.y + (paddle_mrect.height / 2.0);
	}
	/*
	returns the upmost side of the paddle
	X: (boolean) if true returns X coordinate, if false returns Y coordinate
	*/
	double getTop(bool X)
	{
		if (X)
			return paddle_mrect.x + (paddle_mrect.width / 2.0);
		else
			return paddle_mrect.y;
	}
	/*
	returns the downmost side of the paddle
	X: (boolean) if true returns X coordinate, if false returns Y coordinate
	*/
	double getBottom(bool X)
	{
		if (X)
			return paddle_mrect.x + (paddle_mrect.width / 2.0);
		else
			return paddle_mrect.y + paddle_mrect.height;
	}
};
