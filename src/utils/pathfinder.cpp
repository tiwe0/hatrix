// https://github.com/daancode/a-star/blob/master/source/AStar.cpp
/*
    Copyright (c) 2015, Damian Barczynski <daan.net@wp.eu>
    Following tool is licensed under the terms and conditions of the ISC license.
    For more information visit https://opensource.org/licenses/ISC.
*/

#include "hatrix/utils/position.hpp"
#include "hatrix/utils/pathfinder.hpp"

#include <vector>
#include <queue>
#include <unordered_map>
#include <algorithm>
#include <math.h>
#include <functional>
#include <set>

using namespace std::placeholders;

using uint = unsigned int;
using HeuristicFunction = std::function<uint(Vec2, Vec2)>;

struct Node
{
    uint G, H;
    Vec2 coordinates;
    Node *parent;

    Node(Vec2 coord_, Node *parent_ = nullptr);
    uint getScore();
};

using NodeSet = std::vector<Node *>;

Node::Node(Vec2 coordinates_, Node *parent_)
{
    parent = parent_;
    coordinates = coordinates_;
    G = H = 0;
}

uint Node::getScore()
{
    return G + H;
}

Node *findNodeOnList(NodeSet &nodes_, Vec2 coordinates_)
{
    for (auto node : nodes_)
    {
        if (node->coordinates == coordinates_)
        {
            return node;
        }
    }
    return nullptr;
}

void releaseNodes(NodeSet &nodes_)
{
    for (auto it = nodes_.begin(); it != nodes_.end();)
    {
        delete *it;
        it = nodes_.erase(it);
    }
}

Vec2 getDelta(Vec2 source_, Vec2 target_)
{
    return {abs(source_.x - target_.x), abs(source_.y - target_.y)};
}

uint manhattan(Vec2 source_, Vec2 target_)
{
    auto delta = std::move(getDelta(source_, target_));
    return static_cast<uint>(10 * (delta.x + delta.y));
}

uint euclidean(Vec2 source_, Vec2 target_)
{
    auto delta = std::move(getDelta(source_, target_));
    return static_cast<uint>(10 * sqrt(pow(delta.x, 2) + pow(delta.y, 2)));
}

uint octagonal(Vec2 source_, Vec2 target_)
{
    auto delta = std::move(getDelta(source_, target_));
    return 10 * (delta.x + delta.y) + (-6) * std::min(delta.x, delta.y);
}

static int directions = 4;
static std::vector<Vec2> direction = {{0, 1}, {0, -1}, {1, 0}, {-1, 0}};
static HeuristicFunction heuristic = manhattan;

std::vector<Vec2> utils_compute_path(Vec2 source_, Vec2 target_, int radius, std::function<bool(int, int)> is_blocking)
{
    Node *current = nullptr;
    NodeSet openSet, closedSet;
    openSet.reserve(100);
    closedSet.reserve(100);
    openSet.push_back(new Node(source_));

    while (!openSet.empty())
    {
        auto current_it = openSet.begin();
        current = *current_it;

        for (auto it = openSet.begin(); it != openSet.end(); it++)
        {
            auto node = *it;
            if (node->getScore() <= current->getScore())
            {
                current = node;
                current_it = it;
            }
        }

        if (current->coordinates == target_)
        {
            break;
        }

        closedSet.push_back(current);
        openSet.erase(current_it);

        for (uint i = 0; i < directions; ++i)
        {
            Vec2 newCoordinates(current->coordinates + direction[i]);
            int x = newCoordinates.x;
            int y = newCoordinates.y;
            int distance =std::floor((newCoordinates - source_).norm());
            if (is_blocking(x, y) || (distance > radius) ||
                findNodeOnList(closedSet, newCoordinates))
            {
                continue;
            }

            uint totalCost = current->G + ((i < 4) ? 10 : 14);

            Node *successor = findNodeOnList(openSet, newCoordinates);
            if (successor == nullptr)
            {
                successor = new Node(newCoordinates, current);
                successor->G = totalCost;
                successor->H = heuristic(successor->coordinates, target_);
                openSet.push_back(successor);
            }
            else if (totalCost < successor->G)
            {
                successor->parent = current;
                successor->G = totalCost;
            }
        }
    }

    std::vector<Vec2> path;
    while (current != nullptr)
    {
        path.push_back(current->coordinates);
        current = current->parent;
    }

    releaseNodes(openSet);
    releaseNodes(closedSet);

    return path;
}