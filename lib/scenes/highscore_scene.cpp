#include "scenes/highscore_scene.hpp"
#include "brickengine/input.hpp"
#include "player_input.hpp"

HighscoreScene::HighscoreScene(EntityFactory& entity_factory, BrickEngine& engine, ScoreController& score_controller)
    : score_controller(score_controller), BeastScene<HighscoreScene>(entity_factory, engine, GameController::SCREEN_WIDTH, GameController::SCREEN_HEIGHT) {}

void HighscoreScene::performPrepare() {
    scores = score_controller.readScores();
}

void HighscoreScene::start() {
    std::vector<EntityFactory::Components> entity_components;
    // Load the first object
    auto score = scores.begin();
    auto name = score->first;
    auto highscore = score->second;
    {
        auto comps = factory.createText(score->first, { 255, 255, 255, 255}, 25, 100, 100, name.size() * 15, 25, getRelativeModifier());
        entity_components.push_back(std::move(comps));
    }
    {
        auto text = "Kills: " + std::to_string(highscore.kills);
        auto comps = factory.createText(text, { 255, 255, 255, 255}, 25, 100, 150, name.size() * 15, 25, getRelativeModifier());
        entity_components.push_back(std::move(comps));
    }
    {
        auto text = "Deaths: " + std::to_string(highscore.deaths);
        auto comps = factory.createText(text, { 255, 255, 255, 255}, 25, 100, 200, name.size() * 15, 25, getRelativeModifier());
        entity_components.push_back(std::move(comps));
    }
    {
        auto text = "Accidents: " + std::to_string(highscore.accidents);
        auto comps = factory.createText(text, { 255, 255, 255, 255}, 25, 100, 250, name.size() * 15, 25, getRelativeModifier());
        entity_components.push_back(std::move(comps));
    }
    {
        auto text = "Levels won: " + std::to_string(highscore.levels_won);
        auto comps = factory.createText(text, { 255, 255, 255, 255}, 25, 100, 300, name.size() * 15, 25, getRelativeModifier());
        entity_components.push_back(std::move(comps));
    }
    {
        auto text = "Critters killed: " + std::to_string(highscore.killed_critters);
        auto comps = factory.createText(text, { 255, 255, 255, 255}, 25, 100, 350, name.size() * 15, 25, getRelativeModifier());
        entity_components.push_back(std::move(comps));
    }
    for (auto& comp : entity_components) {
        factory.addToEntityManager(std::move(comp), std::nullopt, "highscore_player");
    }
}

void HighscoreScene::leave() {

}