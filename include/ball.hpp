#ifndef PONG_BALL_H
#define PONG_BALL_H
#include "paddle.hpp"


class ball {
public:
    ball();

    ~ball();

    void move();

    bool checkCollision(paddle p);

    void bounce(paddle p);
};


#endif //PONG_BALL_H
