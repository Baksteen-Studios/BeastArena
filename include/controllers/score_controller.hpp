#ifndef FILE_SCORE_CONTROLLER_HPP
#define FILE_SCORE_CONTROLLER_HPP

#include "brickengine/entities/entity_manager.hpp"
#include "data/score.hpp"

class ScoreController {
public:
    using Scores = std::unordered_map<std::string, Score>;
    ScoreController() = default;
    Scores readScores();
    void writeScores(std::unordered_map<std::string, Score> scores);
private:
    inline static const std::string HIGHSCORES_PATH = "assets/highscores.json";
};

#endif // FILE_SCORE_CONTROLLER_HPP