// Copyright (c) 2017 All Rights Reserved WestBot

#include <iostream>
#include <QCoreApplication>
#include <QDebug>
#include <QFile>
#include <math.h>

// 3rdParty library

#include  <3rdparty/Eigen/Dense>
using namespace Eigen;



typedef double fx;
namespace
{
	//const double ODO_ENTRAXE = 0.188;
	//const double ODO_RESOLUTION = 0.032*M_PI/1024;

	const fx TELEM_THETA0 = -0.118;
	const fx TELEM_X = 206.08;
	const fx TELEM_Y = 101.1;
	const fx TELEM_THETA = atan2(TELEM_Y,TELEM_X);
	const fx TELEM_R = sqrt(TELEM_X*TELEM_X+TELEM_Y*TELEM_Y);

	const fx ALGO_TELEM_DROP_MESURE = 100;
	const fx ALGO_BORDER_ASSOCIATE_DROP = 150;
	const fx ALGO_BORDER_FILTER_DROP = 7;
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

	QFile f("../test3.raw");
	if(!f.open(QIODevice::ReadOnly)) {
		std::cout << "error open";
		return EXIT_FAILURE;
	}

	const int size = f.bytesAvailable();
	const int len = size/sizeof(double)/2;
	double data[2][len];
	f.read((char*)(&data[0][0]),size);

	/*MatrixXd m(len,2);
	for(int i =0; i<len; i++) {
		m(i,0) = data[0][i];
		m(i,1) = data[1][i];
	}*/


// fontion traitement
	std::cout << "start" << std::endl;

	//input telem
	fx mesR[len];
	fx mesTheta[len];
	for(int j =0; j<len; j++) {
		mesR[j] = data[0][j];			// mesure radius
		mesTheta[j] = data[1][j];		// mesure theta
	}
	//const int len						// nb de mesures
	if(sizeof(mesR[0])!=sizeof(data[0][0]))
		qDebug() << "error size";

	//input odo
	fx robotTheta = 0;
	fx robotX = 35;
	fx robotY = 0;

	//input table
	typedef struct {
		bool dir;
		fx	ax;
		fx	ay;
		fx	bx;
		fx	by;
	}Border;
	Border tableBorder[] = {
		{ 0,      0, -1500,      0,      1500 },
		{ 1,      0,  1500,   2000,      1500 },
		{ 0,   2000,  1500,   2000,     -1500 },
		{ 1,   2000, -1500,      0,     -1500 },
		{ 0, 360+22, -1500, 360+22, -1500+710 },
		{ 0, 360+22,  1500, 360+22,  1500-710 },
	};
	int tableBorderNb = sizeof(tableBorder)/sizeof(tableBorder[0]);


	{
		qDebug() << "robot" << robotTheta << robotX << robotY;

		//position du télémètre
		fx telemTheta = robotTheta + TELEM_THETA0;
		fx telemX = robotX + TELEM_R*cos(robotTheta+TELEM_THETA);
		fx telemY = robotY + TELEM_R*sin(robotTheta+TELEM_THETA);

		qDebug() << "telem" << telemTheta << telemX << telemY;

		//initialisation tableaux de point associés à une bordure
		typedef struct {
			MatrixXd dot;
			int len;
		}Mesure;
		Mesure mesure[tableBorderNb];
		for(int k=0; k<tableBorderNb; k++) {
			mesure[k].dot.resize(len,2);
			mesure[k].len = 0;
		}

		//association point bordure
		for(int j =0; j<len; j++) {
			//drop false points
			if(mesR[j]<ALGO_TELEM_DROP_MESURE)
				continue;

			//position du point mesuré
			fx theta = telemTheta + mesTheta[j];
			fx x = telemX + mesR[j]*cos(theta);
			fx y = telemY + mesR[j]*sin(theta);

			//associate dot to border
			fx d = INFINITY;
			int K = 0;
			for(int k=0; k<tableBorderNb; k++) {
				fx dist;
				//AB.AP<0 => distance = AP
				if( (tableBorder[k].bx-tableBorder[k].ax)*(x-tableBorder[k].ax) + (tableBorder[k].by-tableBorder[k].ay)*(y-tableBorder[k].ay) < 0 )
					dist = sqrt( (x-tableBorder[k].ax)*(x-tableBorder[k].ax) + (y-tableBorder[k].ay)*(y-tableBorder[k].ay) );
				//BA.BP<0 => distance = BP
				else if( (tableBorder[k].ax-tableBorder[k].bx)*(x-tableBorder[k].bx) + (tableBorder[k].ay-tableBorder[k].by)*(y-tableBorder[k].by) < 0 )
					dist = sqrt( (x-tableBorder[k].bx)*(x-tableBorder[k].bx) + (y-tableBorder[k].by)*(y-tableBorder[k].by) );
				//sinon => distance = norm(AB^AP)/norm(AB)
				else
					dist = abs( (tableBorder[k].bx-tableBorder[k].ax)*(y-tableBorder[k].ay) + (tableBorder[k].by-tableBorder[k].ay)*(x-tableBorder[k].ax) )/( sqrt((tableBorder[k].ax-tableBorder[k].bx)*(tableBorder[k].ax-tableBorder[k].bx)+(tableBorder[k].ay-tableBorder[k].by)*(tableBorder[k].ay-tableBorder[k].by)) );

				if(dist<d) {
					d = dist;
					K = k;
				}
			}
			if(d<ALGO_BORDER_ASSOCIATE_DROP) {
				mesure[K].dot(mesure[K].len,0) = x;
				mesure[K].dot(mesure[K].len,1) = y;
				mesure[K].len++;
			}
		}

		//suppression points
		for(int k=0; k<tableBorderNb; k++) {

			qDebug() << "border" << k << mesure[k].len << "dir" << tableBorder[k].dir;

			if(mesure[k].len<5){
				mesure[k].len = 0;
				continue;
			}

			int X,Y;
			if(tableBorder[k].dir) {
				X = 0;
				Y = 1;
			}
			else {
				X = 1;
				Y = 0;
			}


			MatrixXd in(mesure[k].len,2);
			VectorXd out(mesure[k].len);
			for(int j =0; j<mesure[k].len; j++) {
				in(j,0) = mesure[k].dot(j,X);
				in(j,1) = 1;
				out(j) = mesure[k].dot(j,Y);
			}

			MatrixXd droite = in.jacobiSvd(ComputeThinU|ComputeThinV).solve(out);
			VectorXf u(2);
			u(0) = -droite(0,0);
			u(1) = 1;
			u = u/u.norm();

			int len = 0;
			for(int j =0; j<mesure[k].len; j++) {
				fx d = abs( (mesure[k].dot(j,X))*(u[0]) + (mesure[k].dot(j,Y)-droite(1,0))*(u[1]) );
				if(d<ALGO_BORDER_FILTER_DROP) {
					mesure[k].dot(len,0) = mesure[k].dot(j,0);
					mesure[k].dot(len,1) = mesure[k].dot(j,1);
					len++;
				}
			}
			mesure[k].len = len;

			qDebug() << "border end" << k << mesure[k].len << "dir" << tableBorder[k].dir;
		}

		//calcul erreur odometrie


	}


	qDebug() << "end compute";



    return a.exec();
}








