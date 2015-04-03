#include <cstdio>
#include <vector>
#include "window.h"
#include "painter.h"
#include "map.h"
#include "items.h"

int main(int argc, char* argv[])
{
    bool render = false;

    std::vector<std::string> args(argv, argv + argc);
    for(const std::string& arg : args) {
        if(arg == "-render")
            render = true;
    }

    if(!g_map.load("assets/map.txt"))
        return 1;

    if(!g_items.load("assets/items.txt"))
        return 1;

    if(!render)
        return 0;

    g_window.init();
    while(!g_window.hasFinished()) {
        g_window.poll();
        g_map.draw();
        g_painter.swapBuffers();
    }
    g_window.terminate();

    return 0;
}
