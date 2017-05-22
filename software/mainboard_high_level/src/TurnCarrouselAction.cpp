// Copyright (c) 2017 All Rights Reserved WestBot

#include <WestBot/TurnCarrouselAction.hpp>

using namespace WestBot;

TurnCarrouselAction::TurnCarrouselAction(
    Carrousel& carrousel,
    Sens sens )
    : Action()
    , _carrousel( carrousel )
    ,  _sens( sens )
{
}

void TurnCarrouselAction::execute()
{
    switch( _sens )
    {
    case Sens::CW:
    {
        float pos = _carrousel.position();
        _carrousel.setPosition( pos - 1.0 );
    }
        break;

    case Sens::CCW:
    {
        float pos = _carrousel.position();
        _carrousel.setPosition( pos + 1.0 );
    }
        break;
    }

    emit complete();
}
