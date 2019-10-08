#ifndef FILE_ENTITY_FACTORY_HPP
#define FILE_ENTITY_FACTORY_HPP

#include <vector>

#include "brickengine/entities/entity_manager.hpp"
#include "brickengine/rendering/renderables/texture.hpp"

class EntityFactory {
public:
    EntityFactory(std::shared_ptr<EntityManager> entityManager);
    ~EntityFactory() = default;
    int createBeast(double x, double y, std::unique_ptr<Texture> r) const;
private:
    std::shared_ptr<EntityManager> entityManager;
};

#endif /* FILE_ENTITY_FACTORY_HPP */ 