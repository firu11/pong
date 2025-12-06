#include <utility>

#include "include/ball.hpp"

#include <cmath>
#include <vector>

ball::ball(std::pair<float, float> startPos) : pos(std::move(startPos)), vec(0, 0) {
}

void ball::move() {
    pos.first += vec.first;
    pos.second += vec.second;
}

void ball::startMoving(float speed, int towardsPlayer) {
    if (towardsPlayer == 1) {
        vec.first = -speed;
    } else if (towardsPlayer == 2) {
        vec.first = speed;
    }
}

bool ball::checkCollision(std::vector<paddle> &paddles) {
    if (paddles.empty()) return false;

    float ballX = pos.first;
    float ballY = pos.second;
    auto ballW = static_cast<float>(size.first);
    auto ballH = static_cast<float>(size.second);

    for (auto &paddle: paddles) {
        auto p = paddle.getRect();
        float paddleX = p.x;
        float paddleY = p.x;
        auto paddleW = static_cast<float>(p.width);
        auto paddleH = static_cast<float>(p.height);

        bool collisionX = ballX < paddleX + paddleW && ballX + ballW > paddleX;
        bool collisionY = ballY < paddleY + paddleH && ballY + ballH > paddleY;

        if (collisionX && collisionY) {
            // došlo ke kolizi
            bounce(paddle);
            return true;
        }
    }
    return false; // žádná kolize
}


void ball::bounce(paddle &p) {
    float paddleTop = p.getRect().y;
    float ballCenterY = pos.second + size.first / 2; // or pos.y if using center

    float collisionPercent = (ballCenterY - paddleTop) / p.getRect().height;

    // reverse X direction
    vec.first = -vec.first;

    // adjust Y direction based on where it hit
    float maxBounceAngle = 75.0f; // degrees
    float angle = (collisionPercent - 0.5f) * 2 * maxBounceAngle; // from -75 to +75
    float speed = sqrt(vec.first * vec.first + vec.second * vec.second);
    vec.second = speed * sin(angle * 3.14159f / 180.0f);
    vec.first = (vec.first > 0 ? 1 : -1) * sqrt(speed * speed - vec.second * vec.second);
}

std::pair<int, int> ball::getPosOnScreen() const {
    int roundedX = static_cast<int>(std::round(pos.first));
    int roundedY = static_cast<int>(std::round(pos.second));
    return {roundedX, roundedY};
}
