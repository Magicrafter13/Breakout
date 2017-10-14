#pragma once

/*
Draws a rectangle
*/
void draw_object(mRectangle Rectangle_to_draw);

/*
Draws a brick (Uses texture if one specified)
*/
void draw_object(brick brick_to_draw);

/*
Draws a circle
*/
void draw_object(mCircle circle_to_draw);

/*
Draws a triangle
*/
void draw_object(mTriangle tri_to_draw);

/*
Draws a ball (Uses texture if one specified)
*/
void draw_object(ball ball_to_draw);

/*
Draws a paddle
*/
void draw_object(paddle paddle_to_draw);