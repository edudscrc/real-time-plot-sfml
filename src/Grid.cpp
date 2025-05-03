#include "../include/Grid.hpp"

Grid::Grid(float xMin, float xMax, float yMin, float yMax, float yMinValue, float yMaxValue)
    : m_sizeX{xMax - xMin}, m_sizeY{yMax - yMin},
      m_points{static_cast<int32_t>(xMax - xMin), sf::RectangleShape{{5.f, 5.f}}},
      m_currentPointIdx{static_cast<int32_t>(xMax - xMin) - 1}
{
    for (size_t i{0}; i < this->m_sizeX; ++i)
    {
        this->m_points[i].setPosition({0.f, 0.f});
        this->m_points[i].setFillColor(sf::Color::White);
    }

    this->yMinValue = yMinValue;
    this->yMaxValue = yMaxValue;

    this->min = {xMin, yMin};
    this->max = {xMax, yMax};

    this->m_axisTop[0].position = {xMin, yMin};
    this->m_axisTop[1].position = {xMax, yMin};

    this->m_axisRight[0].position = {xMax, yMin};
    this->m_axisRight[1].position = {xMax, yMax};

    this->m_axisX[0].position = {xMin, yMax};
    this->m_axisX[1].position = {xMax, yMax};

    this->m_axisY[0].position = {xMin, yMin};
    this->m_axisY[1].position = {xMin, yMax};

    this->m_axisTop[0].color = sf::Color::Red;
    this->m_axisTop[1].color = sf::Color::Red;
    this->m_axisRight[0].color = sf::Color::Red;
    this->m_axisRight[1].color = sf::Color::Red;
    this->m_axisX[0].color = sf::Color::Red;
    this->m_axisX[1].color = sf::Color::Red;
    this->m_axisY[0].color = sf::Color::Red;
    this->m_axisY[1].color = sf::Color::Red;

    this->drawVerticalLines();
    this->drawHorizontalLines();
}

void Grid::drawVerticalLines(int32_t numVerticalLines)
{
    float width{(this->max.x - this->min.x) / (numVerticalLines + 1)};

    std::vector<sf::VertexArray> verticalLines(numVerticalLines, sf::VertexArray{sf::PrimitiveType::Lines, 2});

    for (int i{0}; i < numVerticalLines; ++i)
    {
        verticalLines[i][0].position = {this->min.x + width * (i + 1), this->min.y};
        verticalLines[i][0].color = sf::Color(120, 120, 120);
        verticalLines[i][1].position = {this->min.x + width * (i + 1), this->max.y};
        verticalLines[i][1].color = sf::Color(120, 120, 120);
    }

    this->m_verticalLines = verticalLines;
}

void Grid::drawHorizontalLines(int32_t numHorizontalLines)
{
    float height{(this->max.y - this->min.y) / (numHorizontalLines + 1)};

    std::vector<sf::VertexArray> horizontalLines(numHorizontalLines, sf::VertexArray{sf::PrimitiveType::Lines, 2});

    for (int i{0}; i < numHorizontalLines; ++i)
    {
        horizontalLines[i][0].position = {this->min.x, this->min.y + height * (i + 1)};
        horizontalLines[i][0].color = sf::Color(120, 120, 120);
        horizontalLines[i][1].position = {this->max.x, this->min.y + height * (i + 1)};
        horizontalLines[i][1].color = sf::Color(120, 120, 120);
    }

    this->m_horizontalLines = horizontalLines;

    this->drawAxisTicksY(numHorizontalLines);
}

void Grid::drawAxisTicksY(int32_t numHorizontalLines)
{
    float auxValue{(this->yMaxValue - this->yMinValue) / (numHorizontalLines + 2)};

    {
        sf::Text newText{*this->font};
        newText.setString(std::to_string(static_cast<int>(this->yMaxValue)));
        newText.setCharacterSize(12);
        newText.setOrigin({newText.getLocalBounds().size.x * 1.2f, (newText.getLocalBounds().size.y / 2.f) * 1.3f});
        newText.setPosition(sf::Vector2f{this->min.x, this->min.y});
        newText.setFillColor(sf::Color::Black);
        newText.setStyle(sf::Text::Regular);
        this->m_axisTextY.push_back(newText);
    }

    for (int i{0}; i < numHorizontalLines; ++i)
    {
        sf::Text newText{*this->font};
        newText.setString(std::to_string(static_cast<int>(this->yMaxValue - (auxValue * (i + 1)))));
        newText.setCharacterSize(12);
        newText.setOrigin({newText.getLocalBounds().size.x * 1.2f, (newText.getLocalBounds().size.y / 2.f) * 1.3f});
        newText.setPosition(sf::Vector2f{this->min.x, this->m_horizontalLines[i][0].position.y});
        newText.setFillColor(sf::Color::Black);
        newText.setStyle(sf::Text::Regular);
        this->m_axisTextY.push_back(newText);
    }

    {
        sf::Text newText{*this->font};
        newText.setString(std::to_string(static_cast<int>(this->yMinValue)));
        newText.setCharacterSize(12);
        newText.setOrigin({newText.getLocalBounds().size.x * 1.2f, (newText.getLocalBounds().size.y / 2.f) * 1.3f});
        newText.setPosition(sf::Vector2f{this->min.x, this->max.y});
        newText.setFillColor(sf::Color::Black);
        newText.setStyle(sf::Text::Regular);
        this->m_axisTextY.push_back(newText);
    }
}

void Grid::send(const double value)
{
    this->m_points[this->m_currentPointIdx].setPosition({this->max.x, this->mapPointToGridY(value)});
    this->m_points[this->m_currentPointIdx].setFillColor(sf::Color::Red);
    --(this->m_currentPointIdx);
    if (this->m_currentPointIdx < 0)
    {
        this->m_currentPointIdx = this->m_sizeX - 1;
    }
}