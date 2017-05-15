///////////////////////////////////////////////////////////////////////////////////////////////////
/// \file		TypesTest.c
/// \brief		Test des types de variables standards
///
/// \author		Etienne BOILLEY
/// \date		16/04/2015
///	\copyright	IOCENE
///////////////////////////////////////////////////////////////////////////////////////////////////


#include <Rtos.h>




#define RTOS_TYPE_TEST(CONDITION,MSG)		typedef u8 RTOS_ERROR_##MSG[1-2*(CONDITION)]
#define RTOS_TYPE_TEST_SIZE(TYPE,SIZE)		RTOS_TYPE_TEST(sizeof(TYPE)!=SIZE,SIZE_##TYPE)
#define RTOS_TYPE_TEST_UNSIGNED(TYPE)		RTOS_TYPE_TEST(!((TYPE)0<(TYPE)-1),SIGN_##TYPE)
#define RTOS_TYPE_TEST_SIGNED(TYPE)			RTOS_TYPE_TEST(!((TYPE)0>(TYPE)-1),SIGN_##TYPE)



RTOS_TYPE_TEST_SIZE(u8,1);
RTOS_TYPE_TEST_SIZE(u16,2);
RTOS_TYPE_TEST_SIZE(u32,4);
RTOS_TYPE_TEST_SIZE(u64,8);

RTOS_TYPE_TEST_SIZE(s8,1);
RTOS_TYPE_TEST_SIZE(s16,2);
RTOS_TYPE_TEST_SIZE(s32,4);
RTOS_TYPE_TEST_SIZE(s64,8);

RTOS_TYPE_TEST_SIZE(f32,4);
RTOS_TYPE_TEST_SIZE(f64,8);

RTOS_TYPE_TEST_UNSIGNED(u8);
RTOS_TYPE_TEST_UNSIGNED(u16);
RTOS_TYPE_TEST_UNSIGNED(u32);
#ifndef RTOS_UINT64_BUG
RTOS_TYPE_TEST_UNSIGNED(u64);
#endif

RTOS_TYPE_TEST_SIGNED(s8);
RTOS_TYPE_TEST_SIGNED(s16);
RTOS_TYPE_TEST_SIGNED(s32);
#ifndef RTOS_UINT64_BUG
RTOS_TYPE_TEST_SIGNED(s64);
#endif

//#ifdef __WINDOWS32__
//__pragma(pack(push, 1))
//#endif
typedef StructPacked
{
	u8	u8;
	u64	u64;
}StructPackedAssert;
//#ifdef __WINDOWS32__
//__pragma(pack(pop))
//#endif

RTOS_TYPE_TEST_SIZE(StructPackedAssert,1+8);


RTOS_TYPE_TEST( ISUCCESS!=0, SUCCESS);
RTOS_TYPE_TEST( IPROGRESS!=1, PROGRESS);
RTOS_TYPE_TEST( IERROR!=-1, ERROR);
RTOS_TYPE_TEST( IBUSY!=-2, BUSY);




