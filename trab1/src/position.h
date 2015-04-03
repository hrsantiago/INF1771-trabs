#ifndef POSITION_H
#define POSITION_H

#include <vector>
#include <cmath>

enum Direction {
    North = 0,
    East,
    South,
    West,

    InvalidDirection
};

class Position
{
public:
    Position() : x(0), y(0) { }
    Position(int x, int y) : x(x), y(y) { }

    Position translatedToDirection(Direction direction) {
        Position pos = *this;
        switch(direction) {
        case North:
            pos.y--;
            break;
        case East:
            pos.x++;
            break;
        case South:
            pos.y++;
            break;
        case West:
            pos.x--;
            break;
        default:
            break;
        }
        return pos;
    }

    Position translatedToReverseDirection(Direction direction) {
        Position pos = *this;
        switch(direction) {
        case North:
            pos.y++;
            break;
        case East:
            pos.x--;
            break;
        case South:
            pos.y--;
            break;
        case West:
            pos.x++;
            break;
        default:
            break;
        }
        return pos;
    }

    std::vector<Position> translatedToDirections(const std::vector<Direction>& dirs) const {
        Position lastPos = *this;
        std::vector<Position> positions;

        positions.push_back(lastPos);

        for(auto dir : dirs) {
            lastPos = lastPos.translatedToDirection(dir);
            positions.push_back(lastPos);
        }

        return positions;
    }

    float distance(const Position& pos) const { return std::sqrt(std::pow((pos.x - x), 2) + std::pow((pos.y - y), 2)); }
    int manhattanDistance(const Position& pos) const { return std::abs(pos.x - x) + std::abs(pos.y - y); }

    void translate(int dx, int dy) { x += dx; y += dy; }
    Position translated(int dx, int dy) const { Position pos = *this; pos.x += dx; pos.y += dy; return pos; }

    Position operator+(const Position& other) const { return Position(x + other.x, y + other.y);   }
    Position& operator+=(const Position& other) { x+=other.x; y+=other.y; return *this; }
    Position operator-(const Position& other) const { return Position(x - other.x, y - other.y);   }
    Position& operator-=(const Position& other) { x-=other.x; y-=other.y; return *this; }

    Position& operator=(const Position& other) { x = other.x; y = other.y; return *this; }
    bool operator==(const Position& other) const { return other.x == x && other.y == y; }
    bool operator!=(const Position& other) const { return other.x!=x || other.y!=y; }
    bool isInRange(const Position& pos, int xRange, int yRange) const { return std::abs(x-pos.x) <= xRange && std::abs(y-pos.y) <= yRange; }
    bool isInRange(const Position& pos, int minXRange, int maxXRange, int minYRange, int maxYRange) const {
        return (pos.x >= x-minXRange && pos.x <= x+maxXRange && pos.y >= y-minYRange && pos.y <= y+maxYRange);
    }

    int x;
    int y;
};

#endif
