#ifndef FILE_MENU_HPP
#define FILE_MENU_HPP

#include <string>
#include <vector>

#include "scenes/beast_scene.hpp"
#include "menu/button.hpp"
#include "menu/image.hpp"

template<typename MenuType>
class Menu : public BeastScene<MenuType> {
public:
    Menu(EntityFactory& factory, BrickEngine& engine, int width, int height) : BeastScene<MenuType>(factory, engine, width, height) {}

    // Buttons
    std::vector<Button> buttons;

    // Images
    std::vector<Image> images;
};

#endif // FILE_MENU_HPP
