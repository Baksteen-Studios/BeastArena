#ifndef FILE_ENTITY_FACTORY_HPP
#define FILE_ENTITY_FACTORY_HPP

#include <vector>
#include <string>
#include <utility>

#include "menu/button.hpp"
#include "brickengine/entities/entity_manager.hpp"
#include "brickengine/rendering/renderables/texture.hpp"
#include "brickengine/rendering/renderable_factory.hpp"
#include "entities/layers.hpp"
#include "brickengine/rendering/renderables/data/color.hpp"

class EntityFactory {
public:
    EntityFactory(std::shared_ptr<EntityManager> em, RenderableFactory& rf);
    ~EntityFactory() = default;
    int createPanda(double x, double y, int playerId) const;
    int createGorilla(double x, double y, int playerId) const;
    int createWeapon(double xPos, double yPos) const;
    int createImage(std::string path, int x, int y, int xScale, int yScale, Layers layer, int alpha);
    int createPlatform(double x, double y, double xScale, double yScale, std::string path, int alpha);
    std::pair<int, int> createButton(const Button button, const double relative_modifier);
private:
    std::shared_ptr<EntityManager> entityManager;
    RenderableFactory& renderableFactory;
    inline static const std::string graphicsPath = "./assets/graphics/";
};

#endif // FILE_ENTITY_FACTORY_HPP
