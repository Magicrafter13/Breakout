#pragma once
//#include "shapes.hpp"

/**
 * brief: draws a rectangle (using sf2dlib)
 * param: mRectangle: the rectangle you want drawn (has to be of mRectangle class)
 */
void draw_rect(mRectangle Rectangle_to_draw);

/**
 * brief: draws a brick (which is just an mRectangle with a bool that returns true or false for it's existance
 * param: brick: the brick to be drawn (has to be of brick class)
 */
void draw_brick(brick brick_to_draw);

/**
 * brief: draws a circle (using sf2dlib)
 * param: mCircle: the circle you want drawn (has to be of mCircle class)
 */
void draw_circ(mCircle circle_to_draw);

/**
 * brief: draws a ball (which is just an mCircle with a bool that returns true or false for it's existance
 * param: ball: the ball to be drawn (has to be of brick class)
 */
void draw_ball(ball ball_to_draw);
/**
 * brief: draws a triangle (using sf2dlib)
 * param: mTriangle: the triangle you want drawn (has to be of mTriangle class)
 */
void draw_triangle(mTriangle tri_to_draw);
