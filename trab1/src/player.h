#ifndef PLAYER_H
#define PLAYER_H

#include "thing.h"

class Player : public Thing
{
public:
    Player(const Position& position) : Thing(position) {}
    virtual ~Player() {}

    void draw(int x, int y, int size);
};

#endif
