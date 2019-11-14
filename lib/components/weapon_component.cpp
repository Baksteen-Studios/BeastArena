#include "components/weapon_component.hpp"

WeaponComponent::WeaponComponent(DamageComponent bullet_damage, TextureComponent bullet_texture,
                                 PhysicsComponent bullet_physics, DespawnComponent bullet_despawn, 
                                 Scale bullet_scale, double fire_rate, double ammo, double clip_size)
    : bullet_damage(bullet_damage), bullet_texture(bullet_texture), bullet_physics(bullet_physics), bullet_despawn(bullet_despawn),
                                bullet_scale(bullet_scale), fire_rate(fire_rate), ammo(ammo), clip_size(clip_size) {}

std::string WeaponComponent::getNameStatic() {
    return "WeaponComponent";
}