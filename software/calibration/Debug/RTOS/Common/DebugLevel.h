///////////////////////////////////////////////////////////////////////////////////////////////////
/// \file		DebugLevel.h
/// \brief		Suppression des messages de débug de niveaux inférieurs. A placer en haut d'un .c
/// 
/// \author		Etienne BOILLEY
/// \date		21/04/2015
///	\copyright	IOCENE
///////////////////////////////////////////////////////////////////////////////////////////////////


//#ifndef DEBUG_LEVEL_H
//#define DEBUG_LEVEL_H


#ifndef DEBUG_LEVEL_UNDEF_ALL

	#if ( defined(DEBUG_LEVEL) && ( MACRO_IS_EMPTY(DEBUG_LEVEL)) )
		#undef DEBUG_LEVEL
		#warning DEBUG_LEVEL is empty
	#endif
	#if ( defined(DEBUG_LEVEL) && (MACRO_IS_NOT_NUMERIC(DEBUG_LEVEL)) )
		#undef DEBUG_LEVEL
		#warning DEBUG_LEVEL not numeric
	#endif
	#if (DEBUG_LEVEL<0) || (DEBUG_LEVEL>5)
		#undef DEBUG_LEVEL
	#endif
	#ifndef DEBUG_LEVEL
		#define DEBUG_LEVEL	5
		#warning DEBUG_LEVEL error => level=5
	#endif
	#ifndef DEBUG_NAME
		#define DEBUG_NAME "???"
	#endif



    #define error(str,...)			rtosError(	DEBUG_NAME "\t" str "\r\n",##__VA_ARGS__)
	#define errornCR(str,...)		rtosError(	str,##__VA_ARGS__)


	#undef print1
	#undef info1
	#undef success1
	#undef warning1
	#undef error1
	#undef print1nCR
	#undef info1nCR
	#undef success1nCR
	#undef warning1nCR
	#undef error1nCR
	#if DEBUG_LEVEL >= 1
		#define print1(str,...)		rtosPrint(	DEBUG_NAME "\t" str "\r\n",##__VA_ARGS__)
		#define info1(str,...)		rtosInfo(	DEBUG_NAME "\t" str "\r\n",##__VA_ARGS__)
		#define success1(str,...)	rtosSuccess(DEBUG_NAME "\t" str "\r\n",##__VA_ARGS__)
		#define warning1(str,...)	rtosWarning(DEBUG_NAME "\t" str "\r\n",##__VA_ARGS__)
		#define error1(str,...)		rtosError(	DEBUG_NAME "\t" str "\r\n",##__VA_ARGS__)
		#define print1nCR(...)		rtosPrint(__VA_ARGS__)
		#define info1nCR(...)		rtosInfo(__VA_ARGS__)
		#define success1nCR(...)	rtosSuccess(__VA_ARGS__)
		#define warning1nCR(...)	rtosWarning(__VA_ARGS__)
		#define error1nCR(...)		rtosError(__VA_ARGS__)
	#else
	#define print1(...)
	#define info1(...)
	#define success1(...)
	#define warning1(...)
	#define error1(...)
	#define print1nCR(...)
	#define info1nCR(...)
	#define success1nCR(...)
	#define warning1nCR(...)
	#define error1nCR(...)
	#endif


	#undef print2
	#undef info2
	#undef success2
	#undef warning2
	#undef error2
	#undef print2nCR
	#undef info2nCR
	#undef success2nCR
	#undef warning2nCR
	#undef error2nCR
	#if DEBUG_LEVEL >= 2
		#define print2(str,...)		rtosPrint(	DEBUG_NAME "\t" str "\r\n",##__VA_ARGS__)
		#define info2(str,...)		rtosInfo(	DEBUG_NAME "\t" str "\r\n",##__VA_ARGS__)
		#define success2(str,...)	rtosSuccess(DEBUG_NAME "\t" str "\r\n",##__VA_ARGS__)
		#define warning2(str,...)	rtosWarning(DEBUG_NAME "\t" str "\r\n",##__VA_ARGS__)
		#define error2(str,...)		rtosError(	DEBUG_NAME "\t" str "\r\n",##__VA_ARGS__)
		#define print2nCR(...)		rtosPrint(__VA_ARGS__)
		#define info2nCR(...)		rtosInfo(__VA_ARGS__)
		#define success2nCR(...)	rtosSuccess(__VA_ARGS__)
		#define warning2nCR(...)	rtosWarning(__VA_ARGS__)
		#define error2nCR(...)		rtosError(__VA_ARGS__)
	#else
		#define print2(...)
		#define info2(...)
		#define success2(...)
		#define warning2(...)
		#define error2(...)
		#define print2nCR(...)
		#define info2nCR(...)
		#define success2nCR(...)
		#define warning2nCR(...)
		#define error2nCR(...)
	#endif


	#undef print3
	#undef info3
	#undef success3
	#undef warning3
	#undef error3
	#undef print3nCR
	#undef info3nCR
	#undef success3nCR
	#undef warning3nCR
	#undef error3nCR
	#if DEBUG_LEVEL >= 3
		#define print3(str,...)		rtosPrint(	DEBUG_NAME "\t" str "\r\n", ##__VA_ARGS__)
		#define info3(str,...)		rtosInfo(	DEBUG_NAME "\t" str "\r\n",##__VA_ARGS__)
		#define warning3(str,...)	rtosWarning(DEBUG_NAME "\t" str "\r\n",##__VA_ARGS__)
		#define print3nCR(...)		rtosPrint(__VA_ARGS__)
		#define info3nCR(...)		rtosInfo(__VA_ARGS__)
		#define warning3nCR(...)	rtosWarning(__VA_ARGS__)
	#else
		#define print3(...)
		#define info3(...)
		#define warning3(...)
		#define print3nCR(...)
		#define info3nCR(...)
		#define warning3nCR(...)
	#endif


	#undef print4
	#undef info4
	#undef success4
	#undef warning4
	#undef error4
	#undef print4nCR
	#undef info4nCR
	#undef success4nCR
	#undef warning4nCR
	#undef error4nCR
	#if DEBUG_LEVEL >= 4
		#define print4(str,...)		rtosPrint(	DEBUG_NAME "\t" str "\r\n",##__VA_ARGS__)
		#define info4(str,...)		rtosInfo(	DEBUG_NAME "\t" str "\r\n",##__VA_ARGS__)
		#define warning4(str,...)	rtosWarning(DEBUG_NAME "\t" str "\r\n",##__VA_ARGS__)
		#define print4nCR(...)		rtosPrint(__VA_ARGS__)
		#define info4nCR(...)		rtosInfo(__VA_ARGS__)
		#define warning4nCR(...)	rtosWarning(__VA_ARGS__)
	#else
		#define print4(...)
		#define info4(...)
		#define warning4(...)
		#define print4nCR(...)
		#define info4nCR(...)
		#define warning4nCR(...)
	#endif


	#undef print5
	#undef info5
	#undef success5
	#undef warning5
	#undef error5
	#undef print5nCR
	#undef info5nCR
	#undef success5nCR
	#undef warning5nCR
	#undef error5nCR
	#if DEBUG_LEVEL >= 5
		#define print5(str,...)		rtosPrint(	DEBUG_NAME "\t" str "\r\n",##__VA_ARGS__)
		#define info5(str,...)		rtosInfo(	DEBUG_NAME "\t" str "\r\n",##__VA_ARGS__)
		#define print5nCR(...)		rtosPrint(__VA_ARGS__)
		#define info5nCR(...)		rtosInfo(__VA_ARGS__)
	#else
		#define print5(...)
		#define info5(...)
		#define print5nCR(...)
		#define info5nCR(...)
	#endif



#else //DEBUG_LEVEL_UNDEF_ALL

	#undef DEBUG_LEVEL_UNDEF_ALL
	#undef DEBUG_LEVEL
	#undef DEBUG_NAME

	#undef error
	#undef errornCR

	#undef print1
	#undef info1
	#undef success1
	#undef warning1
	#undef error1
	#undef print1nCR
	#undef info1nCR
	#undef success1nCR
	#undef warning1nCR
	#undef error1nCR

	#undef print2
	#undef info2
	#undef success2
	#undef warning2
	#undef error2
	#undef print2nCR
	#undef info2nCR
	#undef success2nCR
	#undef warning2nCR
	#undef error2nCR

	#undef print3
	#undef info3
	#undef success3
	#undef warning3
	#undef error3
	#undef print3nCR
	#undef info3nCR
	#undef success3nCR
	#undef warning3nCR
	#undef error3nCR

	#undef print4
	#undef info4
	#undef success4
	#undef warning4
	#undef error4
	#undef print4nCR
	#undef info4nCR
	#undef success4nCR
	#undef warning4nCR
	#undef error4nCR

	#undef print5
	#undef info5
	#undef success5
	#undef warning5
	#undef error5
	#undef print5nCR
	#undef info5nCR
	#undef success5nCR
	#undef warning5nCR
	#undef error5nCR

#endif //DEBUG_LEVEL_UNDEF_ALL



//#endif//DEBUG_LEVEL_H
