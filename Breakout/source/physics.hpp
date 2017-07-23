#pragma once
/**
 * brief: calculates necessary Physics for next frame, and gameplay to continue
 * param: ball container
 * param: paddle container
 * param: DirectionX of paddle
 * param: DirectionY of paddle
 * param: brick array
 * param: key_left
 * param: key_right
 */
void ballPhysics(ball BALL, paddle PADDLE, double paddle_dx, double paddle_dy, brick BRICKS[50], bool kLeft, bool kRight);
