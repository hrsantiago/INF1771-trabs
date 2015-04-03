#ifndef ITEM_H
#define ITEM_H

#include "thing.h"

class Item : public Thing
{
public:
    Item(char id, const Position& position) : Thing(position), m_id(id) {}
    virtual ~Item() {}

    void draw(int x, int y, int size);

    void setId(char id) { m_id = id; }

    char getId() { return m_id; }

private:
    char m_id;
};

#endif
