#ifndef MAP_H
#define MAP_H

#include <string>
#include <vector>
#include "tile.h"

class Map
{
public:
    ~Map();
    bool load(const std::string& filename);

    void draw();

    Tile *getTile(const Position& position);

    void moveThing(Thing *thing, const Position& position);
    std::vector<Direction> findPath(const Position& start, const Position& end);

private:
    int m_width;
    int m_height;
    std::vector<Tile*> m_tiles;
};

extern Map g_map;

#endif
