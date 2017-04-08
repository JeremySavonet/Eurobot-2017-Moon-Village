// Copyright (c) 2016-2017 All Rights Reserved WestBot

#ifndef WESTBOT_GAMEMAP_HPP_
#define WESTBOT_GAMEMAP_HPP_

#include <memory>

#include <QObject>
#include <QList>
#include <QPair>

#include <WestBot/AStar/AStar.hpp>
#include <WestBot/AStar/MapNode.hpp>
#include <WestBot/AStar/NodeState.hpp>
#include <WestBot/AStar/Utils.hpp>

namespace WestBot {

class GameMap : public QObject
{
public:
    using Ptr = std::shared_ptr< GameMap >;

    GameMap( QObject * parent = nullptr );
    ~GameMap() override;

    void setMap( uint mapWidth, uint mapHeight );
    void showPath();
    void setHeuristics( AStar::AStarHeuristics heuristic );
    void setScale( uint scale );
    void setStart( uint x, uint y );
    void setEnd( uint x, uint y );
    void setToSetState( AStar::ToSetState toSetState );

private:
    void freeNodes();
    void destroyMap();
    void drawPath();
    void obtainPath( bool saveChanges = false );
    void reDrawStartEndNodes();

private:
    AStar::AStar _astar;

    AStar::MapNode** _map;

    QList< QPair< uint, uint > > _path;

    AStar::ToSetState _toSetState;

    uint _mapWidth;
    uint _mapHeight;

    uint _itemSize;
    uint _itemScale;

    bool _pathOnScreen;
};

}

#endif // WESTBOT_GAMEMAP_HPP_
