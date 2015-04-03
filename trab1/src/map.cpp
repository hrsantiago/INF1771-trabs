#include <cstdio>

#include "map.h"
#include "window.h"
#include "painter.h"
#include "player.h"

Map g_map;

Map::~Map()
{
    for(Tile *tile : m_tiles)
        delete tile;
}

bool Map::load(const std::string& filename)
{
    FILE *fp = fopen(filename.c_str(), "r");
    if(!fp) {
        printf("Could not open file %s", filename.c_str());
        return false;
    }

    if(fscanf(fp, "%d %d", &m_width, &m_height) != 2) {
        printf("Could not get map size.\n");
        return false;
    }

    if(m_width > 1000 || m_height > 1000) {
        printf("Map is too big.\n");
        return false;
    }

    m_tiles.resize(m_width * m_height);

    for(int y = 0; y < m_height; ++y) {
        for(int x = 0; x < m_width; ++x) {
            char groundId;
            if(fscanf(fp, " %c", &groundId) != 1) {
                printf("Map is corrupted.\n");
                return false;
            }

            Position position = Position(x, y);

            Item *ground = new Item(groundId);
            ground->setPosition(position);

            Tile *tile = getTile(position);
            tile->setGround(ground);

            if(groundId == 'S') {
                Player *player = new Player();
                tile->addThing(player);
            }
        }
    }

    fclose(fp);
    return true;
}

void Map::draw()
{
    int w, h;
    g_window.getSize(w, h);

    int tileSize = std::min(w, h) / m_width;
    for(int y = 0; y < m_height; ++y) {
        for(int x = 0; x < m_width; ++x) {
            Item *ground = getTile(Position(x, y))->getGround();
            char groundId = ground->getId();
            if(groundId == '0')
                g_painter.setColor(40, 40, 40, 255);
            else if(groundId == '1')
                g_painter.setColor(80, 80, 80, 255);
            else if(groundId == '2')
                g_painter.setColor(160, 160, 160, 255);
            else if(groundId == 'C')
                g_painter.setColor(255, 255, 0, 255);
            else if(groundId == 'S')
                g_painter.setColor(255, 0, 0, 255);
            else if(groundId == 'E')
                g_painter.setColor(0, 255, 0, 255);
            g_painter.drawFilledRect(x*tileSize,y*tileSize, tileSize, tileSize);
        }
    }
}

Tile *Map::getTile(const Position& position)
{
    if(position.x < 0 || position.x >= m_width || position.y < 0 || position.y >= m_height)
        return nullptr;

    Tile *tile = m_tiles[position.y * m_width + position.x];
    if(!tile) {
        tile = new Tile(position);
        m_tiles[position.y * m_width + position.x] = tile;
    }
    return tile;
}

void Map::moveThing(Thing *thing, const Position& position)
{
    Tile *fromTile = getTile(thing->getPosition());
    fromTile->removeThing(thing);
    Tile *toTile = getTile(position);
    toTile->addThing(thing);
}

std::vector<Direction> Map::findPath(const Position& start, const Position& end)
{
    return std::vector<Direction>();
}
