#ifndef POINT_HPP
#define POINT_HPP

#include <SFML/Graphics.hpp>

class Point
{
  private:
    float m_value{-1.f};
    float m_radius{2.f};
    sf::Color m_color{sf::Color::Red};
    sf::Vector2f m_position{-100.f, -100.f};

    sf::CircleShape m_shape;

  public:
    Point();
    ~Point() = default;

    void setPosition(const sf::Vector2f &position);
    void setRadius(float radius);
    void setColor(const sf::Color &color);

    const float &getRadius() const;

    void update();
    void render(sf::RenderWindow &window) const;
};

#endif