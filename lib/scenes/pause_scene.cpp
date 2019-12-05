#include "scenes/pause_scene.hpp"

#include "entities/layers.hpp"
#include "brickengine/components/renderables/texture_component.hpp"

PauseScene::PauseScene(EntityFactory& factory, BrickEngine& engine) : Menu<PauseScene>(factory, engine, WIDTH, HEIGHT) {};

void PauseScene::performPrepare(){entity_components = std::make_unique<std::vector<std::unique_ptr<std::vector<std::unique_ptr<Component>>>>>();

    // Nothing has to changes here as no player or other entities have to be reset to another 'state' or position.
    auto dst = std::unique_ptr<Rect>(new Rect{ 0, 0, 0, 0 });
    auto r = factory.getRenderableFactory().createImage("./assets/graphics/colors/white.png", (int)Layers::UI, std::move(dst), 100);

    auto comps = std::make_unique<std::vector<std::unique_ptr<Component>>>();
    comps->push_back(std::make_unique<TransformComponent>(1600 / 2, 900 / 2, 1600, 900, Direction::POSITIVE, Direction::POSITIVE));
    comps->push_back(std::make_unique<TextureComponent>(std::move(r)));

    entity_components->push_back(std::move(comps));
}

void PauseScene::start() {
}

void PauseScene::leave() {

}