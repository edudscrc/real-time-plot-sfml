#include "../include/GridManager.hpp"

GridManager::GridManager(size_t numRows, size_t numCols, sf::Vector2u windowSize)
    : m_numRows{numRows}, m_numCols{numCols}, m_numGrids{numRows * numCols}, m_windowSize{windowSize},
      m_padding{10.f * m_windowSize.x / 100.f, 10.f * m_windowSize.y / 100.f}

{
    sf::Vector2f gridSize{(windowSize.x - (numCols + 1) * m_padding.x) / numCols,
                          (windowSize.y - (numRows + 1) * m_padding.y) / numRows};

    for (int col{0}; col < numCols; ++col)
    {
        for (int row{0}; row < numRows; ++row)
        {
            float xMin{(col + 1) * m_padding.x + col * gridSize.x};
            float xMax{xMin + gridSize.x};
            float yMin{(row + 1) * m_padding.y + row * gridSize.y};
            float yMax{yMin + gridSize.y};

            m_grids.push_back({{xMin, yMin}, {xMax, yMax}});
        }
    }
}

void GridManager::update()
{
    for (auto &grid : m_grids)
    {
        grid.update();
    }
}

void GridManager::render(sf::RenderWindow &window) const
{
    for (const auto &grid : m_grids)
    {
        grid.render(window);
    }
}