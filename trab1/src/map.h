#ifndef MAP_H
#define MAP_H

#include <string>
#include <vector>
#include "tile.h"
#include "player.h"

class Map
{
public:
    ~Map();
    bool load(const std::string& filename);

    void draw();

    Tile *getTile(const Position& position);
    Player *getPlayer() { return m_player; }
    Position getStartPosition() { return m_startPosition; }
    Position getEndPosition() { return m_endPosition; }

    int moveThing(Thing *thing, const Position& position);
    std::vector<Direction> findPath(const Position& startPos, const Position& endPos);

private:
    int m_width;
    int m_height;
    std::vector<Tile*> m_tiles;

    // Hold some info from loading for faster results
    Player *m_player;
    Position m_startPosition;
    Position m_endPosition;
};

extern Map g_map;

#endif
