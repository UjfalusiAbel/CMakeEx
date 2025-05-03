#include "./Movement.h"
#include "./SoundRecognizer.h"
#include <vector>
#include <map>
#include <fstream>

using namespace std;

class Robot
{
private:
    map<string, char> codes;
    SoundRecognizer recognizer;
    vector<vector<char>> airportMap;
    bool isRunning;
public:
    Robot();
    ~Robot();
    void print_map();
    void connect_to_server();
};