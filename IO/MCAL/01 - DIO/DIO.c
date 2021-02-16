#include "DIO.h"
#include "DIO_reg.h"

#define PORTA_CHANNEL_OFFEST 0
#define PORTB_CHANNEL_OFFEST 16
#define PORTC_CHANNEL_OFFEST 32

/**
 * name : Dio_ReadChannel
 * param: ChannelId: ID of DIO channel 
 * Reentrancy: Reentrant
 * Sync
 * Return: STD_HIGH / STD_LOW
 * Description: Returns the value of the specified DIO channel.
 **/
Dio_LevelType Dio_ReadChannel(Dio_ChannelType ChannelId)
{

    Dio_LevelType channelValue = 0;
    if (ChannelId >= PORTA_CHANNEL_OFFEST &&
        ChannelId < PORTB_CHANNEL_OFFEST)
    {
        channelValue = GET_BIT(GPIOA_IDR, ChannelId);
    }
    else if (ChannelId >= PORTB_CHANNEL_OFFEST &&
             ChannelId < PORTC_CHANNEL_OFFEST)
    {
        ChannelId -= PORTB_CHANNEL_OFFEST;
        channelValue = GET_BIT(GPIOB_IDR, ChannelId);
    }
    else
    {
        ChannelId -= PORTC_CHANNEL_OFFEST;
        channelValue = GET_BIT(GPIOC_IDR, ChannelId);
    }
    return channelValue;
}

/**
 * name : Dio_WriteChannel
 * param: ChannelId: ID of DIO channel 
 *            Level: Value to be written
 * Reentrancy: Reentrant
 * Sync
 * Description: Service to set a level of a channel.
 **/
void Dio_WriteChannel(Dio_ChannelType ChannelId, Dio_LevelType Level)
{
    if (ChannelId >= PORTA_CHANNEL_OFFEST &&
        ChannelId < PORTB_CHANNEL_OFFEST)
    {
        switch (Level)
        {
        case STD_HIGH:
            SET_BIT(GPIOA_BSRR, ChannelId); // Set Channel
            break;
        case STD_LOW:
            SET_BIT(GPIOA_BRR, ChannelId); // Reset Channel
            break;
        default:
            // shouldn't be here
            break;
        }
    }
    else if (ChannelId >= PORTB_CHANNEL_OFFEST &&
             ChannelId < PORTC_CHANNEL_OFFEST)
    {
        ChannelId -= PORTB_CHANNEL_OFFEST;
        switch (Level)
        {
        case STD_HIGH:
            SET_BIT(GPIOB_BSRR, ChannelId); // Set Channel
            break;
        case STD_LOW:
            SET_BIT(GPIOB_BRR, ChannelId); // Reset Channel
            break;
        default:
            // shouldn't be here
            break;
        }
    }
    else
    {
        ChannelId -= PORTC_CHANNEL_OFFEST;
        switch (Level)
        {
        case STD_HIGH:
            SET_BIT(GPIOC_BSRR, ChannelId); // Set Channel
            break;
        case STD_LOW:
            SET_BIT(GPIOC_BRR, ChannelId); // Reset Channel
            break;
        default:
            // shouldn't be here
            break;
        }
    }
}

/**
 * name : Dio_ReadPort
 * param: PortId: ID of DIO Port
 * Reentrancy: Reentrant
 * Sync
 * Return: Level of all channels of that port
 * Description: Returns the level of all channels of that port.
 **/
Dio_PortLevelType Dio_ReadPort(Dio_PortType PortId)
{
    Dio_PortLevelType ret_PortValue = 0;
    switch (PortId)
    {
    case DIO_PORTTYPE_PORTA:
        ret_PortValue = (GPIOA_IDR);
        break;
    case DIO_PORTTYPE_PORTB:
        ret_PortValue = (GPIOB_IDR);
        break;
    case DIO_PORTTYPE_PORTC:
        ret_PortValue = (GPIOC_IDR);
        break;    
    default:
        break;
    }
    return ret_PortValue;
}

/**
 * name : Dio_WriteChannel
 * param: PortId: ID of DIO Port 
 *         Level: Value to be written
 * Reentrancy: Reentrant
 * Sync
 * Description: Service to set a level of the port.
 **/
void Dio_WritePort(Dio_PortType PortId, Dio_PortLevelType Level)
{
    /* Set the value of the output data register */
    switch (PortId)
    {
    case DIO_PORTTYPE_PORTA:
        GPIOA_ODR = Level;
        break;
    case DIO_PORTTYPE_PORTB:
        GPIOB_ODR = Level;
        break;
    case DIO_PORTTYPE_PORTC:
        GPIOC_ODR = Level;
        break;
    default:
        break;
    }
}

/**
 * name : Dio_ReadChannelGroup
 * param: ChannelGroupIdPtr: Pointer to ChannelGroup
 * Reentrancy: Reentrant
 * Sync
 * Return: Level of a subset of the adjoining bits of a port
 * Description: This Service reads a subset of the adjoining bits of a port.
 **/
Dio_PortLevelType Dio_ReadChannelGroup(const Dio_ChannelGroupType *ChannelGroupIdPtr)
{
    Dio_PortLevelType subset_level; 

    switch (ChannelGroupIdPtr->port)
    {
    case DIO_PORTTYPE_PORTA:
        subset_level = (GPIOA_IDR) & (ChannelGroupIdPtr -> mask);
        break;
    case DIO_PORTTYPE_PORTB:
        subset_level = (GPIOB_IDR) & (ChannelGroupIdPtr -> mask);
        break;
    case DIO_PORTTYPE_PORTC:
        subset_level = (GPIOC_IDR) & (ChannelGroupIdPtr -> mask);
        break;
    default:
        break;
    }
    subset_level = subset_level >> (ChannelGroupIdPtr->offset);
    return subset_level; 

}

/**
 * name : Dio_WriteChannelGroup
 * param: ChannelGroupIdPtr: Pointer to ChannelGroup 
 *                    Level: Value to be written
 * Reentrancy: Reentrant
 * Sync
 * Description: Service to set a subset of the adjoining bits of a port to a specified level.
 **/
void Dio_WriteChannelGroup(const Dio_ChannelGroupType *ChannelGroupIdPtr, Dio_PortLevelType Level)
{
    uint32 subset_level; 

    switch (ChannelGroupIdPtr->port)
    {
    case DIO_PORTTYPE_PORTA:
        subset_level = (GPIOA_IDR) & (~ChannelGroupIdPtr -> mask);          // reset channel group bits 
        subset_level |= (Level << (ChannelGroupIdPtr->offset));             // assign chanel group value
        GPIOA_IDR = subset_level;                                           // write new value to register
        break;      
    case DIO_PORTTYPE_PORTB:
        subset_level = (GPIOB_IDR) & (~ChannelGroupIdPtr -> mask);          // reset channel group bits 
        subset_level |= (Level << (ChannelGroupIdPtr->offset));             // assign chanel group value
        GPIOB_IDR = subset_level;                                           // write new value to register
        break;
    case DIO_PORTTYPE_PORTC:
        subset_level = (GPIOC_IDR) & (~ChannelGroupIdPtr -> mask);          // reset channel group bits 
        subset_level |= (Level << (ChannelGroupIdPtr->offset));             // assign chanel group value
        GPIOC_IDR = subset_level;                                           // write new value to register
        break;
    default:
        break;
    }

}
#if DioVersionInfoApi==TRUE
    /**
         * name : Dio_GetVersionInfo
         * param: VersionInfo: Pointer to where to store the version information of this module
         * Reentrancy: Reentrant
         * Sync
         * Description: Service to get the version information of this module.
         **/
    void Dio_GetVersionInfo(Std_VersionInfoType *VersionInfo)
    {

    }
#endif

#if DioFlipChannelApi==TRUE
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
    Dio_LevelType Dio_FlipChannel( Dio_ChannelType ChannelId)
    {
        Dio_LevelType ret_Level; 
        if (Dio_ReadChannel(ChannelId) == STD_HIGH)
        {
            Dio_WriteChannel(ChannelId , STD_LOW);
            ret_Level = STD_LOW;
        }
        else 
        {
            Dio_WriteChannel(ChannelId , STD_HIGH);
            ret_Level = STD_HIGH; 
        }
        return ret_Level; 
    } 
#endif