/******************************************************/
/* Author    : Nourhan MAnsour                        */
/* Date      : 27/1/2021                              */
/* Version   : 1.0                                    */
/* File      : DIO.h                                  */
/******************************************************/

#ifndef DIO_H
#define DIO_H

#include "STD_TYPES.h"
#include "DIO_cfg.h"

/******************************************************/
/*                  Type Definitons                   */
/******************************************************/

/**
 * Numeric ID of a DIO channel.
 * Values :Px_0 ~ Px_15 - x: portNum 
 * Example: PB_1 
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

/**
 * name : Dio_ReadChannel
 * param: ChannelId: ID of DIO channel 
 * Reentrancy: Reentrant
 * Sync
 * Return: STD_HIGH / STD_LOW
 * Description: Returns the value of the specified DIO channel.
 **/ 
Dio_LevelType Dio_ReadChannel( Dio_ChannelType ChannelId );

/**
 * name : Dio_WriteChannel
 * param: ChannelId: ID of DIO channel 
 *            Level: Value to be written
 * Reentrancy: Reentrant
 * Sync
 * Description: Service to set a level of a channel.
 **/ 
void Dio_WriteChannel( Dio_ChannelType ChannelId, Dio_LevelType Level );

/**
 * name : Dio_ReadPort
 * param: PortId: ID of DIO Port
 * Reentrancy: Reentrant
 * Sync
 * Return: Level of all channels of that port
 * Description: Returns the level of all channels of that port.
 **/ 
Dio_PortLevelType Dio_ReadPort( Dio_PortType PortId );

/**
 * name : Dio_WriteChannel
 * param: PortId: ID of DIO Port 
 *         Level: Value to be written
 * Reentrancy: Reentrant
 * Sync
 * Description: Service to set a level of the port.
 **/
void Dio_WritePort( Dio_PortType PortId, Dio_PortLevelType Level );

/**
 * name : Dio_ReadChannelGroup
 * param: ChannelGroupIdPtr: Pointer to ChannelGroup
 * Reentrancy: Reentrant
 * Sync
 * Return: Level of a subset of the adjoining bits of a port
 * Description: This Service reads a subset of the adjoining bits of a port.
 **/ 
Dio_PortLevelType Dio_ReadChannelGroup( const Dio_ChannelGroupType * ChannelGroupIdPtr );

/**
 * name : Dio_WriteChannelGroup
 * param: ChannelGroupIdPtr: Pointer to ChannelGroup 
 *                    Level: Value to be written
 * Reentrancy: Reentrant
 * Sync
 * Description: Service to set a subset of the adjoining bits of a port to a specified level.
 **/
void Dio_WriteChannelGroup( const Dio_ChannelGroupType* ChannelGroupIdPtr, Dio_PortLevelType Level );

/**
 * name : Dio_GetVersionInfo
 * param: VersionInfo: Pointer to where to store the version information of this module
 * Reentrancy: Reentrant
 * Sync
 * Description: Service to get the version information of this module.
 **/
void Dio_GetVersionInfo( Std_VersionInfoType* VersionInfo );

/**
 * name : Dio_FlipChannel
 * param: ChannelId: ID of DIO channel 
 *            Level: Value to be written
 * Reentrancy: Reentrant
 * Sync
 * Return: STD_HIGH / STD_LOW
 * Description: Service to flip (change from 1 to 0 or from 0 to 1) 
 *      the level of a channel and return the level of the channel after flip.
 **/ 
Dio_LevelType Dio_FlipChannel( Dio_ChannelType ChannelId);


#endif
