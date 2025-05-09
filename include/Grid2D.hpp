#ifndef GRID_2D_HPP
#define GRID_2D_HPP

#include <SFML/Graphics.hpp>
#include <deque>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>
#include "Point.hpp"

class Grid2D
{
  private:
    const std::shared_ptr<sf::Font> m_font{std::make_shared<sf::Font>("JetBrainsMono-Regular.ttf")};

    sf::Vector2f m_coordsMin;
    sf::Vector2f m_coordsMax;

    sf::Vector2f m_size;

    // std::deque<sf::CircleShape> m_dataPoints{};
    // std::deque<Point> m_dataPoints{};
    size_t m_currentPointIdx{0};
    size_t m_firstIdx{0};
    size_t m_lastIdx;
    std::vector<Point> m_dataPoints;

    float m_valueMinY{-1000.f};
    float m_valueMaxY{1000.f};

    std::unordered_map<std::string, sf::VertexArray> m_spines;

    std::vector<sf::VertexArray> m_horizontalLines;
    std::vector<sf::VertexArray> m_verticalLines;

    std::vector<sf::VertexArray> m_ticksY;
    std::vector<sf::VertexArray> m_ticksX;

    std::vector<sf::Text> m_tickLabelsY;
    std::vector<sf::Text> m_tickLabelsX;

  public:
    Grid2D() = delete;
    Grid2D(const sf::Vector2f &minCoords, const sf::Vector2f &maxCoords);
    ~Grid2D() = default;

    void initSpines();
    void initGridLines();
    void initTicks();

    void setLimY(float minLimY, float maxLimY);

    void setDataPointsRadius(float radius);
    void setDataPointsColor(const sf::Color& color);

    void update(const float value);
    void render(sf::RenderWindow &window) const;

    const float mapPointToGrid(const float pointValue) const;
};

#endif