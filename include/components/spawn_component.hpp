#ifndef FILE_SPAWN_COMPONENT_HPP
#define FILE_SPAWN_COMPONENT_HPP

#include "brickengine/components/component_impl.hpp"
#include <functional>
#include <memory>
#include <vector>
#include "brickengine/enum/direction.hpp"
#include "enums/gadget_type.hpp"

class SpawnComponent : public ComponentImpl<SpawnComponent> {
public:
    using CreateGadgetCompsFn = std::function<std::unique_ptr<std::vector<std::unique_ptr<Component>>> ()>;

    SpawnComponent(int respawn_timer, std::vector<CreateGadgetCompsFn> gadget_fns);
    static std::string getNameStatic();

    // Data
    std::vector<CreateGadgetCompsFn> gadget_fns;
    double respawn_timer;
    double time_elapsed;
};

#endif // FILE_SPAWN_COMPONENT_HPP