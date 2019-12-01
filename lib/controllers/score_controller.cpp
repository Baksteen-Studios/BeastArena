#include "controllers/score_controller.hpp"

#include "brickengine/json/json.hpp"

#include "brickengine/components/player_component.hpp"
#include "components/stats_component.hpp"

ScoreController::ScoreController(EntityManager& em) : entity_manager(em) {}

void ScoreController::writeScores() {
    auto json = Json(HIGHSCORES_PATH, true);
    auto players = entity_manager.getEntitiesByComponent<PlayerComponent>();
    for (auto& [entity_id, player] : players) {
        auto stats_component = entity_manager.getComponent<StatsComponent>(entity_id);

        // Retrieve statistics from the previous game.
        auto kills = stats_component->kills;
        auto deaths = stats_component->deaths;
        auto killed_critters = stats_component->killed_critters; 
        auto levels_won = stats_component->levels_won;

        // if the player already exists in the json we need to add the values
        try {
            kills += json.getIntFromObject(player->name, "kills");
            deaths += json.getIntFromObject(player->name, "deaths");
            killed_critters += json.getIntFromObject(player->name, "killed_critters");
            levels_won += json.getIntFromObject(player->name, "levels_won");
        }
        catch(...) {
            // No need to do anything, continue normally.
        }

        // Add the statistics to the json.
        json.addIntToObject(player->name, "kills", kills);
        json.addIntToObject(player->name, "deaths", deaths);
        json.addIntToObject(player->name, "killed_critters", killed_critters);
        json.addIntToObject(player->name, "levels_won", levels_won);
    }

    // Write contents to file
    std::ofstream file;
    file.open(HIGHSCORES_PATH);
    file << json;
    file.close();
}