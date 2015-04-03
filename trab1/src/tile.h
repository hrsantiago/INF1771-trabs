#ifndef TILE_H
#define TILE_H

#include <vector>
#include "thing.h"
#include "item.h"
#include "position.h"

class Tile
{
public:
    Tile(const Position& position) : m_position(position) {}
    ~Tile() {
        if(m_ground)
            delete m_ground;
        for(Thing *thing : m_things)
            delete thing;
    }

    void setGround(Item *ground) { m_ground = ground; }
    void addThing(Thing *thing);
    void removeThing(Thing *thing);

    Item *getGround() { return m_ground; }
    const std::vector<Thing*>& getThings() { return m_things; }

private:
    Position m_position;
    Item *m_ground = nullptr;
    std::vector<Thing*> m_things;
};

#endif
