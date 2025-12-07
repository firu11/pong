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

void paddle::updateAIPaddle(const ball *b, int screenHeight) {
    float ballY = b->getPos().second;

    if (ballY < this->r.y + static_cast<float>(this->r.height) / 2) {
        this->moveUp();
    } else if (ballY > this->r.y + static_cast<float>(this->r.height) / 2) {
        this->moveDown(screenHeight);
    }
}
