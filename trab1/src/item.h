#ifndef ITEM_H
#define ITEM_H

#include "thing.h"

class Item : public Thing
{
public:
    Item(char id) : m_id(id) {}

    void setId(char id) { m_id = id; }

    char getId() { return m_id; }

private:
    char m_id;
};

#endif
