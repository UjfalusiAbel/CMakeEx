#include <iostream>
#include <string>
#include <thread>
#include <mutex>
#include <queue>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <atomic>

using namespace std;

class SoundRecognizer
{
private:
    const string server_ip = "127.0.0.1";
    const int server_port = 5000;
    int sockFd;
    thread listener;
    mutex queueMutex;
    queue<string> commandQueue;
    atomic<bool> isRunning;
    void listen_thread();
public:
    SoundRecognizer();
    ~SoundRecognizer();
    string receive_command();
};