#include "scenes/score_scene.hpp"

#include "brickengine/components/renderables/texture_component.hpp"
#include "brickengine/components/player_component.hpp"

#include "components/despawn_component.hpp"
#include "components/health_component.hpp"
#include "components/stats_component.hpp"

ScoreScene::ScoreScene(EntityFactory& entity_factory, BrickEngine& engine, Json json) : 
    json(json), entity_factory(entity_factory), 
    BeastScene<ScoreScene>(entity_factory, engine, json.getInt("width"), json.getInt("height")) {};

void ScoreScene::performPrepare() {
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
}

void ScoreScene::start() {
    auto& em = factory.getEntityManager();
    auto entities_with_player = em.getEntitiesByComponent<PlayerComponent>();

    // entity_id and points
    std::vector<std::pair<int, int>> results;
    for (auto& [ entity_id, player ] : entities_with_player) {
        auto stats = em.getComponent<StatsComponent>(entity_id);
        results.push_back(std::make_pair(entity_id, stats->points));
    }

    std::sort(results.begin(), results.end(), [](auto lhs, auto rhs) {
        return lhs.second > rhs.second;
    });

    int count = 0;
    for(Json player_spawn_json : json.getVector("player_spawns")) {
        int entity_id = results[count].first;
        std::cout << entity_id << " is the entity id." << std::endl;
        auto transformComponent = em.getComponent<TransformComponent>(entity_id);
        transformComponent->x_pos = player_spawn_json.getInt("x") / getRelativeModifier();
        transformComponent->y_pos = player_spawn_json.getInt("y") / getRelativeModifier();

        auto despawn_component = em.getComponent<DespawnComponent>(entity_id);
        despawn_component->despawn_on_out_of_screen = true;

        ++count;
    }


    // Revive the players
    for(auto& [entity_id, player]: entities_with_player) {
        auto health_component = em.getComponent<HealthComponent>(entity_id);
        (*health_component->revive)(entity_id);
        auto despawn_component = em.getComponent<DespawnComponent>(entity_id);
        despawn_component->despawn_on_out_of_screen = true;
    }


}

void ScoreScene::leave() {}
