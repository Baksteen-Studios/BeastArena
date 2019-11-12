#ifndef FILE_IMAGE_HPP
#define FILE_IMAGE_HPP

#include <string>

struct Image {
public:
    std::string texture_path;
    int alpha;

    int x; // Before relative modifier
    int y; // Before relative modifier
    int xScale; // Before relative modifier
    int yScale; // Before relative modifier
};

#endif // FILE_IMAGE_HPP