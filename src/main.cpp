#include "../include/Plot.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/PrimitiveType.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Vertex.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/VideoMode.hpp>
#include <array>
#include <cmath>
#include <condition_variable>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <fcntl.h>
#include <iostream>
#include <mutex>
#include <queue>
#include <random>
#include <string>
#include <termios.h>
#include <thread>
#include <unistd.h>

std::queue<std::string> serialQueue;
std::mutex queueMutex;
std::condition_variable queueCond;
bool running = true;

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
    Plot p{};
    p.createWindow(1280, 720);
    p.createSubplots(2, 2, -500.f, 500.f, 50.f, 50.f);

    std::vector<float> value(p.getNumSubplots(), 0.f);

    std::thread reader(generateRandomValuesThread, p.getNumSubplots());

    while (p.getWindow().isOpen())
    {
        while (const std::optional event{p.pollWindowEvent()})
        {
            if (event->is<sf::Event::Closed>())
            {
                p.closeWindow();
            }
        }

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

        for (int i{0}; i < p.getNumSubplots(); ++i)
        {
            p.sendValueToGrid(i, value[i]);
        }

        p.clear();

        p.show();
    }

    running = false;
    reader.join();

    return 0;
}