#include "./SoundRecognizer.h"

SoundRecognizer::SoundRecognizer()
{
    sockFd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockFd < 0)
    {
        throw runtime_error("Socket creation failed");
    }

    sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_port = htons(server_port);
    inet_pton(AF_INET, server_ip.c_str(), &address.sin_addr);

    if (connect(sockFd, (sockaddr*)&address, sizeof(address)) < 0)
    {
        throw runtime_error("Connection failed!");
    }

    listener = thread(&SoundRecognizer::listen_thread, this);
    listener.detach();
}

SoundRecognizer::~SoundRecognizer()
{
    isRunning = false;
    close(sockFd);
    if (listener.joinable())
    {
        listener.join();
    }
}

string SoundRecognizer::receive_command()
{
    lock_guard<mutex> lock(queueMutex);
    if (commandQueue.empty())
    {
        return "";
    }

    string cmd = commandQueue.front();
    commandQueue.pop();
    return cmd;
}

void SoundRecognizer::listen_thread()
{
    isRunning = true;
    while (isRunning)
    {
        char buffer[128];
        int len = recv(sockFd, buffer, sizeof(buffer) - 1, 0);
        if (len <= 0)
        {
            break;
        }
        buffer[len] = '\0';
        string cmd(buffer);
        cmd.erase(cmd.find_last_not_of(" \n\r\t") + 1);
        cout << cmd << "\n";
        if (!cmd.empty())
        {
            lock_guard<mutex> lock(queueMutex);
            commandQueue.push(cmd);
        }
    }
}