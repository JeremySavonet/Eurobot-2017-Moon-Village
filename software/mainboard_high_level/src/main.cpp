
#include <QObject>
#include <QTimer>
#include <QThread>
#include <QDebug>
#include <QCoreApplication>

#include <WestBot/ExecuteMatch.hpp>

using namespace WestBot;


int main( int argc, char *argv[] )
{
	QCoreApplication app( argc, argv );
	ExecuteMatch match;

	qDebug() << "Thread run";
	match.start();

	while( !match.started ){
		QThread::msleep(100);
		match.displayLed();
	}

	QTimer t;
	t.setSingleShot(true);
	t.start( 90*1000 );

	while( 1 ){
		QThread::msleep(20);

		if( t.remainingTime()<=0 ) {
			qDebug() << "END OF MATCH";
			break;
		}

		if(match.isArretUrgence()) {
			qDebug() << "HARD STOP";
			break;
		}
	}

	match.terminate();
	match.hardStop();

	if(!match.isArretUrgence()) {
		match.funnyAction();
	}


	while(1)
		;

	return 0;
}




