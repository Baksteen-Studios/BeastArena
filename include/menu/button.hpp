#ifndef FILE_BUTTON_HPP
#define FILE_BUTTON_HPP

#include <string>
#include <functional>

#include "menu/text.hpp"

struct Button {
public:
    std::string texture_path;
    int alpha;

    int x; // Before relative modifier
    int y; // Before relative modifier
    int xScale; // Before relative modifier
    int yScale; // Before relative modifier

    Text text;

    std::function<void ()> on_click;
};

#endif // FILE_BUTTON_HPP