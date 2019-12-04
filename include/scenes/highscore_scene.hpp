#ifndef FILE_HIGHSCORE_SCENE_HPP
#define FILE_HIGHSCORE_SCENE_HPP

#include "scenes/beast_scene.hpp"

class HighscoreScene : public BeastScene<HighscoreScene> {
public:
    HighscoreScene(EntityFactory& entity_factory, BrickEngine& engine, ScoreController& score_controller);

    static std::string getTagStatic() {
        return "HighscoreScene";
    }
    GameState getSystemState() const {
        return GameState::Highscore;
    }
    static SceneLayer getLayerStatic() {
        return SceneLayer::Primary;
    }
    void performPrepare();
    void start();
    void leave();
private:
    ScoreController& score_controller;
    ScoreController::Scores scores;
};

#endif // FILE_HIGHSCORE_SCENE_HPP