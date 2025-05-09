#include "../include/Point.hpp"

Point::Point(float value, float radius, const sf::Color &color, const sf::Vector2f &position)
    : m_value(value), m_radius{radius}, m_color{color}, m_position{position}, m_shape{radius}
{
    m_shape.setFillColor(color);
    m_shape.setPosition(position);
}

void Point::update()
{
    m_shape.move({-1.f, 0.f});
}

void Point::render(sf::RenderWindow& window) const
{
    window.draw(m_shape);
}