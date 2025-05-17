#include <iostream>
#include <gpiod.h>
#include <unistd.h>
#include <chrono>
#include <thread>

using namespace std;

class Movement
{
private:
    gpiod_chip* chip;
    const unsigned int LEFT_IN1 = 7;
    const unsigned int LEFT_IN2 = 8;
    const unsigned int RIGHT_IN1 = 9;
    const unsigned int RIGHT_IN2 = 10;
    gpiod_line* l1;
    gpiod_line* l2;
    gpiod_line* r1;
    gpiod_line* r2;
public:
    double measure_distance(gpiod_chip* chip, unsigned int trig_gpio, unsigned int echo_gpio);
    void move_forward();
    void turn_left(int x);
    void turn_right(int x);
    void stop();
    Movement();
    ~Movement();
};

