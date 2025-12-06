#ifndef PONG_CONFIG_H
#define PONG_CONFIG_H
#include <atomic>
#include <utility>

class paddle;
class ball;

static constexpr int screenWidth = 160;
static constexpr int screenHeight = 36;
static constexpr int targetFps = 60;

struct gameInfo {
    std::atomic_bool stop;
    paddle *paddles[2];
    ball *ballInstance;
    std::pair<int, int> score;
    char canvas[screenHeight][screenWidth];
};

#endif //PONG_CONFIG_H
