#include "../include/Grid2D.hpp"

Grid2D::Grid2D(const sf::Vector2f &minCoords, const sf::Vector2f &maxCoords)
    : m_coordsMin{minCoords.x, minCoords.y}, m_coordsMax{maxCoords.x, maxCoords.y},
      m_sizeOld{maxCoords.x - minCoords.x, maxCoords.y - minCoords.y},
      m_size{maxCoords.x - minCoords.x, maxCoords.y - minCoords.y},
      m_dataPoints({static_cast<size_t>(maxCoords.x - minCoords.x),
                    Point{0.f, 0.5f, sf::Color::Red, {-100.f, this->mapPointToGrid(0.f, 0.5f)}}})
{
    this->initSpines();
    this->initGridLines();
    this->initTicks();
}

void Grid2D::initSpines()
{
    m_spines.emplace("top", sf::VertexArray{sf::PrimitiveType::Lines, 2});
    m_spines.emplace("right", sf::VertexArray{sf::PrimitiveType::Lines, 2});
    // x-axis
    m_spines.emplace("bottom", sf::VertexArray{sf::PrimitiveType::Lines, 2});
    // y-axis
    m_spines.emplace("left", sf::VertexArray{sf::PrimitiveType::Lines, 2});

    this->updateSpines();
}

void Grid2D::updateSpines()
{
    // Left pixel
    m_spines.at("top")[0].position = {m_coordsMin.x, m_coordsMin.y};
    m_spines.at("top")[0].color = sf::Color::Black;
    // Right pixel
    m_spines.at("top")[1].position = {m_coordsMax.x, m_coordsMin.y};
    m_spines.at("top")[1].color = sf::Color::Black;

    // Top pixel
    m_spines.at("right")[0].position = {m_coordsMax.x, m_coordsMin.y};
    m_spines.at("right")[0].color = sf::Color::Black;
    // Bottom pixel
    m_spines.at("right")[1].position = {m_coordsMax.x, m_coordsMax.y};
    m_spines.at("right")[1].color = sf::Color::Black;

    // Left pixel
    m_spines.at("bottom")[0].position = {m_coordsMin.x, m_coordsMax.y};
    m_spines.at("bottom")[0].color = sf::Color::Black;
    // Right pixel
    m_spines.at("bottom")[1].position = {m_coordsMax.x, m_coordsMax.y};
    m_spines.at("bottom")[1].color = sf::Color::Black;

    // Top pixel
    m_spines.at("left")[0].position = {m_coordsMin.x, m_coordsMin.y};
    m_spines.at("left")[0].color = sf::Color::Black;
    // Bottom pixel
    m_spines.at("left")[1].position = {m_coordsMin.x, m_coordsMax.y};
    m_spines.at("left")[1].color = sf::Color::Black;
}

void Grid2D::initGridLines()
{
    size_t numHorizontalLines{5};
    float gap{m_size.y / (numHorizontalLines + 1)};

    for (size_t i{0}; i < numHorizontalLines; ++i)
    {
        m_horizontalLines.push_back(sf::VertexArray{sf::PrimitiveType::Lines, 2});
        m_horizontalLines[i][0].position = {m_coordsMin.x, m_coordsMin.y + gap * (i + 1)};
        m_horizontalLines[i][0].color = sf::Color(220, 220, 220);
        m_horizontalLines[i][1].position = {m_coordsMax.x, m_coordsMin.y + gap * (i + 1)};
        m_horizontalLines[i][1].color = sf::Color(220, 220, 220);
    }

    size_t numVerticalLines{5};
    gap = m_size.x / (numVerticalLines + 1);

    for (size_t i{0}; i < numVerticalLines; ++i)
    {
        m_verticalLines.push_back(sf::VertexArray{sf::PrimitiveType::Lines, 2});
        m_verticalLines[i][0].position = {m_coordsMin.x + gap * (i + 1), m_coordsMin.y};
        m_verticalLines[i][0].color = sf::Color(220, 220, 220);
        m_verticalLines[i][1].position = {m_coordsMin.x + gap * (i + 1), m_coordsMax.y};
        m_verticalLines[i][1].color = sf::Color(220, 220, 220);
    }
}

void Grid2D::initTicks()
{
    for (const auto &line : m_horizontalLines)
    {
        sf::VertexArray newTick{sf::PrimitiveType::Lines, 2};
        newTick[0].position = sf::Vector2f{line[0].position} - sf::Vector2f{10.f, 0.f};
        newTick[0].color = sf::Color::Black;
        newTick[1].position = sf::Vector2f{line[0].position};
        newTick[1].color = sf::Color::Black;
        m_ticksY.push_back(newTick);
    }

    for (const auto &line : m_verticalLines)
    {
        sf::VertexArray newTick{sf::PrimitiveType::Lines, 2};
        newTick[0].position = sf::Vector2f{line[1].position};
        newTick[0].color = sf::Color::Black;
        newTick[1].position = sf::Vector2f{line[1].position} + sf::Vector2f{0.f, 10.f};
        newTick[1].color = sf::Color::Black;
        m_ticksY.push_back(newTick);
    }
}

void Grid2D::setLimY(float minLimY, float maxLimY)
{
    m_valueMinY = minLimY;
    m_valueMaxY = maxLimY;
}

void Grid2D::setDataPointsRadius(float radius)
{
    m_coordsMax.x += radius * 2.f;
    m_coordsMax.y += radius * 2.f;

    m_size = {m_coordsMax.x - m_coordsMin.x, m_coordsMax.y - m_coordsMin.y};

    this->updateSpines();
    m_horizontalLines.clear();
    m_verticalLines.clear();
    this->initGridLines();
    m_ticksY.clear();
    m_ticksX.clear();
    this->initTicks();

    for (auto &point : m_dataPoints)
    {
        point.setRadius(radius);
    }
}

void Grid2D::setDataPointsColor(const sf::Color &color)
{
    for (auto &point : m_dataPoints)
    {
        point.setColor(color);
    }
}

void Grid2D::update(const float value)
{
    const float currentDataPointRadius = m_dataPoints[m_currentPointIdx].getRadius();

    m_dataPoints[m_currentPointIdx].setPosition(
        {m_coordsMax.x - currentDataPointRadius * 2.f, this->mapPointToGrid(value, currentDataPointRadius)});

    ++m_currentPointIdx;

    if (m_currentPointIdx >= m_dataPoints.size())
    {
        m_currentPointIdx = 0;
    }

    for (auto &point : m_dataPoints)
    {
        point.update();
    }
}

void Grid2D::render(sf::RenderWindow &window) const
{
    for (const auto &line : m_horizontalLines)
    {
        window.draw(line);
    }

    for (const auto &line : m_verticalLines)
    {
        window.draw(line);
    }

    for (const auto &tick : m_ticksY)
    {
        window.draw(tick);
    }

    for (const auto &tick : m_ticksX)
    {
        window.draw(tick);
    }

    for (const auto &point : m_dataPoints)
    {
        point.render(window);
    }

    window.draw(m_spines.at("top"));
    window.draw(m_spines.at("right"));
    window.draw(m_spines.at("bottom"));
    window.draw(m_spines.at("left"));
}

const float Grid2D::mapPointToGrid(const float pointValue, const float pointRadius) const
{
    float valueRangeY{m_valueMaxY - m_valueMinY};
    float normalizedValue{(pointValue - m_valueMinY) / valueRangeY};
    return (m_coordsMax.y - pointRadius * 2.f) - (normalizedValue * m_sizeOld.y);
}