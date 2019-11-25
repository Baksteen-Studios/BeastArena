#include "scenes/intermission_scene.hpp"
#include "brickengine/components/renderables/texture_component.hpp"

IntermissionScene::IntermissionScene(int timer, EntityFactory& entity_factory)
    : entity_factory(entity_factory), timer(timer) { }

void IntermissionScene::performPrepare(){
    entity_components->push_back(std::move(entity_factory.createText(std::to_string(timer),  {0, 255, 0, 255}, 50, 800, 450, 300, 300, 1920/1600)));
}
void IntermissionScene::start() {
    // Nothing
}
void IntermissionScene::leave() {
    // Nothing
}
