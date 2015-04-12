#include "houses.h"
#include "player.h"
#include "painter.h"
#include "tile.h"

void Player::draw(int x, int y, int size)
{
    g_painter.setColor(0, 255, 255, 255);
    g_painter.drawFilledRect(x+2, y+2, size-4, size-4);
}

float Player::onEnterTile(Tile *tile)
{
    Item *ground = tile->getGround();
    if(ground && ground->getId() == 'C') {
        float additionalCost = g_houses.fight(m_housesFought, m_knights, m_fighters[m_housesFought]);
        ++m_housesFought;
        return additionalCost;
    }
    return 0;
}
