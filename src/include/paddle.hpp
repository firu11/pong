#ifndef PONG_PADDLE_H
#define PONG_PADDLE_H

class ball; // forward declaration to avoid circular imports I guess

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

    void moveDown(int screenHeight);

    // hehehe hezký co GETRECT
    [[nodiscard]] rect getRect() const;

    void updateAIPaddle(const ball *b, int screenHeight);

    void draw(char **screen) const;
};

#endif //PONG_PADDLE_H
