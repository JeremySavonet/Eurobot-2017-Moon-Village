// Copyright (c) 2016-2017 All Rights Reserved WestBot

#include <WestBot/MoveAction.hpp>

using namespace WestBot;

MoveAction::MoveAction( MoveAction::Type type )
    : Action( "MoveAction" )
    , _type( type )
{
}

void MoveAction::execute()
{
    switch( _type )
    {
    case MoveAction::Type::TRAJ_STOP:
        break;
    
    case MoveAction::Type::TRAJ_HARDSTOP:
        break;
    
    case MoveAction::Type::TRAJ_D_REL:
        break;
    
    case MoveAction::Type::TRAJ_ONLY_D_REL:
        break;
    
    case MoveAction::Type::TRAJ_A_REL:
        break;
    
    case MoveAction::Type::TRAJ_A_ABS:
        break;
    
    case MoveAction::Type::TRAJ_ONLY_A_REL:
        break;
    
    case MoveAction::Type::TRAJ_ONLY_A_ABS:
        break;
    
    case MoveAction::Type::TRAJ_D_A_REL:
        break;
    
    case MoveAction::Type::TRAJ_TURNTO_XY:
        break;
    
    case MoveAction::Type::TRAJ_TURNTO_XY_BEHIND:
        break;
    
    case MoveAction::Type::TRAJ_GOTO_XY_ABS:
        break;
    
    case MoveAction::Type::TRAJ_GOTO_FORWARD_XY_ABS:
        break;
    
    case MoveAction::Type::TRAJ_GOTO_BACKWARD_XY_ABS:
        break;
    
    case MoveAction::Type::TRAJ_GOTO_D_A_REL:
        break;
    
    case MoveAction::Type::TRAJ_GOTO_XY_REL:
        break;
    }
    
    emit complete();
}

void MoveAction::skip()
{
    // CALL TRAJ_STOP ACTION
}
