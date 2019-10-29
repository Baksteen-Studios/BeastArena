#include "components/weapon_component.hpp"

WeaponComponent::WeaponComponent(double damage, double fireRate, double ammo, double clipSize)
                    : damage(damage), fireRate(fireRate), ammo(ammo), clipSize(clipSize) {}

std::string WeaponComponent::getNameStatic() {
    return "WeaponComponent";
}