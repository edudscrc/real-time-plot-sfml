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
    }

    ~Grid() = default;

    sf::Vector2f min{};
    sf::Vector2f max{};

    sf::VertexArray m_axisTop{sf::PrimitiveType::Lines, 2};
    sf::VertexArray m_axisRight{sf::PrimitiveType::Lines, 2};
    sf::VertexArray m_axisX{sf::PrimitiveType::Lines, 2};
    sf::VertexArray m_axisY{sf::PrimitiveType::Lines, 2};
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
