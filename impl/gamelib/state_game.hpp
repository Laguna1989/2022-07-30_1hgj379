#ifndef GAME_STATE_GAME_HPP
#define GAME_STATE_GAME_HPP

#include "bar.hpp"
#include "object_group.hpp"
#include "particle_system.hpp"
#include "shot.hpp"
#include "text.hpp"
#include "vector.hpp"
#include <box2dwrapper/box2d_world_interface.hpp>
#include <game_state.hpp>
#include <memory>
#include <vector>

// fwd decls
namespace jt {
class Shape;
class Sprite;
class Vignette;
} // namespace jt

class Hud;

class StateGame : public jt::GameState {
public:
    std::string getName() const override;

private:
    std::shared_ptr<jt::Shape> m_background;
    std::shared_ptr<jt::Vignette> m_vignette;
    std::shared_ptr<jt::Box2DWorldInterface> m_world { nullptr };

    std::shared_ptr<jt::Shape> m_earth { nullptr };
    std::shared_ptr<jt::Shape> m_moon { nullptr };

    jt::Vector2f m_earthPosition;
    jt::Vector2f m_moonPosition;

    std::shared_ptr<jt::ObjectGroup<Shot>> m_shots;

    bool m_running { true };
    bool m_hasEnded { false };

    float m_pressedCounter { 0.0f };

    int m_shotcounter { 0 };

    std::shared_ptr<jt::Text> m_shotCounterText;

    std::shared_ptr<jt::ParticleSystem<jt::Shape, 1000>> m_particleSystem;

    std::shared_ptr<jt::Bar> m_bar;

    void doInternalCreate() override;
    void doInternalUpdate(float const elapsed) override;
    void doInternalDraw() const override;

    void endGame();
    void updateForces();
    void spawnShot(float elapsed);
    void checkShotMoonCollision(float const elapsed);
};

#endif
