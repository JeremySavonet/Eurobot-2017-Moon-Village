#ifndef RECALAGE_HPP
#define RECALAGE_HPP

#include <QObject>


typedef struct {
	double x;
	double y;
	double theta;
}RobotPos;



class Recalage
{

public:
	Recalage();
	void errorInit( double errX, double errY, double errTheta);
	void errorModify( double errX, double errY, double errTheta);
	RobotPos calibrate( int len, const double* mesR, const double* mesTheta, const RobotPos& robotPos);
	RobotPos getPos(const RobotPos& odoPos);
	RobotPos sendPos( const RobotPos& robotPos);

private:
	RobotPos error = {0,0,0};

	const double TELEM_THETA0 = -0.118;
	const double TELEM_X = 206.08;
	const double TELEM_Y = 101.1;
	const double TELEM_THETA = atan2(TELEM_Y,TELEM_X);
	const double TELEM_R = sqrt(TELEM_X*TELEM_X+TELEM_Y*TELEM_Y);

	const double ALGO_TELEM_DROP_MESURE = 100;
	const double ALGO_BORDER_ASSOCIATE_DROP = 150;
	const double ALGO_BORDER_FILTER_DROP = 10;

	struct {
		bool dir;
		double	ax;
		double	ay;
		double	bx;
		double	by;
	} tableBorder[6] = {
		{ 0,      0, -1500,      0,      1500 },
		{ 1,      0,  1500,   2000,      1500 },
		{ 0,   2000,  1500,   2000,     -1500 },
		{ 1,   2000, -1500,      0,     -1500 },
		{ 0, 360+22, -1500, 360+22, -1500+710 },
		{ 0, 360+22,  1500, 360+22,  1500-710 },
	};
	int tableBorderNb = sizeof(tableBorder)/sizeof(tableBorder[0]);
};

#endif // RECALAGE_HPP
