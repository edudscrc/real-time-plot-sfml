#include <cmath>
#include <condition_variable>
// #include <iostream>
#include <mutex>
#include <queue>
#include <string>
#include <thread>

#include "../include/GridManager.hpp"

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
    sf::RenderWindow window{sf::VideoMode{{1280u, 720u}}, "Teste"};

    window.setFramerateLimit(100u);

    GridManager gm{2, 2, window.getSize()};
    gm.setGridLimY(-500.f, 500.f, 0, 0);

    std::vector<float> value(4, 0.f);
    std::thread reader(generateRandomValuesThread, 4);

    while (window.isOpen())
    {
        while (const std::optional event{window.pollEvent()})
        {
            if (event->is<sf::Event::Closed>())
            {
                window.close();
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

        for (size_t col{0}; col < 2; ++col)
        {
            for (size_t row{0}; row < 2; ++row)
            {
                gm.sendData(value[row + col * 2], row, col);
            }
        }

        window.clear(sf::Color::White);
        gm.render(window);
        window.display();
    }

    running = false;
    reader.join();

    return 0;
}