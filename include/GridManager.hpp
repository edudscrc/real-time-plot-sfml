#ifndef GRID_MANAGER_HPP
#define GRID_MANAGER_HPP

#include "Grid2D.hpp"
#include <cstddef>
#include <vector>

class GridManager
{
  private:
    size_t m_numRows;
    size_t m_numCols;
    size_t m_numGrids;

    sf::Vector2u m_windowSize;

    // Gap between grids
    sf::Vector2f m_padding;

    std::vector<Grid2D> m_grids{};

  public:
    GridManager() = delete;
    GridManager(size_t numRows, size_t numCols, const sf::Vector2u &windowSize);
    ~GridManager() = default;

    void setGridLimY(float minLim, float maxLim, size_t row, size_t col);

    void setGridPointSize(float size, size_t row, size_t col);
    void setGridDataColor(const sf::Color &color, size_t row, size_t col);

    void sendData(const float value, size_t row, size_t col);

    Grid2D &getGrid(size_t row, size_t col)
    {
        return m_grids[row + col * m_numRows];
    }

    // void update();
    void render(sf::RenderWindow &window) const;
};

#endif