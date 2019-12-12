#include "components/name_selection_component.hpp"

NameSelectionComponent::NameSelectionComponent(int player_id) : player_id(player_id), selected_name(""), is_picking(false), picked(false) {};

std::string NameSelectionComponent::NameSelectionComponent::getNameStatic() {
    return "NameSelectionComponent";
}