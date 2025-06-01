#include "../include/Plot.hpp"

Plot::Plot(size_t numRows, size_t numCols, const sf::Vector2u &windowSize)
    : m_window{sf::VideoMode{windowSize}, "Real Time Plot"}, m_numRows{numRows}, m_numCols{numCols}
    , m_numGrids{numRows * numCols}, m_windowSize{windowSize},
    m_padding{0.5f * m_windowSize.x / 100.f, 10.f * m_windowSize.y / 100.f}
{
    m_window.setFramerateLimit(60u);

    sf::Vector2f gridSize{(windowSize.x - (numCols + 1) * m_padding.x) / numCols,
                            (windowSize.y - (numRows + 1) * m_padding.y) / numRows};

    for (size_t col{0}; col < numCols; ++col)
    {
        for (size_t row{0}; row < numRows; ++row)
        {
            float xMin{(col + 1) * m_padding.x + col * gridSize.x};
            float xMax{xMin + gridSize.x};
            float yMin{(row + 1) * m_padding.y + row * gridSize.y};
            float yMax{yMin + gridSize.y};

            m_grids.push_back({{xMin, yMin}, {xMax, yMax}});
        }
    }
}

void Plot::render()
{
    m_window.clear(sf::Color::White);
    for (const auto &grid : m_grids)
    {
        grid.render(m_window);
    }
    m_window.display();
}

sf::RenderWindow& Plot::window()
{
    return m_window;
}