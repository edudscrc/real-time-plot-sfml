#include <cmath>
#include <condition_variable>
#include <mutex>
#include <queue>
#include <string>
#include <thread>

#include "../include/Plot.hpp"

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
    Plot p{2, 2, {1280u, 720u}};

    p[0][0].setLimY(-500.f, 500.f);

    p[1][1].setDataPointsColor(sf::Color::Blue);

    p[1][0].setLimY(-500.f, 500.f);
    p[1][0].setDataPointsRadius(5.f);
    p[1][1].setDataPointsRadius(2.f);
    p[0][1].setDataPointsRadius(10.f);
    p[0][1].setLimY(-500.f, 500.f);

    std::vector<float> value(4, 0.f);
    std::thread reader(generateRandomValuesThread, 4);

    while (p.window().isOpen())
    {
        while (const std::optional event{p.window().pollEvent()})
        {
            if (event->is<sf::Event::Closed>())
            {
                p.window().close();
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
                p[row][col].sendData(value[row + col * 2]);
            }
        }

        // p.window().clear(sf::Color::White);
        // gm.render(p.window());
        p.render();
        // p.window().display();
    }

    running = false;
    reader.join();

    return 0;
}