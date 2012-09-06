
#ifndef _BIT_FIELDS_H
#define _BIT_FIELDS_H

//! Create a bitmask of length \a len.
#define BIT_MASK(len)           ( BIT(len)-1 )

//! Create a bitfield mask of length \a starting at bit \a start.
#define BF_MASK(start, len)     ( BIT_MASK(len)<<(start) )

//! Prepare a bitmask for insertion or combining.
#define BF_PREP(x, start, len)  ( ((x)&BIT_MASK(len)) << (start) )


//! Extract a bitfield of length \a len starting at bit \a start from \a y.
#define BF_GET(y, start, len)   ( ((y)>>(start)) & BIT_MASK(len) )

//! Insert a new bitfield value \a x into \a y.
#define BF_SET(y, x, start, len)    \
    ( y= ((y) &~ BF_MASK(start, len)) | BF_PREP(x, start, len) )



//#define UNSTUFF_BITS(resp,start,size)					\
//	({								\
//		const int __size = size;				\
//		const uint32_t __mask = (__size < 32 ? 1 << __size : 0) - 1;	\
//		const int __off = ((start) / 32);			\
//		const int __shft = (start) & 31;			\
//		uint32_t __res;						\
//									\
//		__res = resp[__off] >> __shft;				\
//		if (__size + __shft > 32)				\
//			__res |= resp[__off-1] << ((32 - __shft) % 32);	\
//		__res & __mask;						\
//	})

#define UNSTUFF_BITS(resp,start,size)					\
	({								\
		const int __size = size;				\
		const uint32_t __mask = (__size < 32 ? 1 << __size : 0) - 1;	\
		const int __off =  ((start) / 32);			\
		const int __shft = (start) & 31;			\
		uint32_t __res;						\
									\
		__res = resp[__off] >> __shft;				\
		if (__size + __shft > 32)				\
			__res |= resp[__off-1] << ((32 - __shft) % 32);	\
		__res & __mask;						\
	})



#endif


