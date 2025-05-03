#include "Grid.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/PrimitiveType.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Vertex.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/VideoMode.hpp>
#include <cmath>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <fcntl.h>
#include <iostream>
#include <memory>
#include <termios.h>
#include <unistd.h>

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

    void sendValueToGrid(size_t gridIdx, const double value);

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

    const Grid &getGrid(size_t row, size_t col)
    {
        return this->m_axes.at(row + col * this->m_numRows);
    }
    const Grid &getGrid(size_t idx)
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

    const sf::RenderWindow &getWindow() const
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

    void createSubplots(size_t numRows, size_t numCols, float yMinValue, float yMaxValue, float offsetX = 10.f,
                        float offsetY = 10.f);

    void clear()
    {
        this->m_window.clear(sf::Color::White);
    }

    void plot(const sf::VertexArray &ref)
    {
        this->m_window.draw(ref);
    }
    void plot(const sf::RectangleShape &ref)
    {
        this->m_window.draw(ref);
    }

    void show();
};
