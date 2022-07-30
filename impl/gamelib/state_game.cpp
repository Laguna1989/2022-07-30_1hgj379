#include "state_game.hpp"
#include "drawable_helpers.hpp"
#include "random/random.hpp"
#include <box2dwrapper/box2d_world_impl.hpp>
#include <color/color.hpp>
#include <game_interface.hpp>
#include <game_properties.hpp>
#include <math_helper.hpp>
#include <screeneffects/vignette.hpp>
#include <shape.hpp>
#include <sprite.hpp>
#include <state_menu.hpp>
#include <tweens/tween_alpha.hpp>
#include <algorithm>
#include <iostream>

void StateGame::doInternalCreate()
{
    m_world = std::make_shared<jt::Box2DWorldImpl>(jt::Vector2f { 0.0f, 0.0f });

    float const w = static_cast<float>(GP::GetWindowSize().x);
    float const h = static_cast<float>(GP::GetWindowSize().y);

    using jt::Shape;
    using jt::TweenAlpha;

    m_background = std::make_shared<Shape>();
    m_background->makeRect({ w, h }, getGame()->gfx().textureManager());
    m_background->setColor(GP::PaletteBackground());
    m_background->setIgnoreCamMovement(true);
    m_background->update(0.0f);

    m_earthPosition = jt::Vector2f { 300.0f, 200.0f };
    m_earth = std::make_shared<jt::Shape>();
    float const earthradius = 24.0f;
    m_earth->makeCircle(earthradius, getGame()->gfx().textureManager());
    m_earth->setPosition(m_earthPosition);
    m_earth->setColor(GP::getPalette().getColor(3));
    m_earth->setOffset(jt::Vector2f { -earthradius, -earthradius });

    float moonSize = 10.0f;
    m_moonPosition = jt::Vector2f { 300, 32 };
    m_moon = std::make_shared<jt::Shape>();
    m_moon->makeCircle(moonSize, getGame()->gfx().textureManager());
    m_moon->setPosition(m_moonPosition);
    m_moon->setColor(GP::getPalette().getColor(4));
    m_moon->setOffset(jt::Vector2f { -moonSize, -moonSize });

    m_shots = std::make_shared<jt::ObjectGroup<Shot>>();
    add(m_shots);

    //    m_particleSystem = jt::ParticleSystem<jt::Shape, 1000>::createPS(
    //        []() {
    //            auto s = std::make_shared<jt::Shape>();
    //            return s;
    //        },
    //        [this](std::shared_ptr<jt::Shape> s) {
    //            if (m_shots->size() == 0) {
    //                return;
    //            }
    //            auto const r = jt::Random::getInt(0, m_shots->size());
    //            s->setPosition(m_shots->at(r).lock()->pos);
    //
    //            add(jt::TweenAlpha::create(s, 1.0f, 40, 0));
    //        });
    //
    //    add(m_particleSystem);

    m_bar = std::make_shared<jt::Bar>(4, 16, false, getGame()->gfx().textureManager());
    m_bar->setColor(GP::getPalette().getColor(3));
    m_bar->setBackColor(GP::getPalette().getColor(1));
    m_bar->setMaxValue(6.0f);

    m_vignette = std::make_shared<jt::Vignette>(GP::GetScreenSize());
    add(m_vignette);

    m_shotCounterText = jt::dh::createText(getGame()->gfx().target(),
        "Shots fired: " + std::to_string(m_shotcounter), 16U, GP::getPalette().getColor(4));
    m_shotCounterText->setTextAlign(jt::Text::TextAlign::LEFT);
    m_shotCounterText->setPosition({ 10, 10 });
    m_shotCounterText->setShadow(GP::PaletteFontShadow(), jt::Vector2f { 1, 1 });

    // StateGame will call drawObjects itself.
    setAutoDraw(false);
}

void StateGame::doInternalUpdate(float const elapsed)
{
    if (m_running) {
        m_world->step(elapsed, GP::PhysicVelocityIterations(), GP::PhysicPositionIterations());
        // update game logic here
    }
    m_shots->update(elapsed);
    updateForces();

    m_background->update(elapsed);

    m_earth->update(elapsed);

    spawnShot(elapsed);

    checkShotMoonCollision(elapsed);

    float moonSpeed = 0.5f;
    float const t = getAge() * moonSpeed;
    float const distance = 168.0f;
    m_moonPosition = m_earthPosition + jt::Vector2f { 1.1f * sin(t) * distance, cos(t) * distance };
    m_moon->setPosition(m_moonPosition);
    m_moon->update(elapsed);

    m_bar->setCurrentValue(m_pressedCounter);
    m_bar->setPosition(
        getGame()->input().mouse()->getMousePositionScreen() + jt::Vector2f { 10, 0 });
    m_bar->update(elapsed);

    //    m_particleSystem->update(elapsed);

    m_vignette->update(elapsed);
    m_shotCounterText->update(elapsed);
}

void StateGame::spawnShot(float elapsed)
{
    if (getGame()->input().mouse()->pressed(jt::MouseButtonCode::MBLeft)) {
        m_pressedCounter += elapsed;
        if (m_pressedCounter >= 6.0f) {
            m_pressedCounter = 6.0f;
        }
    }
    if (getGame()->input().mouse()->justReleased(jt::MouseButtonCode::MBLeft)
        || m_pressedCounter >= 6.0f) {

        auto mp = getGame()->input().mouse()->getMousePositionScreen();
        auto emp = mp - m_earthPosition;

        auto l = jt::MathHelper::length(emp);
        if (l < 32) {
            return;
        }
        jt::MathHelper::normalizeMe(emp, 32);

        auto shot = std::make_shared<Shot>();
        shot->pos = m_earthPosition + emp * 24;

        float const velocity = 15.0f;
        shot->vel = emp * ((m_pressedCounter + 0.5f) * velocity);
        m_shots->push_back(shot);
        add(shot);

        m_pressedCounter = 0.0f;

        m_shotcounter++;
        m_shotCounterText->setText("Shots fired: " + std::to_string(m_shotcounter));
    }
}
void StateGame::updateForces()
{
    //    m_particleSystem->fire();
    for (auto& s : *m_shots) {
        auto shot = s.lock();
        auto const p = shot->pos;
    }
}

void StateGame::doInternalDraw() const
{
    m_background->draw(getGame()->gfx().target());
    //    drawObjects();

    for (auto& s : *m_shots) {
        auto shot = s.lock();
        shot->draw();
    }

    m_earth->draw(getGame()->gfx().target());
    m_moon->draw(getGame()->gfx().target());

    //    m_particleSystem->draw();

    m_vignette->draw();

    m_shotCounterText->draw(getGame()->gfx().target());
    m_bar->draw(getGame()->gfx().target());
}

void StateGame::endGame()
{
    if (m_hasEnded) {
        // trigger this function only once
        return;
    }
    m_hasEnded = true;
    m_running = false;

    std::shared_ptr<StateMenu> const& newMenuState = std::make_shared<StateMenu>();
    newMenuState->setScore(m_shotcounter);
    getGame()->stateManager().switchState(newMenuState);
}
std::string StateGame::getName() const { return "Game"; }

void StateGame::checkShotMoonCollision(float const elapsed)
{
    for (auto const& s : *m_shots) {
        auto const shot = s.lock();
        auto const p = shot->pos;

        auto const dir = m_moonPosition - p;
        auto const l = jt::MathHelper::length(dir);

        if (l < 13) {
            shot->kill();
            endGame();
        }
    }
}
