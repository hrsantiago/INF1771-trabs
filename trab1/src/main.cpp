#include <cstdio>
#include <vector>
#include "window.h"
#include "painter.h"

int main(int argc, char* argv[])
{
    bool render = false;

    std::vector<std::string> args(argv, argv + argc);
    for(const std::string& arg : args) {
        if(arg == "-render")
            render = true;
    }

    if(!render)
        return 0;

    g_window.init();
    while(!g_window.hasFinished()) {
        g_window.poll();

        g_painter.swapBuffers();
    }
    g_window.terminate();

    return 0;
}
