// Copyright (c) 2017 All Rights Reserved WestBot

#include <QDebug>

#include <WestBot/FunnyAction.hpp>

using namespace WestBot;

FunnyAction::FunnyAction()
    : Action()
{
}

void FunnyAction::execute()
{
    qDebug() << "Execute funny action";
    emit complete();
}
