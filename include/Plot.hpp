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

    std::vector<float> m_gridCoordsMinX{};
    std::vector<float> m_gridCoordsMaxX{};
    std::vector<float> m_gridCoordsMinY{};
    std::vector<float> m_gridCoordsMaxY{};

    std::vector<float> m_gridCoordsTopMinX{};
    std::vector<float> m_gridCoordsTopMaxX{};
    std::vector<float> m_gridCoordsTopMinY{};
    std::vector<float> m_gridCoordsTopMaxY{};

    std::vector<float> m_gridCoordsRightMinX{};
    std::vector<float> m_gridCoordsRightMaxX{};
    std::vector<float> m_gridCoordsRightMinY{};
    std::vector<float> m_gridCoordsRightMaxY{};

    sf::VertexArray m_gridAxisX{};
    sf::VertexArray m_gridAxisY{};

    sf::VertexArray m_gridTopLimit{};
    sf::VertexArray m_gridRightLimit{};
public:
    Plot() = default;
    ~Plot() = default;

    constexpr uint32_t getWindowWidth() const
    {
        return this->m_windowWidth;
    }

    constexpr uint32_t getWindowHeight() const
    {
        return this->m_windowHeight;
    }

    const std::vector<float>& getGridCoordsMaxX() const
    {
        return this->m_gridCoordsMaxX;
    }

    const std::vector<float>& getGridCoordsMinX() const
    {
        return this->m_gridCoordsMinX;
    }

    const std::vector<float>& getGridCoordsMaxY() const
    {
        return this->m_gridCoordsMaxY;
    }

    const std::vector<float>& getGridCoordsMinY() const
    {
        return this->m_gridCoordsMinY;
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
