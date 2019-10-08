#include "entities/entity_factory.hpp"
#include "brickengine/rendering/renderables/renderable.hpp"

#include "brickengine/components/transform_component.hpp"
#include "brickengine/components/physics_component.hpp"
#include "brickengine/components/renderables/texture_component.hpp"
#include "components/velocity_component.hpp"

EntityFactory::EntityFactory(std::shared_ptr<EntityManager> entityManager) : entityManager(entityManager) {}

int EntityFactory::createBeast(double x, double y, std::unique_ptr<Texture> r) const {

    auto comps = std::unique_ptr<std::vector<std::unique_ptr<Component>>>(new std::vector<std::unique_ptr<Component>>());

    comps.get()->push_back(std::unique_ptr<Component>(new TransformComponent(x, y, 500, 500)));
    comps.get()->push_back(std::unique_ptr<Component>(new PhysicsComponent(1, 0, 1, 1, false, false)));
    comps.get()->push_back(std::unique_ptr<Component>(new TextureComponent(std::move(r))));

    return entityManager->createEntity(std::move(comps));
}