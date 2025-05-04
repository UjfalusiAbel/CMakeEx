#include "./Movement.h"
#include "./SoundRecognizer.h"
#include <vector>
#include <map>
#include <fstream>

using namespace std;

class Robot
{
private:
    struct State {
        int x;
        int y;
        int direction;
        vector<int> path;
    };
    bool checkedIn;
    bool throughSecurity;
    int startX;
    int startY;
    map<string, char> codes;
    SoundRecognizer recognizer;
    vector<vector<char>> airportMap;
    bool isRunning;
    vector<int> GetPath(char target);
    bool isPositionValid(int x, int y, int rows, int columns);
public:
    Robot();
    ~Robot();
    void print_map();
    void connect_to_server();
};