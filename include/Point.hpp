#ifndef POINT_HPP
#define POINT_HPP

#include <SFML/Graphics.hpp>

class Point
{
    private:
        float m_value;
        float m_radius;
        sf::Color m_color;
        sf::Vector2f m_position;

        sf::CircleShape m_shape;

    public:
        Point() = delete;
        Point(float value, float radius, const sf::Color &color, const sf::Vector2f &position);
        ~Point() = default;

        void setPosition(const sf::Vector2f &position);
        void setRadius(float radius);
        void setColor(const sf::Color& color);

        void update();
        void render(sf::RenderWindow& window) const;
};

#endif