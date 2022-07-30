#include "star.hpp"
#include <game_interface.hpp>
#include <game_properties.hpp>
#include <random/random.hpp>

void Star::doCreate()
{
    m_shape = std::make_shared<jt::Shape>();
    m_shape->makeCircle(
        static_cast<float>(jt::Random::getInt(1, 2)), getGame()->gfx().textureManager());

    m_rand1 = jt::Random::getFloat(0.2f, 0.5f);
    m_rand2 = jt::Random::getFloat(0.0f, 1.5f);
    m_rand3 = jt::Random::getFloat(0.5f, 1.0f);

    auto p = jt::Random::getRandomPointIn(
        jt::Rectf { 0.0f, 0.0f, GP::GetScreenSize().x, GP::GetScreenSize().y });

    p.x = static_cast<int>(p.x);
    p.y = static_cast<int>(p.y);

    m_shape->setPosition(p);
}
void Star::doUpdate(float const elapsed)
{
    m_shape->update(elapsed);

    float const t = getAge() * m_rand1 + m_rand2;
    float const a = sin(t) * sin(t);
    std::uint8_t const alpha = a * 255 * m_rand3;
    m_shape->setColor(jt::Color { 255, 255, 255, alpha });
}
void Star::doDraw() const { m_shape->draw(getGame()->gfx().target()); }
