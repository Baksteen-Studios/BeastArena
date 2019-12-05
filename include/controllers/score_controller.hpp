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
    // This is not the assets/highscores.json path since it is not really an asset.
    // The assets/highscores.json is just an example.
    inline static const std::string HIGHSCORES_PATH = "highscores.json";
};

#endif // FILE_SCORE_CONTROLLER_HPP