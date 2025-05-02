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
#include <cmath>
#include <termios.h>
#include <cstring>
#include <fcntl.h>
#include <unistd.h>
#include <iostream>
#include <memory>

class Grid
{
public:
    Grid() = default;
    Grid(float xMin, float xMax, float yMin, float yMax, float yMinValue, float yMaxValue);

    ~Grid() = default;

    void drawVerticalLines(int32_t numVerticalLines = 10);

    void drawHorizontalLines(int32_t numHorizontalLines = 10);

    void drawAxisTicksY(int32_t numHorizontalLines);

    float yMinValue{};
    float yMaxValue{};

    sf::Vector2f min{};
    sf::Vector2f max{};

    sf::VertexArray m_axisTop{sf::PrimitiveType::Lines, 2};
    sf::VertexArray m_axisRight{sf::PrimitiveType::Lines, 2};
    sf::VertexArray m_axisX{sf::PrimitiveType::Lines, 2};
    sf::VertexArray m_axisY{sf::PrimitiveType::Lines, 2};

    std::vector<sf::VertexArray> m_verticalLines{};
    std::vector<sf::VertexArray> m_horizontalLines{};

    const sf::Font* font{ new sf::Font("JetBrainsMono-Regular.ttf") };
    std::vector<sf::Text> m_axisTextY{};
};