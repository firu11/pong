#ifndef PONG_INPUT_H
#define PONG_INPUT_H

void inputThreadFunc(std::atomic_bool &stop);

float get_paddle_movement();

#endif //PONG_INPUT_H
