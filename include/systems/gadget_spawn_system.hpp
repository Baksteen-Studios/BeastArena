#ifndef FILE_GADGET_RESPAWN_SYSTEM_HPP
#define FILE_GADGET_RESPAWN_SYSTEM_HPP

#include "systems/beast_system.hpp"
#include "brickengine/collision_detector.hpp"

class GadgetSpawnSystem : public BeastSystem {
public:
    GadgetSpawnSystem(std::shared_ptr<CollisionDetector> cd, std::shared_ptr<EntityManager> em, std::shared_ptr<EntityFactory> ef);
    void update(double delta_time);
private:
    std::shared_ptr<CollisionDetector> collision_detector;
};

#endif // FILE_GADGET_RESPAWN_SYSTEM_HPP 