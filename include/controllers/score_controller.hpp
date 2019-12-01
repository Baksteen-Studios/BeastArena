#ifndef FILE_SCORE_CONTROLLER_HPP
#define FILE_SCORE_CONTROLLER_HPP

#include "brickengine/entities/entity_manager.hpp"

class ScoreController {
public:
    ScoreController(EntityManager& entity_manager);
    void loadScores();
    void writeScores();
private:
    inline static const std::string HIGHSCORES_PATH = "assets/highscores.json";
    EntityManager& entity_manager;
};

#endif // FILE_SCORE_CONTROLLER_HPP