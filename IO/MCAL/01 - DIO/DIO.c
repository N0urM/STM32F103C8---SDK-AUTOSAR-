/******************************************************/
/* Author    : Nourhan Mansour                        */
/* Date      : 20/2/2021                              */
/* Version   : 1.1.0                                  */
/* File      : DIO.c                                  */
/* Target    : STM32F103C8                            */
/* AUTOSAR Version : 4.3.1                            */
/******************************************************/

#include "DIO.h"
#include "hw_reg.h"
#include "det.h"

#define PORTA_CHANNEL_OFFEST (0U)
#define PORTB_CHANNEL_OFFEST (16U)
#define PORTC_CHANNEL_OFFEST (32U)
#define PORTx_CHANNEL_OFFSET (48U)

/***    API ID    ***/
#define DIO_ApiID_ReadChannel       0x00
#define DIO_ApiID_WriteChannel      0x01
#define DIO_ApiID_ReadPort          0x02
#define DIO_ApiID_WritePort         0x03
#define DIO_ApiID_ReadChannelGroup  0x04
#define DIO_ApiID_WriteChannelGroup 0x05
#define DIO_ApiID_FlipChannel       0x11
#define DIO_ApiID_GetVersionInfo    0x12

//      Local Type Def
/**
 * Numeric ID of a DIO channel. 
 * Values: Dio_ModeInput - Dio_ModeOutput      
 **/
typedef boolean DIO_modeType;

#define Dio_ModeInput 0x00
#define Dio_ModeOutput 0x01

/**** Local Functions *****/

DIO_modeType Dio_GetChannelMode(Dio_ChannelType ChannelId);

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

#if DioDevErrorDetect == TRUE
    if (ChannelId >= PORTA_CHANNEL_OFFEST && ChannelId < PORTx_CHANNEL_OFFSET)
    {
        // OK proceed
    }
    else
    {
        Det_ReportError(DIO_ModuleId, 0, DIO_ApiID_ReadChannel, DIO_E_PARAM_INVALID_CHANNEL_ID);
        return 0;
    }
#endif
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
    else if (ChannelId >= PORTC_CHANNEL_OFFEST &&
             ChannelId < PORTx_CHANNEL_OFFSET)
    {
        ChannelId -= PORTC_CHANNEL_OFFEST;
        channelValue = GET_BIT(GPIOC_IDR, ChannelId);
    }
    else
    {
        // Argument passing error
        // shouldn't be here
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
#if DioDevErrorDetect == TRUE
    if (ChannelId >= PORTA_CHANNEL_OFFEST && ChannelId < PORTx_CHANNEL_OFFSET)
    {
        // OK proceed
    }
    else
    {
        Det_ReportError(DIO_ModuleId, 0, DIO_ApiID_WriteChannel, DIO_E_PARAM_INVALID_CHANNEL_ID);
        return 0;
    }
#endif

    /* 
     * USE BSRR and BRR registers to provide atomic channel access 
     */
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
#if DioDevErrorDetect == TRUE
    if (PortId >= DIO_PORTTYPE_PORTA && PortId <= DIO_PORTTYPE_PORTC)
    {
        // OK proceed
    }
    else
    {
        Det_ReportError(DIO_ModuleId, 0, DIO_ApiID_ReadPort, DIO_E_PARAM_INVALID_PORT_ID);
        return 0;
    }
#endif

    /**
     * Note: Data is sampled into IDR Every APB2 clock sycle
     */
    Dio_PortLevelType ret_PortValue;

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
        ret_PortValue = 0; // Error occured in argument passing
        break;
    }
    return ret_PortValue;
}

/**
 * @name : Dio_WriteChannel
 * @param PortId: ID of DIO Port 
 * @param Level: Value to be written
 * Reentrancy: Reentrant
 * Sync
 * Description: Service to set a level of the port.
 **/
void Dio_WritePort(Dio_PortType PortId, Dio_PortLevelType Level)
{
#if DioDevErrorDetect == TRUE
    if (PortId >= DIO_PORTTYPE_PORTA && PortId <= DIO_PORTTYPE_PORTC)
    {
        // OK proceed
    }
    else
    {
        Det_ReportError(DIO_ModuleId, 0, DIO_ApiID_WritePort, DIO_E_PARAM_INVALID_PORT_ID);
        return 0;
    }
#endif

    /* Set the value of the output data register */

    /*
        Note: if a DIO pin is configured as input, 
        ODR is an open switch and doesn't affect the 
        physical pin level nor the value in the IDR register
    */
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
#if DioDevErrorDetect == TRUE
    // TODO
#endif
    Dio_PortLevelType subset_level;

    switch (ChannelGroupIdPtr->port)
    {
    case DIO_PORTTYPE_PORTA:
        subset_level = (GPIOA_IDR) & (ChannelGroupIdPtr->mask);
        break;
    case DIO_PORTTYPE_PORTB:
        subset_level = (GPIOB_IDR) & (ChannelGroupIdPtr->mask);
        break;
    case DIO_PORTTYPE_PORTC:
        subset_level = (GPIOC_IDR) & (ChannelGroupIdPtr->mask);
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

#if DioDevErrorDetect == TRUE
    // TODO
#endif
    uint32 subset_level;

    switch (ChannelGroupIdPtr->port)
    {
    case DIO_PORTTYPE_PORTA:
        subset_level = (GPIOA_IDR) & (~ChannelGroupIdPtr->mask); // reset channel group bits
        subset_level |= (Level << (ChannelGroupIdPtr->offset));  // assign chanel group value
        GPIOA_IDR = subset_level;                                // write new value to register
        break;
    case DIO_PORTTYPE_PORTB:
        subset_level = (GPIOB_IDR) & (~ChannelGroupIdPtr->mask); // reset channel group bits
        subset_level |= (Level << (ChannelGroupIdPtr->offset));  // assign chanel group value
        GPIOB_IDR = subset_level;                                // write new value to register
        break;
    case DIO_PORTTYPE_PORTC:
        subset_level = (GPIOC_IDR) & (~ChannelGroupIdPtr->mask); // reset channel group bits
        subset_level |= (Level << (ChannelGroupIdPtr->offset));  // assign chanel group value
        GPIOC_IDR = subset_level;                                // write new value to register
        break;
    default:
        break;
    }
}
#if DioVersionInfoApi == TRUE
/**
         * name : Dio_GetVersionInfo
         * param: VersionInfo: Pointer to where to store the version information of this module
         * Reentrancy: Reentrant
         * Sync
         * Description: Service to get the version information of this module.
         **/
void Dio_GetVersionInfo(Std_VersionInfoType *VersionInfo)
{
#if DioDevErrorDetect == TRUE
    if (VersionInfo == NULL_PTR)
    {
        Det_ReportError(DIO_ModuleId, 0, DIO_ApiID_GetVersionInfo, DIO_E_PARAM_POINTER);
        return ;
    }
    else
    {
        // Do nothing
    }
#endif
    VersionInfo->moduleID = DIO_ModuleId;
    VersionInfo->sw_major_version = DIO_SW_MAJOR_VERSION;
    VersionInfo->sw_minor_version = DIO_SW_MINOR_VERSION;
    VersionInfo->sw_patch_version = DIO_SW_PATCH_VERSION;
    VersionInfo->vendorID = DIO_VendorId;
}
#endif

#if DioFlipChannelApi == TRUE
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
Dio_LevelType Dio_FlipChannel(Dio_ChannelType ChannelId)
{
#if DioDevErrorDetect == TRUE
    if (ChannelId >= PORTA_CHANNEL_OFFEST && ChannelId < PORTx_CHANNEL_OFFSET)
    {
        // OK proceed
    }
    else
    {
        Det_ReportError(DIO_ModuleId, 0, DIO_ApiID_FlipChannel, DIO_E_PARAM_INVALID_CHANNEL_ID);
        return 0;
    }
#endif

    Dio_LevelType ret_Level;

    if (Dio_GetChannelMode(ChannelId) == Dio_ModeInput)
    {
        return Dio_ReadChannel(ChannelId);
    }
    else
    {
        if (Dio_ReadChannel(ChannelId) == STD_HIGH)
        {
            Dio_WriteChannel(ChannelId, STD_LOW);
            ret_Level = STD_LOW;
        }
        else
        {
            Dio_WriteChannel(ChannelId, STD_HIGH);
            ret_Level = STD_HIGH;
        }
    }
    return ret_Level;
}
#endif

/***************** Local Functions definition **************/

DIO_modeType Dio_GetChannelMode(Dio_ChannelType ChannelId)
{
    Dio_LevelType channelMode = 0;
    if (ChannelId >= PORTA_CHANNEL_OFFEST &&
        ChannelId < PORTB_CHANNEL_OFFEST)
    {
        if (ChannelId < 8)
        {
            if ((GPIOA_CRL) & (0x0003 << (ChannelId * 4) ) == 0x00)
            {
                channelMode = Dio_ModeInput;
            }
            else
            {
                channelMode = Dio_ModeOutput;
            }
        }
        else
        {
            ChannelId -= 8;
            if ((GPIOA_CRH) & (0x0003 << (ChannelId * 4)) == 0x00)
            {
                channelMode = Dio_ModeInput;
            }
            else
            {
                channelMode = Dio_ModeOutput;
            }
        }
    }
    else if (ChannelId >= PORTB_CHANNEL_OFFEST &&
             ChannelId < PORTC_CHANNEL_OFFEST)
    {
        ChannelId -= PORTB_CHANNEL_OFFEST;
        if (ChannelId < 8)
        {

            if (GPIOB_CRL & (0x0003 << (ChannelId * 4)) == 0x00)
            {
                channelMode = Dio_ModeInput;
            }
            else
            {
                channelMode = Dio_ModeOutput;
            }
        }
        else
        {
            ChannelId -=8;
            if (GPIOB_CRH & (0x0003 << (ChannelId * 4)) == 0x00)
            {
                channelMode = Dio_ModeInput;
            }
            else
            {
                channelMode = Dio_ModeOutput;
            }
        }
    }
    else if (ChannelId >= PORTC_CHANNEL_OFFEST &&
             ChannelId < PORTx_CHANNEL_OFFSET)
    {
        ChannelId -= PORTC_CHANNEL_OFFEST;
        if (ChannelId < 8)
        {
            if (GPIOC_CRL & (0x0003 << (ChannelId * 4)) == 0x00)
            {
                channelMode = Dio_ModeInput;
            }
            else
            {
                channelMode = Dio_ModeOutput;
            }
        }
        else
        {
            ChannelId -=8;
            if (GPIOC_CRH & (0x0003 << (ChannelId * 4)) == 0x00)
            {
                channelMode = Dio_ModeInput;
            }
            else
            {
                channelMode = Dio_ModeOutput;
            }
        }
    }
    else
    {
        // Argument passing error
        // shouldn't be here
    }
    return channelMode;
}