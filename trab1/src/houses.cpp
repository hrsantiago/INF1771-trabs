#include <queue>
#include <algorithm>
#include <unordered_map>
#include "houses.h"
#include "knights.h"

Houses g_houses;

Houses::~Houses()
{
    for(House *house : m_houses)
        if(house)
            delete house;
}

bool Houses::load(const std::string& filename)
{
    FILE *fp = fopen(filename.c_str(), "r");
    if(!fp) {
        printf("Could not open file %s\n", filename.c_str());
        return false;
    }

    int count;
    if(fscanf(fp, "%d", &count) != 1) {
        printf("Could not get houses size.\n");
        return false;
    }

    if(count > 16) {
        printf("Too many houses.\n");
        return false;
    }

    m_houses.resize(count);
    for(int i = 0; i < count; i++) {
        char name[80];
        int cost;
        if(fscanf(fp, " %80s %d", name, &cost) != 2) {
            printf("Malformed houses file\n");
            return false;
        }

        House *house = new House();
        house->name = name;
        house->cost = cost;
        m_houses[i] = house;
    }

    fclose(fp);
    return true;
}

float Houses::fight(int houseId, std::vector<Knight>& knights, const std::vector<int>& fighters)
{
    int houseCost = m_houses[houseId]->cost;
    float fightersPower = 0;
    for(int fighter : fighters) {
        Knight& knight = knights[fighter];
        --knight.health;
        fightersPower += g_knights.getKnightType(fighter).power;
    }
    return (float)houseCost / fightersPower;
}

float Houses::getEstimatedCost(int currentHouse, const std::vector<Knight>& knights)
{
    float cost = 0;

    for(uint i = currentHouse; i < m_houses.size(); ++i)
        cost += m_houses[i]->cost;

    float power = 0;
    for(const Knight& knight : knights) {
        if(knight.health > 0)
            power += g_knights.getKnightType(knight.id).power;
    }
    cost /= power;
    return cost;
}

// Returns list of knights indexes that will fight
std::vector<std::vector<int>> Houses::findFights(const std::vector<Knight>& knights)
{
    struct NodeState {
        std::vector<Knight> knights;
        int housesFought;

        bool operator==(const NodeState& other) const {
            return std::equal(knights.begin(), knights.end(), other.knights.begin()) && other.housesFought == housesFought;
        }
    };

    struct NodeStateHasher : std::unary_function<NodeState, std::size_t> {
        std::size_t operator()(const NodeState& nodeState) const {
            std::size_t ret = nodeState.housesFought;
            for(uint i = 0; i < nodeState.knights.size(); ++i)
                ret |= nodeState.knights[i].health << (4*(i+1));
            return ret;
        }
    };

    struct Node {
        Node(const NodeState& nodeS) : prev(nullptr), nodeState(nodeS) { }
        float cost;
        float totalCost;
        Node *prev;
        NodeState nodeState;
        std::vector<int> fighters;
    };

    struct LessNode : std::binary_function<std::pair<Node*, float>, std::pair<Node*, float>, bool> {
        bool operator()(std::pair<Node*, float> a, std::pair<Node*, float> b) const {
            return b.second < a.second;
        }
    };

    std::unordered_map<NodeState, Node*, NodeStateHasher> nodes;
    std::priority_queue<std::pair<Node*, float>, std::vector<std::pair<Node*, float>>, LessNode> searchList;

    NodeState nodeState = { knights, 0 };
    Node *currentNode = new Node(nodeState);
    currentNode->cost = 0;
    currentNode->totalCost = 0;
    nodes[currentNode->nodeState] = currentNode;
    Node *foundNode = nullptr;

    int knightsCount = knights.size();
    std::vector<std::vector<int>> fightsPermutation;
    for(int i = 0; i < knightsCount; ++i) {
        std::vector<int> v0(knightsCount);
        for(int j = 0; j < knightsCount; ++j)
            v0[j] = (j >= knightsCount-i-1) ? 1 : 0;

        do {
            fightsPermutation.push_back(v0);
        }
        while(std::next_permutation(v0.begin(), v0.end()));
    }

    for(std::vector<int>& fight : fightsPermutation) {
        std::vector<int> finalFight;
        for(int i = 0; i < knightsCount; ++i) {
            if(fight[i] == 1)
                finalFight.push_back(i);
        }
        fight = finalFight;
    }

    int housesCount = m_houses.size();
    while(currentNode) {
        if(currentNode->nodeState.housesFought == housesCount && (!foundNode || currentNode->cost < foundNode->cost))
            foundNode = currentNode;

        if(currentNode->nodeState.housesFought < housesCount) {
            for(std::vector<int>& fighters : fightsPermutation) {
                std::vector<Knight> knights = currentNode->nodeState.knights;
                float cost = currentNode->cost + fight(currentNode->nodeState.housesFought, knights, fighters);

                bool isValid = true;
                bool hasAlive = false;
                for(const Knight& knight : knights) {
                    if(knight.health < 0) {
                        isValid = false;
                        break;
                    }
                    else if(knight.health > 0)
                        hasAlive = true;
                }
                if(!isValid || !hasAlive)
                    continue;

                NodeState nodeState = { knights, currentNode->nodeState.housesFought + 1 };
                Node *neighborNode;

                auto it = nodes.find(nodeState);
                if(it == nodes.end()) {
                    neighborNode = new Node(nodeState);
                    nodes[nodeState] = neighborNode;
                } else {
                    neighborNode = (*it).second;
                    if(neighborNode->cost <= cost)
                        continue;
                }

                neighborNode->prev = currentNode;
                neighborNode->cost = cost;
                neighborNode->totalCost = cost + getEstimatedCost(neighborNode->nodeState.housesFought, knights);
                neighborNode->fighters = fighters;
                searchList.push(std::make_pair(neighborNode, neighborNode->totalCost));
            }
        }

        if(!searchList.empty()) {
            currentNode = searchList.top().first;
            searchList.pop();
        } else
            currentNode = nullptr;
    }

    std::vector<std::vector<int>> fights;
    if(foundNode) {
        currentNode = foundNode;
        while(currentNode) {
            fights.push_back(currentNode->fighters);
            currentNode = currentNode->prev;
        }
        fights.pop_back();
        std::reverse(fights.begin(), fights.end());
    }

    for(auto& it : nodes)
        delete it.second;

    return fights;
}
