#include <algorithm>
#include "tile.h"

void Tile::addThing(Thing *thing)
{
    m_things.push_back(thing);
}

void Tile::removeThing(Thing *thing)
{
    auto end = std::remove_if(m_things.begin(), m_things.end(), [thing](Thing *a) {
        return a == thing;
    });
    m_things.erase(end, m_things.end());
}
