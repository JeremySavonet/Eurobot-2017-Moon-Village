// Copyright (c) 2016-2017 All Rights Reserved WestBot

#include "FunnyAction.hpp"

using namespace WestBot;

// XXX: TODO
FunnyAction::FunnyAction()
    : Action( "FunnyAction" )
{
}

void FunnyAction::execute()
{
    emit complete();
}

void FunnyAction::skip()
{
}
