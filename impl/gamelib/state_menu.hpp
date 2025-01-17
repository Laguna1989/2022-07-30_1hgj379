﻿#ifndef GAME_STATE_MENU_HPP
#define GAME_STATE_MENU_HPP

#include "star.hpp"
#include <game_state.hpp>

namespace jt {

class Text;
class Shape;
class Animation;
class Sprite;
class Vignette;

} // namespace jt

class StateMenu : public jt::GameState {
public:
    std::string getName() const override;

    void setScore(int s) { m_highscore = s; };

private:
    std::shared_ptr<jt::Shape> m_background;

    std::shared_ptr<jt::Text> m_text_Title;
    std::shared_ptr<jt::Text> m_text_Explanation;
    std::shared_ptr<jt::Text> m_text_Credits;
    std::shared_ptr<jt::Text> m_text_Version;

    std::shared_ptr<jt::Shape> m_overlay;
    std::shared_ptr<jt::Vignette> m_vignette;

    std::vector<std::shared_ptr<Star>> m_stars;

    bool m_started { false };

    int m_highscore { 0 };

    void doInternalCreate() override;

    void createVignette();
    void createShapes();

    void createMenuText();
    void createTextCredits();
    void createTextExplanation();
    void createTextTitle();

    void createTweens();
    void createInstructionTweenScaleUp();
    void createInstructionTweenScaleDown();
    void createTweenTitleAlpha();
    void createTweenOverlayAlpha();
    void createTweenCreditsPosition();
    void createTweenExplanationScale();

    void doInternalUpdate(float const elapsed) override;
    void updateDrawables(const float& elapsed);
    void checkForTransitionToStateGame();
    void startTransitionToStateGame();

    void doInternalDraw() const override;
};

#endif
