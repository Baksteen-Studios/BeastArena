#include "scenes/level_scene.hpp"

#include "brickengine/json/json.hpp"
#include "brickengine/components/transform_component.hpp"
#include "brickengine/components/player_component.hpp"
#include "components/despawn_component.hpp"
#include "scenes/exceptions/not_enough_player_spawns_exception.hpp"

LevelScene::LevelScene(EntityFactory& factory, Json json)
    : json(json), BeastScene<LevelScene>(factory, json.getInt("width"), json.getInt("height")) {}

void LevelScene::performPrepare() {
    this->description = json.getString("description");
    this->version = json.getDouble("version");
    this->name = json.getString("name");
    this->bg_path = json.getString("bg_path");
    this->bg_music = json.getString("bg_music");

    // Create player spawns
    if(json.getVector("player_spawns").size() < 4) {
        throw NotEnoughPlayerSpawnsException();
    }

    for(Json player_spawn_json : json.getVector("player_spawns")) { 
        PlayerSpawn player_spawn = PlayerSpawn();

        player_spawn.x = player_spawn_json.getInt("x");
        player_spawn.y = player_spawn_json.getInt("y");

        this->player_spawns.push_back(player_spawn);
    }

    // Create weapon and item spawns
    for(Json gadget_spawn_json : json.getVector("gadget_spawns")) {
        GadgetSpawn gadget_spawn = GadgetSpawn();

        if(gadget_spawn_json.getString("type") == "weapon") {
            gadget_spawn.gadget_spawn_type = GadgetSpawnType::WEAPON;
        } else if(gadget_spawn_json.getString("type") == "item") {
            gadget_spawn.gadget_spawn_type = GadgetSpawnType::ITEM;
        }

        gadget_spawn.available_spawns = gadget_spawn_json.getStringVector("available_spawns");
        gadget_spawn.respawn_timer = gadget_spawn_json.getInt("respawn_timer");
        gadget_spawn.x = gadget_spawn_json.getInt("x");
        gadget_spawn.y = gadget_spawn_json.getInt("y");

        this->gadget_spawns.push_back(gadget_spawn);
    }

    // Create platforms
    for(Json solid_json : json.getVector("solids")) {
        Solid solid = Solid();

        if(solid_json.getString("shape") == "rectangle") {
            solid.shape = SolidShape::RECTANGLE;
        }

        if(solid_json.getString("effect") == "none") {
            solid.effect = SolidEffect::NONE;
        }

        solid.texture_path = solid_json.getString("texture_path");
        solid.alpha = solid_json.getInt("alpha");

        solid.x = solid_json.getInt("x");
        solid.y = solid_json.getInt("y");
        solid.xScale = solid_json.getInt("xScale");
        solid.yScale = solid_json.getInt("yScale");

        this->solids.push_back(solid);
    }
}
void LevelScene::start() {
    auto em = factory.getEntityManager();

    // Create the players
    factory.createWeapon(1000, 200, true);
    factory.createWeapon(1100, 200, false);
    factory.createWeapon(600, 200, true);
    factory.createWeapon(500, 200, false);

    // Create the background
    factory.createImage(this->bg_path, this->screen_width / 2, this->screen_height / 2, this->screen_width, this->screen_height, Layers::Background, 255);

    // Load the players on the spawn locations
    auto entities_with_player = em->getEntitiesByComponent<PlayerComponent>();

    int count = 0;
    for(auto& [entity_id, player]: entities_with_player) {
        player->disabled = false;
        auto transform_component = em->getComponent<TransformComponent>(entity_id);

        transform_component->x_pos = player_spawns[count].x / getRelativeModifier();
        transform_component->y_pos = player_spawns[count].y / getRelativeModifier();

        auto despawn_component = em->getComponent<DespawnComponent>(entity_id);
        despawn_component->despawn_on_out_of_screen = true;

        auto health_component = em->getComponent<HealthComponent>(entity_id);
        (*health_component->revive)(entity_id);

        ++count;
    }

    // Create the platforms
    for(Solid platform : solids) {
        if(platform.shape == SolidShape::RECTANGLE && platform.effect == SolidEffect::NONE) {
            int x = platform.x / getRelativeModifier();
            int y = platform.y / getRelativeModifier();
            int xScale = platform.xScale / getRelativeModifier();
            int yScale = platform.yScale / getRelativeModifier();
            factory.createPlatform(x, y, xScale, yScale, platform.texture_path, platform.alpha);
        }
    }

    //engine->toggleCursor(false);
}
void LevelScene::leave() {
    auto em = factory.getEntityManager();
    auto entities_with_player = em->getEntitiesByComponent<PlayerComponent>();
    for(auto& [entity_id, player]: entities_with_player) {
        auto transform_component = em->getComponent<TransformComponent>(entity_id);
        transform_component->x_pos = -2000;
        transform_component->y_pos = -2000;
        player->disabled = true;
        auto despawn_component = em->getComponent<DespawnComponent>(entity_id);
        despawn_component->despawn_on_out_of_screen = false;
    }
}