#include "systems/hud_system.hpp"

#include "brickengine/components/player_component.hpp"
#include "components/hud_component.hpp"
#include "entities/layers.hpp"

HUDSystem::HUDSystem(std::shared_ptr<EntityManager> em, std::shared_ptr<EntityFactory> ef, int screen_width, int screen_height)
    : BeastSystem(ef, em), screen_width(screen_width), screen_height(screen_height) {}

void HUDSystem::update(double) {
    // auto player_entities = entityManager->getEntitiesByComponent<PlayerComponent>();

    // int spacing = screen_width / (player_entities.size() + 1);

    // int count = 0;
    // for (auto& [entity_id, player] : player_entities) {
    //     auto hud_component = entityManager->getComponent<HUDComponent>(entity_id);

    //     int x_pos = spacing + (count * spacing);
    //     int y_pos = screen_height * 0.2;
        
    //     // This is infinite loopy
    //     entity_factory->addToEntityManager(entity_factory->createImage(hud_component->texture, x_pos, y_pos, hud_component->x_scale, hud_component->y_scale, 1, Layers::UI, 255));

    //     ++count;
    // }
}