#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/PrimitiveType.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Vertex.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/VideoMode.hpp>
#include <cstdint>
#include <cstdlib>
#include <cmath>
#include <termios.h>
#include <cstring>
#include <fcntl.h>
#include <unistd.h>
#include <iostream>
#include <memory>

class Grid
{
public:
    Grid() = default;
    Grid(float xMin, float xMax, float yMin, float yMax)
    {
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

    ~Grid() = default;

    void drawVerticalLines(int32_t numVerticalLines = 10)
    {
        float width {(this->max.x - this->min.x) / (numVerticalLines + 1)};

        std::vector<sf::VertexArray> verticalLines(numVerticalLines, sf::VertexArray{sf::PrimitiveType::Lines, 2});

        for (int i {0}; i < numVerticalLines; ++i)
        {
            verticalLines[i][0].position = {this->min.x + width * (i + 1), this->min.y};
            verticalLines[i][0].color = sf::Color(120, 120, 120);
            verticalLines[i][1].position = {this->min.x + width * (i + 1), this->max.y};
            verticalLines[i][1].color = sf::Color(120, 120, 120);
        }

        this->m_verticalLines = verticalLines;
    }

    void drawHorizontalLines(int32_t numHorizontalLines = 10)
    {
        float height {(this->max.y - this->min.y) / (numHorizontalLines + 1)};

        std::vector<sf::VertexArray> horizontalLines(numHorizontalLines, sf::VertexArray{sf::PrimitiveType::Lines, 2});

        for (int i {0}; i < numHorizontalLines; ++i)
        {
            horizontalLines[i][0].position = {this->min.x, this->min.y + height * (i + 1)};
            horizontalLines[i][0].color = sf::Color(120, 120, 120);
            horizontalLines[i][1].position = {this->max.x, this->min.y + height * (i + 1)};
            horizontalLines[i][1].color = sf::Color(120, 120, 120);
        }

        this->m_horizontalLines = horizontalLines;

        // sf::Text
        // std::vector<sf::Text> axisTextY(numHorizontalLines, sf::Text{this->font});

        for (int i {0}; i < numHorizontalLines; ++i) 
        {
            sf::Text newText{*this->font};
            newText.setPosition(sf::Vector2f{this->min.x, this->m_horizontalLines[i][0].position.y});
            newText.setString("1000");
            newText.setCharacterSize(12);
            newText.setFillColor(sf::Color::Black);
            newText.setStyle(sf::Text::Regular);
            this->m_axisTextY.push_back(newText);
        }

        // this->m_axisTextY = axisTextY;
    }

    sf::Vector2f min{};
    sf::Vector2f max{};

    sf::VertexArray m_axisTop{sf::PrimitiveType::Lines, 2};
    sf::VertexArray m_axisRight{sf::PrimitiveType::Lines, 2};
    sf::VertexArray m_axisX{sf::PrimitiveType::Lines, 2};
    sf::VertexArray m_axisY{sf::PrimitiveType::Lines, 2};

    std::vector<sf::VertexArray> m_verticalLines{};
    std::vector<sf::VertexArray> m_horizontalLines{};

    const sf::Font* font{ new sf::Font("JetBrainsMono-Regular.ttf") };
    std::vector<sf::Text> m_axisTextY{};
};

class Plot
{
private:
    sf::RenderWindow m_window{};

    uint32_t m_windowWidth{};
    uint32_t m_windowHeight{};

    size_t m_numSubplots{};
    size_t m_numRows{};
    size_t m_numCols{};

    float m_subplotSizeX{};
    float m_subplotSizeY{};

    std::vector<Grid> m_axes{};
public:
    Plot() = default;
    ~Plot() = default;

    // const

    constexpr size_t getNumSubplots() const
    {
        return this->m_numSubplots;
    }

    constexpr uint32_t getWindowWidth() const
    {
        return this->m_windowWidth;
    }

    constexpr uint32_t getWindowHeight() const
    {
        return this->m_windowHeight;
    }

    const Grid& getGrid(size_t row, size_t col)
    {
        return this->m_axes.at(row + col * this->m_numRows);
    }
    const Grid& getGrid(size_t idx)
    {
        return this->m_axes.at(idx);
    }

    constexpr float getSubplotSizeX() const
    {
        return this->m_subplotSizeX;
    }

    constexpr float getSubplotSizeY() const
    {
        return this->m_subplotSizeY;
    }

    const sf::RenderWindow& getWindow() const
    {
        return this->m_window;
    }

    void closeWindow()
    {
        this->m_window.close();
    }

    std::optional<sf::Event> pollWindowEvent()
    {
        return this->m_window.pollEvent();
    }

    void createWindow(uint32_t width, uint32_t height);

    void createSubplots(size_t numRows, size_t numCols, float offsetX = 10.f, float offsetY = 10.f);

    void clear()
    {
        this->m_window.clear(sf::Color::White);
    }

    void plot(const sf::VertexArray& ref)
    {
        this->m_window.draw(ref);
    }
    void plot(const sf::RectangleShape& ref)
    {
        this->m_window.draw(ref);
    }

    void show();
};
