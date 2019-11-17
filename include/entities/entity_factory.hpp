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
    int createGorilla(double xPos, double yPos, int player_id) const;
    int createPanda1(double xPos, double yPos, int player_id) const;
    int createPanda2(double xPos, double yPos, int player_id) const;
    int createPanda3(double xPos, double yPos, int player_id) const;
    int createWeapon(double xPos, double yPos, bool ammo) const;
    int createImage(std::string path, int xPos, int yPos, int xScale, int yScale, Layers layer, int alpha);
    int createPlatform(double xPos, double yPos, double xScale, double yScale, std::string path, int alpha);
    std::pair<int, int> createButton(const Button button, const double relative_modifier);
private:
    std::shared_ptr<EntityManager> entityManager;
    RenderableFactory& renderableFactory;
    inline static const std::string graphicsPath = "./assets/graphics/";
};

#endif // FILE_ENTITY_FACTORY_HPP
