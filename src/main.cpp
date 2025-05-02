#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/PrimitiveType.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Vertex.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/VideoMode.hpp>
#include <cstdint>
#include <cstdlib>
#include <array>
#include <cmath>
#include <queue>
#include <iostream>
#include <string>
// #include <windows.h>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <termios.h>
#include <cstring>
#include <fcntl.h>
#include <unistd.h>
#include <random>
#include "../include/Plot.hpp"

std::queue<std::string> serialQueue;
std::mutex queueMutex;
std::condition_variable queueCond;
bool running = true;

// void serialReaderThread() {
//     HANDLE hSerial = CreateFile(
//         L"\\\\.\\COM3",
//         GENERIC_READ,
//         0,
//         NULL,
//         OPEN_EXISTING,
//         0,
//         NULL);

//     if (hSerial == INVALID_HANDLE_VALUE) {
//         std::cerr << "Erro ao abrir COM3." << std::endl;
//         return;
//     }

//     DCB dcbSerialParams = { 0 };
//     dcbSerialParams.DCBlength = sizeof(dcbSerialParams);
//     if (!GetCommState(hSerial, &dcbSerialParams)) {
//         std::cerr << "Erro ao obter parâmetros da COM3." << std::endl;
//         CloseHandle(hSerial);
//         return;
//     }

//     dcbSerialParams.BaudRate = CBR_115200;
//     dcbSerialParams.ByteSize = 8;
//     dcbSerialParams.StopBits = ONESTOPBIT;
//     dcbSerialParams.Parity = NOPARITY;
//     dcbSerialParams.fOutxCtsFlow = FALSE;
//     dcbSerialParams.fRtsControl = RTS_CONTROL_DISABLE;

//     if (!SetCommState(hSerial, &dcbSerialParams)) {
//         std::cerr << "Erro ao configurar COM3." << std::endl;
//         CloseHandle(hSerial);
//         return;
//     }

//     COMMTIMEOUTS timeouts = { 0 };
//     timeouts.ReadIntervalTimeout = 50;
//     timeouts.ReadTotalTimeoutConstant = 50;
//     timeouts.ReadTotalTimeoutMultiplier = 10;
//     SetCommTimeouts(hSerial, &timeouts);

//     FlushFileBuffers(hSerial);

//     std::string buffer;
//     char ch;
//     DWORD bytesRead;

//     while (running) {
//         if (ReadFile(hSerial, &ch, 1, &bytesRead, NULL) && bytesRead > 0) {
//             if (ch == '\n') {
//                 std::lock_guard<std::mutex> lock(queueMutex);
//                 serialQueue.push(buffer);
//                 buffer.clear();
//                 queueCond.notify_one();
//             }
//             else {
//                 buffer += ch;
//             }
//         }
//     }

//     CloseHandle(hSerial);
// }

void generateRandomValuesThread(size_t numSubplots) {
    std::random_device rd;
    std::mt19937 gen { rd() };
    std::uniform_int_distribution<int32_t> dist { 100, 1000 };

    std::string buffer{};

    while (running) {
        for (size_t i{0}; i < numSubplots; ++i)
        {
            buffer += std::to_string(dist(gen)) + "|";
        }
        buffer += '\n';
        std::lock_guard<std::mutex> lock(queueMutex);
        serialQueue.push(buffer);
        buffer.clear();
        queueCond.notify_one();
    }
}

const float mapPointToGridY(const float value, const float yLimMin, const float yLimMax, const float yGridMin, const float yGridMax, const float windowHeight) {
    float v = ((value - yLimMin) / (yLimMax - yLimMin) * (yGridMax - yGridMin) + yGridMin);
    return yGridMax - v + yGridMin;
}

int main()
{
    Plot p{};
    p.createWindow(1280, 720);
    p.createSubplots(2, 2, 50.f, 50.f);

    const int numPoints{ static_cast<int>(p.getSubplotSizeX()) };

    std::vector<std::vector<sf::RectangleShape>> shapes(p.getNumSubplots(), {numPoints, sf::RectangleShape{}});

    for (int i{0}; i < p.getNumSubplots(); ++i)
    {
        for (int j{0}; j < numPoints; ++j)
        {
            shapes[i][j].setFillColor(sf::Color::Black);
            shapes[i][j].setPosition({ 0.f, 0.f });
            shapes[i][j].setSize({ 1.f, 5.f });
        }
    }

    std::vector<int> currentPointsIdx(p.getNumSubplots(), numPoints - 1);
    std::vector<float> value(p.getNumSubplots(), 0.f);

    std::thread reader(generateRandomValuesThread, p.getNumSubplots());

    while (p.getWindow().isOpen())
    {
        while (const std::optional event{ p.pollWindowEvent() })
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

        std::cout << "data: " << data << "\n";

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
            shapes[i][currentPointsIdx[i]].setPosition({p.getGrid(i).max.x, mapPointToGridY(value[i], 0.f, 1500.f, p.getGrid(i).min.y, p.getGrid(i).max.y, static_cast<float>(p.getWindowHeight()))});
            shapes[i][currentPointsIdx[i]].setFillColor(sf::Color::Magenta);
            currentPointsIdx[i] -= 1;
            if (currentPointsIdx[i] < 0)
            {
                currentPointsIdx[i] = numPoints - 1;
            }
        }

        p.clear();

        for (int i{0}; i < p.getNumSubplots(); ++i)
        {
            for (int j{0}; j < numPoints; ++j)
            {
                p.plot(shapes[i][j]);
            }
        }

        p.show();

        for (int i{0}; i < p.getNumSubplots(); ++i)
        {
            for (int j{0}; j < numPoints; ++j)
            {
                if (shapes[i][j].getPosition().x > p.getGrid(i).min.x)
                {
                    shapes[i][j].move({-1.f, 0.f});
                }
            }
        }
    }

    running = false;
    reader.join();

    return 0;
}