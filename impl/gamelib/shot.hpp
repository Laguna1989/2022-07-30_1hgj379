#ifndef INC_1HGJ379_SHOT_HPP
#define INC_1HGJ379_SHOT_HPP

#include "shape.hpp"
#include "vector.hpp"
#include <game_object.hpp>

class Shot : public jt::GameObject {
public:
    jt::Vector2f pos;
    jt::Vector2f vel;

    std::shared_ptr<jt::Shape> m_shape;

    void doCreate() override;
    void doUpdate(float const elapsed) override;
    void doDraw() const override;
};

#endif // INC_1HGJ379_SHOT_HPP
