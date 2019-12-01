#ifndef FILE_READY_UP_SYSTEM_HPP
#define FILE_READY_UP_SYSTEM_HPP

#include <functional>

#include "systems/beast_system.hpp"
#include "brickengine/collision_detector.hpp"

class ReadyUpSystem : public BeastSystem {
public:
    ReadyUpSystem(std::shared_ptr<EntityManager> em, std::shared_ptr<EntityFactory> ef, std::function<void()> ready_function);
    void update(double delta_time);
private:
    std::function<void()> ready_function; 
};

#endif // FILE_READY_UP_SYSTEM_HPP