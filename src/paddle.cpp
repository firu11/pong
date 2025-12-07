#include "include/paddle.hpp"

#include "include/ball.hpp"

#include <cmath>


paddle::paddle(float x, float y, int width, int height) : r(x, y, width, height) {
}

void paddle::moveUp() {
    if (r.y <= 0) return;
    r.y -= 1;
}

void paddle::moveDown(int screenHeight) {
    if (r.y + static_cast<float>(r.height) > static_cast<float>(screenHeight) - 1) return;
    r.y += 1;
}

void paddle::draw(char **screen) const {
    int rx = static_cast<int>(std::round(r.x));
    int ry = static_cast<int>(std::round(r.y));

    for (int i = 0; i < r.height; i++) {
        screen[ry + i][rx] = '|';
    }
}

paddle::rect paddle::getRect() const {
    return r;
}

void aiPaddle::update(int screenHeight, const ball *b) {
    float ballY = b->getPos().second;

    if (ballY < r.y + static_cast<float>(r.height) / 2) {
        moveUp();
    } else if (ballY > r.y + static_cast<float>(r.height) / 2) {
        moveDown(screenHeight);
    }
}

void playerPaddle::update(int screenHeight, const ball *b) {
    auto move = static_cast<float>(get_paddle_movement());
    if (move < 0)
        moveUp();
    else if (move > 0)
        moveDown(screenHeight);
}
