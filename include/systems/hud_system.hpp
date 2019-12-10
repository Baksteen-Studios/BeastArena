#ifndef FILE_HUD_SYSTEM_HPP
#define FILE_HUD_SYSTEM_HPP

#include "systems/beast_system.hpp"

class HUDSystem : public BeastSystem {
public:
    HUDSystem(std::shared_ptr<EntityManager> entity_manager, std::shared_ptr<EntityFactory> entity_factory, int screen_width, int screen_height);
    void update(double deltatime);
private:
    int screen_width;
    int screen_height;
};

#endif // FILE_HUD_SYSTEM_HPP