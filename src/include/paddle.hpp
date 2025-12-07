#ifndef PONG_PADDLE_H
#define PONG_PADDLE_H

#include "input.hpp"

class ball; // forward declaration to avoid circular imports I guess

class paddle {
protected:
    struct rect {
        float x;
        float y;
        int width;
        int height;
    };

    rect r;

    void moveUp();

    void moveDown(int screenHeight);

public:
    paddle(float x, float y, int width, int height);

    virtual ~paddle() = default;

    virtual void update(int screenHeight, const ball *b) = 0;

    [[nodiscard]] rect getRect() const;

    void draw(char **screen) const;
};

class playerPaddle : public paddle {
public:
    playerPaddle(float x, float y, int width, int height)
        : paddle(x, y, width, height) {
    }

    void update(int screenHeight, const ball *b) override;
};

class aiPaddle : public paddle {
public:
    aiPaddle(float x, float y, int width, int height)
        : paddle(x, y, width, height) {
    }

    void update(int screenHeight, const ball *b) override;
};


#endif //PONG_PADDLE_H
