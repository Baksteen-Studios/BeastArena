#ifndef FILE_WANDERING_SYSTEM_HPP
#define FILE_WANDERING_SYSTEM_HPP

#include <memory>
#include "brickengine/collision_detector.hpp"
#include "systems/beast_system.hpp"

class WanderingSystem : public BeastSystem {
public:
    WanderingSystem(std::shared_ptr<CollisionDetector> cd, std::shared_ptr<EntityManager> em, std::shared_ptr<EntityFactory> ef);
    void update(double deltatime);
private:
    std::shared_ptr<CollisionDetector> collision_detector;
};

#endif // FILE_WANDERING_SYSTEM_HPP