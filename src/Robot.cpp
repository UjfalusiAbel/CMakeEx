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

    bool foundStart = false;
    for (int i = 0;i < airportMap.size() && !foundStart;i++)
    {
        for (int j = 0;j < airportMap[i].size() && !foundStart;j++)
        {
            if (airportMap[i][j] == 'X')
            {
                startX = i;
                startY = j;
                foundStart = true;
            }
        }
    }

    checkedIn = false;
    throughSecurity = false;
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
            cout << codes[command] << "\n";

            if (!checkedIn && codes[command] == 'S')
            {
                cout << "You must first go to checking, you can't go directly to secutiry!\n";
            }
            else if (checkedIn && !throughSecurity && codes[command] != 'S')
            {
                cout << "You must go to security first!\n";
            }
            else
            {
                vector<int> pathToTarget = GetPath(codes[command]);
                for (auto x : pathToTarget)
                {
                    cout << x << " ";
                }
                cout << "\n";
            }
        }
        sleep(1);
    }
}

vector<int> Robot::GetPath(char target)
{
    //0 - forward
    //1 - turn right
    //2 - turn left
    const int rows = airportMap.size();
    const int columns = airportMap[0].size();

    int dx[] = { -1, 0, 1, 0 };
    int dy[] = { 0, 1, 0, -1 };

    queue<State> queue;
    bool visited[rows][columns][4] = { {{false}} };
    int targetX;
    int targetY;

    bool foundTarget = false;

    for (int i = 0; i < rows && !foundTarget; i++)
    {
        for (int j = 0; j < columns && !foundTarget; j++)
        {

            if (airportMap[i][j] == target)
            {
                targetX = i;
                targetY = j;
                foundTarget = true;
            }
        }
    }

    queue.push({ startX, startY, 1, {} });
    visited[startX][startY][1] = true;

    while (!queue.empty())
    {
        State currentState = queue.front();
        queue.pop();

        if (airportMap[currentState.x][currentState.y] == 'C')
        {
            checkedIn = true;
        }

        if (airportMap[currentState.x][currentState.y] == 'S')
        {
            throughSecurity = true;
        }

        if (currentState.x == targetX && currentState.y == targetY)
        {
            if (airportMap[startX][startY] == 'C' || airportMap[startX][startY] == 'S')
            {
                airportMap[startX][startY] = '#';
            }
            startX = targetX;
            startY = targetY;
            return currentState.path;
        }

        //Move forwards
        int newX = currentState.x + dx[currentState.direction];
        int newY = currentState.y + dy[currentState.direction];
        if (isPositionValid(newX, newY, rows, columns) && !visited[newX][newY][currentState.direction])
        {
            auto newPath = currentState.path;
            newPath.push_back(0);
            queue.push({ newX, newY, currentState.direction, newPath });
            visited[newX][newY][currentState.direction] = true;
        }

        //Turn right
        int rightDirection = (currentState.direction + 1) % 4;
        if (!visited[currentState.x][currentState.y][rightDirection])
        {
            auto newPath = currentState.path;
            newPath.push_back(1);
            queue.push({ currentState.x,currentState.y, rightDirection, newPath });
            visited[currentState.x][currentState.y][rightDirection] = true;
        }

        //Turn left
        int leftDirection = (currentState.direction + 3) % 4;
        if (!visited[currentState.x][currentState.y][leftDirection])
        {
            auto newPath = currentState.path;
            newPath.push_back(2);
            queue.push({ currentState.x,currentState.y,leftDirection, newPath });
            visited[currentState.x][currentState.y][leftDirection] = true;
        }
    }

    return {};
}

bool Robot::isPositionValid(int x, int y, int rows, int columns)
{
    return x > -1 && y > -1 && y < columns && x < rows && airportMap[x][y] != '#';
}

Robot::~Robot()
{
    isRunning = false;
}

