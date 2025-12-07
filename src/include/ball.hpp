#ifndef PONG_BALL_H
#define PONG_BALL_H
#include <utility>
#include <vector>

#include "paddle.hpp"

class ball {
    std::pair<float, float> pos;
    std::pair<float, float> vec;

    std::pair<int, int> size = std::pair(1, 1);
    bool collided = false; // prevent from bouncing multiple times from a single paddle

public:
    ball(std::pair<float, float> startPos);

    void startMoving(float speed = 1, int towardsPlayer = 1);

    void move();

    bool checkCollision(paddle paddles[2]);

    int checkGoalCollision();

    std::pair<float, float> getPos() const;

    void draw(char (&screen)[screenHeight][screenWidth]);

private:
    void bounce(paddle &p);

    void speedUp();

    bool checkWallsCollision();
};


#endif //PONG_BALL_H
