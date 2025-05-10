#include "../include/Point.hpp"

Point::Point(float value, float radius, const sf::Color &color, const sf::Vector2f &position)
    : m_value(value), m_radius{radius}, m_color{color}, m_position{position}, m_shape{radius}
{
    m_shape.setFillColor(color);
    m_shape.setPosition(position);
}

void Point::setPosition(const sf::Vector2f &position)
{
    m_position = position;
    m_shape.setPosition(position);
}

void Point::setRadius(float radius)
{
    m_radius = radius;
    m_shape.setRadius(radius);
}

void Point::setColor(const sf::Color &color)
{
    m_color = color;
    m_shape.setFillColor(color);
}

const float &Point::getRadius() const
{
    return m_radius;
}

void Point::update()
{
    m_shape.move({-1.f, 0.f});
    m_position = m_shape.getPosition();
}

void Point::render(sf::RenderWindow &window) const
{
    window.draw(m_shape);
}