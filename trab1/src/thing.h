#ifndef THING_H
#define THING_H

#include "position.h"

class Tile;

class Thing
{
public:
    Thing(const Position& position) : m_position(position) {}
    virtual ~Thing() {}

    virtual void draw(int x, int y, int size) = 0;
    virtual float onEnterTile(Tile *tile) { return 0; }

    void setPosition(const Position& position) { m_position = position; }
    Position getPosition() const { return m_position; }
    Position& getPosition() { return m_position; }

protected:
    Position m_position;
};

#endif
