#ifndef FILE_LOBBY_SYSTEM_HPP
#define FILE_LOBBY_SYSTEM_HPP

#include <vector>
#include <utility>

#include "systems/beast_system.hpp"
#include "entities/entity_factory.hpp"
#include "enums/character.hpp"

class LobbySystem : public BeastSystem {
public:
    LobbySystem(std::shared_ptr<EntityFactory> ef, std::shared_ptr<EntityManager> em);
    void update(double deltatime);
private:
    std::shared_ptr<EntityFactory> ef;
    std::shared_ptr<EntityManager> em;

    std::vector<std::pair<Character, bool>> picked_characters = {
        std::pair<Character, bool>(Character::GORILLA, false),
        std::pair<Character, bool>(Character::PANDA, false),
        std::pair<Character, bool>(Character::CHEETAH, false),
        std::pair<Character, bool>(Character::ELEPHANT, false),
        std::pair<Character, bool>(Character::RANDOM, false)
    };
};

#endif // FILE_LOBBY_SYSTEM_HPP