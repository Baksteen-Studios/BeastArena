#include "scenes/end_scene.hpp"

#include "brickengine/components/colliders/rectangle_collider_component.hpp"
#include "brickengine/components/renderables/texture_component.hpp"
#include "brickengine/components/player_component.hpp"

#include "components/despawn_component.hpp"
#include "components/health_component.hpp"
#include "components/stats_component.hpp"
#include "components/pickup_component.hpp"

EndScene::EndScene(EntityFactory& entity_factory, BrickEngine& engine) : 
    BeastScene<EndScene>(entity_factory, engine, WIDTH, HEIGHT) {};

void EndScene::performPrepare() {
    entity_components = std::make_unique<std::vector<std::unique_ptr<std::vector<std::unique_ptr<Component>>>>>();

    // Background
    entity_components->push_back(factory.createImage("backgrounds/arena.jpg", WIDTH / 2, HEIGHT / 2, WIDTH, HEIGHT, getRelativeModifier(), Layers::Background, 255));

    // Borders
    entity_components->push_back(factory.createPlatform(1159, 975, 200, 10, getRelativeModifier(), "colors/black.jpg", 255));
    entity_components->push_back(factory.createPlatform(855, 840, 10, 120, getRelativeModifier(), "colors/black.jpg", 255));
    entity_components->push_back(factory.createPlatform(955, 780, 209, 10, getRelativeModifier(), "colors/black.jpg", 255));
    entity_components->push_back(factory.createPlatform(760, 900, 200, 10, getRelativeModifier(), "colors/black.jpg", 255));
    entity_components->push_back(factory.createPlatform(660, 988, 10, 187, getRelativeModifier(), "colors/black.jpg", 255));
    entity_components->push_back(factory.createPlatform(1054, 880, 10, 200, getRelativeModifier(), "colors/black.jpg", 255));
    entity_components->push_back(factory.createPlatform(1254, 1030, 10, 101, getRelativeModifier(), "colors/black.jpg", 255));

    // Blue blocks
    entity_components->push_back(factory.createPlatform(960, 930, 200, 300, getRelativeModifier(), "colors/blue.jpg", 255));
    entity_components->push_back(factory.createPlatform(1159, 1020, 200, 100, getRelativeModifier(), "colors/blue.jpg", 255));
    entity_components->push_back(factory.createPlatform(760, 1000, 200, 200, getRelativeModifier(), "colors/blue.jpg", 255));
    entity_components->push_back(factory.createPlatform(1254, 1030, 10, 10, getRelativeModifier(), "colors/blue.jpg", 255));

    // Letters
    entity_components->push_back(factory.createText("1", {255, 255, 255, 255}, 25, 960, 960, 200, 400, getRelativeModifier()));
    entity_components->push_back(factory.createText("2", {255, 255, 255, 255}, 25, 770, 1025, 100, 200, getRelativeModifier()));
    entity_components->push_back(factory.createText("3", {255, 255, 255, 255}, 25, 1150, 1050, 50, 100, getRelativeModifier()));

    // Prepare playerspawns;
    player_spawns.push_back({ 950, 500});
    player_spawns.push_back({ 750, 500});
    player_spawns.push_back({ 1150, 500});

    // Spawn the trophy
    {
        auto comps = factory.createImage("/items/trophy.png", WIDTH / 1.8, 400, 50, 75, getRelativeModifier(), Layers::Middleground, 255);
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

        // Revive the player
        auto health_component = em.getComponent<HealthComponent>(entity_id);
        (*health_component->revive)(entity_id);
        auto despawn_component = em.getComponent<DespawnComponent>(entity_id);
        despawn_component->despawn_on_out_of_screen = true;
    }

    std::sort(results.begin(), results.end(), [](auto lhs, auto rhs) {
        return lhs.second > rhs.second;
    });


    int spawn = 0;
    int count = 1;  
    int y = 200;
    for(auto [entity_id, result] : results) {
        // Put player on the right position
        auto player_spawn = player_spawns.at(spawn);
        auto transformComponent = em.getComponent<TransformComponent>(entity_id);
        if(transformComponent) {
            transformComponent->x_pos = player_spawn.x / getRelativeModifier();
            transformComponent->y_pos = player_spawn.y / getRelativeModifier();
        }
        ++spawn;
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

        // Add the player's last position to his stats component.
        stats_component->last_game_result = count;

        ++count;
        y += 50;
    }
    engine.getSoundManager().playMusic("music/endgame.mp3");
}

void EndScene::leave() {
    engine.getSoundManager().stopMusic();
}
