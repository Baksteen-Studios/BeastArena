#ifndef FILE_VELOCITY_COMPONENT_HPP
#define FILE_VELOCITY_COMPONENT_HPP

#include "brickengine/components/component_impl.hpp"

class VelocityComponent : public ComponentImpl<VelocityComponent> {
public:
    VelocityComponent(double vx, double vy);
    static std::string getNameStatic();

    // Getters and Setters
    void setXVelocity(double v);
    double getXVelocity() const;
    void setYVelocity(double v);
    double getYVelocity() const;
private:
    double vx, vy;
};

#endif /* FILE_VELOCITY_COMPONENT_HPP */