#include "./Movement.h"

using namespace std::chrono;
using namespace std;


double Movement::measure_distance(gpiod_chip* chip, unsigned int trig_gpio, unsigned int echo_gpio)
{
    gpiod_line* trig = gpiod_chip_get_line(chip, trig_gpio);
    gpiod_line* echo = gpiod_chip_get_line(chip, echo_gpio);

    gpiod_line_request_output(trig, "ultrasonic", 0);
    gpiod_line_request_input(echo, "ultrasonic");
    gpiod_line_set_value(trig, 0);

    std::this_thread::sleep_for(microseconds(2));
    gpiod_line_set_value(trig, 1);

    std::this_thread::sleep_for(microseconds(10));
    gpiod_line_set_value(trig, 0);


    auto start_wait = high_resolution_clock::now();

    while (gpiod_line_get_value(echo) == 0)
    {
        if (duration_cast<milliseconds>(high_resolution_clock::now() - start_wait).count() > 100) {

            cerr << "Echo timeout (start)\n";
            return -1;

        }

    }

    auto start = high_resolution_clock::now();

    while (gpiod_line_get_value(echo) == 1)
    {

        if (duration_cast<milliseconds>(high_resolution_clock::now() - start).count() > 100)
        {
            cerr << "Echo timeout (end)\n";
            return -1;

        }

    }

    auto end = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(end - start).count();
    double distance = duration * 0.0343 / 2;

    gpiod_line_release(trig);
    gpiod_line_release(echo);

    return distance;
}



void Movement::move_forward()
{
    gpiod_line_set_value(l1, 1);
    gpiod_line_set_value(l2, 0);
    gpiod_line_set_value(r1, 1);
    gpiod_line_set_value(r2, 0);
    sleep(2.18);
}



void Movement::turn_left(int x)
{
    gpiod_line_set_value(l1, 1);
    gpiod_line_set_value(l2, 0);
    gpiod_line_set_value(r1, 0);
    gpiod_line_set_value(r2, 1);
    sleep(float(x) / 90);

    gpiod_line_set_value(l1, 0);
    gpiod_line_set_value(l2, 0);
    gpiod_line_set_value(r1, 0);
    gpiod_line_set_value(r2, 0);
}



void Movement::turn_right(int x)
{
    gpiod_line_set_value(l1, 0);
    gpiod_line_set_value(l2, 1);
    gpiod_line_set_value(r1, 1);
    gpiod_line_set_value(r2, 0);

    sleep(float(x) / 90);

    gpiod_line_set_value(l1, 0);
    gpiod_line_set_value(l2, 0);
    gpiod_line_set_value(r1, 0);
    gpiod_line_set_value(r2, 0);
}

Movement::Movement()
{
    const char* chipname = "gpiochip0";

    unsigned int TRIG_FRONT = 4;
    unsigned int ECHO_FRONT = 27;
    unsigned int TRIG_BACK = 17;
    unsigned int ECHO_BACK = 18;


    chip = gpiod_chip_open_by_name(chipname);

    if (!chip)
    {
        std::cerr << "Failed to open\n";
        return;
    }

    l1 = gpiod_chip_get_line(chip, LEFT_IN1);
    l2 = gpiod_chip_get_line(chip, LEFT_IN2);
    r1 = gpiod_chip_get_line(chip, RIGHT_IN1);
    r2 = gpiod_chip_get_line(chip, RIGHT_IN2);

    gpiod_line_request_output(l1, "motor", 0);
    gpiod_line_request_output(l2, "motor", 0);
    gpiod_line_request_output(r1, "motor", 0);
    gpiod_line_request_output(r2, "motor", 0);
}

Movement::~Movement()
{
    gpiod_chip_close(chip);
}
