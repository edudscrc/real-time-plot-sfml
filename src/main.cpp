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

const float mapPointToGridY(const float value, const float yLimMin, const float yLimMax, const float yGridMin, const float yGridMax, const float windowHeight) {
    float v = ((value - yLimMin) / (yLimMax - yLimMin) * (yGridMax - yGridMin) + yGridMin);
    return yGridMax - v + yGridMin;
}

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

    sf::VertexArray m_gridAxisX{};
    sf::VertexArray m_gridAxisY{};
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

    void createWindow(uint32_t width, uint32_t height)
    {
        this->m_windowWidth = width;
        this->m_windowHeight = height;

        this->m_window = { sf::VideoMode{ {width, height} }, "Real-Time Plot" };
    }

    void createSubplots(size_t numRows, size_t numCols, float offsetX = 10.f, float offsetY = 10.f)
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
            }
        }

        this->m_gridAxisX = sf::VertexArray{sf::PrimitiveType::Lines, this->m_numSubplots * 2};
        for (int i{ 0 }, j{ 0 }; i < this->m_numSubplots && j < (this->m_numSubplots * 2) - 1; ++i, j += 2)
        {
            this->m_gridAxisX[j].position = sf::Vector2f(this->m_gridCoordsMinX[i], this->m_gridCoordsMaxY[i]);
            this->m_gridAxisX[j].color = sf::Color::Red;
            this->m_gridAxisX[j + 1].position = sf::Vector2f(this->m_gridCoordsMaxX[i], this->m_gridCoordsMaxY[i]);
            this->m_gridAxisX[j + 1].color = sf::Color::Red;
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

    void show()
    {
        this->m_window.draw(this->m_gridAxisX);
        this->m_window.draw(this->m_gridAxisY);
        this->m_window.display();
    }
};

int main()
{
    Plot p{};
    p.createWindow(800, 600);
    p.createSubplots(2, 2, 50.f, 50.f);

    while (p.getWindow().isOpen())
    {
        while (const std::optional event{ p.pollWindowEvent() })
        {
            if (event->is<sf::Event::Closed>())
            {
                p.closeWindow();
            }
        }

        p.clear();

        // for (int i{ 0 }; i < numPoints; ++i) {
        //     p.plot(shapes1[i]);
        //     p.plot(shapes2[i]);
        //     p.plot(shapes3[i]);
        //     p.plot(shapes4[i]);
        // }

        p.show();
    }

    return 0;
}