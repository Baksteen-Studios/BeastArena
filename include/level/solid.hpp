#ifndef FILE_SOLID_HPP
#define FILE_SOLID_HPP

enum SolidType { RECTANGLE };

struct Solid {
public:
    SolidType type;
    int x; // Before relative modifier
    int y; // Before relative modifier
    int xScale; // Before relative modifier
    int yScale; // Before relative modifier
};

#endif // FILE_SOLID_HPP