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
        map.push_back(row);
    }
}

void Robot::print_map()
{
    for (auto line : map)
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
    while (true) {
        string command = recognizer.receive_command();
        cout << command << "\n";
        sleep(1);
    }

}

