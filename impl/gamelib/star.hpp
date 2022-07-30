#ifndef INC_1HGJ379_STAR_HPP
#define INC_1HGJ379_STAR_HPP

#include <game_object.hpp>
#include <shape.hpp>

class Star : public jt::GameObject {
public:
    std::shared_ptr<jt::Shape> m_shape;

    float m_rand1;
    float m_rand2;
    float m_rand3;

    void doCreate() override;
    void doUpdate(float const elapsed) override;
    void doDraw() const override;
};

#endif // INC_1HGJ379_STAR_HPP
