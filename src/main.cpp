#include <iostream>
#include "./Robot.h"

int main(int argc, char** argv)
{
    Robot robot;
    robot.print_map();
    robot.connect_to_server();
    return 0;
}