#ifndef FILE_BILLBOARD_SPAWN_HPP
#define FILE_BILLBOARD_SPAWN_HPP

#include <string>

struct Billboard {
public:
    int x;
    int y;
    int alpha;

    int x_scale;
    int y_scale;

    std::string content_path;
};

#endif // FILE_BILLBOARD_SPAWN_HPP