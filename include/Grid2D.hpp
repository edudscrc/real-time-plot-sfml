#ifndef GRID_2D_HPP
#define GRID_2D_HPP

#include "Point.hpp"
#include <SFML/Graphics.hpp>
#include <deque>
#include <iomanip>
#include <memory>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

class Grid2D
{
  private:
    const std::shared_ptr<sf::Font> m_font{std::make_shared<sf::Font>("JetBrainsMono-Regular.ttf")};
    size_t m_currentPointIdx{0};

    sf::Vector2f m_coordsMin;
    sf::Vector2f m_coordsMax;

    // For reference when resizing points' radius
    sf::Vector2f m_sizeOld;
    sf::Vector2f m_size;

    std::vector<Point> m_dataPoints;

    float m_valueMinY{-300.f};
    float m_valueMaxY{300.f};

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
    void updateSpines(const float pointRadius);
    void initGridLines(const float pointRadius);
    void initTicks();
    void initTickLabels();

    void setLimY(float minLimY, float maxLimY);

    void setDataPointsRadius(float radius);
    void setDataPointsColor(const sf::Color &color);

    void sendData(const float value);

    void update(const float value);
    void render(sf::RenderWindow &window) const;

    const float mapPointToGrid(const float pointValue, const float pointRadius) const;
    const float mapGridToPoint(const float gridValue) const;
};

#endif