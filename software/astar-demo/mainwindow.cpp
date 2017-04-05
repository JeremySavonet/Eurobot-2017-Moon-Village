#include "mainwindow.h"
#include "ui_mainwindow.h"

using namespace WestBot::AStar;

MainWindow::MainWindow( QWidget *parent )
    : QMainWindow( parent )
    , ui( new Ui::MainWindow )
{
    ui->setupUi( this );

    mapScene = new MapScene( ui->viewport );
    ui->viewport->setScene( mapScene );

    connect(
        ui->btnBuildMap,
        SIGNAL( clicked() ),
        this,
        SLOT( buildMap() ) );

    connect(
        ui->spinMapWidth,
        SIGNAL( valueChanged( int ) ),
        this,
        SLOT( prepareToBuildMap() ) );

    connect(
        ui->spinMapHeight,
        SIGNAL( valueChanged( int ) ),
        this,
        SLOT( prepareToBuildMap() ) );

    connect(
        ui->btnShowPath,
        SIGNAL( clicked() ),
        this,
        SLOT( buildPath() ) );

    connect(ui->spinMapScale, SIGNAL(valueChanged(int)), this, SLOT(prepareToBuildMap()));
    connect(ui->btnWall, SIGNAL(clicked()), this, SLOT(toSetWall()));
    connect(ui->btnWay, SIGNAL(clicked()), this, SLOT(toSetWay()));
    connect(ui->btnDirt, SIGNAL(clicked()), this, SLOT(toSetDirt()));
    connect(ui->btnStart, SIGNAL(clicked()), this, SLOT(toSetStart()));
    connect(ui->btnEnd, SIGNAL(clicked()), this, SLOT(toSetEnd()));
    connect(ui->radioBtnEuclidean, SIGNAL(toggled(bool)), this, SLOT(toEuclidean()));
    connect(ui->radioBtnManhattan, SIGNAL(toggled(bool)), this, SLOT(toManhattan()));
}

MainWindow::~MainWindow()
{
    delete ui;
    delete mapScene;
}

void MainWindow::buildMap()
{
    uint width, height, scale;

    width = (uint)ui->spinMapWidth->value();
    height = (uint)ui->spinMapHeight->value();
    scale = (uint)ui->spinMapScale->value();

    mapScene->setScale(scale);
    mapScene->setMap(width, height);

    ui->btnBuildMap->setText("Rebuild Map");
    ui->btnShowPath->setEnabled(true);
}

void MainWindow::prepareToBuildMap()
{
    ui->btnBuildMap->setText("Build Map");
    ui->btnShowPath->setEnabled(false);
}

void MainWindow::buildPath()
{
    mapScene->showPath();
}

void MainWindow::toEuclidean()
{
    ui->radioBtnManhattan->setChecked(false);
    mapScene->setHeuristics( AStarHeuristics::euclidean );
}

void MainWindow::toManhattan()
{
    ui->radioBtnEuclidean->setChecked(false);
    mapScene->setHeuristics( AStarHeuristics::manhattan );
}

void MainWindow::toSetWall()
{
    mapScene->setToSetState( ToSetState::TOSETWALL );
}

void MainWindow::toSetWay()
{
    mapScene->setToSetState( ToSetState::TOSETWAY );
}

void MainWindow::toSetDirt()
{
    mapScene->setToSetState( ToSetState::TOSETDIRT );
}

void MainWindow::toSetStart()
{
    mapScene->setToSetState( ToSetState::TOSETSTART );
}

void MainWindow::toSetEnd()
{
    mapScene->setToSetState( ToSetState::TOSETEND );
}
