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

float mapPointToGridY(float value, float yLimMin, float yLimMax, float yGridMin, float yGridMax, float windowHeight) {
    return windowHeight - ((value - yLimMin) / (yLimMax - yLimMin) * (yGridMax - yGridMin) + yGridMin);
}

float mapPointToGridX(float value, float xLimMin, float xLimMax, float xGridMin, float xGridMax) {
    return (value - xLimMin) / (xLimMax - xLimMin) * (xGridMax - xGridMin) + xGridMin;
}

int main()
{
    constexpr uint16_t windowWidth { 500u };
    constexpr uint16_t windowHeight { 500u };

    sf::RenderWindow window {sf::VideoMode{{windowWidth, windowHeight}}, "Real-Time Plot"};

    window.setFramerateLimit(100);

    constexpr float xGridMin { 50.f };
    constexpr float xGridMax { 450.f };
    constexpr float yGridMin { 50.f };
    constexpr float yGridMax { 450.f };

    sf::VertexArray xAxis {sf::PrimitiveType::Lines, 2};
    xAxis[0].position = sf::Vector2f{xGridMin, yGridMax};
    xAxis[1].position = sf::Vector2f{xGridMax, yGridMax};
    xAxis[0].color = sf::Color::Blue;
    xAxis[1].color = sf::Color::Blue;

    sf::VertexArray yAxis {sf::PrimitiveType::Lines, 2};
    yAxis[0].position = sf::Vector2f{xGridMin, yGridMin};
    yAxis[1].position = sf::Vector2f{xGridMin, yGridMax};
    yAxis[0].color = sf::Color::Blue;
    yAxis[1].color = sf::Color::Blue;

    sf::VertexArray topLimit {sf::PrimitiveType::Lines, 2};
    topLimit[0].position = sf::Vector2f{xGridMin, yGridMin};
    topLimit[1].position = sf::Vector2f{xGridMax, yGridMin};
    topLimit[0].color = sf::Color::Green;
    topLimit[1].color = sf::Color::Green;

    sf::VertexArray rightLimit {sf::PrimitiveType::Lines, 2};
    rightLimit[0].position = sf::Vector2f{xGridMax, yGridMin};
    rightLimit[1].position = sf::Vector2f{xGridMax, yGridMax};
    rightLimit[0].color = sf::Color::Green;
    rightLimit[1].color = sf::Color::Green;

    constexpr int numPoints { 400 };
    constexpr double twoPi = 2.0 * M_PI;
    std::queue<float> myQueue {};

    std::array<sf::Vertex, numPoints> points {};

    for (int i { 0 }; i < 10000; ++i)
    {
        myQueue.push(std::sin(twoPi * (static_cast<float>(i) / numPoints)));
    }

    for (int i { 0 }; i < numPoints; ++i)
    {
        points[i].position = sf::Vector2f{0.f, 0.f};
        points[i].color = sf::Color::Black;
    }

    int currentPointsIdx = numPoints - 1;

    while (window.isOpen())
    {
        while (const std::optional event {window.pollEvent()})
        {
            if (event->is<sf::Event::Closed>())
            {
                window.close();
            }
        }

        if (!myQueue.empty())
        {
            points[currentPointsIdx].position.y = mapPointToGridY(myQueue.front(), -5.f, 5.f, yGridMin, yGridMax, windowHeight);
            myQueue.pop();
            points[currentPointsIdx].position.x = 450.f;
            points[currentPointsIdx].color = sf::Color::White;
            --currentPointsIdx;
            if (currentPointsIdx < 0) {
                currentPointsIdx = numPoints - 1;
            }
        }

        window.clear();

        window.draw(xAxis);
        window.draw(yAxis);
        window.draw(topLimit);
        window.draw(rightLimit);

        window.draw(points.data(), points.size(), sf::PrimitiveType::Points);

        window.display();

        for (auto& p : points) {
            p.position.x -= 1.f;
        }
    }

    return 0;
}