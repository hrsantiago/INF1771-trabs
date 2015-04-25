#include <cstdio>
#include <vector>
#include <chrono>
#include <cstdint>

#include "houses.h"
#include "items.h"
#include "knights.h"
#include "map.h"
#include "painter.h"
#include "window.h"

typedef long ticks_t;
const static auto startup_time = std::chrono::high_resolution_clock::now();
bool g_breakA = true;

ticks_t millis()
{
    return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - startup_time).count();
}

int64_t micros()
{
    return std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - startup_time).count();
}

int main(int argc, char* argv[])
{
    bool render = true;

    std::vector<std::string> args(argv, argv + argc);
    for(const std::string& arg : args) {
        if(arg == "-no-render")
            render = false;
        else if(arg == "-no-break")
            g_breakA = false;
    }

    if(!g_houses.load("assets/houses.txt"))
        return 1;

    if(!g_items.load("assets/items.txt"))
        return 1;

    if(!g_map.load("assets/map.txt"))
        return 1;

    if(!g_knights.load("assets/knights.txt"))
        return 1;

    Player *player = g_map.getPlayer();
    player->setKnights(g_knights.createKnights());

    float fightsCost = 0;
    long unsigned int startCalc = micros();
    std::vector<std::vector<int>> fights = g_houses.findFights(player->getKnights());
    player->setFighters(fights);
    long unsigned int fightsCalcTime = micros() - startCalc;

    std::vector<Knight> knights = g_knights.createKnights();
    for(size_t i = 0; i < fights.size(); ++i) {
        fightsCost += g_houses.fight(i, knights, fights[i]);
        printf("Fight: ");
        g_knights.printKnights(knights);
    }

    printf("Fights statistics:\n");
    printf("Time to calculate: %lu us\n", fightsCalcTime);
    printf("Cost: %.2f\n\n", fightsCost);

    int pathCost = 0;
    startCalc = micros();
    std::vector<Direction> dirs = g_map.findPath(g_map.getStartPosition(), g_map.getEndPosition());
    long unsigned int pathCalcTime = micros() - startCalc;
    Position p = g_map.getStartPosition();
    for(Direction dir : dirs) {
        p = p.translatedToDirection(dir);
        pathCost += g_items.getItem(g_map.getTile(p)->getGround()->getId())->cost;
    }

    printf("Path statistics:\n");
    printf("Total directions: %d\n", (int)dirs.size());
    printf("Time to calculate: %lu us\n", pathCalcTime);
    printf("Cost: %d\n\n", pathCost);

    printf("Total expected statistics:\n");
    printf("Total time to calculate: %lu us\n", fightsCalcTime + pathCalcTime);
    printf("Total cost: %.2f\n", fightsCost + pathCost);
    fflush(stdout);

    if(!render)
        return 0;

    g_window.init();

    ticks_t lastUpdate = millis();
    unsigned int dirIndex = 0;

    float totalCost = 0;
    while(!g_window.hasFinished()) {
        g_window.poll();

        if(millis() - lastUpdate > 50) {
            if(dirIndex < dirs.size()) {
                totalCost += g_map.moveThing(player, player->getPosition().translatedToDirection(dirs[dirIndex++]));
                printf("Total viewed cost: %.2f\n", totalCost);
                fflush(stdout);
            }
            lastUpdate = millis();
        }
        g_painter.clear(0, 0, 0);
        g_map.draw();
        g_painter.swapBuffers();
    }
    g_window.terminate();

    printf("Total viewed cost: %.2f\n", totalCost);

    return 0;
}
