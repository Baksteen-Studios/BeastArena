#include "entities/entity_factory.hpp"
#include "brickengine/rendering/renderables/renderable.hpp"

#include "brickengine/components/transform_component.hpp"
#include "brickengine/components/colliders/rectangle_collider_component.hpp"
#include "brickengine/components/physics_component.hpp"
#include "brickengine/components/player_component.hpp"
#include "brickengine/components/click_component.hpp"
#include "brickengine/components/renderables/texture_component.hpp"
#include <iostream>

EntityFactory::EntityFactory(std::shared_ptr<EntityManager> em, RenderableFactory& rf) : entityManager(em), renderableFactory(rf) {}

int EntityFactory::createGorilla(double xPos, double yPos, double xScale, double yScale, int playerId) const {
    auto dst = std::unique_ptr<Rect>(new Rect{ 0, 0, 0, 0 });
    auto r = renderableFactory.createImage(graphicsPath + "beasts/gorilla/idle-1.png", (int)Layers::Foreground, std::move(dst));
    auto comps = std::make_unique<std::vector<std::unique_ptr<Component>>>();

    comps->push_back(std::make_unique<TransformComponent>(xPos, yPos, xScale, yScale, Direction::POSITIVE, Direction::POSITIVE));
    comps->push_back(std::make_unique<RectangleColliderComponent>(1, 1, 1, false));
    comps->push_back(std::make_unique<PhysicsComponent>(105, 0, 0, 0, true, Kinematic::IS_NOT_KINEMATIC, true, false));
    comps->push_back(std::make_unique<TextureComponent>(std::move(r)));
    comps->push_back(std::make_unique<PlayerComponent>(playerId));

    return entityManager->createEntity(std::move(comps), std::nullopt);
}

int EntityFactory::createPanda(double xPos, double yPos, double xScale, double yScale, int playerId, std::pair<int, bool> parent) const {
    auto dst = std::unique_ptr<Rect>(new Rect{ 0, 0, 0, 0 });
    auto r = renderableFactory.createImage(graphicsPath + "beasts/panda/idle-1.png", (int)Layers::Foreground, std::move(dst));
    auto comps = std::make_unique<std::vector<std::unique_ptr<Component>>>();

    comps->push_back(std::make_unique<TransformComponent>(xPos, yPos, xScale, yScale, Direction::POSITIVE, Direction::POSITIVE));
    comps->push_back(std::make_unique<RectangleColliderComponent>(1, 1, 1, false));
    comps->push_back(std::make_unique<PhysicsComponent>(100, 0, 0, 0, true, Kinematic::IS_NOT_KINEMATIC, true, false));
    comps->push_back(std::make_unique<TextureComponent>(std::move(r)));
    comps->push_back(std::make_unique<PlayerComponent>(playerId));
    comps->push_back(std::make_unique<ClickComponent>([]() -> void {
        std::cout << "clicked" << std::endl;
    }, 1, 1));

    return entityManager->createEntity(std::move(comps), parent);
}

int EntityFactory::createWeapon(double xPos, double yPos, double xScale, double yScale) const{
    auto dst = std::unique_ptr<Rect>(new Rect{ 0, 0, 0, 0 });
    auto r = renderableFactory.createImage(graphicsPath + "weapons/pistol-1.png", (int)Layers::Foreground, std::move(dst));
    auto comps = std::make_unique<std::vector<std::unique_ptr<Component>>>();

    comps->push_back(std::make_unique<TransformComponent>(xPos, yPos, xScale, yScale, Direction::POSITIVE, Direction::POSITIVE));
    comps->push_back(std::make_unique<RectangleColliderComponent>(1, 1, 1, true));
    comps->push_back(std::make_unique<PhysicsComponent>(50, 0, 0, 0, true, Kinematic::IS_NOT_KINEMATIC, true, true));
    comps->push_back(std::make_unique<TextureComponent>(std::move(r)));
    //comps->push_back(std::make_unique<WeaponComponent>(1, 1, 10, 5));

    return entityManager->createEntity(std::move(comps), std::nullopt);
}

int EntityFactory::createImage(std::string path, int xPos, int yPos, int width, int height, Layers layer) {
    auto dst = std::unique_ptr<Rect>(new Rect{ 0, 0, 0, 0 });
    auto r = renderableFactory.createImage(graphicsPath + path, (int)layer, std::move(dst));

    auto comps = std::make_unique<std::vector<std::unique_ptr<Component>>>();
    comps->push_back(std::make_unique<TransformComponent>(xPos, yPos, width, height, Direction::POSITIVE, Direction::POSITIVE));
    comps->push_back(std::make_unique<TextureComponent>(std::move(r)));

    return entityManager->createEntity(std::move(comps), std::nullopt);
}

int EntityFactory::createPlatform(double xPos, double yPos, double xScale, double yScale) {
    auto dst = std::unique_ptr<Rect>(new Rect{ 0, 0, 0, 0 });
    auto r = renderableFactory.createImage(graphicsPath + "black.jpg", (int)Layers::Foreground, std::move(dst));

    auto comps = std::make_unique<std::vector<std::unique_ptr<Component>>>();
    comps->push_back(std::make_unique<TransformComponent>(xPos, yPos, xScale, yScale, Direction::POSITIVE, Direction::POSITIVE));
    comps->push_back(std::make_unique<RectangleColliderComponent>(1, 1, 1, false));
    comps->push_back(std::make_unique<TextureComponent>(std::move(r)));

    return entityManager->createEntity(std::move(comps), std::nullopt);
}
