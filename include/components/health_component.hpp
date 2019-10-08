#ifndef FILE_HEALTH_COMPONENT_HPP
#define FILE_HEALTH_COMPONENT_HPP

#include "brickengine/components/component_impl.hpp"

class HealthComponent : public ComponentImpl<HealthComponent> {
public:
    HealthComponent(int hp);
    static std::string getNameStatic();
private:
    int hp;
};

#endif /* FILE_HEALTH_COMPONENT_HPP */