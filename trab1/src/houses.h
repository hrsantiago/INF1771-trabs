#ifndef HOUSES_H
#define HOUSES_H

#include <string>
#include <vector>
#include "player.h"

struct House
{
    std::string name;
    int cost;
};

class Houses
{
public:
    ~Houses();

    bool load(const std::string& filename);
    float fight(int houseId, std::vector<Knight>& knights, const std::vector<int>& fighters);
    std::vector<std::vector<int>> findFights(const std::vector<Knight>& knights);
    float getEstimatedCost(int currentHouse, const std::vector<Knight>& knights);

private:
    std::vector<House*> m_houses;
};

extern Houses g_houses;

#endif
