// Copyright (c) 2017 All Rights Reserved WestBot

#include <iostream>
#include <math.h>
#include <QCoreApplication>
#include <QDebug>
#include <QFile>
#include <QElapsedTimer>

#include "Recalage.hpp"

// 3rdParty library




int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

	{
		Recalage recalage;
		recalage.errorInit(35,540,3520.5512);

		RobotPos odoPos;
		odoPos.x = 3251;
		odoPos.y = 2346312;
		odoPos.theta = 1032103.545;

		odoPos = recalage.sendPos(recalage.getPos(odoPos));
		qDebug() << "robotpos " << odoPos.x << odoPos.y << odoPos.theta;

	}

	{
		QFile f("../test3.raw");
		//QFile f("test3.raw");
		if(!f.open(QIODevice::ReadOnly)) {
			std::cout << "error open";
			return EXIT_FAILURE;
		}

		const int size = f.bytesAvailable();
		int len = size/sizeof(double)/2;
		double data[2][len];
		f.read((char*)(&data[0][0]),size);
		double* mesR = data[0];
		double* mesTheta = data[1];
		f.close();

		RobotPos robotPos;
		robotPos.theta = 0;
		robotPos.x = 35;
		robotPos.y = 0;

		robotPos.theta += 0.005;
		robotPos.x += 15;
		robotPos.y += 45;

		std::cout << "start" << std::endl;
		QElapsedTimer rtosTime;
		rtosTime.start();
		rtosTime.restart();

		Recalage recalage;
		RobotPos robotPosNew = recalage.calibrate(len,mesR,mesTheta,robotPos);
		//qDebug() << robotPosNew.x << robotPosNew.y << robotPosNew.theta;
		qDebug() << "end compute " << rtosTime.nsecsElapsed()/1e9;

		robotPosNew = recalage.calibrate(len,mesR,mesTheta,robotPosNew);
		//qDebug() << robotPosNew.x << robotPosNew.y << robotPosNew.theta;
	}

	qDebug() << " ";
	qDebug() << " ";

	double startPosition[][3] = {
		{0, 0, 0},
		{100.03963, -0.21994, -0.00209},
		{1000.499633789063, 1001.026367187500, 0.792186796665},
		{800.557006835937, 1.106220602989, 0.035510759801},
		{1000.055541992188, -999.942504882813, -1.551474213600}
	};

	Recalage recalage;
	recalage.errorInit(35,0,0);

	for(int i=0; i<5; i++)
	{
		QFile f(QString("test3.%0.raw").arg(i));
		if(!f.open(QIODevice::ReadOnly)) {
			std::cout << "error open" << std::endl;
			return EXIT_FAILURE;
		}

		const int size = f.bytesAvailable();
		int len = size/sizeof(double)/2;
		double data[2][len];
		f.read((char*)(&data[0][0]),size);
		double* mesR = data[0];
		double* mesTheta = data[1];
		f.close();

		RobotPos odoPos;
		odoPos.x = startPosition[i][0];
		odoPos.y = startPosition[i][1];
		odoPos.theta = startPosition[i][2];

		std::cout << "start " << i << std::endl;
		qDebug() << "robotpos " << odoPos.x << odoPos.y << odoPos.theta;
		QElapsedTimer rtosTime;
		rtosTime.start();
		rtosTime.restart();

		recalage.calibrate(len,mesR,mesTheta, recalage.getPos(odoPos));
		qDebug() << "end compute " << rtosTime.nsecsElapsed()/1e9;
		qDebug() << " ";
	}

	return a.exec();
}








