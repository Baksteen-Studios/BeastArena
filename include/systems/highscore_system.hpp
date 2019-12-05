#ifndef FILE_HIGHSCORE_SYSTEM_HPP
#define FILE_HIGHSCORE_SYSTEM_HPP

#include <map>
#include <array>

#include "controllers/score_controller.hpp"
#include "data/score.hpp"
#include "systems/beast_system.hpp"

class HighscoreSystem : public BeastSystem {
public:
    HighscoreSystem(std::shared_ptr<EntityManager> entity_manager, std::shared_ptr<EntityFactory> entity_factory, ScoreController& score_manager);
    void update(double deltatime);
    void createHighscores(std::string name, Score score);
    void reset();
private:
    ScoreController& score_controller;
    std::vector<std::pair<std::string, Score>> scores;
    int selector = 0;
    bool initialized = false;;
};

#endif // FILE_HIGHSCORE_SYSTEM_HPP