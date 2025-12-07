#include "include/paddle.hpp"

#include <cmath>

#include "include/ball.hpp"

paddle::paddle(float x, float y, int width, int height) {
    r = rect(x, y, width, height);
}

void paddle::moveUp() {
    if (r.y <= 0) return;
    r.y -= 1;
}

void paddle::moveDown() {
    if (r.y + r.height > screenHeight - 1) return;
    r.y += 1;
}

void paddle::draw(char (&screen)[screenHeight][screenWidth]) {
    int rx = std::round(r.x);
    int ry = std::round(r.y);

    for (int i = 0; i < r.height; i++) {
        screen[ry + i][rx] = '|';
    }
}

paddle::rect paddle::getRect() {
    return r;
}

void paddle::updateAIPaddle(ball *b) {
    float ballY = b->getPos().second;

    if (ballY < this->r.y + this->r.height / 2) {
        this->moveUp();
    } else if (ballY > this->r.y + this->r.height / 2) {
        this->moveDown();
    }
}
