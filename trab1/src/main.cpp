#include <cstdio>
#include <vector>
#include "window.h"
#include "painter.h"
#include "map.h"
#include "items.h"
#include <chrono>

typedef long ticks_t;
const static auto startup_time = std::chrono::high_resolution_clock::now();

ticks_t millis()
{
    return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - startup_time).count();
}

int main(int argc, char* argv[])
{
    bool render = false;

    std::vector<std::string> args(argv, argv + argc);
    for(const std::string& arg : args) {
        if(arg == "-render")
            render = true;
    }

    if(!g_items.load("assets/items.txt"))
        return 1;

    if(!g_map.load("assets/map.txt"))
        return 1;

    int totalCost = 0;
    ticks_t startCalc = millis();
    std::vector<Direction> dirs = g_map.findPath(g_map.getStartPosition(), g_map.getEndPosition());
    Position p = g_map.getStartPosition();
    for(Direction dir : dirs) {
        p = p.translatedToDirection(dir);
        totalCost += g_items.getItem(g_map.getTile(p)->getGround()->getId())->cost;
    }

    printf("Total directions: %d\n", (int)dirs.size());
    printf("Time to calculate: %lu\n", millis() - startCalc);
    printf("Total cost: %d\n", totalCost);

    if(!render)
        return 0;

    g_window.init();

    ticks_t lastUpdate = millis();
    Player *player = g_map.getPlayer();
    unsigned int dirIndex = 0;

    totalCost = 0;
    while(!g_window.hasFinished()) {
        g_window.poll();

        if(millis() - lastUpdate > 500) {
            if(dirIndex < dirs.size())
                totalCost += g_map.moveThing(player, player->getPosition().translatedToDirection(dirs[dirIndex++]));
            lastUpdate = millis();
        }
        g_map.draw();
        g_painter.swapBuffers();
    }
    g_window.terminate();

    printf("Total viewed cost: %d\n", totalCost);

    return 0;
}
