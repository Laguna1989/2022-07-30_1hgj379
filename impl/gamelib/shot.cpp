#include "shot.hpp"
#include "game_properties.hpp"
#include <game_interface.hpp>
#include <iostream>

void Shot::doCreate()
{
    m_shape = std::make_shared<jt::Shape>();
    m_shape->makeRect(jt::Vector2f { 3, 3 }, getGame()->gfx().textureManager());

    m_shape->setColor(GP::getPalette().getColor(1));
    m_shape->setOffset(jt::Vector2f { -1.5, -1.5f });
}
void Shot::doUpdate(float const elapsed)
{
    if (pos.x < -20 || pos.x > GP::GetScreenSize().x + 20) {
        kill();
    }

    if (pos.y < -20 || pos.y > GP::GetScreenSize().y + 20) {
        kill();
    }

    pos += vel * elapsed;
    m_shape->setPosition(pos);
    m_shape->update(elapsed);
}
void Shot::doDraw() const { m_shape->draw(getGame()->gfx().target()); }
