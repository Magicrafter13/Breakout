#pragma once

extern std::vector<std::vector<size_t>*> powerup_texture_id;
extern size_t brick_texture_by_type[brick_types];
extern size_t brick_second_texture_by_type[brick_types];
/*point value by brick type*/
extern int brick_point_value[brick_types];

/*
Creates a rectangle object.
Used for drawing or hit mask

public contains x, y, width, and height
use setDefaults before using the rectangle
*/
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

/*
laser object
for use with a paddle object

public variables are x, y, width, height, texture_id, and mask
use setDefaults before anything else.
*/
class laser {
public:
	/*laser x coordinate*/
	int x;
	/*laser y coordinate*/
	int y;
	/*laser width*/
	int width;
	/*laser height*/
	int height;
	/*laser texture*/
	int texture_id;
	/*laser mask*/
	mRectangle mask;
	/*
	Sets the lasers default values
	
	int set_x: x coordinate
	int set_y: y coordinate
	int set_width: mask width
	int set_height: mask height
	int set_texture: texture_id
	*/
	void setDefaults(int set_x, int set_y, int set_width, int set_height, int set_texture) {
		mask.setDefaults(set_x, set_y, set_width, set_height);
		x = set_x;
		y = set_y;
		width = set_width;
		height = set_height;
		texture_id = set_texture;
	}
	/*
	Sets the lasers position
	(aboslute, not relative)
	
	int set_x: new x coordinate
	int set_y: new y coordinate
	*/
	void setPosition(int set_x, int set_y) {
		x = set_x;
		y = set_y;
		mask.setPosition(set_x, set_y);
	}
};

/*
powerup object
public values: x, y, width, height, powerup type, mask, and texture
use setDefaults before anything else
*/
class powerup {
public:
	/*x coordinate*/
	int x;
	/*y coordinate*/
	int y;
	/*width (mask)*/
	int width;
	/*height (mask)*/
	int height;
	/*powerup type*/
	int my_type;
	/*mask*/
	mRectangle mask;
	/*texture id*/
	size_t texture_id;
	/*texture id array [animation]*/
	std::vector<size_t> animation_id;
	/*frame count*/
	int frame_count;
	/*
	Sets the powerups default values
	
	int set_x: x coordinate
	int set_y: y coordinate
	int set_width: width (mask)
	int set_height: height(mask)
	int set_texture: texture_id
	int type: powerup type
	*/
	void setDefaults(int set_x, int set_y, int set_width, int set_height, size_t set_texture, int type, std::vector<size_t> set_animate, int set_frame_count) {
		mask.setDefaults(set_x, set_y, set_width, set_height);
		x = set_x;
		y = set_y;
		width = set_width;
		height = set_height;
		my_type = type;
		texture_id = set_texture;
		animation_id = set_animate;
		frame_count = set_frame_count;
	}
};

/*
Brick object
public values: texture, remaining hits, (powerup, <- existance <- timer), existance, and mask
*/
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
		case 11: hits_left = std::numeric_limits<int>::max(); // Technically it is possible to destroy these bricks... but come on, it takes like 2+ billion hits so yeah.
		}
	}
	bool internal_is_used;
	int get_powerup()
	{
		switch (internal_brick_type) {
		case 0: return 0;
		case 1:
		case 2: 
		case 6:
		case 7: {
			int temp_number = rand() % 20;
			if (temp_number <= 3)
			{
				temp_number = rand() % 5;
				return (temp_number + 1);
			}
			return 0;
			}
		case 3:
		case 4:
		case 5:
		case 8:
		case 9:
		case 10: {
			int temp_number = rand() % 4;
			if (temp_number == 1)
			{
				temp_number = rand() % 5;
				return (temp_number + 1);
			}
			return 0;
			}
		}
		return 0;
	}
	int wait_frame;
public:
	/*texture*/
	int texture_id;
	/*remaining hits*/
	int hits_left;
	/*whether or not powerup exists*/
	bool has_powerup_on_screen;
	/*individual powerup*/
	powerup my_powerup;
	/*powerup animation timer*/
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
		wait_frame = 0;
	}
	void draw() {
		if (wait_frame == 9) {
			wait_frame = 0;
			powerup_timer++;
		}
		if (powerup_timer >= my_powerup.frame_count)
			powerup_timer = 0;
		pp2d_draw_texture(my_powerup.animation_id[powerup_timer], my_powerup.x, my_powerup.y);
		wait_frame++;
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
		powerup_timer = 0;
		has_powerup_on_screen = false;
		wait_frame = 0;
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
		my_powerup.setDefaults(brick_mrect.x + ((brick_mrect.width - 28) / 2), brick_mrect.y + ((brick_mrect.height - 7) / 2), 18, 7, (*powerup_texture_id[type - 1])[0], type, *powerup_texture_id[type - 1], powerup_texture_id[type - 1]->size());
		has_powerup_on_screen = true;
	}
};

/*
Circle object
Used for drawing* or hit mask

public contains: x, y, and radius
*no longer used for drawing, only for hit mask
*/
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
extern std::vector<size_t*> ball_texture_id;

extern size_t stZ;

/*
Ball object
public values: existance, mask, and texture
*/
class ball {
	int internal_ball_type;
public:
	bool brickHitH, brickHitV;
	double dx, dy;
	std::vector<double> trail_new_frame_x, trail_new_frame_y;
	std::vector<mCircle> trail_new_frame_circle;
	/*whether or not the ball exists*/
	bool exists;
	/*ball angle*/
	double angle;
	/*mask*/
	mCircle ball_mcirc;
	/*texture*/
	size_t texture_id;
	/*Whether or not the ball has interacted this frame*/
	bool has_interacted;
	/*Whether or not the ball has hit the paddle this frame*/
	bool hasHitPadd;
	bool has_hit_paddle;
	/*Whether or not the ball has hit a wall this frame*/
	bool hasHitWall;
	bool has_hit_wall;
	/*Whether or not the ball is attached to the paddle currently*/
	bool is_attached;
	/*Whether or not the ball is in the wall*/
	bool isInWall;
	/*Whether or not the ball is in the paddle*/
	bool isInPaddle;
	/*Bricks hit this frame*/
	int bricks_hit;
	/*
	sets ball's default position, size, and type
	*/
	void setDefaults(double bx, double by, double brad, int ball_type)
	{
		ball_mcirc.setDefaults(bx, by, brad);
		internal_ball_type = ball_type;
		if (!(*ball_texture_id[ball_type] == stZ))
			texture_id = *ball_texture_id[ball_type];
		exists = true;
		trail_new_frame_x.resize(8);
		trail_new_frame_y.resize(8);
		trail_new_frame_circle.resize(8);
	}
	/*resets the ball to it's default values*/
	void reset()
	{
		ball_mcirc.reset();
		exists = true;
		hasHitPadd = false, hasHitWall = false;
		isInPaddle = false, isInWall = false;
		brickHitV = false, brickHitH = false;
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

/*
Paddle object
public values: texture, mask, powerup <- (existance, on_screen), and * <- (whatever powerup object, ie laser or big)
*/
class paddle {
	int default_texture_id;
	double default_width;
	double default_height;
public:
	/*texture*/
	size_t texture_id;
	/*mask*/
	mRectangle paddle_mrect;
	/*whether or not laser ability active*/
	bool has_laser;
	/*whether or not laser on screen*/
	bool laser_on_screen;
	/*whether or not big paddle active*/
	bool has_big;
	/*whether or not small paddle active*/
	bool has_small;
	/*whether or not multi ball*/
	bool has_multi;
	/*laser object*/
	laser the_laser;
	/*multi ball 0 - 3*/
	std::vector<ball> multi_ball;
	/*
	sets the paddle's default position size and texture id
	bx: X position
	by: Y position
	bwidth: paddle width (for collision)
	bheight: paddle height (for collision)
	paddle_texture_id: texture id for paddle
	*/
	void setDefaults(double bx, double by, double bwidth, double bheight, size_t paddle_texture_id)
	{
		paddle_mrect.setDefaults(bx, by, bwidth, bheight);
		texture_id = paddle_texture_id;
		default_texture_id = texture_id;
		default_width = bwidth;
		default_height = bheight;
	}
	/*spawns extra balls*/
	void spawn_multi(ball tBall, std::vector<ball> &ball_vec) {
		ball new_ball_1, new_ball_2;
		new_ball_1.setDefaults(tBall.ball_mcirc.x, tBall.ball_mcirc.y, tBall.ball_mcirc.rad, 1);
		new_ball_2.setDefaults(tBall.ball_mcirc.x, tBall.ball_mcirc.y, tBall.ball_mcirc.rad, 1);
		new_ball_1.angle = tBall.angle - 20.0;
		new_ball_2.angle = tBall.angle + 20.0;
		for (int i = 0; i < 8; i++) {
			new_ball_1.trail_new_frame_circle[7 - i].setDefaults(tBall.ball_mcirc.x, tBall.ball_mcirc.y, (0.875 * (i + 1)));
			new_ball_2.trail_new_frame_circle[7 - i].setDefaults(tBall.ball_mcirc.x, tBall.ball_mcirc.y, (0.875 * (i + 1)));
		}
		new_ball_1.is_attached = false;
		new_ball_2.is_attached = false;
		ball_vec.push_back(new_ball_1);
		ball_vec.push_back(new_ball_2);
	}
	/*runs script for enlarging paddle*/
	void getBig() {
		paddle_mrect.width = 70;
		paddle_mrect.x -= 10;
		if (paddle_mrect.x < 1) paddle_mrect.x = 1;
		if (paddle_mrect.x > 329) paddle_mrect.x = 329;
		texture_id = paddleBigID;
	}
	/*runs script for shrinking paddle*/
	void getSmall() {
		paddle_mrect.width = 30;
		paddle_mrect.x += 10;
		texture_id = paddleSmallID;
	}
	/*removes powerups from paddle*/
	void remove_powerups() {
		if (has_big) {
			paddle_mrect.width = default_width;
			paddle_mrect.x += 10;
		}
		if (has_small) {
			paddle_mrect.width = default_width;
			paddle_mrect.x -= 10;
			if (paddle_mrect.x < 1) paddle_mrect.x = 1;
			if (paddle_mrect.x > 329) paddle_mrect.x = 329;
		}
		has_laser = false;
		has_big = false;
		has_small = false;
		texture_id = default_texture_id;
	}
	/*resets the paddle to it's default values*/
	void reset()
	{
		paddle_mrect.reset();
		has_laser = false;
		has_big = false;
		has_small = false;
		texture_id = default_texture_id;
		laser_on_screen = false;
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
