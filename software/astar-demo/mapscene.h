// Copyright (c) 2016-2017 All Rights Reserved WestBot

#ifndef WESTBOT_MAPSCENE_HPP_
#define WESTBOT_MAPSCENE_HPP_

#include <QDir>
#include <QGraphicsPixmapItem>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QList>
#include <QMessageBox>
#include <QPair>
#include <QPointF>
#include <QString>

#include <WestBot/AStar/AStar.hpp>
#include <WestBot/AStar/MapNode.hpp>
#include <WestBot/AStar/NodeState.hpp>
#include <WestBot/AStar/Utils.hpp>

namespace WestBot {
namespace AStar {

class MapScene : public QGraphicsScene
{
public:
    MapScene( QObject * parent = nullptr );
    ~MapScene();

    void setMap( uint mapWidth, uint mapHeight );
    void showPath();
    void setHeuristics( AStarHeuristics heuristic );
    void setScale( uint scale );
    void setStart( uint x, uint y );
    void setEnd( uint x, uint y );
    void setToSetState( ToSetState toSetState );
    void setStepsDelay( unsigned long delay );

protected:
    void mouseReleaseEvent( QGraphicsSceneMouseEvent* event );

private:
    void freeNodes();
    void destroyMap();
    void drawPath();
    void obtainPath( bool saveChanges = false );
    void reDrawStartEndNodes();

private:
    AStar astar;

    MapNode< QGraphicsPixmapItem >** map;

    QList< QPair< uint, uint > > path;
    QList< QList< NodeState > > steps;
    QList< QList< NodeState > >::iterator stepsIt;

    QString freeNodeImgPath;
    QString dirtNodeImgPath;
    QString openedNodeImgPath[ 8 ];
    QString closedNodeImgPath;
    QString wallNodeImgPath;
    QString pathNodeImgPath;
    QString startNodeImgPath;
    QString endNodeImgPath;

    ToSetState toSetState;

    uint mapWidth;
    uint mapHeight;

    uint itemSize;
    uint itemScale;

    bool pathOnScreen;
};

}
}

#endif // WESTBOT_MAPSCENE_HPP_
