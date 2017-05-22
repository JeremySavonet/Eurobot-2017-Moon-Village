// Copyright (c) 2017 All Rights Reserved WestBot

#include <WestBot/TurbineAction.hpp>

using namespace WestBot;

TurbineAction::TurbineAction( Turbine& turbine, Type type )
    : Action()
    , _turbine( turbine )
    , _type( type )
{
}

void TurbineAction::execute()
{
    switch( _type )
    {
    case Type::Expulse:
        _turbine.setValue( TURBINE_EXPULSE_VALUE );
        break;

    case Type::Insuffle:
        _turbine.setValue( TURBINE_INSUFFLE_VALUE );
        break;
    }

    emit complete();
}
