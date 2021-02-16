/******************************************************/
/* Author    : Nourhan MAnsour                        */
/* Date      : 27/1/2021                              */
/* Version   : 1.0                                    */
/* File      : DIO.h                                  */
/******************************************************/

#ifndef DIO_H
#define DIO_H

#include "STD_TYPES.h"
#include "BIT_MATH.h"
#include "DIO_cfg.h"


/******************************************************/
/*                  Type Definitons                   */
/******************************************************/

/**
 * Numeric ID of a DIO channel.
 * Values :DIO_CHANNEL_xx 
 * Range: DIO_CHANNEL_A0 ~ DIO_CHANNEL_C15 
 * Note: This configuration is valid for bluepill board
 **/
typedef uint8 Dio_ChannelType ;

/**
 * Numeric ID of a DIO port. 
 * Values: PORTx
 * Example: PORTA
 **/ 
typedef uint8 Dio_PortType ;


/**
 * Type for the definition of a channel group, which consists 
 * of several adjoining channels within a port.
 **/ 
typedef struct 
{
    // defines the positions of the channel group.
    uint16 mask;         
    // position of the Channel Group on the port, counted from the LSB.   
    uint8 offset;          
    // the port on which the Channel group is defined.
    Dio_PortType port;     
}Dio_ChannelGroupType;

/**
 * possible levels a DIO channel can have (input or output)
 * Range: STD_HIGH , STD_LOW
 **/ 
typedef uint8 Dio_LevelType ;

/**
 * possible levels a DIO port can have (input or output)
 * Range: depending on the port size 
 *        for bluepill borad: 0 ~ 16 bits
 **/ 
typedef uint16 Dio_PortLevelType;

/******************************************************/
/*               Functions Definitons                 */
/******************************************************/

Dio_LevelType Dio_ReadChannel( Dio_ChannelType ChannelId );

void Dio_WriteChannel( Dio_ChannelType ChannelId, Dio_LevelType Level );

Dio_PortLevelType Dio_ReadPort( Dio_PortType PortId );

void Dio_WritePort( Dio_PortType PortId, Dio_PortLevelType Level );

Dio_PortLevelType Dio_ReadChannelGroup( const Dio_ChannelGroupType * ChannelGroupIdPtr );

void Dio_WriteChannelGroup( const Dio_ChannelGroupType* ChannelGroupIdPtr, Dio_PortLevelType Level );

#if DioVersionInfoApi==TRUE
    void Dio_GetVersionInfo( Std_VersionInfoType* VersionInfo );
#endif

#if DioFlipChannelApi==TRUE
    Dio_LevelType Dio_FlipChannel( Dio_ChannelType ChannelId);
#endif

#endif         // EOF
