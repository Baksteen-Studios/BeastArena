#include "components/velocity_component.hpp"

VelocityComponent::VelocityComponent(double vx, double vy) : vx(vx), vy(vy) {}

std::string VelocityComponent::getNameStatic() {
    return "VelocityComponent";
}

// Getters and Setters
void VelocityComponent::setXVelocity(double v){
    vx = v;
}
double VelocityComponent::getXVelocity() const {
    return vx;
}
void VelocityComponent::setYVelocity(double v) {
    vy = v;
}
double VelocityComponent::getYVelocity() const {
    return vy;
}