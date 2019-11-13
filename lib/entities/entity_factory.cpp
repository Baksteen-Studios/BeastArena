#include "entities/entity_factory.hpp"

#include <string>

#include "brickengine/components/transform_component.hpp"
#include "brickengine/components/colliders/rectangle_collider_component.hpp"
#include "brickengine/components/physics_component.hpp"
#include "brickengine/components/player_component.hpp"
#include "brickengine/components/click_component.hpp"
#include "brickengine/components/renderables/texture_component.hpp"
#include "brickengine/rendering/renderables/data/color.hpp"
#include "brickengine/rendering/renderables/renderable.hpp"
#include <iostream>

EntityFactory::EntityFactory(std::shared_ptr<EntityManager> em, RenderableFactory& rf) : entityManager(em), renderableFactory(rf) {}

int EntityFactory::createPanda(double x, double y, int playerId) const {
    auto dst = std::unique_ptr<Rect>(new Rect{ 0, 0, 0, 0 });
    auto r = renderableFactory.createImage(graphicsPath + "beasts/panda/idle-1.png", (int)Layers::Foreground, std::move(dst), 255);
    auto comps = std::make_unique<std::vector<std::unique_ptr<Component>>>();

    comps->push_back(std::make_unique<TransformComponent>(x, y, 63, 100));
    comps->push_back(std::make_unique<RectangleColliderComponent>(1, 1, 1));
    comps->push_back(std::make_unique<PhysicsComponent>(100, 0, 0, 0, true, false));
    comps->push_back(std::make_unique<TextureComponent>(std::move(r)));
    comps->push_back(std::make_unique<PlayerComponent>(playerId));
    comps->push_back(std::make_unique<ClickComponent>([]() -> void {
        std::cout << "clicked" << std::endl;
    }, 1, 1));

    return entityManager->createEntity(std::move(comps));
}

int EntityFactory::createGorilla(double x, double y, int playerId) const {
    auto dst = std::unique_ptr<Rect>(new Rect{ 0, 0, 0, 0 });
    auto r = renderableFactory.createImage(graphicsPath + "beasts/gorilla/idle-1.png", (int)Layers::Foreground, std::move(dst), 255);
    auto comps = std::make_unique<std::vector<std::unique_ptr<Component>>>();

    comps->push_back(std::make_unique<TransformComponent>(x, y, 50, 100));
    comps->push_back(std::make_unique<RectangleColliderComponent>(1, 1, 1));
    comps->push_back(std::make_unique<PhysicsComponent>(105, 0, 0, 0, true, false));
    comps->push_back(std::make_unique<TextureComponent>(std::move(r)));
    comps->push_back(std::make_unique<PlayerComponent>(playerId));

    return entityManager->createEntity(std::move(comps));
}

int EntityFactory::createImage(std::string path, int x, int y, int xScale, int yScale, Layers layer, int alpha) {
    auto dst = std::unique_ptr<Rect>(new Rect{ 0, 0, 0, 0 });
    auto r = renderableFactory.createImage(graphicsPath + path, (int)layer, std::move(dst), alpha);

    auto comps = std::make_unique<std::vector<std::unique_ptr<Component>>>();
    comps->push_back(std::make_unique<TransformComponent>(x, y, xScale, yScale));
    comps->push_back(std::make_unique<TextureComponent>(std::move(r)));

    return entityManager->createEntity(std::move(comps));
}


int EntityFactory::createPlatform(double x, double y, double xScale, double yScale, std::string path, int alpha) {
    auto dst = std::unique_ptr<Rect>(new Rect{ 0, 0, 0, 0 });
    auto r = renderableFactory.createImage(graphicsPath + path, (int)Layers::Foreground, std::move(dst), alpha);
 
    auto comps = std::make_unique<std::vector<std::unique_ptr<Component>>>();
    comps->push_back(std::make_unique<TransformComponent>(x, y, xScale, yScale));
    comps->push_back(std::make_unique<RectangleColliderComponent>(1, 1, 1));
    comps->push_back(std::make_unique<PhysicsComponent>(130, 0, 0, 0, false, true));
    comps->push_back(std::make_unique<TextureComponent>(std::move(r)));

    return entityManager->createEntity(std::move(comps));
}

int EntityFactory::createButton(const Button button, const double relative_modifier) {
    // Make background
    auto dst = std::unique_ptr<Rect>(new Rect{ 0, 0 , 0, 0});
    auto r = renderableFactory.createImage(graphicsPath + button.texture_path, (int)Layers::Middleground, std::move(dst), button.alpha);

    auto comps = std::make_unique<std::vector<std::unique_ptr<Component>>>();
    comps->push_back(std::make_unique<TransformComponent>(button.x / relative_modifier, button.y / relative_modifier, button.xScale / relative_modifier, button.yScale / relative_modifier));
    comps->push_back(std::make_unique<TextureComponent>(std::move(r)));
    // comps->push_back(std::make_unique<ClickComponent>(button.on_click, 1, 1));
    comps->push_back(std::make_unique<ClickComponent>(button.on_click, 1, 1));

    int id = entityManager->createEntity(std::move(comps));

    auto dstText = std::unique_ptr<Rect>(new Rect{ 0, 0 , 0, 0});
    auto rText = renderableFactory.createText(button.text.text, button.text.font_size, button.text.color, (int)Layers::Foreground, std::move(dstText));

    auto compsText = std::make_unique<std::vector<std::unique_ptr<Component>>>();
    int x = button.text.x / relative_modifier;
    int y = button.text.y / relative_modifier;
    int xScale = (button.text.xScale / relative_modifier) / 1.5;
    int yScale = (button.text.yScale / relative_modifier) / 1.5;
    compsText->push_back(std::make_unique<TransformComponent>(x, y, xScale, yScale));
    compsText->push_back(std::make_unique<TextureComponent>(std::move(rText)));

    entityManager->createEntity(std::move(compsText));

    return id;
}