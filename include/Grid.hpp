#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/PrimitiveType.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Vertex.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/VideoMode.hpp>
#include <cmath>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <fcntl.h>
#include <iostream>
#include <memory>
#include <termios.h>
#include <unistd.h>

class Grid
{
  public:
    Grid() = delete;
    Grid(float xMin, float xMax, float yMin, float yMax, float yMinValue, float yMaxValue);

    ~Grid() = default;

    void drawVerticalLines(int32_t numVerticalLines = 10);

    void drawHorizontalLines(int32_t numHorizontalLines = 10);

    void drawAxisTicksY(int32_t numHorizontalLines);

    void send(const double value);

    const float mapPointToGridY(const float value)
    {
        float v = ((value - this->yMinValue) / (this->yMaxValue - this->yMinValue) * m_sizeY + this->min.y);
        return this->max.y - v + this->min.y;
    }

    const float mapPointToGridY_2(const float value)
    {
        float dataRange = this->yMaxValue - this->yMinValue;
        float pixelRange = this->max.y - this->min.y;

        // normalized_value = (data_y_value - data_y_min) / data_range
        float normalizedValue = (value - this->yMinValue) / dataRange;
        float pixel_y = this->max.y - (normalizedValue * pixelRange);

        return pixel_y;
        // float v = ((value - this->yMinValue) / (this->yMaxValue - this->yMinValue) * m_sizeY + this->min.y);
        // return this->max.y - v + this->min.y;
    }

    float m_pixelSize{5.f};

    float m_sizeX{};
    float m_sizeY{};

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

    const sf::Font *font{new sf::Font("JetBrainsMono-Regular.ttf")};
    std::vector<sf::Text> m_axisTextY{};

    std::vector<sf::RectangleShape> m_points{};
    int32_t m_currentPointIdx{};
};