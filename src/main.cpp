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

int main()
{
    sf::RenderWindow window {sf::VideoMode{{500u, 500u}}, "Real-Time Plot"};

    window.setFramerateLimit(100);

    sf::VertexArray xAxis {sf::PrimitiveType::Lines, 2};
    xAxis[0].position = sf::Vector2f{50.f, 450.f};
    xAxis[1].position = sf::Vector2f{450.f, 450.f};
    xAxis[0].color = sf::Color::Blue;
    xAxis[1].color = sf::Color::Red;

    sf::VertexArray yAxis {sf::PrimitiveType::Lines, 2};
    yAxis[0].position = sf::Vector2f{50.f, 50.f};
    yAxis[1].position = sf::Vector2f{50.f, 450.f};
    yAxis[0].color = sf::Color::Blue;
    yAxis[1].color = sf::Color::Red;

    sf::VertexArray topLimit {sf::PrimitiveType::Lines, 2};
    topLimit[0].position = sf::Vector2f{50.f, 50.f};
    topLimit[1].position = sf::Vector2f{450.f, 50.f};
    topLimit[0].color = sf::Color::Green;
    topLimit[1].color = sf::Color::Green;

    sf::VertexArray bottomLimit {sf::PrimitiveType::Lines, 2};
    bottomLimit[0].position = sf::Vector2f{450.f, 50.f};
    bottomLimit[1].position = sf::Vector2f{450.f, 450.f};
    bottomLimit[0].color = sf::Color::Green;
    bottomLimit[1].color = sf::Color::Green;

    std::vector<sf::VertexArray> yFixedValues {};

    float yAxisHeight { std::abs(yAxis[0].position.y - yAxis[1].position.y) };
    int32_t yAxisSections = static_cast<int32_t>(yAxisHeight / 10.f);

    std::cout << yAxisSections << "\n";

    float yAxisFirstPosition { yAxis[0].position.y };
    
    for (size_t i {1}; i < yAxisSections - 1; ++i) {
        sf::VertexArray newVertexArray {sf::PrimitiveType::Lines, 2};

        newVertexArray[0].position = sf::Vector2f{45.f, yAxisFirstPosition + (yAxisHeight / yAxisSections) * i};
        newVertexArray[1].position = sf::Vector2f{55.f, yAxisFirstPosition + (yAxisHeight / yAxisSections) * i};

        yFixedValues.push_back(newVertexArray);
    }

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
        window.draw(bottomLimit);
        for (const auto& dash : yFixedValues)
        {
            window.draw(dash);
        }
        window.display();
    }
}