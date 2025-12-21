#include "include/paddle.hpp"
#include "include/ball.hpp"

#include <numbers>
#include <cmath>
#include <utility>

#include <algorithm>


ball::ball(std::pair<float, float> startPos) : pos(std::move(startPos)), vec(0, 0) {
}

void ball::move(int screenHeight) {
    pos.first += vec.first;
    pos.second += vec.second;

    if (checkWallsCollision(screenHeight)) vec.second = -vec.second;
}

void ball::startMoving(float speed, int towardsPlayer) {
    if (towardsPlayer == 1) {
        vec.first = -speed;
    } else if (towardsPlayer == 2) {
        vec.first = speed;
    }
}

bool ball::checkCollision(paddle *paddles[2]) {
    float ballX = pos.first;
    float ballY = pos.second;
    auto ballW = static_cast<float>(size.first);
    auto ballH = static_cast<float>(size.second);

    for (int i = 0; i < 2; ++i) {
        auto [paddleX, paddleY, paddleW, paddleH] = paddles[i]->getRect();

        bool collisionX = ballX < paddleX + static_cast<float>(paddleW) && ballX + ballW > paddleX;
        bool collisionY = ballY < paddleY + static_cast<float>(paddleH) && ballY + ballH > paddleY;

        if (collisionX && collisionY) {
            // došlo ke kolizi
            if (!collided) {
                bounce(*paddles[i]);
                collided = true;
            }
            return true;
        }
    }
    collided = false;
    return false; // žádná kolize
}

bool ball::checkWallsCollision(int screenHeight) const {
    return pos.second <= 0 || pos.second >= static_cast<float>(screenHeight);
}

int ball::checkGoalCollision(int screenWidth) const {
    if (pos.first <= 0) {
        return 1;
    }
    if (pos.first >= static_cast<float>(screenWidth)) {
        return 2;
    }
    return 0;
}

void ball::bounce(paddle &p) {
    float paddleTop = p.getRect().y;
    float ballCenterY = pos.second + static_cast<float>(size.second) / 2.0f;

    float collisionPercent = (ballCenterY - paddleTop) / static_cast<float>(p.getRect().height);
    collisionPercent = std::clamp(collisionPercent, 0.0f, 1.0f);

    vec.first = -vec.first;

    float maxBounceAngle = 60.0f; // degrees
    float angle = (collisionPercent - 0.5f) * 2 * maxBounceAngle;
    float speed = std::sqrt(vec.first * vec.first + vec.second * vec.second);
    vec.second = speed * static_cast<float>(std::sin(angle * std::numbers::pi / 180.0f));
    vec.first = static_cast<float>(vec.first > 0 ? 1 : -1) * std::sqrt(speed * speed - vec.second * vec.second);

    speedUp();
}

void ball::speedUp() {
    vec.first *= 1.1;
    vec.second *= 1.1;
}

std::pair<float, float> ball::getPos() const {
    return pos;
}

void ball::draw(char **screen, const std::pair<int, int> &canvasSize) const {
    int rx = static_cast<int>(std::round(pos.first));
    int ry = static_cast<int>(std::round(pos.second));

    if (rx < 0 || ry < 0 || rx > canvasSize.first - 1 || ry > canvasSize.second - 1) return;

    screen[ry][rx] = '@';
}
