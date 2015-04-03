#ifndef THING_H
#define THING_H

#include "position.h"

class Thing
{
public:
    void setPosition(const Position& position) { m_position = position; }
    Position getPosition() const { return m_position; }
    Position& getPosition() { return m_position; }

private:
    Position m_position;
};

#endif
