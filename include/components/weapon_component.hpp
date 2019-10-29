#ifndef FILE_WEAPON_COMPONENT_HPP
#define FILE_WEAPON_COMPONENT_HPP

#include "brickengine/components/component_impl.hpp"

class WeaponComponent : public ComponentImpl<WeaponComponent> {
public:
    WeaponComponent(double damage, double fireRate, double ammo, double clipSize);
    static std::string getNameStatic();

    // Data
    double damage;
    double fireRate;
    double ammo;
    double clipSize;
};

#endif // FILE_WEAPON_COMPONENT_HPP
