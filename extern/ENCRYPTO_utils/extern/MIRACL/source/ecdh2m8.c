/*

Example GF(2^m) Elliptic Curve Diffie-Hellman program for very constrained environments. Uses point compression.
Stack-only memory allocation. 8-bit version. The Koblitz curve variant is ideal for wireless sensor networks.

Note that using a Koblitz curve, precomputation may be inefficient, so either just use
regular point multiplication, or use a larger precomputed table. But the code is left in for illustration
purposes.

This has been tested on an Atmel Atmeg128 8-bit processor using this mirdef.h file with avr-gcc, and a general 
point multiplication takes 3 seconds at 4MHz, without any assembly language used.

#define MR_LITTLE_ENDIAN
#define MIRACL 8
#define mr_utype char 
#define MR_IBITS 16  
#define MR_LBITS 32  
#define mr_unsign32 unsigned long 
#define mr_dltype short 
#define MR_STATIC 21 
#define MR_ALWAYS_BINARY
#define MR_NOASM     
#define MR_STRIPPED_DOWN
#define MR_GENERIC_MT 
#define MAXBASE ((mr_small)1<<(MIRACL-1))
#define MR_BITSINCHAR 8
#define MR_SMALL_EWINDOW
#define MR_NO_STANDARD_IO 
#define MR_NO_FILE_IO  
#define MR_NO_ECC_MULTIADD
#define MR_SIMPLE_BASE
#define MR_SIMPLE_IO
#define MR_NO_SS
#define MR_AVR

Use with this mirdef.h header (for a PC using MS C)

#define MR_LITTLE_ENDIAN
#define MIRACL 8
#define mr_utype char
#define MR_IBITS 32
#define MR_LBITS 32
#define mr_unsign32 unsigned int
#define mr_dltype short
#define MR_STATIC 21
#define MR_ALWAYS_BINARY
#define MR_NOASM
#define MR_STRIPPED_DOWN
#define MR_GENERIC_MT
#define MAXBASE ((mr_small)1<<(MIRACL-1))
#define MR_BITSINCHAR 8
#define MR_SMALL_EWINDOW
#define MR_NO_SS
#define MR_SIMPLE_BASE
#define MR_SIMPLE_IO

In an embedded environment perhaps these should be defined as well..

#define MR_NO_STANDARD_IO
#define MR_NO_FILE_IO

Build the library from these modules (Example using MS C compiler)

cl /c /O2 /W3 mrcore.c
cl /c /O2 /W3 mrarth0.c
cl /c /O2 /W3 mrarth1.c
cl /c /O2 /W3 mrio1.c
cl /c /O2 /W3 mrbits.c
cl /c /O2 /W3 mrgf2m.c
cl /c /O2 /W3 mrec2m.c

rem
rem Create library 'miracl.lib'
del miracl.lib

lib /OUT:miracl.lib mrbits.obj mrio1.obj
lib /OUT:miracl.lib miracl.lib mrarth0.obj mrarth1.obj mrcore.obj 
lib /OUT:miracl.lib miracl.lib mrec2m.obj mrgf2m.obj
del mr*.obj

rem Create the program

cl /O2 ecdh2m8.c miracl.lib

*/

#include <stdio.h>
#include <string.h>
#include "miracl.h"


#define HEXDIGS (MIRACL/4)

/* !!!!!! THIS CODE AND THESE ROMS ARE NOW CREATED AUTOMATICALLY USING THE ROMAKER2.C APPLICATION !!!!!!!! */
/* !!!!!! READ COMMENTS IN ROMAKER2.C !!!!!! */

#define CURVE_M 163
#define CURVE_A 7
#define CURVE_B 6
#define CURVE_C 3

/* NIST k163 Koblitz curve */
#ifdef MR_AVR
__attribute__((__progmem__))
#endif 
static const mr_small rom[]=
{
0x1,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0xEF,0xA5,0x58,0x99,0x0D,0xCC,0xE0,0x8A,0x10,0x20,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x4,
0x0F,0xA8,0x9C,0x10,0x0E,0xE9,0x2D,0x27,0x1D,0x91,0xCA,0x37,0x2B,0x7A,0xF8,0x5E,0x47,0x0B,0xC3,0x96,0x3,
0x91,0x63,0xE7,0xD6,0x53,0xA2,0x23,0x29,0x2D,0x14,0xBB,0xBA,0xC3,0x1D,0xB4,0x8B,0x4C,0x0E,0x7D,0x94,0x3};

#define WINDOW 4

/* 2^4 =16 precomputed points based on fixed generator G(x,y) */
/* (created using romaker2.c program with window size of 4)   */


/* Koblitz curve - precomputed data based on fixed generator G */
#ifdef MR_AVR
__attribute__((__progmem__))
#endif 
static const mr_small prom[]=
{
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0xf,0xa8,0x9c,0x10,0xe,0xe9,0x2d,0x27,0x1d,0x91,0xca,0x37,0x2b,0x7a,0xf8,0x5e,0x47,0xb,0xc3,0x96,0x3,
0x91,0x63,0xe7,0xd6,0x53,0xa2,0x23,0x29,0x2d,0x14,0xbb,0xba,0xc3,0x1d,0xb4,0x8b,0x4c,0xe,0x7d,0x94,0x3,
0x5f,0x99,0x28,0xc0,0x93,0xc1,0x22,0x25,0x39,0xd9,0x65,0x5e,0xb2,0xa7,0x29,0xb5,0xa7,0xbf,0x3b,0xcf,0x3,
0x9b,0x8b,0xa9,0x15,0x96,0x5,0xdc,0x8,0x69,0xf7,0x43,0xb,0xbe,0x9d,0x49,0xf4,0xb5,0xd1,0x9e,0x46,0x3,
0x58,0x6d,0x30,0x84,0x6f,0x71,0x47,0x9e,0xe1,0xa1,0x42,0xb6,0x27,0xc6,0xa3,0x42,0x2d,0xc4,0xc0,0x3b,0x4,
0xc5,0x22,0x77,0x47,0x28,0x13,0x37,0x7d,0xee,0xcc,0xaa,0xf2,0xda,0x48,0x49,0xb6,0x75,0x19,0x58,0x32,0x4,
0xec,0xf8,0x6f,0x9f,0x7a,0x65,0x7,0x54,0xdd,0x6,0xb6,0x64,0xc4,0xd3,0xd2,0x2e,0xce,0xb3,0x59,0xb3,0x2,
0xbc,0xc1,0x61,0xe1,0xaf,0x2a,0x68,0xa1,0x51,0xe1,0x99,0xb3,0x3e,0xe4,0xe2,0x50,0x25,0x3d,0xf5,0x36,0x2,
0x3d,0x1b,0xe5,0xe2,0x41,0x6c,0xb6,0xfc,0x93,0x9a,0x68,0xb1,0xa5,0xb6,0x0,0xdb,0x24,0x8b,0x7,0x8b,0x6,
0x6d,0x98,0x7e,0x48,0x59,0xb2,0x67,0x6e,0x61,0xd4,0x4b,0xa4,0xb8,0xdb,0xa4,0xc3,0xb9,0x1c,0x36,0x2c,0x5,
0xa6,0x16,0xcf,0x69,0x27,0x1b,0xd6,0x95,0x2f,0xae,0xe9,0xa0,0x40,0x6e,0xa0,0x8a,0x92,0x19,0x63,0x76,0x0,
0xb6,0x5b,0xdb,0xd3,0x3e,0xe7,0xa7,0x9b,0x49,0x2a,0xfb,0xed,0xc8,0x54,0x3,0xbd,0x14,0xaa,0x76,0x57,0x4,
0xea,0x6c,0x52,0x79,0x95,0x94,0xd0,0x17,0x57,0x98,0xab,0xc8,0xe9,0x28,0x18,0x99,0x1b,0xbd,0x88,0x70,0x4,
0xc8,0x84,0x42,0xca,0xd9,0xbc,0x6b,0x8,0x33,0x59,0x69,0x21,0x51,0x37,0xcf,0x69,0xb1,0x8e,0x14,0x66,0x0,
0x3c,0x44,0xf9,0xba,0x50,0xd8,0x37,0x5c,0x1d,0x8c,0x2,0x1,0xae,0xc5,0xdc,0x48,0x24,0x89,0xf7,0xbd,0x7,
0x90,0xae,0xf0,0x64,0xa,0x1c,0x1d,0x2b,0xae,0x39,0x92,0xe2,0x98,0x68,0x85,0x3,0x3c,0x5c,0xa9,0x7e,0x5,
0xd5,0xe7,0xc1,0x13,0x93,0xd1,0x75,0xaf,0x53,0x72,0xb4,0xa5,0xf4,0xf,0xa0,0x8b,0x42,0x69,0xa5,0x8b,0x5,
0xa8,0x2,0xe8,0xd2,0xd2,0x2b,0x3e,0xc2,0xa9,0xb5,0xbf,0xbd,0x7b,0x9b,0x81,0xf1,0xb7,0x68,0xfc,0xb5,0x1,
0x39,0xf5,0x19,0xef,0x18,0xca,0xaf,0xc,0xbd,0x47,0x3a,0x5,0x44,0xc7,0x58,0x61,0xeb,0x86,0x62,0x8a,0x1,
0x27,0x18,0x84,0x8c,0x7f,0x23,0x33,0x85,0xad,0xf9,0x90,0xcb,0x83,0xff,0x5b,0x3e,0x50,0x53,0xb0,0x34,0x2,
0x91,0x73,0xa3,0x8e,0xd3,0xb6,0xc2,0xa2,0xb4,0x99,0xdb,0xf3,0x6b,0x1a,0x68,0x8f,0x88,0xb7,0x6d,0xca,0x7,
0x7a,0x10,0xe9,0x10,0xe2,0xdd,0x4c,0xfc,0x38,0x3,0xd9,0xf2,0xd8,0xd6,0xcf,0xf4,0xe0,0xff,0xb5,0x28,0x7,
0x97,0xee,0xd7,0x44,0xc2,0x81,0x22,0xb,0xfa,0xdc,0xc9,0xcf,0xf8,0xc,0xd9,0x9a,0xd3,0x21,0x4b,0xc8,0x7,
0x33,0x13,0xd5,0x4b,0x3d,0xe6,0xa2,0xb9,0xfe,0x6,0x5a,0x69,0xd8,0xc8,0xed,0x78,0xa4,0xcb,0x42,0x9b,0x7,
0x3,0xfb,0x95,0xb9,0x77,0xba,0xa,0x8c,0x4c,0xc4,0x3,0x65,0x5a,0x3b,0xc6,0xaa,0x6d,0x81,0x22,0x58,0x6,
0x59,0x36,0x7,0xef,0x3f,0x6e,0x68,0xd3,0x7f,0x59,0x9,0xde,0xdc,0xb5,0x4c,0x1c,0x2d,0x32,0x80,0x80,0x7,
0x1,0x81,0xb0,0x22,0xd8,0x40,0xfd,0x8f,0x52,0xff,0xfa,0x96,0x69,0x9d,0x58,0x88,0xc2,0x9f,0x7b,0x18,0x4,
0x55,0x59,0x79,0xf9,0x7b,0xad,0x5b,0x80,0x73,0x86,0xe4,0x8,0x73,0x4f,0x1e,0x89,0xc6,0xda,0x21,0x7c,0x3,
0x6d,0xd6,0x3c,0xd0,0x32,0xc7,0xb2,0x2,0x5c,0xad,0xb8,0x60,0x89,0x6b,0xc2,0x1a,0xbc,0x99,0xd3,0x18,0x3,
0xb4,0x8,0x52,0xa2,0x5d,0xe,0x3a,0x54,0x78,0xe1,0x81,0x0,0x39,0x4,0xad,0x5e,0x3b,0xf3,0x22,0x1d,0x4};

#define WORDS 21  /* Number of words per big variable 21*8 > 163 */

/* Note that in a real application a source of real random numbers would be required, to
   replace those generated by MIRACL's internal pseudo-random generator "bigbits"  
   Alternatively from a truly random and unguessable seed, use MIRACL's strong random 
   number generator */

/* Elliptic Curve Diffie-Hellman, using point compression to minimize bandwidth, 
   and precomputation to speed up off-line calculation */

int main()
{
    int promptr;
    epoint *PA,*PB;
    big A,B,a,b,q,pa,pb,key,x,y;
    ebrick2 binst;
    miracl instance;      /* create miracl workspace on the stack */

/* Specify base 16 here so that HEX can be read in directly without a base-change */

    miracl *mip=mirsys(&instance,WORDS*HEXDIGS,16); /* size of bigs is fixed */
    char mem_big[MR_BIG_RESERVE(10)];         /* we need 10 bigs... */
    char mem_ecp[MR_ECP_RESERVE(2)];          /* ..and two elliptic curve points */
 	memset(mem_big, 0, MR_BIG_RESERVE(10));   /* clear the memory */
	memset(mem_ecp, 0, MR_ECP_RESERVE(2));

/* printf("sizeof(miracl)= %d\n",sizeof(miracl)); */

    A=mirvar_mem(mip, mem_big, 0);       /* Initialise big numbers */
    B=mirvar_mem(mip, mem_big, 1);
    pa=mirvar_mem(mip, mem_big, 2);
    pb=mirvar_mem(mip, mem_big, 3);
    key=mirvar_mem(mip, mem_big, 4);
    x=mirvar_mem(mip, mem_big, 5);
    y=mirvar_mem(mip, mem_big, 6);
    q=mirvar_mem(mip,mem_big,7);
    a=mirvar_mem(mip, mem_big, 8);
    b=mirvar_mem(mip, mem_big, 9);

    PA=epoint_init_mem(mip, mem_ecp, 0); /* initialise Elliptic Curve points */
    PB=epoint_init_mem(mip, mem_ecp, 1);

    irand(mip, 3L);                      /* change parameter for different random numbers */
    promptr=0;
    init_big_from_rom(B,WORDS,rom,WORDS*4,&promptr);  /* Read in curve parameter B from ROM */
                                                 /* don't need q or G(x,y) (we have precomputed table from it) */
    init_big_from_rom(q,WORDS,rom,WORDS*4,&promptr);
    init_big_from_rom(x,WORDS,rom,WORDS*4,&promptr);
    init_big_from_rom(y,WORDS,rom,WORDS*4,&promptr);

    convert(mip,1,A);                            /* set A=1 */

/* Create precomputation instance from precomputed table in ROM */

    ebrick2_init(&binst,prom,A,B,CURVE_M,CURVE_A,CURVE_B,CURVE_C,WINDOW,CURVE_M);

/* offline calculations */

    bigbits(mip,CURVE_M,a);  /* A's random number */

    mul2_brick(mip,&binst,a,pa,pa);    /* a*G =(pa,ya) */

    bigbits(mip,CURVE_M,b);  /* B's random number */
    mul2_brick(mip,&binst,b,pb,pb);    /* b*G =(pb,yb) */

/* Swap X values */

/* online calculations */
    ecurve2_init(mip,CURVE_M,CURVE_A,CURVE_B,CURVE_C,A,B,FALSE,MR_PROJECTIVE);

    epoint2_set(mip,pb,pb,0,PB); /* decompress PB */
    ecurve2_mult(mip,a,PB,PB);
    epoint2_get(mip,PB,key,key);

/* since internal base is HEX, can use otnum instead of cotnum - avoiding a base change */

#ifndef MR_NO_STANDARD_IO
printf("Alice's Key= ");
otnum(mip,key,stdout);
#endif

    epoint2_set(mip,pa,pa,0,PB); /* decompress PA */
    ecurve2_mult(mip,b,PB,PB);
    epoint2_get(mip,PB,key,key);

#ifndef MR_NO_STANDARD_IO
printf("Bob's Key=   ");
otnum(mip,key,stdout);
#endif

/* clear the memory - good security measure */

	memset(mem_big, 0, MR_BIG_RESERVE(10));
	memset(mem_ecp, 0, MR_ECP_RESERVE(2));

	return 0;
}
