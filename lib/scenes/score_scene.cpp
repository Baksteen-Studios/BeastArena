#include "scenes/score_scene.hpp"
#include "brickengine/components/renderables/texture_component.hpp"

ScoreScene::ScoreScene(EntityFactory& entity_factory, BrickEngine& engine, Json json) : 
    json(json), entity_factory(entity_factory), 
    BeastScene<ScoreScene>(entity_factory, engine, json.getInt("width"), json.getInt("height")) {};

void ScoreScene::performPrepare() {
    entity_components = std::make_unique<std::vector<std::unique_ptr<std::vector<std::unique_ptr<Component>>>>>();

    // Background
    auto comps = factory.createImage(json.getString("bg_path"), json.getInt("width") / 2, json.getInt("height") / 2, json.getInt("width"), json.getInt("height"), getRelativeModifier(), Layers::Background, 255);
    entity_components->push_back(std::move(comps));

    for (auto solid : json.getVector("solids")) {
        auto comps = factory.createPlatform(solid.getInt("x"), solid.getInt("y"), solid.getInt("xScale"), solid.getInt("yScale"), getRelativeModifier(), solid.getString("texture_path"), solid.getInt("alpha"));
        entity_components->push_back(std::move(comps));
    }

    for (auto text : json.getVector("texts")){
        entity_components->push_back(std::move(entity_factory.createText(text.getString("text"), 
        {(unsigned short int)text.getInt("r"), (unsigned short int)text.getInt("g"), (unsigned short int)text.getInt("b"), (unsigned short int)text.getInt("alpha")}, 
        text.getInt("font_size"), text.getInt("x"), text.getInt("y"), text.getInt("x_scale"), text.getInt("y_scale"), getRelativeModifier())));
    }
}

void ScoreScene::start() {}

void ScoreScene::leave() {}