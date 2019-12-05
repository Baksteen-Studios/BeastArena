#include "systems/highscore_system.hpp"
#include "brickengine/components/player_component.hpp"
#include "brickengine/input.hpp"
#include "player_input.hpp"

HighscoreSystem::HighscoreSystem(std::shared_ptr<EntityManager> entity_manager,std::shared_ptr<EntityFactory> entity_factory, ScoreController& score_controller)
    : score_controller(score_controller), BeastSystem(entity_factory, entity_manager) {};

void HighscoreSystem::update(double) {
    if(!initialized) {
        for(auto& score : score_controller.readScores()) {
            scores.push_back(std::make_pair(score.first, score.second));
        }
        if(scores.size() == 0) {
            if(!empty) {
                auto comps = entity_factory->createText("No highscores recorded yet!", { 255, 255, 255, 255}, 100, 800, 250, 27 * 30, 100, 1);
                entity_factory->addToEntityManager(std::move(comps));
                empty = true;
            }
        }
        else {
            auto score = scores.at(selector);
            createHighscores(score.first, score.second);
            selector++;
            initialized = true;
        }
    }
    auto& input = BrickInput<PlayerInput>::getInstance();
    int x = input.checkInput(1, PlayerInput::X_AXIS);
    if(x > 0) {
        entityManager->removeEntitiesWithTag("HighscoreScene_player");
        auto score = scores.at(selector);
        createHighscores(score.first, score.second);
        selector++;
        if (selector > scores.size() - 1) {
            selector = 0;
        }
    } else if(x < 0) {
        entityManager->removeEntitiesWithTag("HighscoreScene_player");
        auto score = scores.at(selector);
        createHighscores(score.first, score.second);
        selector--;
        if (selector < 0) {
            selector = scores.size() - 1;
        }
    }
}

void HighscoreSystem::reset() {
    initialized = false;
    empty = false;
    selector = 0;
    scores.clear();
}

void HighscoreSystem::createHighscores(std::string name, Score score) {
    // Watch out! This method does not use the relative modifier as it is not possible to access this in a system
    // Because we do not have scaling, we won't bother with it for now but we have a Github Issue

    std::vector<EntityFactory::Components> entity_components;
    // Load the first object
    {
        auto comps = entity_factory->createText(name, { 255, 255, 255, 255}, 100, 800, 250, name.size() * 30, 100, 1);
        entity_components.push_back(std::move(comps));
    }
    {
        std::string text = "Kills: " + std::to_string(score.kills);
        auto comps = entity_factory->createText(text, { 255, 255, 255, 255}, 50, 800, 350, text.size() * 30, 50, 1);
        entity_components.push_back(std::move(comps));
    }
    {
        std::string text = "Deaths: " + std::to_string(score.deaths);
        auto comps = entity_factory->createText(text, { 255, 255, 255, 255}, 50, 800, 400, text.size() * 30, 50, 1);
        entity_components.push_back(std::move(comps));
    }
    {
        std::string text = "Accidents: " + std::to_string(score.accidents);
        auto comps = entity_factory->createText(text, { 255, 255, 255, 255}, 50, 800, 450, text.size() * 30, 50, 1);
        entity_components.push_back(std::move(comps));
    }
    {
        std::string text = "Levels won: " + std::to_string(score.levels_won);
        auto comps = entity_factory->createText(text, { 255, 255, 255, 255}, 50, 800, 500, text.size() * 30, 50, 1);
        entity_components.push_back(std::move(comps));
    }
    {
        std::string text = "Critters killed: " + std::to_string(score.killed_critters);
        auto comps = entity_factory->createText(text, { 255, 255, 255, 255}, 25, 800, 550, text.size() * 30, 50, 1);
        entity_components.push_back(std::move(comps));
    }
    for (auto& comp : entity_components) {
        entity_factory->addToEntityManager(std::move(comp), std::nullopt, "HighscoreScene_player");
    }
}