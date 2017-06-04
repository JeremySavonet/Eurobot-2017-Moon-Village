///////////////////////////////////////////////////////////////////////////////////////////////////
/// \file		Rtos.rpi.cpp
/// \brief		Démarrage de la Raspberry et RTOS
///
/// \author		Etienne BOILLEY
/// \date		17/04/2015
/// \copyright	IOCENE
///////////////////////////////////////////////////////////////////////////////////////////////////




#include <QProcess>
#include <QThread>

#ifdef __RASPBERRY__
    //#include <wiringPi.h>
	#include <sys/types.h>
	#include <sys/syscall.h>
	#include <unistd.h>
#endif

#include <Rtos.h>




int rtosInit()
{
	rtosTimeInit();

	consoleInit();
	debugInit(&rtosConsole.f,consoleFlush,NULL);

	#ifdef rtosDebugWelcome
		rtosDebugWelcome();
	#endif
	debugFlush();

	return ISUCCESS;
}//rtosInit



int rtosStart()
{
	return ISUCCESS;
}//rtosStart



int rtosExit()
{
	return ISUCCESS;
}//rtosStart


int rtosDebugWelcome()
{
	rtosSuccess( "\r\nRaspberry pi 2 - %s v%u.%u build " __DATE__ " @ " __TIME__ "\r\n", PROJECT_NAME, PROJECT_VERSION, PROJECT_REVISION);
	return ISUCCESS;
}//rtosDebugWelcome


/*
#ifdef __RASPBERRY__

	static int rtosProcessorAffinity = -1;
	static int rtosIsolatedProcessors = 0;
	int rtosProcessorAffinityInit( int numberOfProcessorsToIsoalte)
	{
		rtosIsolatedProcessors = numberOfProcessorsToIsoalte;
		if(rtosIsolatedProcessors<=0 || QThread::idealThreadCount()-rtosIsolatedProcessors<1) {
			rtosError("RTOS\tprocessor affinity init error %d\r\n", rtosIsolatedProcessors);
			return IERROR;
		}

		int i=QThread::idealThreadCount()-1;
		QString processorString;
		processorString += QString("%1").arg(i--);
		while(i>rtosIsolatedProcessors-1)
			processorString += QString(",%1").arg(i--);

		QProcess p;
		p.start("ps aux");
		p.waitForStarted(100);
		p.waitForFinished(2000);
		QString str = p.readAllStandardOutput();
		QStringList list = str.split('\n');
		for(int i=0;i<list.length();i++) {
			QStringList line = list[i].split(QRegExp("\\s+"));
			if(line.length()>2) {
				QString value = line[1];
				bool ok = 0;
				int pid = value.toInt(&ok);
				if(ok) {
					QProcess p;
					//QString cmd = QString("sudo taskset -cp 3 %2").arg(pid);
					QString cmd = QString("sudo taskset -cp %1 %2").arg(processorString).arg(pid);
					//QString cmd = QString("sudo taskset -cp %1 %2").arg(QThread::idealThreadCount()-1).arg(pid);
					p.start(cmd);
					p.waitForStarted(100);
					p.waitForFinished(1000);
				}
			}
		}

		rtosProcessorAffinity = 0;
		return ISUCCESS;
	}//rtosProcessorAffinityInit

	int rtosProcessorAffinityIsInitialized()
	{
		if(rtosProcessorAffinity==-1) {
			return IERROR;
		}
		return ISUCCESS;
	}//rtosProcessorAffinityIsInitialized

	int rtosProcessorAffinitySet( int processor)
	{
		if(processor>=QThread::idealThreadCount()) {
			rtosError("RTOS\tset affinity error %d\r\n", processor);
			return IERROR;
		}
		QProcess p;
		QString cmd = QString("sudo taskset -cp %1 %2").arg(processor).arg((u32)syscall(__NR_gettid));
		p.start(cmd);
		p.waitForStarted(100);
		p.waitForFinished(1000);
		rtosWarning("RTOS\tset process affinity \"%s\"\r\n", cmd.toStdString().c_str());
		return ISUCCESS;
	}//rtosProcessorAffinitySet

	int rtosProcessorAffinityIsolate()
	{
		if(rtosProcessorAffinity==-1) {
			rtosError("RTOS\tprocessor affinity not initialized\r\n");
			return IERROR;
		}
		if(rtosProcessorAffinity>=rtosIsolatedProcessors) {
			rtosError("RTOS\tthread not isolated, not enough processors %d %d\r\n", rtosProcessorAffinity, rtosIsolatedProcessors);
			return IERROR;
		}
		return rtosProcessorAffinitySet(rtosProcessorAffinity++);
	}//rtosProcessorAffinityIsolate

	int rtosProcessorPriority( RtosPriority p)
	{
		piHiPri(p);
		rtosWarning("RTOS\tset thread priority %d: %d\r\n", (u32)syscall(__NR_gettid), p);
		return ISUCCESS;
	}//rtosProcessorPriority

#endif
*/


