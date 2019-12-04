#include "systems/highscore_system.hpp"
#include "brickengine/components/player_component.hpp"
#include "brickengine/input.hpp"
#include "player_input.hpp"

HighscoreSystem::HighscoreSystem(std::shared_ptr<EntityManager> entity_manager,std::shared_ptr<EntityFactory> entity_factory, ScoreController& score_controller)
    : BeastSystem(entity_factory, entity_manager) {
        for(auto& score : score_controller.readScores()) {
            scores.push_back(std::make_pair(score.first, score.second));
        }
};

void HighscoreSystem::update(double) {
    auto& input = BrickInput<PlayerInput>::getInstance();
    int x = input.checkInput(1, PlayerInput::X_AXIS);
    if(x > 0) {
        entityManager->removeEntitiesWithTag("highscore_player");
        auto score = scores.at(selector);
        createHighscores(score.first, score.second);
        selector++;
        if (selector > scores.size() - 1) {
            selector = 0;
        }
    } else if(x < 0) {
        entityManager->removeEntitiesWithTag("highscore_player");
        auto score = scores.at(selector);
        createHighscores(score.first, score.second);
        selector--;
        if (selector < 0) {
            selector = scores.size() - 1;
        }
    }
}

void HighscoreSystem::createHighscores(std::string name, Score score) {
    // Watch out! This method does not use the relative modifier as it is not possible to access this in a system
    // Because we do not have scaling, we won't bother with it for now but we have a Github Issue

    std::vector<EntityFactory::Components> entity_components;
    // Load the first object
    {
        auto comps = entity_factory->createText(name, { 255, 255, 255, 255}, 25, 100, 100, name.size() * 15, 25, 1);
        entity_components.push_back(std::move(comps));
    }
    {
        auto text = "Kills: " + std::to_string(score.kills);
        auto comps = entity_factory->createText(text, { 255, 255, 255, 255}, 25, 100, 150, name.size() * 15, 25, 1);
        entity_components.push_back(std::move(comps));
    }
    {
        auto text = "Deaths: " + std::to_string(score.deaths);
        auto comps = entity_factory->createText(text, { 255, 255, 255, 255}, 25, 100, 200, name.size() * 15, 25, 1);
        entity_components.push_back(std::move(comps));
    }
    {
        auto text = "Accidents: " + std::to_string(score.accidents);
        auto comps = entity_factory->createText(text, { 255, 255, 255, 255}, 25, 100, 250, name.size() * 15, 25, 1);
        entity_components.push_back(std::move(comps));
    }
    {
        auto text = "Levels won: " + std::to_string(score.levels_won);
        auto comps = entity_factory->createText(text, { 255, 255, 255, 255}, 25, 100, 300, name.size() * 15, 25, 1);
        entity_components.push_back(std::move(comps));
    }
    {
        auto text = "Critters killed: " + std::to_string(score.killed_critters);
        auto comps = entity_factory->createText(text, { 255, 255, 255, 255}, 25, 100, 350, name.size() * 15, 25, 1);
        entity_components.push_back(std::move(comps));
    }
    for (auto& comp : entity_components) {
        entity_factory->addToEntityManager(std::move(comp), std::nullopt, "highscore_player");
    }
}