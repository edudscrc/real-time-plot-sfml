#ifndef PLOT_HPP
#define PLOT_HPP

#include "Grid2D.hpp"
#include <cstddef>
#include <vector>
#include <SFML/Graphics.hpp>

#include <iostream>

class Plot
{
  private:
  	sf::RenderWindow m_window;

	size_t m_numRows;
	size_t m_numCols;
	size_t m_numGrids;

	sf::Vector2u m_windowSize;

    // Gap between grids
	sf::Vector2f m_padding;

	std::vector<Grid2D> m_grids{};

    class PlotProxy
    {
      private:
	    std::vector<Grid2D>& m_grids;
		size_t m_numRows;
        size_t m_row;

      public:
        PlotProxy(std::vector<Grid2D>& grids, size_t numRows, size_t row) : m_grids{grids}, m_numRows{numRows}, m_row{row}
        {
        }

        Grid2D &operator[](size_t col)
        {
			return m_grids[m_row + col * m_numRows];
        }
    };

  public:
    Plot() = delete;
    Plot(size_t numRows, size_t numCols, const sf::Vector2u &windowSize);
    ~Plot() = default;

    PlotProxy operator[](size_t row)
    {
        return PlotProxy(m_grids, m_numRows, row);
    }

    sf::RenderWindow &window();

    void render();
};

#endif