#include "systems/damage_system.hpp"
#include "components/health_component.hpp"
#include "components/stats_component.hpp"

DamageSystem::DamageSystem(CollisionDetector2& cd, std::shared_ptr<EntityManager> em, std::shared_ptr<EntityFactory> ef)
    : BeastSystem(ef, em), collision_detector(cd) {}

void DamageSystem::update(double) {
    auto entities_with_damage_component = entityManager->getEntitiesByComponent<DamageComponent>();

    for (auto& [entity_id, damage_comp]: entities_with_damage_component) {
        auto collisions = collision_detector.detectCollision(entity_id);
        if (!collisions.empty()) {
            for (const Collision& c : collisions) {
                this->collide(*damage_comp, c);
            }
            if (damage_comp->despawn)
                entityManager->removeEntity(entity_id);
        }
    }
}

void DamageSystem::collide(DamageComponent& damage_comp, const Collision& collision) {
    auto health_hit_entity = entityManager->getComponent<HealthComponent>(collision.opposite_id);
    if (health_hit_entity) {
        health_hit_entity->health -= damage_comp.damage;
        // u ded m8
        if (health_hit_entity->health <= 0) {
            health_hit_entity->on_death(collision.opposite_id);

            if (damage_comp.damage_dealer_entity_id) {
                auto points_gainer = entityManager->getComponent<StatsComponent>(*damage_comp.damage_dealer_entity_id);
                if (points_gainer && health_hit_entity->points_on_kill) {
                    points_gainer->points += *health_hit_entity->points_on_kill;
                }
            }
        }
    }
}