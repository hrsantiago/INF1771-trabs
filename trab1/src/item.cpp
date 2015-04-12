#include "item.h"
#include "painter.h"

void Item::draw(int x, int y, int size)
{
    if(m_id == '0')
        g_painter.setColor(40, 40, 40, 255);
    else if(m_id == '1')
        g_painter.setColor(80, 80, 80, 255);
    else if(m_id == '2')
        g_painter.setColor(160, 160, 160, 255);
    else if(m_id == 'C')
        g_painter.setColor(255, 255, 0, 255);
    else if(m_id == 'S')
        g_painter.setColor(255, 0, 0, 255);
    else if(m_id == 'E')
        g_painter.setColor(0, 255, 0, 255);

    g_painter.drawFilledRect(x, y, size, size);
}
