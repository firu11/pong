#ifndef PONG_BALL_H
#define PONG_BALL_H
#include <utility>
#include <vector>

#include "paddle.hpp"

class ball {
    std::pair<float, float> pos;
    std::pair<float, float> vec;

    std::pair<int, int> size = std::pair(1, 1);

public:
    ball(std::pair<float, float> startPos);

    void startMoving(float speed = 1, int towardsPlayer = 1);

    void move();

    bool checkCollision(std::vector<paddle> &paddles);

    std::pair<int, int> getPosOnScreen() const;

private:
    void bounce(paddle &p);
};


#endif //PONG_BALL_H
