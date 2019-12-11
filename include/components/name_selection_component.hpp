#ifndef FILE_NAME_SELECTION_COMPONENT_HPP
#define FILE_NAME_SELECTION_COMPONENT_HPP

#include <string>

#include "brickengine/components/component_impl.hpp"

class NameSelectionComponent : public ComponentImpl<NameSelectionComponent> {
public:
    NameSelectionComponent(int player_id);
    static std::string getNameStatic();
    
    // Data
    int player_id;
    
    std::string selected_name;
    bool is_picking;
    bool picked;
    int text_entity_id;
};

#endif // FILE_NAME_SELECTION_COMPONENT_HPP