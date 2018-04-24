#pragma once

extern std::vector<std::vector<size_t>*> powerup_texture_id;
extern size_t brick_texture_by_type[brick_types];
extern size_t brick_second_texture_by_type[brick_types];
extern int brick_point_value[brick_types];
extern std::vector<size_t*> ball_texture_id;
extern size_t stZ;

/*
laser object
for use with a paddle object
*/
class laser {
public:
	int x, y;
	int width, height;
	int texture_id;
	/*
	Sets the lasers default values
	
	int fx, fy: coordinates
	int fwidth, fheight: Laser size
	int texture: texture_id
	*/
	void setDefaults(int fx, int fy, int fwidth, int fheight, int texture) {
		x = fx;
		y = fy;
		width = fwidth;
		height = fheight;
		texture_id = texture;
	}
	void setPosition(double X, double Y) {
		x = X;
		y = Y;
	}
	void move(double X, double Y) {
		x += X;
		y += Y;
	}
};

/*
powerup object
*/
class powerup {
	bool defaults_set = false;
	int dX, dY;
	int dWidth, dHeight;
public:
	int x, y;
	int width, height;
	int type;
	std::vector<size_t> animation_id;
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
	void setDefaults(int fx, int fy, int fwidth, int fheight, int ftype, std::vector<size_t> textures) {
		if (!defaults_set) {
			x = fx; dX = fx;
			y = fy; dY = fx;
			width = fwidth; dWidth = fwidth;
			height = fheight; dHeight = fheight;
			type = ftype;
			animation_id = textures;
			frame_count = textures.size();
			defaults_set = true;
		}
	}
	/*
	Resets to defaults values
	*/
	void reset() {
		x = dX;
		y = dY;
		width = dWidth;
		height = dHeight;
	}
	void setPosition(double X, double Y) {
		x = X;
		y = Y;
	}
	void move(double X, double Y) {
		x += X;
		y += Y;
	}
};

/*
Brick object
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
	int wait_frame = 0;
	double dX, dY;
	double dWidth, dHeight;
public:
	double x, y;
	int texture_id;
	int hits_left;
	bool has_powerup_on_screen = false;
	powerup my_powerup;
	int powerup_timer = 0;
	bool exists;
	double width, height;
	/*
	fx: Default X
	fy: Default Y
	fwidth: Default Width
	fheight: Default Height
	is_used: If false, brick.exist will be false by default
	brick_type: value to be used to determine point value and powerup chance
	
	Sets the default X,Y Coordinate, Width, Height, Palette, Existance, and Brick Type of the Brick
	*/
	void setDefaults(double fx, double fy, double fwidth, double fheight, bool is_used, int brick_type)
	{
		x = fx; dX = fx;
		y = fy; dY = fy;
		width = fwidth; dWidth = fwidth;
		height = fheight; dHeight = fheight;
		internal_is_used = is_used;
		exists = is_used;
		internal_brick_type = brick_type;
		if (brick_type != 0) texture_id = brick_texture_by_type[brick_type];
		set_hits();
	}
	int type() {
		return internal_brick_type;
	}
	void draw() {
		if (wait_frame == 9) {
			wait_frame = 0;
			powerup_timer++;
		}
		if (powerup_timer >= my_powerup.frame_count) powerup_timer = 0;
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
			if (internal_brick_type >= 6 && internal_brick_type <= 10) texture_id = brick_second_texture_by_type[internal_brick_type];
		}
	}
	/*
	Resets the brick values to their defaults
	*/
	void reset()
	{
		exists = internal_is_used;
		x = dX;
		y = dY;
		width = dWidth;
		height = dHeight;
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
	1: Laser
	2: Big Paddle
	3: Small Paddle
	4: Life
	5: Multi-Ball
	*/
	int random_powerup()
	{
		return get_powerup();
	}
	/*spawns powerup*/
	void spawn_powerup(int type) {
		my_powerup.setDefaults(x + ((width - 28) / 2), y + ((height - 7) / 2), 18, 7, type, *powerup_texture_id[type - 1]);
		has_powerup_on_screen = true;
	}
	void setPosition(double X, double Y) {
		x = X;
		y = Y;
	}
	void move(double X, double Y) {
		x += X;
		y += Y;
	}
};

/*
Circle:
X, Y, and Radius
*/
struct circle {
	double x, y, rad;
	/*
	Sets the circles default values for:
	i_x: X coord
	i_y: Y coord
	i_rad: radius
	*/
	void setDefaults(double fx, double fy, double frad) {
		x = fx;
		default_x = fx;
		y = fy;
		default_y = fy;
		rad = frad;
		default_rad = frad;
	}
	/*resets circle's position and size*/
	void reset() {
		x = default_x;
		y = default_y;
		rad = default_rad;
	}
	void setPosition(double X, double Y) {
		x = X;
		y = Y;
	}
	void move(double X, double Y) {
		x += X;
		y += Y;
	}
private:
	double default_x, default_y, default_rad;
};

/*
Ball object
*/
class ball {
	int internal_ball_type;
	double dX, dY;
	double dRad;
public:
	double x, y;
	bool brickHitH, brickHitV;
	double dx, dy;
	double rad;
	std::vector<double> trail_x, trail_y;
	std::vector<circle> trail_circle;
	bool exists;
	double angle;
	size_t texture_id;
	bool has_interacted;
	bool hasHitPadd, has_hit_paddle;
	bool hasHitWall, has_hit_wall;
	bool is_attached;
	bool isInWall, isInPaddle;
	int bricks_hit;
	/*
	sets ball's default position, size, and type
	*/
	void setDefaults(double fx, double fy, double frad, int ball_type)
	{
		x = fx; dX = fx;
		y = fy; dY = fy;
		rad = frad; dRad = frad;
		internal_ball_type = ball_type;
		if (*ball_texture_id[ball_type] != stZ) texture_id = *ball_texture_id[ball_type];
		exists = true;
		trail_x.resize(8);
		trail_y.resize(8);
		trail_circle.resize(8);
	}
	/*resets the ball to it's default values*/
	void reset() {
		x = dX;
		y = dY;
		rad = dRad;
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
		return (X ? x - rad : y);
	}
	/*
	returns the rightmost side of the ball
	X: (boolean) if true returns X coordinate, if false returns Y coordinate
	*/
	double getRight(bool X) {
		return (X ? x + rad : y);
	}
	/*
	returns the upmost side of the ball
	X: (boolean) if true returns X coordinate, if false returns Y coordinate
	*/
	double getTop(bool X) {
		return (X ? x : y - rad);
	}
	/*
	returns the downmost side of the ball
	X: (boolean) if true returns X coordinate, if false returns Y coordinate
	*/
	double getBottom(bool X) {
		return (X ? x : y + rad);
	}
	void setPosition(double X, double Y) {
		x = X;
		y = Y;
	}
	/*
	moves the ball to a new position
	dx: horizontal direction
	dy: vertical direction
	*/
	void move(double X, double Y) {
		x += X;
		y += Y;
	}
};

/*
Paddle object
*/
class paddle {
	double dX, dY;
	double dWidth, dHeight;
	bool defaults_set = false;
	size_t dTexture_id;
public:
	double x, y;
	double width, height;
	size_t texture_id;
	bool has_laser;
	bool laser_on_screen;
	bool has_big;
	bool has_small;
	bool has_multi;
	laser the_laser;
	std::vector<ball> multi_ball;
	/*
	sets the paddle's default position size and texture id
	fx: X position
	fy: Y position
	fwidth: paddle width (for collision)
	fheight: paddle height (for collision)
	ftexture_id: texture id for paddle
	*/
	void setDefaults(double fx, double fy, double fwidth, double fheight, size_t ftexture_id)
	{
		if (!defaults_set) {
			x = fx; dX = fx;
			y = fy; dY = fy;
			width = fwidth; dWidth = fwidth;
			height = fheight; dHeight = fheight;
			texture_id = ftexture_id;
			dTexture_id = ftexture_id;
			defaults_set = true;
		}
	}
	/*spawns extra balls*/
	void spawn_multi(ball tBall, std::vector<ball> &ball_vec) {
		ball *new_ball[2] = { new ball, new ball };
		for (int i = 0; i < 2; i++) {
			new_ball[i]->setDefaults(tBall.x, tBall.y, tBall.rad, 1);
			new_ball[i]->angle = tBall.angle + (-20 + (40 * i));
			for (int j = 0; j < 8; j++)
				new_ball[i]->trail_circle[7 - j].setDefaults(tBall.x, tBall.y, (0.875 * (j + 1)));
			new_ball[i]->is_attached = false;
			ball_vec.push_back(*new_ball[i]);
			delete new_ball[i];
		}
	}
	/*runs script for enlarging paddle*/
	void getBig() {
		width = 70;
		x -= 10;
		x = (x < 1 ? 1 : (x > 329 ? 329 : x));
		texture_id = paddleBigID;
	}
	/*runs script for shrinking paddle*/
	void getSmall() {
		width = 30;
		x += 10;
		texture_id = paddleSmallID;
	}
	/*removes powerups from paddle*/
	void remove_powerups() {
		if (has_big || has_small) {
			width = dWidth;
			x += (has_big ? 10 : (has_small ? -10 : NULL));
			x = (x < 1 ? 1 : (x > 329 ? 329 : x));
		}
		has_laser = false;
		has_big = false;
		has_small = false;
		texture_id = dTexture_id;
	}
	/*resets the paddle to it's default values*/
	void reset()
	{
		x = dX;
		y = dY;
		width = dWidth;
		height = dHeight;
		has_laser = false;
		has_big = false;
		has_small = false;
		texture_id = dTexture_id;
		laser_on_screen = false;
	}
	/*
	returns the leftmost side of the paddle
	X: (boolean) if true returns X coordinate, if false returns Y coordinate
	*/
	double getLeft(bool X)
	{
		return (X ? x : y + (height / 2.0));
	}
	/*
	returns the rightmost side of the paddle
	X: (boolean) if true returns X coordinate, if false returns Y coordinate
	*/
	double getRight(bool X)
	{
		return (X ? x + width : y + (height / 2.0));
	}
	/*
	returns the upmost side of the paddle
	X: (boolean) if true returns X coordinate, if false returns Y coordinate
	*/
	double getTop(bool X)
	{
		return (X ? x + (width / 2.0) : y);
	}
	/*
	returns the downmost side of the paddle
	X: (boolean) if true returns X coordinate, if false returns Y coordinate
	*/
	double getBottom(bool X)
	{
		return (X ? x + (width / 2.0) : y + height);
	}
	void setPosition(double X, double Y) {
		x = X;
		y = Y;
	}
	void move(double X, double Y) {
		x += X;
		y += Y;
	}
};
