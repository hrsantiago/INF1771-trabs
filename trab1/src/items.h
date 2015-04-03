#ifndef ITEMS_H
#define ITEMS_H

#include <string>
#include <vector>

struct ItemType
{
    std::string name;
    int cost;
};

class Items
{
public:
    Items();
    bool load(const std::string& filename);

    ItemType *getItem(char id);

private:
    std::vector<ItemType*> m_items;
};

extern Items g_items;

#endif
