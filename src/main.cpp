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
#include "../include/Plot.hpp"

// std::queue<std::string> serialQueue;
// std::mutex queueMutex;
// std::condition_variable queueCond;
// bool running = true;

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

// void serialReaderThread() {
//     const char* port = "/dev/ttyUSB0";  // Altere se necessário
//     int serialFd = open(port, O_RDONLY | O_NOCTTY);
//     if (serialFd < 0) {
//         std::cerr << "Erro ao abrir " << port << ": " << strerror(errno) << std::endl;
//         return;
//     }

//     termios tty;
//     memset(&tty, 0, sizeof tty);
//     if (tcgetattr(serialFd, &tty) != 0) {
//         std::cerr << "Erro em tcgetattr: " << strerror(errno) << std::endl;
//         close(serialFd);
//         return;
//     }

//     // Configurações da porta serial
//     cfsetospeed(&tty, B115200);
//     cfsetispeed(&tty, B115200);

//     tty.c_cflag |= (CLOCAL | CREAD);    // Habilita recepção
//     tty.c_cflag &= ~CSIZE;
//     tty.c_cflag |= CS8;                 // 8 bits
//     tty.c_cflag &= ~PARENB;             // Sem paridade
//     tty.c_cflag &= ~CSTOPB;             // 1 stop bit
//     tty.c_cflag &= ~CRTSCTS;            // Sem controle de fluxo

//     tty.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG); // Modo raw
//     tty.c_iflag &= ~(IXON | IXOFF | IXANY);         // Sem controle de fluxo por software
//     tty.c_oflag &= ~OPOST;                          // Saída raw

//     // Timeouts de leitura
//     tty.c_cc[VMIN] = 0;
//     tty.c_cc[VTIME] = 1; // Timeout de 100ms

//     if (tcsetattr(serialFd, TCSANOW, &tty) != 0) {
//         std::cerr << "Erro em tcsetattr: " << strerror(errno) << std::endl;
//         close(serialFd);
//         return;
//     }

//     std::string buffer;
//     char ch;

//     while (running) {
//         ssize_t n = read(serialFd, &ch, 1);
//         if (n > 0) {
//             if (ch == '\n') {
//                 std::lock_guard<std::mutex> lock(queueMutex);
//                 serialQueue.push(buffer);
//                 buffer.clear();
//                 queueCond.notify_one();
//             } else {
//                 buffer += ch;
//             }
//         } else {
//             usleep(1000); // evita uso excessivo de CPU
//         }
//     }

//     close(serialFd);
// }

// const float mapPointToGridY(const float value, const float yLimMin, const float yLimMax, const float yGridMin, const float yGridMax, const float windowHeight) {
//     float v = ((value - yLimMin) / (yLimMax - yLimMin) * (yGridMax - yGridMin) + yGridMin);
//     return yGridMax - v + yGridMin;
// }

int main()
{
    Plot p{};
    p.createWindow(800, 600);
    p.createSubplots(2, 2, 50.f, 50.f);

    // const int numPoints{ static_cast<int>(p.getSubplotSizeX()) };

    // std::vector<sf::RectangleShape> shapes1;
    // std::vector<sf::RectangleShape> shapes2;
    // std::vector<sf::RectangleShape> shapes3;
    // std::vector<sf::RectangleShape> shapes4;

    // for (int i{ 0 }; i < numPoints; ++i) {
    //     shapes1.push_back(sf::RectangleShape{});
    //     shapes1[i].setFillColor(sf::Color::Black);
    //     shapes1[i].setPosition({ 0.f, 0.f });
    //     shapes1[i].setSize({ 1.f, 5.f });
        
    //     shapes2.push_back(sf::RectangleShape{});
    //     shapes2[i].setFillColor(sf::Color::Black);
    //     shapes2[i].setPosition({ 0.f, 0.f });
    //     shapes2[i].setSize({ 1.f, 5.f });

    //     shapes3.push_back(sf::RectangleShape{});
    //     shapes3[i].setFillColor(sf::Color::Black);
    //     shapes3[i].setPosition({ 0.f, 0.f });
    //     shapes3[i].setSize({ 1.f, 5.f });

    //     shapes4.push_back(sf::RectangleShape{});
    //     shapes4[i].setFillColor(sf::Color::Black);
    //     shapes4[i].setPosition({ 0.f, 0.f });
    //     shapes4[i].setSize({ 1.f, 5.f });
    // }

    // int currentPointsIdx1 = numPoints - 1;
    // int currentPointsIdx2 = numPoints - 1;
    // int currentPointsIdx3 = numPoints - 1;
    // int currentPointsIdx4 = numPoints - 1;

    // float value1{ 0.f };
    // float value2{ 0.f };
    // float value3{ 0.f };
    // float value4{ 0.f };
    // std::thread reader(serialReaderThread);

    while (p.getWindow().isOpen())
    {
        while (const std::optional event{ p.pollWindowEvent() })
        {
            if (event->is<sf::Event::Closed>())
            {
                p.closeWindow();
            }
        }

        // std::unique_lock<std::mutex> lock(queueMutex);
        // queueCond.wait(lock, [] { return !serialQueue.empty(); });
        // std::string data = serialQueue.front();
        // serialQueue.pop();
        // lock.unlock();

        // std::cout << "data: " << data << "\n";

        // size_t pos1 = data.find("|");
        // if (pos1 != std::string::npos) {
        //     std::string part1 = data.substr(0, pos1);
        //     data = data.substr(pos1 + 1, data.size());
        //     value1 = std::stof(part1);
        // }
        // size_t pos2 = data.find("|");
        // if (pos2 != std::string::npos) {
        //     std::string part2 = data.substr(0, pos2);
        //     data = data.substr(pos2 + 1, data.size());
        //     value2 = std::stof(part2);
        // }
        // size_t pos3 = data.find("|");
        // if (pos3 != std::string::npos) {
        //     std::string part3 = data.substr(0, pos3);
        //     data = data.substr(pos3 + 1, data.size());
        //     value3 = std::stof(part3);
        // }
        // size_t pos4 = data.find("|");
        // if (pos4 != std::string::npos) {
        //     std::string part4 = data.substr(0, pos4);
        //     data = data.substr(pos4 + 1, data.size());
        //     value4 = std::stof(part4);
        // }

        // shapes1[currentPointsIdx1].setPosition({ p.getGridCoordsMaxX()[0], mapPointToGridY(value1, 0.f, 1500.f, p.getGridCoordsMinY()[0], p.getGridCoordsMaxY()[0], static_cast<float>(p.getWindowHeight())) });
        // shapes1[currentPointsIdx1].setFillColor(sf::Color::Magenta);
        // --currentPointsIdx1;
        // if (currentPointsIdx1 < 0) {
        //     currentPointsIdx1 = numPoints - 1;
        // }

        // shapes2[currentPointsIdx2].setPosition({ p.getGridCoordsMaxX()[1], mapPointToGridY(value2, 0.f, 1500.f, p.getGridCoordsMinY()[1], p.getGridCoordsMaxY()[1], static_cast<float>(p.getWindowHeight())) });
        // shapes2[currentPointsIdx2].setFillColor(sf::Color::Red);
        // --currentPointsIdx2;
        // if (currentPointsIdx2 < 0) {
        //     currentPointsIdx2 = numPoints - 1;
        // }

        // shapes3[currentPointsIdx3].setPosition({ p.getGridCoordsMaxX()[2], mapPointToGridY(value3, 0.f, 1500.f, p.getGridCoordsMinY()[2], p.getGridCoordsMaxY()[2], static_cast<float>(p.getWindowHeight())) });
        // shapes3[currentPointsIdx3].setFillColor(sf::Color::Blue);
        // --currentPointsIdx3;
        // if (currentPointsIdx3 < 0) {
        //     currentPointsIdx3 = numPoints - 1;
        // }

        // shapes4[currentPointsIdx4].setPosition({ p.getGridCoordsMaxX()[3], mapPointToGridY(value4, 0.f, 1500.f, p.getGridCoordsMinY()[3], p.getGridCoordsMaxY()[3], static_cast<float>(p.getWindowHeight())) });
        // shapes4[currentPointsIdx4].setFillColor(sf::Color::Green);
        // --currentPointsIdx4;
        // if (currentPointsIdx4 < 0) {
        //     currentPointsIdx4 = numPoints - 1;
        // }

        p.clear();

        // for (int i{ 0 }; i < numPoints; ++i) {
        //     p.plot(shapes1[i]);
        //     p.plot(shapes2[i]);
        //     p.plot(shapes3[i]);
        //     p.plot(shapes4[i]);
        // }

        p.show();
        
        // for (auto& s : shapes1)
        // {
        //     if (s.getPosition().x > p.getGridCoordsMinX()[0])
        //     {
        //         s.move({ -1.f, 0.f });
        //     }
        // }

        // for (auto& s : shapes2)
        // {
        //     if (s.getPosition().x > p.getGridCoordsMinX()[1])
        //     {
        //         s.move({ -1.f, 0.f });
        //     }
        // }

        // for (auto& s : shapes3)
        // {
        //     if (s.getPosition().x > p.getGridCoordsMinX()[2])
        //     {
        //         s.move({ -1.f, 0.f });
        //     }
        // }

        // for (auto& s : shapes4)
        // {
        //     if (s.getPosition().x > p.getGridCoordsMinX()[3])
        //     {
        //         s.move({ -1.f, 0.f });
        //     }
        // }
    }

    // running = false;
    // reader.join();

    return 0;
}