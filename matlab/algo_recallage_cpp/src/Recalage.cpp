#include "Recalage.hpp"

#include <iostream>
#include <math.h>
#include <QDebug>

#include  <3rdparty/Eigen/Dense>
using namespace Eigen;



Recalage::Recalage()
{

}

RobotPos Recalage::getPos( const RobotPos& odoPos)
{
	RobotPos pos;
	pos.x =  odoPos.x*cos(error.theta) +odoPos.y*sin(error.theta) +error.x;
	pos.y = -odoPos.x*sin(error.theta) +odoPos.y*cos(error.theta) +error.y;
	pos.theta = odoPos.theta -error.theta;
	return pos;
}//getPos

RobotPos Recalage::sendPos( const RobotPos& robotPos)
{
	RobotPos pos;
	pos.x =  (robotPos.x-error.x)*cos(error.theta) -(robotPos.y-error.y)*sin(error.theta);
	pos.y =  (robotPos.x-error.x)*sin(error.theta) +(robotPos.y-error.y)*cos(error.theta);
	pos.theta = robotPos.theta +error.theta;
	return pos;
}//sendPos

void Recalage::errorInit( double errX, double errY, double errTheta)
{
	error.x = errX;
	error.y = errY;
	error.theta = errTheta;
}//errorInit

void Recalage::errorModify( double errX, double errY, double errTheta)
{
	error.x =  error.x*cos(errTheta) +error.y*sin(errTheta) +errX;
	error.y = -error.x*sin(errTheta) +error.y*cos(errTheta) +errY;
	error.theta += errTheta;
}//errorModify


RobotPos Recalage::calibrate(
		int len,					///< nb de mesures télémètre
		const double* mesR,			///< mesure télémètre
		const double* mesTheta,		///< angle correspondant à la masure
		const RobotPos& robotPos	///< position du robot
		)
{
	double errX,errY,errTheta;//output

	{
		//position du télémètre
		double telemTheta = robotPos.theta + TELEM_THETA0;
		double telemX = robotPos.x + TELEM_R*cos(robotPos.theta+TELEM_THETA);
		double telemY = robotPos.y + TELEM_R*sin(robotPos.theta+TELEM_THETA);

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
			double theta = telemTheta + mesTheta[j];
			double x = telemX + mesR[j]*cos(theta);
			double y = telemY + mesR[j]*sin(theta);

			//associate dot to border
			double d = INFINITY;
			int K = 0;
			for(int k=0; k<tableBorderNb; k++) {
				double dist;
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
		{
			int lenUpdate = 0;
			for(int k=0; k<tableBorderNb; k++) {

				//qDebug() << "border" << k << mesure[k].len << "dir" << tableBorder[k].dir;

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
					double d = abs( (mesure[k].dot(j,X))*(u[0]) + (mesure[k].dot(j,Y)-droite(1,0))*(u[1]) );
					if(d<ALGO_BORDER_FILTER_DROP) {
						mesure[k].dot(len,0) = mesure[k].dot(j,0);
						mesure[k].dot(len,1) = mesure[k].dot(j,1);
						len++;
					}
				}
				mesure[k].len = len;
				lenUpdate += len;

				//qDebug() << "border end" << k << mesure[k].len << "dir" << tableBorder[k].dir;
			}
			len = lenUpdate;
		}

		//calcul erreur odometrie
		/*{
			MatrixXd in(len,4);
			VectorXd out(len);
			int i = 0;
			for(int k=0; k<tableBorderNb; k++) {
				if(mesure[k].len==0)
					continue;

				for(int j =0; j<mesure[k].len; j++) {
					if(tableBorder[k].dir) {
						in(i,0) = mesure[k].dot(j,1);
						in(i,1) = -mesure[k].dot(j,0);
						in(i,2) = 0;
						in(i,3) = 1;
						out(i) = tableBorder[k].ay;
					}
					else {
						in(i,0) = mesure[k].dot(j,0);
						in(i,1) = mesure[k].dot(j,1);
						in(i,2) = 1;
						in(i,3) = 0;
						out(i) = tableBorder[k].ax;
					}
					i++;
				}
			}

			MatrixXd A = in.jacobiSvd(ComputeThinU|ComputeThinV).solve(out);
			errX = A(2,0);
			errY = A(3,0);
			MatrixXd R(2,2);
			R(0,0) = A(0,0);
			R(0,1) = A(1,0);
			R(1,0) = -A(1,0);
			R(1,1) = A(0,0);
			MatrixXd C;
			C = R.transpose()*R;
			LLT<MatrixXd> lltOfA(C);
			C = lltOfA.matrixL();
			R = R*C.inverse();
			errTheta = asin(R(0,1));
		}*/
		{
			MatrixXd in(len,3);
			VectorXd out(len);
			int i = 0;
			for(int k=0; k<tableBorderNb; k++) {
				if(mesure[k].len==0)
					continue;

				for(int j =0; j<mesure[k].len; j++) {
					if(tableBorder[k].dir) {
						in(i,0) = -mesure[k].dot(j,0);
						in(i,1) = 0;
						in(i,2) = 1;
						out(i) = tableBorder[k].ay - mesure[k].dot(j,1);
					}
					else {
						in(i,0) = mesure[k].dot(j,1);
						in(i,1) = 1;
						in(i,2) = 0;
						out(i) = tableBorder[k].ax - mesure[k].dot(j,0);
					}
					i++;
				}
			}

			MatrixXd A = in.jacobiSvd(ComputeThinU|ComputeThinV).solve(out);
			errX = A(1,0);
			errY = A(2,0);
//			MatrixXd R(2,2);
//			R(0,0) = A(0,0);
//			R(0,1) = A(1,0);
//			R(1,0) = -A(1,0);
//			R(1,1) = A(0,0);
//			MatrixXd C;
//			C = R.transpose()*R;
//			LLT<MatrixXd> lltOfA(C);
//			C = lltOfA.matrixL();
//			R = R*C.inverse();
//			errTheta = asin(R(0,1));
			errTheta = asin(A(0,0));
		}

	}

	errorModify(errX,errY,errTheta);

	qDebug() << "calibrate error" << errX << errY << errTheta;

	RobotPos robotPosNew;
	robotPosNew.theta = robotPos.theta - errTheta;
	robotPosNew.x = robotPos.x + errX;
	robotPosNew.y = robotPos.y + errY;
	return robotPosNew;
}//calibrate
