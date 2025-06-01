#include <cerrno>
#include <cmath>
#include <condition_variable>
#include <cstring>
#include <fcntl.h>
#include <iostream>
#include <mutex>
#include <queue>
#include <string>
#include <termios.h>
#include <thread>
#include <unistd.h>

#include "../include/Plot.hpp"

std::queue<std::string> serialQueue;
std::mutex queueMutex;
std::condition_variable queueCond;
bool running = true;

void serialReaderThread()
{
    const char *portName = "/dev/ttyACM0";
    int hSerial = open(portName, O_RDWR | O_NOCTTY);

    if (hSerial == -1)
    {
        std::cerr << "Erro ao abrir " << portName << ". Erro: " << strerror(errno) << std::endl;
        return;
    }

    struct termios tty;
    if (tcgetattr(hSerial, &tty) != 0)
    {
        std::cerr << "Erro ao obter parâmetros da serial (" << portName << "). Erro: " << strerror(errno) << std::endl;
        close(hSerial);
        return;
    }

    cfsetospeed(&tty, B115200);
    cfsetispeed(&tty, B115200);

    tty.c_cflag &= ~PARENB;
    tty.c_cflag &= ~CSTOPB;
    tty.c_cflag &= ~CSIZE;
    tty.c_cflag |= CS8;

    tty.c_cflag &= ~CRTSCTS;

    tty.c_cflag |= CREAD | CLOCAL;

    tty.c_iflag &= ~(IXON | IXOFF | IXANY);

    tty.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);

    tty.c_oflag &= ~OPOST;

    tty.c_cc[VMIN] = 0;
    tty.c_cc[VTIME] = 1;

    if (tcsetattr(hSerial, TCSANOW, &tty) != 0)
    {
        std::cerr << "Erro ao configurar " << portName << ". Erro: " << strerror(errno) << std::endl;
        close(hSerial);
        return;
    }

    tcflush(hSerial, TCIFLUSH);

    std::string buffer;
    char ch;
    ssize_t bytesRead;

    std::cout << "Serial reader thread started on " << portName << std::endl;

    while (running)
    {
        bytesRead = read(hSerial, &ch, 1);

        if (bytesRead == -1)
        {
            if (errno == EINTR)
            {
                continue;
            }
            std::cerr << "Erro ao ler da serial (" << portName << "). Erro: " << strerror(errno) << std::endl;
            break;
        }
        else if (bytesRead == 0)
        {
            continue;
        }
        else
        {
            if (ch == '\n')
            {
                std::lock_guard<std::mutex> lock(queueMutex);
                serialQueue.push(buffer);
                buffer.clear();
                queueCond.notify_one();
            }
            else if (ch == '\r')
            {
                buffer += ch;
            }
            else
            {
                buffer += ch;
            }
        }
    }

    close(hSerial);
    std::cout << "Serial reader thread on " << portName << " finished." << std::endl;
}

void generateRandomValuesThread(size_t numSubplots)
{
    constexpr double amplitude = 500.0;
    constexpr double frequency = 0.5;
    double time = 0.0;
    constexpr double timeStep = 0.01;

    std::string buffer{};

    while (running)
    {
        for (size_t i{0}; i < numSubplots; ++i)
        {
            buffer += std::to_string(amplitude * std::sin(2.0 * M_PI * frequency * time)) + "|";
        }
        time += timeStep;
        buffer += '\n';
        std::lock_guard<std::mutex> lock(queueMutex);
        serialQueue.push(buffer);
        buffer.clear();
        queueCond.notify_one();
    }
}

int main()
{
    size_t rows{2};
    size_t cols{2};
    size_t total{rows * cols};
    Plot p{rows, cols, {1920u, 1080u}};

    for (size_t col{0}; col < cols; ++col)
    {
        for (size_t row{0}; row < rows; ++row)
        {
            p[row][col].setDataPointsRadius(2.f);
        }
    }

    std::vector<float> value(total, 0.f);
    std::thread reader(generateRandomValuesThread, total);
    // std::thread reader(serialReaderThread);

    while (p.window().isOpen())
    {
        while (const std::optional event{p.window().pollEvent()})
        {
            if (event->is<sf::Event::Closed>())
            {
                p.window().close();
            }
        }

        // Getting right values from queue (This has nothing to do with the Plot class)
        std::unique_lock<std::mutex> lock(queueMutex);
        queueCond.wait(lock, [] { return !serialQueue.empty(); });
        std::string data = serialQueue.front();
        serialQueue.pop();
        lock.unlock();
        size_t pos{};
        size_t tmpI{0};
        while ((pos = data.find("|")) != std::string::npos)
        {
            std::string part = data.substr(0, pos);
            data = data.substr(pos + 1, data.size());
            value[tmpI] = std::stof(part);
            ++tmpI;
        }

        for (size_t col{0}; col < cols; ++col)
        {
            for (size_t row{0}; row < rows; ++row)
            {
                p[row][col].sendData(value[row + col * rows]);
            }
        }

        p.render();
    }

    running = false;
    reader.join();

    return 0;
}