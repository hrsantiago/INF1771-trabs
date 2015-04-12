#include "player.h"
#include "painter.h"

void Player::draw(int x, int y, int size)
{
    g_painter.setColor(0, 255, 255, 255);
    g_painter.drawFilledRect(x+2, y+2, size-4, size-4);
}
