#ifndef PLAYER_H
#define PLAYER_H

#include "thing.h"
#include "knights.h"
#include <vector>
#include <string>

class Player : public Thing
{
public:
    Player(const Position& position) : Thing(position) {}
    virtual ~Player() {}

    void draw(int x, int y, int size);

    void setKnights(const std::vector<Knight>& knights) { m_knights = knights; }

    const std::vector<Knight>& getKnights() { return m_knights; }

private:
    std::vector<Knight> m_knights;
};

#endif
