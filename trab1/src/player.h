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
    float onEnterTile(Tile *tile);

    void setKnights(const std::vector<Knight>& knights) { m_knights = knights; }
    void setFighters(const std::vector<std::vector<int>>& fighters) { m_fighters = fighters; }

    const std::vector<Knight>& getKnights() { return m_knights; }

private:
    int m_housesFought = 0;
    std::vector<std::vector<int>> m_fighters;
    std::vector<Knight> m_knights;
};

#endif
