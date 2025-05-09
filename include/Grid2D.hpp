#ifndef GRID_2D_HPP
#define GRID_2D_HPP

#include <SFML/Graphics.hpp>
#include <deque>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

class Grid2D
{
  private:
    const std::shared_ptr<sf::Font> m_font{std::make_shared<sf::Font>("JetBrainsMono-Regular.ttf")};

    sf::Vector2f m_coordsMin;
    sf::Vector2f m_coordsMax;

    sf::Vector2f m_size;

    std::deque<sf::CircleShape> m_dataPoints{};

    float m_valueMinY{};
    float m_valueMaxY{};

    std::unordered_map<std::string, sf::VertexArray> m_spines;

    std::vector<sf::VertexArray> m_horizontalLines;
    std::vector<sf::VertexArray> m_verticalLines;

    std::vector<sf::VertexArray> m_ticksY;
    std::vector<sf::VertexArray> m_ticksX;

    std::vector<sf::Text> m_tickLabelsY;
    std::vector<sf::Text> m_tickLabelsX;

  public:
    Grid2D() = delete;
    Grid2D(sf::Vector2f minCoords, sf::Vector2f maxCoords);
    ~Grid2D() = default;

    void initSpines();
    void initGridLines();
    void initTicks();

    void initLim(float minLimY, float maxLimY);

    void update();
    void render(sf::RenderWindow &window) const;
};

#endif