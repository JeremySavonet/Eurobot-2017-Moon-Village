// Copyright (c) 2016-2017 All Rights Reserved WestBot

#include <QDebug>

#include <WestBot/GameMap.hpp>

using namespace WestBot;

GameMap::GameMap( QObject* parent )
    : QObject( parent )
    , _map( nullptr )
    , _toSetState( AStar::ToSetState::TOSETWALL )
    , _mapWidth( 0 )
    , _mapHeight( 0 )
    , _itemSize( 8 )
    , _itemScale( 1 )
    , _pathOnScreen( false )
{
}

GameMap::~GameMap()
{
    destroyMap();
}

void GameMap::freeNodes()
{
    for( uint i = 0; i < _mapWidth; i++ )
    for( uint j = 0; j < _mapHeight; j++ )
    {
        if( _map[ i ][ j ].type != AStar::NodeType::WALLNODE )
        {
            if( _map[ i ][ j ].expandCost != 1 )
            {
                _map[i][j].pixmapItem = "D"; //setPixmap(dirtNodeImgPath);
            }
            else
            {
                _map[i][j].pixmapItem = "o"; //setPixmap(freeNodeImgPath);
            }
        }
    }

    _pathOnScreen = false;
}

void GameMap::destroyMap()
{
    if( _map != NULL )
    {
        for( uint i = 0; i < _mapWidth; i++ )
        {
            delete [] _map[ i ];

        }

        delete [] _map;

        _pathOnScreen = false;
        _astar.destroyMatrix();
        _path.clear();
        _map = NULL;
    }
}

void GameMap::setMap( uint mapWidth, uint mapHeight )
{
    destroyMap();

    _mapWidth = mapWidth;
    _mapHeight = mapHeight;

    if( _mapWidth == 0 || _mapHeight == 0 )
    {
        return;
    }

    _map = new AStar::MapNode< QString >*[ _mapWidth ];

    for( uint i = 0; i < _mapWidth; i++ )
    {
        _map[ i ] = new AStar::MapNode< QString >[ _mapHeight ];
    }

    for( uint i = 0; i < _mapWidth; i++ )
    for( uint j = 0; j < _mapHeight; j++ )
    {
        _map[ i ][ j ].type = AStar::NodeType::WAYNODE;
        _map[ i ][ j ].expandCost = 1;
        _map[i][j].pixmapItem = "o"; //.setPixmap(QPixmap(freeNodeImgPath));
    }

    _astar.setMatrix( _mapWidth, _mapHeight );
    _astar.setEnd( _mapWidth - 1, _mapHeight - 1 );
    reDrawStartEndNodes();
}

void GameMap::drawPath()
{
    if( _path.size() == 0 )
    {
        qWarning() << "Error: No path found...";
        return;
    }

    for( auto pathIt = _path.begin(); pathIt != _path.end(); ++pathIt )
    {
        _map[pathIt->first][pathIt->second].pixmapItem = "X"; //setPixmap(pathNodeImgPath);
    }

    _pathOnScreen = true;

    reDrawStartEndNodes();
}

void GameMap::reDrawStartEndNodes()
{
   QPair< uint, uint > start = _astar.getStart();
   QPair< uint, uint > end = _astar.getEnd();

    _map[start.first][start.second].pixmapItem = "s";//.setPixmap(startNodeImgPath);
    _map[end.first][end.second].pixmapItem = "e"; //.setPixmap(endNodeImgPath);
}

void GameMap::obtainPath( bool saveChanges )
{
    _path = _astar.getPath( saveChanges );
}

void GameMap::showPath()
{
    freeNodes();
    obtainPath( true );
    drawPath();
}

void GameMap::setHeuristics( AStar::AStarHeuristics heuristic )
{
    _astar.setHeuristics( heuristic );
}

void GameMap::setScale( uint scale )
{
    _itemScale = scale;
}

void GameMap::setStart( uint x, uint y )
{
    _astar.setStart( x, y );
}

void GameMap::setEnd( uint x, uint y )
{
    _astar.setEnd( x, y );
}

void GameMap::setToSetState( AStar::ToSetState toSetState )
{
    _toSetState = toSetState;
}

void GameMap::setElements()
{
    /*for( uint i = 0; i < _mapWidth; i++ )
    for( uint j = 0; j < _mapHeight; j++ )
    {
        if( ( i < 108 && j < 37 ) || ( i > 192 && j < 37 ) )
        {
            _map[ i ][ j ].type = AStar::NodeType::WALLNODE;
            _map[ i ][ j ].expandCost = 1;
            _map[i][j].pixmapItem = "W";
        }
    }
    */
}

void GameMap::addElement( uint x, uint y, uint size )
{
    for( uint i = x - (size / 2); i < x + (size / 2); i++ )
    {
        for( uint j = y - (size / 2); j < y + (size / 2); j++ )
        {
            _map[ i ][ j ].type = AStar::NodeType::WALLNODE;
            _map[ i ][ j ].expandCost = 1;
            _map[ i ][ j ].pixmapItem = "W";
        }

        std::cout << std::endl;
    }
}

void GameMap::dumpMap()
{
    for( uint i = 0; i < _mapWidth; i++ )
    {
        for( uint j = 0; j < _mapHeight; j++ )
        {
            std::cout << _map[i][j].pixmapItem.toStdString();
        }

        std::cout << std::endl;
    }
}
