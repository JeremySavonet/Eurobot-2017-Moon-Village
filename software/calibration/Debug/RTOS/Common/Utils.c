///////////////////////////////////////////////////////////////////////////////////////////////////
/// \file		Utils.c
/// \brief		Fonctions basiques
///
/// \author		Brice VINCENT
/// \author		Etienne BOILLEY
/// \date		16/05/2008
///	\copyright	IOCENE
///////////////////////////////////////////////////////////////////////////////////////////////////


#include <Rtos.h>





/*


/////////////////////////////////////////////////////////////////////
///////////////////// fonctions de maths ////////////////////////////
/////////////////////////////////////////////////////////////////////
s16 roundint(double x)
{
	double fractpart, intpart;
	fractpart = modf(x , &intpart);
	if (fractpart <= 0.5) return (int)x;
	else return (int)x+1;
} // round

double mod2pi(double x)
{
	while (x >= M_PI) x -= 2*M_PI;
	while (x < M_PI) x += 2*M_PI;
	return x;
} // mod2pi

double signe(double x)
{
	if (x>0.0)
		return 1.0;
	if (x<0.0)
		return -1.0;
	// sinon == donc
	return 0.0;
} // signe

// do a modulo 2.pi -> [-Pi,+Pi], knowing that 'a' is in [-3Pi,+3Pi]
f32 simple_modulo_2pi(f32 a)
{
	if(a < -M_PI) {
		a += M_2PI;
	}
	else if(a > M_PI) {
		a -= M_2PI;
	}
	return a;
} // simple_modulo_2pi


// do a modulo 2.pi -> [-Pi,+Pi]
f32 modulo_2pi(f32 a)
{
	//f32 res = a - (((f32) (a/M_2PI)) * M_2PI);
	return simple_modulo_2pi(a);
} // modulo_2pi


const u8 oneBits[] = {0,1,1,2,1,2,2,3,1,2,2,3,2,3,3,4};

u8 count_ones_u8(u8 x)
{
    u8 results;
    results = oneBits[x&0x0f];
    results += oneBits[x>>4];
    return results;
}

u8 count_ones_u16(u16 x)
{
    u8 results;
    results = oneBits[x&0x0f];
    results += oneBits[(x>>4)&0x0f];
    results += oneBits[(x>>8)&0x0f];
    results += oneBits[(x>>12)&0x0f];
    return results;
}


/////////////////////////////////////////////////////////////////////
///////////////////// fonctions décodages ///////////////////////////
/////////////////////////////////////////////////////////////////////


u32 checksum32(u8 * array,u32 count)
{
	u32 checksum = 0;
	u16 i;

	for (i=0;i<count;i++) checksum+=array[i];
	return ~checksum+1;

} // checksum32

boolean checksum32_test(u8 * array,u16 count,u32 tocheck)
{
	u32 checksum = 0;
	u16 i;

	for (i=0;i<count;i++) checksum+=array[i];
	checksum+= tocheck;
	if (checksum == 0) return true;
	else return false;

} // checksum32_test


u16 checksum16(u8 * array,u16 count)
{
	u16 i,checksum = 0;

	for (i=0;i<count;i++) checksum+=array[i];
	return ~checksum+1;

} // checksum16

boolean checksum16_test(u8 * array,u16 count,u16 tocheck)
{
	u16 i,checksum = 0;

	for (i=0;i<count;i++) checksum+=array[i];
	checksum+= tocheck;
	if (checksum == 0) return true;
	else return false;

} // checksum16

u8 checksum8(u8 * array,u16 count)
{
	u8 checksum = 0;
	u16 i;

	for (i=0;i<count;i++) checksum+=array[i];
	return (u8)(~checksum+1);

} // checksum8

boolean checksum8_test(u8 * array,u16 count,u8 tocheck)
{
	u8 checksum = 0;
	u16 i;

	for (i=0;i<count;i++) checksum+=array[i];
	checksum+= tocheck;
	if (checksum == 0) return true;
	else return false;

} // checksum8_test


u16 decode16(u8 * tab)
{
	return ((tab[1] << 8) | (tab[0]));
} // decode16

void code16(u16 value,u8 * tab)
{
	tab[1] = value >> 8;
	tab[0] = value & 0x00FF;
} // code16

u32 decode32(u8 * tab)
{
	return (((u32)tab[3] << 24) | ((u32)tab[2] << 16) | ((u32)tab[1] << 8) | ((u32)tab[0]));
} // decode16

void code32(u32 value,u8 * tab)
{
	tab[3] = (u32)value >> 24;
	tab[2] = (u32)value >> 16;
	tab[1] = (u32)value >> 8;
	tab[0] = (u32)value & 0x00FF;
} // code32


DWORD decoded32(u8 * tab)
{
	return (((DWORD)tab[3] << 24) | ((DWORD)tab[2] << 16) | ((DWORD)tab[1] << 8) | ((DWORD)tab[0]));
} // decode16

void coded32(DWORD value,u8 * tab)
{
	tab[3] = (u32)value >> 24;
	tab[2] = (u32)value >> 16;
	tab[1] = (u32)value >> 8;
	tab[0] = (u32)value & 0x00FF;
} // coded32

/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////





/////////////////////////////////////////////////////////////////////
//////////////////////// outils strings /////////////////////////////
/////////////////////////////////////////////////////////////////////
s16 ABS_(s16 i)
{
	if (i < 0)
	return -i;
	else
	return i;
}
#define MAX_MANTISA 6

#define DIV(x,y) (y==0)?HUGE_VAL:x/y

s16 ftoa (double x, char *str, char  prec, char format)
{

	s16 ie, i, k, ndig, fstyle;
	double y;
	char *start;

	start=str;

	//based on percission set number digits
	ndig=prec+1;
	if (prec<0)
		ndig=7;
	if (prec>22)
		ndig=23;

	fstyle = 0;	//exponent 'e'
	if  (format == 'f' || format == 'F')
		fstyle = 1;	//normal 'f'
	if (format=='g' || format=='G')
		fstyle=2;

	ie = 0;
	// if x negative, write minus and reverse
	if ( x < 0)
	{
	  *str++ = '-';
	  x = -x;
	}

	//if (x<0.0) then increment by 10 till betwen 1.0 and 10.0
	if (x!=0.0)
	{
		while (x < 1.0)
		{
		  x =x* 10.0;
		  ie--;
		}
	}

   	//if x>10 then let's shift it down
	while (x >= 10.0)
	{
		x = x*(1.0/10.0);
		ie++;
	}

	if (ABS_(ie)>MAX_MANTISA)
	{
		if (fstyle==1)
		{
			fstyle=0;
			format='e';
			//ie=2;
		}
	}


	// in f format, number of digits is related to size
	if (fstyle)
		ndig =ndig + ie;

	if(prec==0 && ie>ndig && fstyle)
	{
		ndig=ie;
	}

	// round. x is between 1 and 10 and ndig will be printed to
	   right of decimal point so rounding is ...
	y=1;
	for (i = 1; i < ndig; i++)	//find lest significant digit
	  y = y *(1.0/10.0); 		//multiply by 1/10 is faster than divides

	x = x+ y *(1.0/2.0);			//add rounding

	// repair rounding disasters
	if (x >= 10.0)
	{
		x = 1.0;
		ie++;
		ndig++;
	}

	//check and see if the number is less than 1.0
	if (fstyle && ie<0)
	{
		*str++ = '0';
		if (prec!=0)
			*str++ = '.';
		if (ndig < 0)
			ie = ie-ndig; // limit zeros if underflow
		for (i = -1; i > ie; i--)
			*str++ = '0';
	}

	//for each digit
	for (i=0; i < ndig; i++)
	{
		float b;
		k = x;						//k = most significant digit
		*str++ = k + '0';			//output the char representation
		if (((!fstyle && i==0) || (fstyle && i==ie)) && prec!=0)
			*str++ = '.';			//output a decimal point
		b=(float)k;
		//multiply by 10 before subtraction to remove
		//errors from limited number of bits in float.
		b=b*10.0;
		x=x*10.0;
		x =x - b;				//subtract k from x
		//b=x+b;
		//x =x* 10.0;					//get next digit
	}

// now, in estyle,  put out exponent if not zero
	if (!fstyle && ie != 0)
	{
		*str++ = format;
		if (ie < 0)		//if number has negative exponent
		{
			ie = -ie;
			*str++ = '-';
		}

		//now we need to convert the exponent to string
		for (k=1000; k>ie; k=k/10);		//find the decade of exponent

		for (; k > 0; k=k/10)
		{
			char t;
			t=DIV(ie,k);
			*str++ = t + '0';
			ie = ie -(t*k);
		}

	}
	*str++ = '\0';
	return (str-start);	//return string length
} // ftoa





// Convertit un u8 en string
char *u8toa (unsigned char value, char *pBuffer, unsigned char radix)
{
   unsigned char digit;

  pBuffer += 18;
  *--pBuffer = '\0';

   do {
      digit = value % radix;
      *--pBuffer = '0' + digit + ((digit > 9) ? 7 : 0);
  } while ((value /= radix) != 0);

   return (pBuffer);
} // u8toa


// Convertit un signed s8 en string
char *s8toa (char number, char *pBuffer, unsigned char radix)
{
   if (number >= 0)
       return (u8toa (number, pBuffer, radix));
   else {
      pBuffer = u8toa (-number, pBuffer, radix);
     *--pBuffer = '-';
      return (pBuffer);
  } // if
} // s8toa



// Convertit un u16 en string
char *u16toa (u16 value, char *pBuffer, unsigned char radix)
{
   unsigned char digit;

  pBuffer += 18;
  *--pBuffer = '\0';

   do {
      digit = value % radix;
      *--pBuffer = '0' + digit + ((digit > 9) ? 7 : 0);
  } while ((value /= radix) != 0);

   return (pBuffer);
} // u16toa


// Convertit un signed s16 en string
char *i16toa (s16 number, char *pBuffer, unsigned char radix)
{
   if (number >= 0)
       return (u16toa (number, pBuffer, radix));
   else {
      pBuffer = u16toa (-number, pBuffer, radix);
     *--pBuffer = '-';
      return (pBuffer);
  } // if
} // i16toa




// Convertit un u32 en string
char *u32toa (u32 value, char *pBuffer, unsigned char radix)
{
   unsigned char digit;

  pBuffer += 18;
  *--pBuffer = '\0';

   do {
      digit = value % radix;
      *--pBuffer = '0' + digit + ((digit > 9) ? 7 : 0);
  } while ((value /= radix) != 0);

   return (pBuffer);
} // u32toa


// Convertit un signed s32 en string
char *i32toa (s32 number, char *pBuffer, unsigned char radix)
{
   if (number >= 0)
	   return (u32toa (number, pBuffer, radix));
   else {
      pBuffer = u32toa (-number, pBuffer, radix);
     *--pBuffer = '-';
      return (pBuffer);
  } // if
} // i32toa





boolean strequal(char *str1, char *str2) {
   while (*str1 && *str2) {
      if (*str1 != *str2) return false;
      str1++;
      str2++;
   }
   if (*str1 != *str2) return false;
   else return true;
}

// ici on check pas la longueur identique
boolean strequal_v1(char *str1, char *str2) {
   while (*str1 && *str2) {
      if (*str1 != *str2) return false;
      str1++;
      str2++;
   }
   return true;
}

boolean strhex(char *s, unsigned char *val, unsigned char n) {
   boolean both = false;
   *val = 0;
   while (*s && n) {
      if (((*s >= '0') && (*s <= '9')) || ((*s >= 'a') && (*s <= 'f')) || ((*s >= 'A') && (*s <= 'F'))) {
         *val <<= 4;
         *val |= (*s >= '0' && *s <= '9' ? *s - '0' : *s - 'a' + 10);
         if (both && --n) *(++val) = 0;
         both = !both;
      } else return false;
      s++;
   }
   if ((n && !*s) || (!n && *s)) return false;
   return true;
}

boolean strhexbyte(char *s, char *val) {
   if (!*s) return false;
   *val = (*s >= '0' && *s <= '9' ? *s - '0' : *s - 'a' + 10) << 4;
   if (!*(++s)) return false;
   *val |= (*s >= '0' && *s <= '9' ? *s - '0' : *s - 'a' + 10);
   return true;
}

boolean strnbr(char *str, u32 *val) {
   *val = 0;
   while (*str) {
      if (*str >= '0' && *str <= '9') {
         *val *= 10;
         *val += *str - '0';
      } else return false;
      str++;
   }
   return true;
}

void bytetohex(unsigned char *val,char *s)
{
   unsigned char n;

   n = (*val) & 0x0f;
   if (n < 10) s[0] = n + '0';
   else s[0] = n + 'a' - 10;

   n = (*val >> 4) & 0x0f;
   if (n < 10) s[1] = n + '0';
   else s[1] = n + 'a' - 10;

} // bytetohex



void wordtohex(s16 *val,char *s)
{
   unsigned char n;

   n = (*val) & 0x0f;
   if (n < 10) s[4] = n + '0';
   else s[4] = n + 'a' - 10;

   n = (*val >> 4) & 0x0f;
   if (n < 10) s[3] = n + '0';
   else s[3] = n + 'a' - 10;

   n = (*val >> 8) & 0x0f;
   if (n < 10) s[2] = n + '0';
   else s[2] = n + 'a' - 10;

   n = (*val >> 12) & 0x0f;
   if (n < 10) s[1] = n + '0';
   else s[1] = n + 'a' - 10;

	s[0] = 0;

} // bytetohex

boolean ishex(char c)
{
	if (((c >= '0') && (c <= '9')) || ((c >= 'a') && (c <= 'f')) || ((c >= 'A') && (c <= 'F')))
		return true;
	else
		return false;
} // ishex

/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////









///////////////////////////// MEMSET /////////////////////////////

inline void fast_memset(void * p, int fill, size_t size)
{
   #ifdef __dsPIC33F__
   size_t s = size - 1;
   asm volatile ( "REPEAT %2\nMOV.B %1,[%0++]"
         : "+r" (p)
         : "r" (fill), "r" (s)
		 : "_RCOUNT", "memory");
   #else
   memset(p,fill,size);
   #endif
} // fast_memset
















#endif
*/
