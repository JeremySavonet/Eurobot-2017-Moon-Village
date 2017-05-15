///////////////////////////////////////////////////////////////////////////////////////////////////
/// \file		Utils.h
/// \brief		Fonctions basiques
///
/// \author		Brice VINCENT
/// \author		Etienne BOILLEY
/// \date		16/05/2008
///	\copyright	IOCENE
///////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef UTILS_H
#define UTILS_H
#ifdef __cplusplus
extern "C" {
#endif



#include <Configuration.h>



#if defined __dsPIC33F__ || defined __dsPIC33E__
	#include <Pic/Types.pic.h>
#else
	#error Processor not supported
#endif




#ifndef abs
	#define abs(x)	( ((x)>0)? x : -x )
#endif





/*

#warning do not use

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <Common\GenericTypeDefs.h>



//#include <p33Fxxxx.h>


#define M_2PI (2*M_PI)

#define DEG(x) ((x) * (180.0 / M_PI))
#define RAD(x) ((x) * (M_PI / 180.0))









/////////////////////////////////////////////////////////////////////
///////////////////// fonctions de maths ////////////////////////////
/////////////////////////////////////////////////////////////////////

s16 roundint(double x);

double mod2pi(double x);

double signe(double x);



// do a modulo 2.pi -> [-Pi,+Pi], knowing that 'a' is in [-3Pi,+3Pi]
f32 simple_modulo_2pi(f32 a);


// do a modulo 2.pi -> [-Pi,+Pi]
f32 modulo_2pi(f32 a);

u8 count_ones_u8(u8 x);
u8 count_ones_u16(u16 x);

/////////////////////////////////////////////////////////////////////
///////////////////// fonctions d?dages ///////////////////////////
/////////////////////////////////////////////////////////////////////


u32  checksum32(u8 * array,u32 count);
boolean checksum32_test(u8 * array,u16 count,u32 tocheck);

u16  checksum16(u8 * array,u16 count);
boolean checksum16_test(u8 * array,u16 count,u16 tocheck);

u8   checksum8(u8 * array,u16 count);
boolean checksum8_test(u8 * array,u16 count,u8 tocheck);



u16 decode16(u8 * tab);
void   code16(u16 value,u8 * tab);

u32 decode32(u8 * tab);
void   code32(u32 value,u8 * tab);

DWORD decoded32(u8 * tab);
void  coded32(DWORD value,u8 * tab);


/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
//////////////////////// outils strings /////////////////////////////
/////////////////////////////////////////////////////////////////////

s16 ftoa (double x, char *str, char  prec, char format);
char *u8toa (unsigned char value, char *pBuffer, unsigned char radix);
char *s8toa (char number, char *pBuffer, unsigned char radix);
char *u16toa (u16 value, char *pBuffer, unsigned char radix);
char *i16toa (s16 number, char *pBuffer, unsigned char radix);
char *u32toa (u32 value, char *pBuffer, unsigned char radix);
char *i32toa (s32 number, char *pBuffer, unsigned char radix);


boolean strequal(char *str1, char *str2);

boolean strequal_v1(char *str1, char *str2);

boolean strhex(char *s, unsigned char *val, unsigned char n);

boolean strhexbyte(char *s, char *val);

boolean strnbr(char *str, u32 *val);

void bytetohex(unsigned char *val,char *s);

void wordtohex(s16 *val,char *s);

boolean ishex(char c);





/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////



///////////////////////////// MEMSET /////////////////////////////
inline void fast_memset(void * p, int fill, size_t size);



#ifdef __cplusplus
    }
#endif



*/

#ifdef __cplusplus
}//extern C
#endif
#endif // UTILS_H

