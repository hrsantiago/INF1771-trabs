#include "items.h"

Items g_items;

Items::Items()
{
    m_items.resize(256);
}

bool Items::load(const std::string& filename)
{
    FILE *fp = fopen(filename.c_str(), "r");
    if(!fp) {
        printf("Could not open file %s\n", filename.c_str());
        return false;
    }

    int count;
    if(fscanf(fp, "%d", &count) != 1) {
        printf("Could not get items size.\n");
        return false;
    }

    if(count > 1000) {
        printf("Too many items.\n");
        return false;
    }

    for(int i = 0; i < count; i++) {
        char id;
        char name[80];
        int cost;
        if(fscanf(fp, " %c %80s %d", &id, name, &cost) != 3) {
            printf("Malformed items file\n");
            return false;
        }

        ItemType *itemType = new ItemType();
        itemType->name = name;
        itemType->cost = cost;
        m_items[id] = itemType;
    }

    fclose(fp);
    return true;
}

ItemType *Items::getItem(char id)
{
    return m_items[id];
}
