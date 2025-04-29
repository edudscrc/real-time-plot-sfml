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
#include <vector>
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

    int numPoints = 4500;
    sf::VertexArray points {sf::PrimitiveType::Points, numPoints};
    for (int i = 0; i < numPoints; ++i)
    {
        points[i].position = sf::Vector2f{mapPointToGridX(i, 0, 4500, xGridMin, xGridMax), mapPointToGridY(i, 0, 4500, yGridMin, yGridMax, windowHeight)};
        // points[i].position = sf::Vector2f{mapPointToGridX(numPoints + i, 0, 4500, xGridMin, xGridMax), 250.f};
        points[i].color = sf::Color::White;
    }

    // std::vector<sf::VertexArray> yFixedValues {};

    // float yAxisHeight { std::abs(yAxis[0].position.y - yAxis[1].position.y) };
    // int32_t yAxisSections = static_cast<int32_t>(yAxisHeight / 10.f);

    // std::cout << yAxisSections << "\n";

    // float yAxisFirstPosition { yAxis[0].position.y };
    
    // for (size_t i {1}; i < yAxisSections - 1; ++i) {
    //     sf::VertexArray newVertexArray {sf::PrimitiveType::Lines, 2};

    //     newVertexArray[0].position = sf::Vector2f{45.f, yAxisFirstPosition + (yAxisHeight / yAxisSections) * i};
    //     newVertexArray[1].position = sf::Vector2f{55.f, yAxisFirstPosition + (yAxisHeight / yAxisSections) * i};

    //     yFixedValues.push_back(newVertexArray);
    // }

    while (window.isOpen())
    {
        while (const std::optional event {window.pollEvent()})
        {
            if (event->is<sf::Event::Closed>())
            {
                window.close();
            }
        }

        window.clear();

        window.draw(xAxis);
        window.draw(yAxis);
        window.draw(topLimit);
        window.draw(rightLimit);

        window.draw(points);

        // for (const auto& dash : yFixedValues)
        // {
        //     window.draw(dash);
        // }

        for (int i = 0; i < numPoints; ++i)
        {
            // std::cout << mapPointToGridX(1, 0, 4500, xGridMin, xGridMax) << "\n";
            points[i].position -= sf::Vector2f(mapPointToGridX(1, 0, 4500, xGridMin, xGridMax), 0.f);
        }

        window.display();

        sf::sleep(sf::milliseconds(60));
    }
}