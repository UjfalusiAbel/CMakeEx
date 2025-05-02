#include "./Movement.h"
#include "./SoundRecognizer.h"
#include <vector>
#include <fstream>

using namespace std;

class Robot
{
private:
    SoundRecognizer recognizer;
    vector<vector<char>> map;
public:
    Robot();
    void print_map();
    void connect_to_server();
};