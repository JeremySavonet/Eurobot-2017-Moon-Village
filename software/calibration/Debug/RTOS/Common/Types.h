///////////////////////////////////////////////////////////////////////////////////////////////////
/// \file		Types.h
/// \brief		Définitions des types de variables standards
/// 
/// \author		Etienne BOILLEY
/// \date		19/04/2015
///	\copyright	IOCENE
///////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef TYPES_H
#define TYPES_H
#ifdef __cplusplus
extern "C" {
#endif



#include <Configuration.h>



#if defined __dsPIC33F__ || defined __dsPIC33E__
	#include <Pic/Types.pic.h>
#elif defined __STM32F7__
	#include <Stm/Types.stm.h>
#elif defined __QT__
	#include <Rpi/Types.rpi.h>
#else
	#error Processor not supported
#endif



//General
	#define b32(s,t,u,v,w,x,y,z)	0b##s##t##u##v##w##x##y##z
	#define b24(u,v,w,x,y,z)		0b##u##v##w##x##y##z
	#define b16(w,x,y,z)			0b##w##x##y##z
	#define b8(y,z)					0b##y##z
	#ifndef NULL
		#define NULL		((void*)0)				///< Définition d'un pointeur sur rien
	#endif

	typedef u64				Time;					///< Temps en µs depuis le démarrage du PIC

#if defined(RTOS_SIZE_32BITS) || defined(__QT__)
	typedef u32					Size;			///< Index pour les fifo, buffers...
#else
	typedef u16					Size;			///< Index pour les fifo, buffers...
#endif


//Fonctions
	typedef int (*Func)(void);						///< Fonction sans argument
	typedef int (*Func1)(void*);					///< Fonction avec 1 argument
	typedef int (*Func2)(void*,void*);				///< Fonction avec 2 arguments
	typedef int (*Func3)(void*,void*,void*);		///< Fonction avec 3 arguments

	/// Valeurs de retour des fonctions
	typedef enum
	{
		ISUCCESS	= 0,							///< La fonction s'est correctement déroulée
		IPROGRESS	= 1,							///< La commande s'est partiellement déroulée, elle continuera au prochain appel
		IERROR		= -1,							///< Erreur lors de l'exécution
		IBUSY		= -2,							///< La fonction traite une commande précédente, attendre et réessayer
	}IStatus;

	#ifndef ignoreArg
		#define ignoreArg(x)	(void)(x)			///< Enlève le warning unused
	#endif



//Bits
	/// Pointeur permettant l'accés à un bit
	typedef struct
	{
		volatile unsigned int	*p;					///< Pointeur sur le registre
		u8						b;					///< Numéro du bit
	}Bit;

	#ifndef __DOXYGEN_IGNORE__
		#ifndef setBit
			#define setBit(x,value) { if (value) *(x.p) |= (0x0001 << x.b); else *(x.p) &= ~(0x0001 << x.b); }
		#endif
		#ifndef toggleBit
			#define toggleBit(x)	(*((x).p) ^= (0x0001 << x.b))
		#endif
		#ifndef getBit
			#define getBit(x)		((*((x).p) >> x.b)&0x01)
		#endif
		#define bitInit(x,y)		(Bit){ .p=(u32*)(x), .b=y}

	#else//__DOXYGEN_IGNORE__
		void setBit( Bit bit, u8 value);				///< Affectation du bit
		void getBit( Bit bit);							///< Lecture du bit
		Bit bitInit( unsigned int *pointeur, u8 bit);	///< Initialisation du bit

	#endif//__DOXYGEN_IGNORE__



//Network
	/// Contient une adresse IP
	typedef union
	{
		u32		U32;
		u8		U8[4];
	}Ip;

	/// Contient une adresse MAC
	typedef union
	{
		u64		U64;
		u8		U8[8];
		struct {
			u8	value[6];
			u16	dummy;
		};
	}Mac;
	#define macInit(a0,a1,a2,a3,a4,a5)	((Mac){.U8={0x##a5,0x##a4,0x##a3,0x##a2,0x##a1,0x##a0,0,0}})
	#define macCopy(dst,src)			dst.U64 = src.U64
	#define macFromU8(dst,srcU8)		{dst.U8[0]=*(srcU8+5); dst.U8[1]=*(srcU8+4); dst.U8[2]=*(srcU8+3); dst.U8[3]=*(srcU8+2); dst.U8[4]=*(srcU8+1); dst.U8[5]=*(srcU8+0); dst.U8[6]=0; dst.U8[7]=0;}
	#define macEqual(mac1,mac2)			((mac1).U64 == (mac2).U64)



// RF
	typedef enum {
		I = 0,
		Q = 1,
		RF_CHANNEL_NB
	}RfChannel;



// Struct
	typedef UnionPacked
	{
		u8	U8;
		s8	S8;
		StructPacked
		{
			unsigned char	B0 :1;
			unsigned char	B1 :1;
			unsigned char	B2 :1;
			unsigned char	B3 :1;
			unsigned char	B4 :1;
			unsigned char	B5 :1;
			unsigned char	B6 :1;
			unsigned char	B7 :1;
		};
	}Struct8;

	typedef UnionPacked
	{
		u16	U16;
		s16	S16;
		u8	U8[2];
		s8	S8[2];
		StructPacked
		{
			u8	U0;
			u8	U1;
		};
		StructPacked
		{
			s8	S0;
			s8	S1;
		};
		StructPacked
		{
			unsigned char	B0 :1;
			unsigned char	B1 :1;
			unsigned char	B2 :1;
			unsigned char	B3 :1;
			unsigned char	B4 :1;
			unsigned char	B5 :1;
			unsigned char	B6 :1;
			unsigned char	B7 :1;
			unsigned char	B8 :1;
			unsigned char	B9 :1;
			unsigned char	B10:1;
			unsigned char	B11:1;
			unsigned char	B12:1;
			unsigned char	B13:1;
			unsigned char	B14:1;
			unsigned char	B15:1;
		};
	}Struct16;

	typedef UnionPacked
	{
		u8	U8[3];
		s8	S8[3];
		StructPacked
		{
			u8	U0;
			u8	U1;
			u8	U3;
		};
		StructPacked
		{
			s8	S0;
			s8	S1;
			s8	S3;
		};
		StructPacked
		{
			unsigned char	B0 :1;
			unsigned char	B1 :1;
			unsigned char	B2 :1;
			unsigned char	B3 :1;
			unsigned char	B4 :1;
			unsigned char	B5 :1;
			unsigned char	B6 :1;
			unsigned char	B7 :1;
			unsigned char	B8 :1;
			unsigned char	B9 :1;
			unsigned char	B10:1;
			unsigned char	B11:1;
			unsigned char	B12:1;
			unsigned char	B13:1;
			unsigned char	B14:1;
			unsigned char	B15:1;
			unsigned char	B16:1;
			unsigned char	B17:1;
			unsigned char	B18:1;
			unsigned char	B19:1;
			unsigned char	B20:1;
			unsigned char	B21:1;
			unsigned char	B22:1;
			unsigned char	B23:1;
		};
	}Struct24;

	typedef UnionPacked
	{
		u32	U32;
		s32	S32;
		u16	U16[2];
		s16	S16[2];
		u8	U8[4];
		s8	S8[4];
		StructPacked
		{
			u8	U0;
			u8	U1;
			u8	U2;
			u8	U3;
		};
		StructPacked
		{
			s8	S0;
			s8	S1;
			s8	S2;
			s8	S3;
		};
		StructPacked
		{
			unsigned char	B0 :1;
			unsigned char	B1 :1;
			unsigned char	B2 :1;
			unsigned char	B3 :1;
			unsigned char	B4 :1;
			unsigned char	B5 :1;
			unsigned char	B6 :1;
			unsigned char	B7 :1;
			unsigned char	B8 :1;
			unsigned char	B9 :1;
			unsigned char	B10:1;
			unsigned char	B11:1;
			unsigned char	B12:1;
			unsigned char	B13:1;
			unsigned char	B14:1;
			unsigned char	B15:1;
			unsigned char	B16:1;
			unsigned char	B17:1;
			unsigned char	B18:1;
			unsigned char	B19:1;
			unsigned char	B20:1;
			unsigned char	B21:1;
			unsigned char	B22:1;
			unsigned char	B23:1;
			unsigned char	B24:1;
			unsigned char	B25:1;
			unsigned char	B26:1;
			unsigned char	B27:1;
			unsigned char	B28:1;
			unsigned char	B29:1;
			unsigned char	B30:1;
			unsigned char	B31:1;
		};
	}Struct32;

	typedef UnionPacked
	{
		f64	F64;
		u64	U64;
		s64	S64;
		f32	F32[2];
		u32	U32[2];
		s32	S32[2];
		u16	U16[4];
		s16	S16[4];
		u8	U8[8];
		s8	S8[8];
		StructPacked
		{
			u8	U0;
			u8	U1;
			u8	U2;
			u8	U3;
			u8	U4;
			u8	U5;
			u8	U6;
			u8	U7;
		};
		StructPacked
		{
			s8	S0;
			s8	S1;
			s8	S2;
			s8	S3;
			s8	S4;
			s8	S5;
			s8	S6;
			s8	S7;
		};
		StructPacked
		{
			unsigned char	B0 :1;
			unsigned char	B1 :1;
			unsigned char	B2 :1;
			unsigned char	B3 :1;
			unsigned char	B4 :1;
			unsigned char	B5 :1;
			unsigned char	B6 :1;
			unsigned char	B7 :1;
			unsigned char	B8 :1;
			unsigned char	B9 :1;
			unsigned char	B10:1;
			unsigned char	B11:1;
			unsigned char	B12:1;
			unsigned char	B13:1;
			unsigned char	B14:1;
			unsigned char	B15:1;
			unsigned char	B16:1;
			unsigned char	B17:1;
			unsigned char	B18:1;
			unsigned char	B19:1;
			unsigned char	B20:1;
			unsigned char	B21:1;
			unsigned char	B22:1;
			unsigned char	B23:1;
			unsigned char	B24:1;
			unsigned char	B25:1;
			unsigned char	B26:1;
			unsigned char	B27:1;
			unsigned char	B28:1;
			unsigned char	B29:1;
			unsigned char	B30:1;
			unsigned char	B31:1;
			unsigned char	B32:1;
			unsigned char	B33:1;
			unsigned char	B34:1;
			unsigned char	B35:1;
			unsigned char	B36:1;
			unsigned char	B37:1;
			unsigned char	B38:1;
			unsigned char	B39:1;
			unsigned char	B40:1;
			unsigned char	B41:1;
			unsigned char	B42:1;
			unsigned char	B43:1;
			unsigned char	B44:1;
			unsigned char	B45:1;
			unsigned char	B46:1;
			unsigned char	B47:1;
			unsigned char	B48:1;
			unsigned char	B49:1;
			unsigned char	B50:1;
			unsigned char	B51:1;
			unsigned char	B52:1;
			unsigned char	B53:1;
			unsigned char	B54:1;
			unsigned char	B55:1;
			unsigned char	B56:1;
			unsigned char	B57:1;
			unsigned char	B58:1;
			unsigned char	B59:1;
			unsigned char	B60:1;
			unsigned char	B61:1;
			unsigned char	B62:1;
			unsigned char	B63:1;
		};
	}Struct64;


#ifdef __cplusplus
}//extern C
#endif
#endif//TYPES_H

