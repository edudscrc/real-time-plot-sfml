#ifndef PLOT_HPP
#define PLOT_HPP

#include "GridManager.hpp"
#include <SFML/Graphics.hpp>

class Plot
{
  private:
    GridManager m_gridManager;

    sf::RenderWindow m_window;

    class PlotProxy
    {
      private:
        GridManager &m_gm;
        size_t m_row;

      public:
        PlotProxy(GridManager &gm, size_t row) : m_gm{gm}, m_row{row}
        {
        }

        Grid2D &operator[](size_t col)
        {
            return m_gm.getGrid(m_row, col);
        }
    };

  public:
    Plot() = delete;
    Plot(size_t numRows, size_t numCols, const sf::Vector2u &windowSize)
        : m_window{sf::VideoMode{{1280u, 720u}}, "Real Time Plot"}, m_gridManager{numRows, numCols, windowSize}
    {
        m_window.setFramerateLimit(100u);
    }
    ~Plot() = default;

    PlotProxy operator[](size_t row)
    {
        return PlotProxy(m_gridManager, row);
    }

    sf::RenderWindow &window()
    {
        return m_window;
    }

    void render()
    {
        m_window.clear(sf::Color::White);
        m_gridManager.render(m_window);
        m_window.display();
    }
};

#endif