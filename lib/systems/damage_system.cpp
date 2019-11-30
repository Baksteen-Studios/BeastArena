#include "systems/damage_system.hpp"
#include "components/health_component.hpp"
#include "components/stats_component.hpp"

DamageSystem::DamageSystem(std::shared_ptr<CollisionDetector> cd, std::shared_ptr<EntityManager> em, std::shared_ptr<EntityFactory> ef)
    : BeastSystem(ef, em), collision_detector(cd) {}

void DamageSystem::update(double) {
    auto entities_with_damage_component = entityManager->getEntitiesByComponent<DamageComponent>();
    
    for (auto& [entity_id, damage_comp]: entities_with_damage_component) {
        {
            auto collision = collision_detector->spaceLeft(entity_id, Axis::X, Direction::NEGATIVE);
            if (collision.space_left >= 0 && collision.object_id && !collision.is_trigger)
                this->collide(damage_comp, collision);
        }
        {
            auto collision = collision_detector->spaceLeft(entity_id, Axis::X, Direction::POSITIVE);
            if (collision.space_left <= 0 && collision.object_id && !collision.is_trigger)
                this->collide(damage_comp, collision);
        }
        {
            auto collision = collision_detector->spaceLeft(entity_id, Axis::Y, Direction::NEGATIVE);
            if (collision.space_left >= 0 && collision.object_id && !collision.is_trigger)
                this->collide(damage_comp, collision);
        }
        {
            auto collision = collision_detector->spaceLeft(entity_id, Axis::Y, Direction::POSITIVE);
            if (collision.space_left <= 0 && collision.object_id && !collision.is_trigger)
                this->collide(damage_comp, collision);
        }
    }
}

void DamageSystem::collide(DamageComponent* damage_comp, CollisionReturnValues collision) {
    auto health_hit_entity = entityManager->getComponent<HealthComponent>(*collision.object_id);
    if (health_hit_entity) {
        health_hit_entity->health -= damage_comp->damage;
        // u ded m8
        if (health_hit_entity->health <= 0) {
            auto stats_hit_entity = entityManager->getComponent<StatsComponent>(*collision.object_id);
            // User's stats are updated with one dead added.
            stats_hit_entity->died+=1;

            health_hit_entity->on_death(*collision.object_id);

            if(damage_comp->damage_dealer_entity_id){
                auto points_gainer = entityManager->getComponent<StatsComponent>(*damage_comp->damage_dealer_entity_id);
                if(points_gainer && health_hit_entity->points_on_kill.has_value()){
                    // User's stats are updated with kill points.
                    points_gainer->kill_points += *health_hit_entity->points_on_kill;
                }else{
                    // User killed a critter and that stat is updated.
                    points_gainer->killed_critters += 1;
                }
            }
        }
    }
}