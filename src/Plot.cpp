#include "../include/Plot.hpp"

void Plot::createWindow(uint32_t width, uint32_t height)
{
    this->m_windowWidth = width;
    this->m_windowHeight = height;

    this->m_window = { sf::VideoMode{ {width, height} }, "Real-Time Plot" };
}

void Plot::createSubplots(size_t numRows, size_t numCols, float offsetX, float offsetY)
{
    this->m_numRows = numRows;
    this->m_numCols = numCols;

    this->m_numSubplots = numRows * numCols;

    this->m_subplotSizeX = (static_cast<float>(this->m_windowWidth) - ((numCols + 1) * offsetX)) / static_cast<float>(numCols);
    this->m_subplotSizeY = (static_cast<float>(this->m_windowHeight) - ((numRows + 1) * offsetY)) / static_cast<float>(numRows);

    for (int i{ 0 }; i < numRows; ++i)
    {
        for (int j{ 0 }; j < numCols; ++j)
        {
            this->m_gridCoordsMinX.push_back((offsetX * (j + 1)) + (this->m_subplotSizeX * j));
            this->m_gridCoordsMaxX.push_back(((offsetX * (j + 1)) + (this->m_subplotSizeX * j)) + this->m_subplotSizeX);
            this->m_gridCoordsMinY.push_back((offsetY * (i + 1)) + (this->m_subplotSizeY * i));
            this->m_gridCoordsMaxY.push_back(((offsetY * (i + 1)) + (this->m_subplotSizeY * i)) + this->m_subplotSizeY);

            this->m_gridCoordsTopMinX.push_back(this->m_gridCoordsMinX.back());
            this->m_gridCoordsTopMaxX.push_back(this->m_gridCoordsMaxX.back());
            this->m_gridCoordsTopMinY.push_back(this->m_gridCoordsMinY.back());
            this->m_gridCoordsTopMaxY.push_back(this->m_gridCoordsMinY.back());

            this->m_gridCoordsRightMinX.push_back(this->m_gridCoordsMaxX.back());
            this->m_gridCoordsRightMaxX.push_back(this->m_gridCoordsMaxX.back());
            this->m_gridCoordsRightMinY.push_back(this->m_gridCoordsMinY.back());
            this->m_gridCoordsRightMaxY.push_back(this->m_gridCoordsMaxY.back());
        }
    }
    
    this->m_gridRightLimit = sf::VertexArray{sf::PrimitiveType::Lines, this->m_numSubplots * 2};
    this->m_gridTopLimit = sf::VertexArray{sf::PrimitiveType::Lines, this->m_numSubplots * 2};
    this->m_gridAxisX = sf::VertexArray{sf::PrimitiveType::Lines, this->m_numSubplots * 2};
    for (int i{ 0 }, j{ 0 }; i < this->m_numSubplots && j < (this->m_numSubplots * 2) - 1; ++i, j += 2)
    {
        this->m_gridAxisX[j].position = sf::Vector2f(this->m_gridCoordsMinX[i], this->m_gridCoordsMaxY[i]);
        this->m_gridAxisX[j].color = sf::Color::Red;
        this->m_gridAxisX[j + 1].position = sf::Vector2f(this->m_gridCoordsMaxX[i], this->m_gridCoordsMaxY[i]);
        this->m_gridAxisX[j + 1].color = sf::Color::Red;

        this->m_gridTopLimit[j].position = sf::Vector2f(this->m_gridCoordsTopMinX[i], this->m_gridCoordsTopMinY[i]);
        this->m_gridTopLimit[j].color = sf::Color::Blue;
        this->m_gridTopLimit[j + 1].position = sf::Vector2f(this->m_gridCoordsTopMaxX[i], this->m_gridCoordsTopMinY[i]);
        this->m_gridTopLimit[j + 1].color = sf::Color::Blue;

        this->m_gridRightLimit[j].position = sf::Vector2f(this->m_gridCoordsRightMaxX[i], this->m_gridCoordsRightMinY[i]);
        this->m_gridRightLimit[j].color = sf::Color::Blue;
        this->m_gridRightLimit[j + 1].position = sf::Vector2f(this->m_gridCoordsRightMaxX[i], this->m_gridCoordsRightMaxY[i]);
        this->m_gridRightLimit[j + 1].color = sf::Color::Blue;
    }

    this->m_gridAxisY = sf::VertexArray{ sf::PrimitiveType::Lines, this->m_numSubplots * 2 };
    for (int i{ 0 }, j{ 0 }; i < this->m_numSubplots && j < (this->m_numSubplots * 2) - 1; ++i, j += 2)
    {
        this->m_gridAxisY[j].position = sf::Vector2f(this->m_gridCoordsMinX[i], this->m_gridCoordsMinY[i]);
        this->m_gridAxisY[j].color = sf::Color::Red;
        this->m_gridAxisY[j + 1].position = sf::Vector2f(this->m_gridCoordsMinX[i], this->m_gridCoordsMaxY[i]);
        this->m_gridAxisY[j + 1].color = sf::Color::Red;
    }


}

void Plot::show()
{
    this->m_window.draw(this->m_gridAxisX);
    this->m_window.draw(this->m_gridAxisY);
    this->m_window.draw(this->m_gridTopLimit);
    this->m_window.draw(this->m_gridRightLimit);
    this->m_window.display();
}
