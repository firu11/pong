//
// Created by Filip Růžička on 19.11.2025.
//

#ifndef PONG_BALL_H
#define PONG_BALL_H
#include "paddle.h"


class ball {
public:
    ball();

    ~ball();

    void move();

    bool checkCollision(paddle p);

    void bounce(paddle p);
};


#endif //PONG_BALL_H
