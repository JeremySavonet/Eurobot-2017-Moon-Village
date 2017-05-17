///////////////////////////////////////////////////////////////////////////////////////////////////
/// \file		Types.rpi.h
/// \brief		Définitions des types de variables standards
/// 
/// \author		Brice VINCENT
///	\author		Etienne BOILLEY
/// \date		25/09/2008
///	\copyright	IOCENE
///////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef TYPE_RPI_H
#define TYPE_RPI_H
#ifdef __cplusplus
extern "C" {
#endif



	#define b16(w,x,y,z)		0b##w##x##y##z
	#define b8(y,z)				0b##y##z


	typedef unsigned char		u8;				///< 8 bits unsigned integer
	typedef unsigned short    	u16;			///< 16 bits unsigned integer
	typedef unsigned int		u32;			///< 32 bits unsigned integer
	typedef unsigned long long	u64;			///< 64 bits unsigned integer
	typedef signed char			s8;				///< 8 bits signed integer
	typedef signed short		s16;			///< 16 bits signed integer
	typedef signed int			s32;			///< 32 bits signed integer
	typedef signed long long	s64;			///< 64 bits unsigned integer
	typedef float				f32;			///< 32 bits floting point
	typedef double				f64;			///< 64 bits floting point


	#ifdef __WINDOWS32__
		#define UnionPacked		union __attribute__((gcc_struct, packed))		///< Union dont tous les champs sont collés (utile pour les communications)
		#define StructPacked	struct __attribute__((gcc_struct, packed))		///< Structure dont tous les champs sont collés (utile pour les communications)
	#else
		#define UnionPacked		union __attribute__((packed))					///< Union dont tous les champs sont collés (utile pour les communications)
		#define StructPacked	struct __attribute__((packed))					///< Structure dont tous les champs sont collés (utile pour les communications)
	#endif


	#define INLINE			static inline __attribute__((always_inline))	///< Fonction copiée collée ou elle est appelée


	#define STRX8		"%x"
	#define STRX16		"%x"
	#define STRX32		"%x"
	#define STRX64		"%llx"
	#define STRU8		"%u"
	#define STRU16		"%u"
	#define STRU32		"%u"
	#define STRU64		"%llu"
	#define STRS8		"%d"
	#define STRS16		"%d"
	#define STRS32		"%d"
	#define STRS64		"%lld"
	#define STRF32		"%g"
	#define STRF64		"%lg"
	


#ifdef __cplusplus
}//extern C
#endif
#endif//TYPE_RPI_H
