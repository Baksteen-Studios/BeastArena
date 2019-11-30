#ifndef FILE_STATS_COMPONENT_HPP
#define FILE_STATS_COMPONENT_HPP

#include "brickengine/components/component_impl.hpp"

class StatsComponent : public ComponentImpl<StatsComponent> {
public:
    static std::string getNameStatic();
    // Data
    int levels_won = 0;
    int kill_points = 0;
    int killed_critters = 0;
    int died = 0;
};

#endif // FILE_STATS_COMPONENT_HPP