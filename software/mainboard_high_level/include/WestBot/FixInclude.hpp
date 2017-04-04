// Copyright (c) 2016-2017 All Rights Reserved WestBot

#ifndef WESTBOT_FIXINCLUDE_HPP_
#define WESTBOT_FIXINCLUDE_HPP_

#include <QGraphicsPixmapItem>
#include <QList>
#include <QPair>

namespace WestBot {

#ifndef NULL
#define NULL 0
#endif

enum ToSetState
{
    TOSETWALL,
    TOSETWAY,
    TOSETDIRT,
    TOSETSTART,
    TOSETEND
};

enum NodeType
{
    NONE = 0,
    WAYNODE,
    WALLNODE,
    OPENEDNODE,
    VISITEDNODE
};

enum AStarHeuristics
{
    none,
    euclidean,
    manhattan,
    diagonal,
    newH
};

enum AncestorDirection
{
    SELF,
    UPLEFT,
    UP,
    UPRIGHT,
    LEFT,
    RIGHT,
    DOWNLEFT,
    DOWN,
    DOWNRIGHT
};

class SearchNode
{
public:
    NodeType type;

    uint originX;
    uint originY;
    double h;
    uint g;
    uint expandCost;
    double cost;
    double pathCost;

    SearchNode();
    ~SearchNode();
};

class PathNode
{
public:
    uint x;
    uint y;

    PathNode();
    ~PathNode();
};

class NodeState
{
public:
    uint x;
    uint y;

    NodeType state;

    AncestorDirection ancestor;

    NodeState();
    NodeState( uint x, uint y, NodeType state );
    ~NodeState();
};

class MapNode
{
public:
    NodeType type;

    uint expandCost;

    QGraphicsPixmapItem pixmapItem;
};

struct RunResources
{
    class MapNode** map;
    QList< QPair< uint, uint> >* path;
    QList< QList< NodeState > >* steps;
    QPair< uint, uint > start;
    QPair< uint, uint > end;
};

}

#endif // WESTBOT_FIXINCLUDE_HPP_
