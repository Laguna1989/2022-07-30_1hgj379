#include "game_properties.hpp"

jt::Palette GP::getPalette()
{
    static auto const p = jt::Palette::parseGPL(R"(GIMP Palette
#Palette Name: Dreamer5
#Description:
#Colors: 5
47	48	48	2f3030
186	44	56	ba2c38
255	107	107	ff6b6b
249	227	225	f9e3e1
255	255	255	ffffff
)");
    return p;
}

int GP::PhysicVelocityIterations() { return 20; }

int GP::PhysicPositionIterations() { return 20; }
jt::Vector2f GP::PlayerSize() { return jt::Vector2f { 16.0f, 16.0f }; }
