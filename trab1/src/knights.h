#ifndef KNIGHTS_H
#define KNIGHTS_H

#include <string>
#include <vector>

struct KnightType
{
    std::string name;
    float power;
    int maxHealth;
};

struct Knight
{
    int id;
    int health;

    bool operator==(const Knight& other) const {
        return id == other.id && health == other.health;
    }
};

class Knights
{
public:
    bool load(const std::string& filename);

    std::vector<Knight> createKnights();
    void printKnights(const std::vector<Knight>& knights);

    const KnightType& getKnightType(int id) { return m_knights[id]; }

private:
    std::vector<KnightType> m_knights;
};

extern Knights g_knights;

#endif
