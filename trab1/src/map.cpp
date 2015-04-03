#include <cstdio>
#include <unordered_map>
#include <queue>
#include <algorithm>

#include "items.h"
#include "map.h"
#include "window.h"
#include "painter.h"
#include "player.h"

Map g_map;

Map::~Map()
{
    for(Tile *tile : m_tiles)
        delete tile;
}

bool Map::load(const std::string& filename)
{
    FILE *fp = fopen(filename.c_str(), "r");
    if(!fp) {
        printf("Could not open file %s", filename.c_str());
        return false;
    }

    if(fscanf(fp, "%d %d", &m_width, &m_height) != 2) {
        printf("Could not get map size.\n");
        return false;
    }

    if(m_width > 1000 || m_height > 1000) {
        printf("Map is too big.\n");
        return false;
    }

    m_tiles.resize(m_width * m_height);

    for(int y = 0; y < m_height; ++y) {
        for(int x = 0; x < m_width; ++x) {
            char groundId;
            if(fscanf(fp, " %c", &groundId) != 1) {
                printf("Map is corrupted.\n");
                return false;
            }

            Position position = Position(x, y);

            Item *ground = new Item(groundId, position);

            Tile *tile = getTile(position);
            tile->setGround(ground);

            if(groundId == 'S') {
                Player *player = new Player(position);
                tile->addThing(player);
                m_player = player;
                m_startPosition = position;
            }
            else if(groundId == 'E') {
                m_endPosition = position;
            }
        }
    }

    fclose(fp);
    return true;
}

void Map::draw()
{
    int w, h;
    g_window.getSize(w, h);

    int size = std::min(w, h) / m_width;
    for(int y = 0; y < m_height; ++y) {
        for(int x = 0; x < m_width; ++x) {
            Tile *tile = getTile(Position(x, y));
            Item *ground = tile->getGround();
            ground->draw(x * size, y * size, size);

            for(Thing *thing : tile->getThings())
                thing->draw(x * size, y * size, size);
        }
    }
}

Tile *Map::getTile(const Position& position)
{
    if(position.x < 0 || position.x >= m_width || position.y < 0 || position.y >= m_height)
        return nullptr;

    Tile *tile = m_tiles[position.y * m_width + position.x];
    if(!tile) {
        tile = new Tile(position);
        m_tiles[position.y * m_width + position.x] = tile;
    }
    return tile;
}

int Map::moveThing(Thing *thing, const Position& position)
{
    Tile *fromTile = getTile(thing->getPosition());
    fromTile->removeThing(thing);
    Tile *toTile = getTile(position);
    toTile->addThing(thing);
    thing->setPosition(position);

    return g_items.getItem(toTile->getGround()->getId())->cost;
}

std::vector<Direction> Map::findPath(const Position& startPos, const Position& endPos)
{
    struct Node {
        Node(const Position& pos) : cost(0), totalCost(0), pos(pos), prev(nullptr), dir(InvalidDirection) { }
        float cost;
        float totalCost;
        Position pos;
        Node *prev;
        Direction dir;
        int n = 0;
    };

    struct LessNode : std::binary_function<std::pair<Node*, float>, std::pair<Node*, float>, bool> {
        bool operator()(std::pair<Node*, float> a, std::pair<Node*, float> b) const {
            return b.second < a.second;
            //return b.first->n < a.first->n;
        }
    };

    std::unordered_map<Position, Node*, PositionHasher> nodes;
    std::priority_queue<std::pair<Node*, float>, std::vector<std::pair<Node*, float>>, LessNode> searchList;

    Node *currentNode = new Node(startPos);
    currentNode->pos = startPos;
    nodes[startPos] = currentNode;
    Node *foundNode = nullptr;

    std::vector<Direction> dirs;

    while(currentNode) {
        if(currentNode->pos == endPos && (!foundNode || currentNode->cost < foundNode->cost))
            foundNode = currentNode;

        for(int i=-1;i<=1;++i) {
            for(int j=-1;j<=1;++j) {
                if((i == 0 && j == 0) || (i == -1 && j == -1) || (i == 1 && j == -1) || (i == -1 && j == 1) || (i == 1 && j == 1))
                    continue;

                Position neighborPos = currentNode->pos.translated(i, j);
                Tile *tile = getTile(neighborPos);
                if(!tile)
                    continue;
                float cost = currentNode->cost + g_items.getItem(tile->getGround()->getId())->cost;

                Direction walkDir = currentNode->pos.getDirectionFromPosition(neighborPos);

                Node *neighborNode;
                if(nodes.find(neighborPos) == nodes.end()) {
                    neighborNode = new Node(neighborPos);
                    nodes[neighborPos] = neighborNode;
                } else {
                    neighborNode = nodes[neighborPos];
                    if(neighborNode->cost <= cost)
                        continue;
                }

                neighborNode->prev = currentNode;
                neighborNode->cost = cost;
                neighborNode->totalCost = neighborNode->cost + neighborPos.manhattanDistance(endPos);
                neighborNode->dir = walkDir;
                neighborNode->n = currentNode->n + 1;
                searchList.push(std::make_pair(neighborNode, neighborNode->totalCost));
            }
        }

        if(!searchList.empty()) {
            currentNode = searchList.top().first;
            searchList.pop();
        } else
            currentNode = nullptr;
    }

    if(foundNode) {
        currentNode = foundNode;
        while(currentNode) {
            dirs.push_back(currentNode->dir);
            currentNode = currentNode->prev;
        }
        dirs.pop_back();
        std::reverse(dirs.begin(), dirs.end());
    }

    for(auto it : nodes)
        delete it.second;

    return dirs;
}
