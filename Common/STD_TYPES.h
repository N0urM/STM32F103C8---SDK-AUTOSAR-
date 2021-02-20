/******************************************************/
/* Author    : Nourhan Mansour                        */
/* Date      : 27/1/2021                              */
/* Version   : 1.0                                    */
/* File      : STD_TYPRS.h                            */
/******************************************************/
#ifndef STD_TYPES_H
#define STD_TYPES_H

#include "Platform_TYPES.h"
#include "compiler.h"

/******************************************************/
/*                  Type Definitons                   */
/******************************************************/
typedef uint8 Std_ReturnType;

typedef struct 
{
    uint16 vendorID;
    uint16 moduleID;
    uint8 sw_major_version;
    uint8 sw_minor_version;
    uint8 sw_patch_version;

}Std_VersionInfoType;

/******************************************************/
/*                 Symbol Definitons                  */
/******************************************************/
#define E_OK            ((Std_ReturnType)0U)
#define E_NOT_OK        ((Std_ReturnType)1U)

#define STD_ACTIVE      0x01        // Logical state active 
#define STD_IDLE        0x00        // Logical state idle 

#define STD_LOW         0x00        // Physical state 0V
#define STD_HIGH        0x01        // Physical state 5V or 3.3V

#define STD_ON          0x01         
#define STD_OFF         0x00

#endif