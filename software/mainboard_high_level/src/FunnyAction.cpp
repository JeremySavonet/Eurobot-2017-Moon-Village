// Copyright (c) 2017 All Rights Reserved WestBot

// Author: B. Vincent
// Author: J.Savonet

#include <QDebug>
#include <QProcess>
#include <QStringList>

#include <WestBot/FunnyAction.hpp>

using namespace WestBot;

FunnyAction::FunnyAction()
    : Action()
{
}

void FunnyAction::execute()
{
    qDebug() << "Execute funny action";

    /*
     # -> premiere commande à faire pour que ça colle à l'adresse MAC de la carte WIFI (à voir si c'est utile)

    tcprewrite -i ~/Downloads/drone_high_down.pcap -o ~/Downloads/drone_high_down2.pcap --enet-smac=9c:4e:36:a5:53:70 --enet-dmac=e8:ab:fa:8c:4a:db

    # -> ici on replay sur l'interface "wlp3s0"

    tcpreplay -i wlp3s0 ~/Downloads/drone_high_down2.pcap

    Ca marche niquel en tout cas !! faudra juste qu'on vérifie que le drone a bien de la batterie avant le match
    */

    QProcess* exec;
    exec = new QProcess( this );

    connect(
        exec,
        SIGNAL( finished( int ) ),
        this,
        SLOT( doFunny( int ) ) );

    exec->start( "/bin/sh", QStringList() << "./mapMacAddress.sh" );

    emit complete();
}


void FunnyAction::doFunny( int errorCode )
{

    if( 0 == errorCode )
    {
        QProcess* funny;
        funny = new QProcess( this );
        funny->start( "/bin/sh", QStringList() << "replayFunnyAction.sh" );
    }
}
