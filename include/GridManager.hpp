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

    sf::Vector2f m_padding;

    std::vector<Grid2D> m_grids{};

  public:
    GridManager() = delete;
    GridManager(size_t numRows, size_t numCols, sf::Vector2u windowSize);
    ~GridManager() = default;

    void update();
    void render(sf::RenderWindow &window) const;
};

#endif