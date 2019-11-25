#include "components/character_selection_component.hpp"

CharacterSelectionComponent::CharacterSelectionComponent(int player_id) : player_id(player_id), joined(false), selected_character(Character::GORILLA), picked(false) {};

std::string CharacterSelectionComponent::CharacterSelectionComponent::getNameStatic() {
    return "CharacterSelectionComponent";
}