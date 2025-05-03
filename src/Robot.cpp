#include "./Robot.h"

Robot::Robot()
{
    ifstream file("./map.data");
    if (!file)
    {
        throw runtime_error("Cannot open file!");
    }

    string line;
    while (getline(file, line))
    {
        vector<char> row(line.begin(), line.end());
        airportMap.push_back(row);
    }

    codes.insert({ "restaurant", 'R' });
    codes.insert({ "gate", 'G' });
    codes.insert({ "restroom", 'W' });
    codes.insert({ "toilet", 'W' });
    codes.insert({ "check", 'C' });
    codes.insert({ "security", 'S' });
    codes.insert({ "shops", 'D' });
}

void Robot::print_map()
{
    for (auto line : airportMap)
    {
        for (auto cell : line)
        {
            cout << cell << " ";
        }
        cout << "\n";
    }
}

void Robot::connect_to_server()
{
    isRunning = true;
    while (isRunning)
    {
        string command = recognizer.receive_command();
        if (codes.find(command) != codes.end())
        {
            cout << codes[command]<<"\n";
        }
        sleep(1);
    }
}

Robot::~Robot()
{
    isRunning = false;
}

