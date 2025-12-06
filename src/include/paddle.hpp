#ifndef PONG_PADDLE_H
#define PONG_PADDLE_H

#include "config.hpp"

class paddle {
    struct rect {
        float x;
        float y;
        int width;
        int height;
    };

    rect r;

public:
    paddle(float x, float y, int width, int height);

    void moveUp();

    void moveDown();

    // hehehe hezký co GETRECT
    rect getRect();

    void draw(char (&screen)[screenHeight][screenWidth]);
};

#endif //PONG_PADDLE_H
