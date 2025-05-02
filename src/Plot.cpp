#include "../include/Plot.hpp"

void Plot::createWindow(uint32_t width, uint32_t height)
{
    this->m_windowWidth = width;
    this->m_windowHeight = height;

    this->m_window = { sf::VideoMode{ {width, height} }, "Real-Time Plot" };

    this->m_window.setFramerateLimit(100u);
}

void Plot::createSubplots(size_t numRows, size_t numCols, float offsetX, float offsetY)
{
    this->m_numRows = numRows;
    this->m_numCols = numCols;

    this->m_numSubplots = numRows * numCols;

    this->m_subplotSizeX = (static_cast<float>(this->m_windowWidth) - ((numCols + 1) * offsetX)) / static_cast<float>(numCols);
    this->m_subplotSizeY = (static_cast<float>(this->m_windowHeight) - ((numRows + 1) * offsetY)) / static_cast<float>(numRows);

    // int32_t numVerticalLines { 10 };

    float xMin {};
    float xMax {};
    float yMin {};
    float yMax {};

    for (int j{ 0 }; j < numCols; ++j)
    {
        for (int i{ 0 }; i < numRows; ++i)
        {
            xMin = {(offsetX * (j + 1)) + (this->m_subplotSizeX * j)};
            xMax = {((offsetX * (j + 1)) + (this->m_subplotSizeX * j)) + this->m_subplotSizeX};
            yMin = {(offsetY * (i + 1)) + (this->m_subplotSizeY * i)};
            yMax = {((offsetY * (i + 1)) + (this->m_subplotSizeY * i)) + this->m_subplotSizeY};

            this->m_axes.push_back({xMin, xMax, yMin, yMax});
        }
    }
}

void Plot::show()
{
    for (int j{ 0 }; j < this->m_numCols; ++j)
    {
        for (int i{ 0 }; i < this->m_numRows; ++i)
        {
            this->m_window.draw(this->m_axes[static_cast<int>(i + j * this->m_numRows)].m_axisTop);
            this->m_window.draw(this->m_axes[static_cast<int>(i + j * this->m_numRows)].m_axisRight);
            this->m_window.draw(this->m_axes[static_cast<int>(i + j * this->m_numRows)].m_axisX);
            this->m_window.draw(this->m_axes[static_cast<int>(i + j * this->m_numRows)].m_axisY);
        }
    }
    this->m_window.display();
}
