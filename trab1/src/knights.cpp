#include <cstdio>
#include "knights.h"

Knights g_knights;

bool Knights::load(const std::string& filename)
{
    FILE *fp = fopen(filename.c_str(), "r");
    if(!fp) {
        printf("Could not open file %s\n", filename.c_str());
        return false;
    }

    int count;
    if(fscanf(fp, "%d", &count) != 1) {
        printf("Could not get knights size.\n");
        return false;
    }

    if(count > 16) {
        printf("Too many knights.\n");
        return false;
    }

    m_knights.resize(count);
    for(int i = 0; i < count; i++) {
        char name[80];
        float power;
        int maxHealth;
        if(fscanf(fp, " %80s %f %d", name, &power, &maxHealth) != 3) {
            printf("Malformed knights file\n");
            return false;
        }

        KnightType& knightType = m_knights[i];
        knightType.name = name;
        knightType.power = power;
        knightType.maxHealth = maxHealth;
    }

    fclose(fp);
    return true;
}

std::vector<Knight> Knights::createKnights()
{
    std::vector<Knight> knights(m_knights.size());
    for(size_t i = 0; i < m_knights.size(); ++i) {
        Knight& knight = knights[i];
        knight.id = i;
        knight.health = m_knights[i].maxHealth;
    }
    return knights;
}

void Knights::printKnights(const std::vector<Knight>& knights)
{
    for(const Knight& knight : knights) {
        const KnightType& knightType = m_knights[knight.id];
        printf("%s (%d) ", knightType.name.c_str(), knight.health);
    }
    printf("\n");
}
