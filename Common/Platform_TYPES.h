/******************************************************/
/* Author    : Nourhan Mansour                        */
/* Date      : 27/1/2021                              */
/* Version   : 1.0                                    */
/* File      : PLATFORM_TYPES.h                       */
/******************************************************/

#ifndef PLATFORM_TYPES_H
#define PLATFORM_TYPES_H

#ifndef TRUE 
    #define TRUE            1 
#endif 

#ifndef FALSE 
    #define FALSE           0 
#endif
/******** CPU TYPE *********/
#define CPU_TYPE_8          8U
#define CPU_TYPE_16         16U
#define CPU_TYPE_32         32U
#define CPU_TYPE_64         64U
#define CPU_TYPE            CPU_TYPE_64

/******* Bit Order *********/
#define MSB_FIRST           0U
#define LSB_FIRST           1U
#define CPU_BIT_ORDER       LSB_FIRST

/******* Byte Order *********/
#define HIGH_BYTE_FIRST     0U
#define LOW_BYTE_FIRST      1u
#define CPU_BYTE_ORDER      LOW_BYTE_FIRST


/******** Data Types **********/
typedef unsigned char       boolean;
typedef unsigned char       uint8 ;
typedef unsigned short int  uint16;
typedef unsigned long  int  uint32;
typedef unsigned long long  uint64;

typedef signed char         sint8 ;
typedef signed short int    sint16;
typedef signed long  int    sint32;
typedef signed long long    sint64;

typedef signed char         sint8_least;
typedef unsigned char       uint8_least;
typedef signed short        sint16_least;
typedef unsigned short      uint16_least;
typedef signed long         sint32_least;
typedef unsigned long       uint32_least;

typedef float               float32;
typedef double              float64;

#endif  