#ifndef FILE_ENTITY_FACTORY_HPP
#define FILE_ENTITY_FACTORY_HPP

#include <vector>
#include <string>
#include <utility>
#include <optional>

#include "brickengine/entities/entity_manager.hpp"
#include "brickengine/rendering/renderables/texture.hpp"
#include "brickengine/rendering/renderable_factory.hpp"
#include "brickengine/rendering/renderables/data/color.hpp"
#include "entities/layers.hpp"
#include "components/health_component.hpp"
#include "scenes/data/menu/button.hpp"

class EntityFactory {
public:
    using Components = std::unique_ptr<std::vector<std::unique_ptr<Component>>>;
    EntityFactory(std::shared_ptr<EntityManager> em, RenderableFactory& rf);
    ~EntityFactory() = default;

    EntityManager& getEntityManager() {
        return *entityManager;
    }
    RenderableFactory& getRenderableFactory() {
        return renderableFactory;
    };

    int createGorilla(int player_id) const;
    int createPanda(int player_id) const;
    int createCheetah(int player_id) const;
    int createElephant(int player_id) const;
    int createPistol(double x_pos, double y_pos, bool ammo) const;
    int createRifle(double x_pos, double y_pos, bool ammo) const;
    int createSniper(double x_pos, double y_pos, bool ammo) const;
    int createCritter(double x_pos, double y_pos) const;
    Components createImage(std::string path, int x_pos, int y_pos, int x_scale, int y_scale, double relative_modifier, Layers layer, int alpha);
    Components createPlatform(double x_pos, double y_pos, double x_scale, double y_scale, double relative_modifier, std::string path, int alpha);
    std::pair<int, int> createButton(const Button button, const double relative_modifier);
    Components createText(std::string text, Color color, int font_size, int x, int y, int x_scale, int y_scale, double relative_modifier);

    int addToEntityManager(std::unique_ptr<std::vector<std::unique_ptr<Component>>> component_list, 
                    std::optional<std::pair<int,bool>> parent_opt = std::nullopt, std::optional<std::string> scene_tag = std::nullopt);

private:
    std::shared_ptr<EntityManager> entityManager;
    RenderableFactory& renderableFactory;

    inline static const std::string GRAPHICS_PATH = "./assets/graphics/";
    inline static const int POINTS_ON_KILL_PLAYER = 10;
    HealthComponent::EntityFunction player_on_death;
    HealthComponent::EntityFunction player_revive;
};

#endif // FILE_ENTITY_FACTORY_HPP
