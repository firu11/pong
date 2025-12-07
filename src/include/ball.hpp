#ifndef PONG_BALL_H
#define PONG_BALL_H
#include <utility>
#include <vector>

class paddle; // forward declaration to avoid circular imports I guess

class ball {
    std::pair<float, float> pos;
    std::pair<float, float> vec;

    std::pair<int, int> size = std::pair(1, 1);
    bool collided = false; // prevent from bouncing multiple times from a single paddle

public:
    ball(std::pair<float, float> startPos);

    void startMoving(float speed = 1, int towardsPlayer = 1);

    void move(int screenHeight);

    bool checkCollision(paddle (&paddles)[2]);

    [[nodiscard]] int checkGoalCollision(int screenWidth) const;

    [[nodiscard]] std::pair<float, float> getPos() const;

    void draw(char **screen, std::pair<int, int> screenSize);

private:
    void bounce(paddle &p);

    void speedUp();

    [[nodiscard]] bool checkWallsCollision(int screenHeight) const;
};


#endif //PONG_BALL_H
