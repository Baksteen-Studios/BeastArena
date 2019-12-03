#include "systems/lobby_system.hpp"

#include <algorithm>

#include "brickengine/input.hpp"
#include "player_input.hpp"
#include "components/character_selection_component.hpp"
#include "brickengine/components/renderables/texture_component.hpp"
#include "brickengine/components/transform_component.hpp"
#include "brickengine/components/player_component.hpp"

LobbySystem::LobbySystem(std::shared_ptr<EntityFactory> ef, std::shared_ptr<EntityManager> em) : BeastSystem(ef, em), ef(ef), em(em) {};

void LobbySystem::update(double) {
    auto& input = BrickInput<PlayerInput>::getInstance();

    auto selector_entities = em->getEntitiesByComponent<CharacterSelectionComponent>();

    for(auto& [entity_id, selector] : selector_entities) {
        if(input.checkInput(selector->player_id, PlayerInput::GRAB)) {
            if(!selector->joined) {
                // Player wants to play
                selector->joined = true;
                for(auto& character : picked_characters) {
                    // Select the first available character
                    if(!character.second) {
                        selector->selected_character = character.first;
                        ef->changeCharacterSelectorTexture(entity_id, character.first, true);
                        break;
                    }
                }
            } else {
                // Player selected character
                if(!selector->picked) {
                    // Check if character is available
                    bool available = true;
                    for(auto character : picked_characters) {
                        if(character.first == selector->selected_character && character.second) {
                            available = false;
                            break;
                        }
                    }

                    if(available) {
                        if(selector->selected_character != Character::RANDOM) {
                            for(auto& character : picked_characters) {
                                if(character.first == selector->selected_character) {
                                    character.second = true;
                                    break;
                                }
                            }

                            // Update the other selector so they do not show the character chosen by this player
                            auto selector_entities_2 = em->getEntitiesByComponent<CharacterSelectionComponent>();
                            for(auto& [entity_id_2, selector_2] : selector_entities_2) {
                                if(entity_id != entity_id_2 && selector_2->joined && !selector_2->picked && selector_2->selected_character == selector->selected_character) {
                                    // This is not the current selector, the player has joined but not picked a character and the character currently displayed is just picked by the other player
                                    for(auto& character : picked_characters) {
                                    // Select the first available character
                                    if(!character.second) {
                                        selector_2->selected_character = character.first;
                                        ef->changeCharacterSelectorTexture(entity_id_2, character.first, true);
                                        break;
                                    }
                                }
                                }
                            }
                        }
                        selector->picked = true;

                        auto transform_component = em->getComponent<TransformComponent>(entity_id);
                        selector->player_entity_id = ef->createPlayer(selector->player_id, selector->selected_character, transform_component->x_pos, transform_component->y_pos);

                        ef->createImage("menu/check.png", transform_component->x_pos, transform_component->y_pos, 84, 66, Layers::Foreground, 255);

                        input.removeTimeToWait(selector->player_id, PlayerInput::X_AXIS);
                    }
                } else {
                    // The player already picked a character
                    continue;
                }
            }
        }

        bool changed = false;
        int change_amount = 1; // This int is needed when looking for the first available character
        if(selector->joined && !selector->picked) {
            auto x_movement = input.checkInput(selector->player_id, PlayerInput::X_AXIS);
            if (x_movement < 0) {
                // Left
                for(auto character : picked_characters) {
                    if(character.first == selector->selected_character) {
                        while(!changed) {
                            auto iterator = std::find(picked_characters.begin(), picked_characters.end(), character);
                            int index = std::distance(picked_characters.begin(), iterator);

                            int new_index = 0;
                            if(index - change_amount < 0) {
                                new_index = picked_characters.size() - ((index - change_amount) * -1);
                            } else {
                                new_index = index - change_amount;
                            }

                            if(picked_characters.at(new_index).second) {
                                // This character is already picked
                                change_amount++;
                            } else {
                                selector->selected_character = picked_characters.at(new_index).first;
                                changed = true;
                            }
                        }
                        break;
                    }
                }
            } else if (x_movement > 0) {
                // Right
                for(auto character : picked_characters) {
                    if(character.first == selector->selected_character) {
                        while(!changed) {
                            auto iterator = std::find(picked_characters.begin(), picked_characters.end(), character);
                            int index = std::distance(picked_characters.begin(), iterator);

                            int new_index = 0;
                            if(index + change_amount > picked_characters.size() - 1) {
                                new_index = 0 + ((index + change_amount) - picked_characters.size());
                            } else {
                                new_index = index + change_amount;
                            }

                            if(picked_characters.at(new_index).second) {
                                // This character is already picked
                                change_amount++;
                            } else {
                                selector->selected_character = picked_characters.at(new_index).first;
                                changed = true;
                            }
                        }
                        break;
                    }
                }
            }
        }

        if(changed) {
            entity_factory->changeCharacterSelectorTexture(entity_id, selector->selected_character, false);
        }
    }
}