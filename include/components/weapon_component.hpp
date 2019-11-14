#ifndef FILE_WEAPON_COMPONENT_HPP
#define FILE_WEAPON_COMPONENT_HPP

#include <memory>

#include "brickengine/components/component_impl.hpp"
#include "brickengine/components/physics_component.hpp"
#include "brickengine/components/renderables/texture_component.hpp"
#include "brickengine/components/data/scale.hpp"

#include "components/damage_component.hpp"
#include "components/despawn_component.hpp"

class WeaponComponent : public ComponentImpl<WeaponComponent> {
public:
    WeaponComponent(DamageComponent bullet_damage, TextureComponent bullet_texture, PhysicsComponent bullet_physics, 
                    DespawnComponent bullet_despawn, Scale bullet_scale, double fire_rate, double ammo, double clip_size);
    static std::string getNameStatic();

    // Data
    DamageComponent bullet_damage;
    TextureComponent bullet_texture;
    // The velocity should ALWAYS be towards the positive directions,
    // when the weapon is faced in the negative direction, the velocity will get flipped for you
    PhysicsComponent bullet_physics;
    DespawnComponent bullet_despawn;
    Scale bullet_scale;
    
    double fire_rate;
    double ammo;
    double clip_size;
};

#endif // FILE_WEAPON_COMPONENT_HPP