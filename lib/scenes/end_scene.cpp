#include "scenes/end_scene.hpp"

#include "brickengine/components/colliders/rectangle_collider_component.hpp"
#include "brickengine/components/renderables/texture_component.hpp"
#include "brickengine/components/player_component.hpp"

#include "components/despawn_component.hpp"
#include "components/health_component.hpp"
#include "components/stats_component.hpp"
#include "components/pickup_component.hpp"

EndScene::EndScene(EntityFactory& entity_factory, BrickEngine& engine, Json json) : 
    json(json), entity_factory(entity_factory), 
    BeastScene<EndScene>(entity_factory, engine, json.getInt("width"), json.getInt("height")) {};

void EndScene::performPrepare() {
    entity_components = std::make_unique<std::vector<std::unique_ptr<std::vector<std::unique_ptr<Component>>>>>();

    // Background
    auto comps = factory.createImage(json.getString("bg_path"), json.getInt("width") / 2, json.getInt("height") / 2, json.getInt("width"), json.getInt("height"), getRelativeModifier(), Layers::Background, 255);
    entity_components->push_back(std::move(comps));

    for (auto solid : json.getVector("solids")) {
        auto comps = factory.createPlatform(solid.getInt("x"), solid.getInt("y"), solid.getInt("xScale"), solid.getInt("yScale"), getRelativeModifier(), solid.getString("texture_path"), solid.getInt("alpha"));
        entity_components->push_back(std::move(comps));
    }

    auto& em = factory.getEntityManager();
    
    for (auto text : json.getVector("texts")){
        entity_components->push_back(std::move(entity_factory.createText(text.getString("text"), 
        {(unsigned short int)text.getInt("r"), (unsigned short int)text.getInt("g"), (unsigned short int)text.getInt("b"), (unsigned short int)text.getInt("alpha")}, 
        text.getInt("font_size"), text.getInt("x"), text.getInt("y"), text.getInt("x_scale"), text.getInt("y_scale"), getRelativeModifier())));
    }

    // Spawn the trophy
    {
        auto comps = factory.createImage("/items/trophy.png", this->screen_width / 1.8, 400, 50, 75, getRelativeModifier(), Layers::Middleground, 255);
        comps->push_back(std::make_unique<RectangleColliderComponent>(1, 1, 1, true));
        comps->push_back(std::make_unique<PhysicsComponent>(50, false, 0, 0, true, Kinematic::IS_NOT_KINEMATIC, true, true));
        comps->push_back(std::make_unique<PickupComponent>());
        comps->push_back(std::make_unique<DespawnComponent>(false, true));
        entity_components->push_back(std::move(comps));
    }
}

void EndScene::start() {
    auto& em = factory.getEntityManager();
    auto entities_with_player = em.getEntitiesByComponent<PlayerComponent>();

    // entity_id and points
    std::vector<std::pair<int, int>> results;
  
    for (auto& [ entity_id, player ] : entities_with_player) {
        auto stats = em.getComponent<StatsComponent>(entity_id);
        results.push_back(std::make_pair(entity_id, stats->levels_won));
    }

    std::sort(results.begin(), results.end(), [](auto lhs, auto rhs) {
        return lhs.second > rhs.second;
    });

    int player_count = 0;
    for(Json player_spawn_json : json.getVector("player_spawns")) {
        int entity_id = results[player_count].first;
        auto transformComponent = em.getComponent<TransformComponent>(entity_id);
        transformComponent->x_pos = player_spawn_json.getInt("x") / getRelativeModifier();
        transformComponent->y_pos = player_spawn_json.getInt("y") / getRelativeModifier();

        auto despawn_component = em.getComponent<DespawnComponent>(entity_id);
        despawn_component->despawn_on_out_of_screen = true;

        ++player_count;
    }

 
    // Revive the players
    for(auto& [entity_id, player]: entities_with_player) {
        auto health_component = em.getComponent<HealthComponent>(entity_id);
        (*health_component->revive)(entity_id);
        auto despawn_component = em.getComponent<DespawnComponent>(entity_id);
        despawn_component->despawn_on_out_of_screen = true;
    }

    int count = 1;  
    int y = 200;
    for(auto [entity_id, result] : results) {
        // Render the leaderboard
        auto stats_component = em.getComponent<StatsComponent>(entity_id);
        auto player_component = em.getComponent<PlayerComponent>(entity_id);
        // Check how many dots we have to insert
        auto text = std::to_string(count) + ": " + player_component->name;
        auto level_digits = 1;
        if(stats_component->levels_won > 0)
            level_digits = floor(log10(stats_component->levels_won));
        auto dots_to_add = 60 - level_digits - player_component->name.size() - text.size();
        while(text.size() < dots_to_add) {
            text = text + ".";
        }
        text = text + std::to_string(result);

        auto comps = factory.createText(text, { 255, 255, 255, 255 }, 50, this->screen_width / 2, y, 750, 50, 1);
        factory.addToEntityManager(std::move(comps));
        ++count;
        y += 50;
    }
}

void EndScene::leave() {}
