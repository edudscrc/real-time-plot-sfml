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
    return (yGridMax + yGridMin) - ((value - yLimMin) / (yLimMax - yLimMin) * (yGridMax - yGridMin) + yGridMin);
}

float mapPointToGridX(float value, float xLimMin, float xLimMax, float xGridMin, float xGridMax) {
    return (value - xLimMin) / (xLimMax - xLimMin) * (xGridMax - xGridMin) + xGridMin;
}

int main()
{
    constexpr uint16_t windowWidth { 800u };
    constexpr uint16_t windowHeight { 800u };

    constexpr float xGridMin { 100.f };
    constexpr float xGridMax { 700.f };
    constexpr float yGridMin { 50.f };
    constexpr float yGridMax { 450.f };

    sf::Font font { "JetBrainsMono-Regular.ttf" };
    sf::Text xAxisText { font };
    xAxisText.setString("X Axis");
    xAxisText.setFillColor(sf::Color::White);
    xAxisText.setCharacterSize(16);
    xAxisText.setPosition({static_cast<float>((xGridMax + xGridMin) / 2.f), yGridMax + 16.f });
    sf::Text yAxisText { font };
    yAxisText.setString("Y Axis");
    yAxisText.setFillColor(sf::Color::White);
    yAxisText.setCharacterSize(16);
    yAxisText.rotate(sf::degrees(-90));
    yAxisText.setPosition({xGridMin - 24.f, static_cast<float>((xGridMax + yGridMin) / 2.f)});

    sf::RenderWindow window {sf::VideoMode{{windowWidth, windowHeight}}, "Real-Time Plot"};

    window.setFramerateLimit(100);

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

    constexpr int numPoints { static_cast<int>(xGridMax) - static_cast<int>(xGridMin) };
    constexpr double twoPi = 2.0 * M_PI;
    std::queue<float> myQueue {};

    // std::array<sf::Vertex, numPoints> points {};

    std::array<sf::RectangleShape, numPoints> shapes {};

    for (int i { 0 }; i < 10000; ++i)
    {
        myQueue.push(std::sin(twoPi * (static_cast<float>(i) / numPoints)));
    }

    // for (auto& p : points)
    // {
    //     p.position = sf::Vector2f{0.f, 0.f};
    //     p.color = sf::Color::Black;
    // }

    for (auto& s : shapes)
    {
        s.setFillColor(sf::Color::Black);
        s.setPosition({0.f, 0.f});
        s.setSize({1.f, 5.f});
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
            // points[currentPointsIdx].position.y = mapPointToGridY(myQueue.front(), -5.f, 5.f, yGridMin, yGridMax, windowHeight);
            shapes[currentPointsIdx].setPosition({xGridMax, mapPointToGridY(myQueue.front(), -5.f, 5.f, yGridMin, yGridMax, windowHeight)});
            myQueue.pop();
            shapes[currentPointsIdx].setFillColor(sf::Color::White);
            // points[currentPointsIdx].position.x = 450.f;
            // points[currentPointsIdx].color = sf::Color::White;
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

        window.draw(xAxisText);
        window.draw(yAxisText);

        // window.draw(points.data(), points.size(), sf::PrimitiveType::Points);
        for (auto& s : shapes) {
            window.draw(s);
        }

        window.display();

        // for (auto& p : points) {
        //     p.position.x -= 1.f;
        // }

        for (auto& s : shapes) {
            s.move({-1.f, 0.f});
        }
    }

    return 0;
}