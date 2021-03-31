/******************************************************/
/* Author    : Nourhan Mansour                        */
/* Date      : 30/3/2021                              */
/* Version   : 1.1.0                                  */
/* File      : Port.c                                 */
/******************************************************/
#include "hw_reg.h"
#include "det.h"
#include "Port.h"

/************************************************************************/
/*                          Local functions                             */
/************************************************************************/
static void PortLocal_SetPinDirection(Port_PinType Pin, Port_PinDirectionType Direction);
static void PortLocal_SetPinMode(Port_PinType Pin, Port_PinModeType Mode);
static void PortLocal_SetPinInitialValue(Port_PinType Pin, uint8 Pin_Level);
static void PorttLocal_ActivateInternalPullUp(Port_PinType Pin);

/************************************************************************/
/*                          Local variables                             */
/************************************************************************/
Port_ConfigType *local_PortConfigPtr = NULL_PTR;

/************************************************************************/
/*                         APIS definitions                             */
/************************************************************************/
/**
 * @name : Port_Init
 * @param: ConfigPtr: Pointer to configuration set.
 * Service ID : 0x00 
 * Non Reentrant
 * Sync
 * Initializes the Port Driver module.
 * Note: call the function Port_Init first in order to initialize the port for use
 **/
void Port_Init(const Port_ConfigType *ConfigPtr)
{
    uint8 idx = 0;
    uint8 portNum = 0;
#ifdef PortDevErrorDetect
    if (ConfigPtr == NULL_PTR)
    {
        Det_ReportError(PORT_ModuleId, 0x00, 0x00, PORT_E_INIT_FAILED);
    }
    else
    {
        // ok
    }
#endif
    // Store configuration array
    local_PortConfigPtr = ConfigPtr;

    // loop on all available mcu pins
    for (idx = 0; idx < PORT_MAX_PINS_NUM; idx)
    {
        // set port pin direction
        PortLocal_SetPinDirection((ConfigPtr->PortPinsConfig[idx]).PortPinId,
                                  (ConfigPtr->PortPinsConfig[idx]).PortPinDirection);
        // configure port pin mode
        PortLocal_SetPinMode((ConfigPtr->PortPinsConfig[idx]).PortPinId,
                             (ConfigPtr->PortPinsConfig[idx]).PortPinInitialMode);
        // set initial value
        if (((ConfigPtr->PortPinsConfig[idx]).PortPinInitialMode | 0b1100) != 0b1100) // make sure pin is not input
        {
            PortLocal_SetPinInitialValue((ConfigPtr->PortPinsConfig[idx]).PortPinId,
                                         (ConfigPtr->PortPinsConfig[idx]).PortPinLevelValue);
        }
        // Activate / Deactivate internal pull up
        if (((ConfigPtr->PortPinsConfig[idx]).PortPinInitialMode | 0b1100) == 0b1100) // make sure pin is input
        {
            if ((ConfigPtr->PortPinsConfig[idx]).Pin_Internal_PullUp == STD_ON)
            {
                PorttLocal_ActivateInternalPullUp((ConfigPtr->PortPinsConfig[idx]).PortPinId);
            }
            else
            {
                // Do nothing
            }
        }
        else
        {
            // Do nothing
        }
    }
    return;
}

#ifdef PortSetPinDirectionApi

/***
 * @name : Port_SetPinDirection
 * @param Pin: Port Pin ID number
 * @param Direction: Port Pin Direction
 * Service ID : 0x01
 * Reentrant
 * Sync
 * Sets the port pin direction
 **/
void Port_SetPinDirection(Port_PinType Pin, Port_PinDirectionType Direction)
{
    uint8 idx = 0;
#ifdef PortDevErrorDetect
    if (Pin >= PORT_MAX_PINS_NUM)
    {
        Det_ReportError(PORT_ModuleId, 0x00, 0x01, PORT_E_PARAM_PIN);
    }
    if (local_PortConfigPtr == NULL_PTR)
    {
        Det_ReportError(PORT_ModuleId, 0x00, 0x01, PORT_E_UNINIT);
    }
#endif

    for (idx = 0; idx < PORT_MAX_PINS_NUM; idx++)
    {
        if ((local_PortConfigPtr->PortPinsConfig[idx]).PortPinId == Pin)
        {
            if ((local_PortConfigPtr->PortPinsConfig[idx]).PortPinDirectionChangeable == STD_ON)
            {
                PortLocal_SetPinDirection(Pin, Direction);
            }
            else
            {
#ifdef PortDevErrorDetect
                Det_ReportError(PORT_ModuleId, 0x00, 0x01, PORT_E_DIRECTION_UNCHANGEABLE);
#endif
            }
            break;
        }
    }

    return;
}
#endif

#ifdef PortSetPinModeApi

/***
 * @name : Port_SetPinMode
 * @param Pin: Port Pin ID number
 * @param Mode: Port Pin Mode
 * Service ID : 0x04
 * Reentrant
 * Sync
 * Change the port pin mode, if available
 **/
void Port_SetPinMode(Port_PinType Pin, Port_PinModeType Mode)
{
    uint8 idx = 0;
#ifdef PortDevErrorDetect
    if (Pin >= PORT_MAX_PINS_NUM)
    {
        Det_ReportError(PORT_ModuleId, 0x00, 0x04, PORT_E_PARAM_PIN);
    }
    if (local_PortConfigPtr == NULL_PTR)
    {
        Det_ReportError(PORT_ModuleId, 0x00, 0x04, PORT_E_UNINIT);
    }
#endif

    for (idx = 0; idx < PORT_MAX_PINS_NUM; idx++)
    {
        if ((local_PortConfigPtr->PortPinsConfig[idx]).PortPinId == Pin)
        {
            if ((local_PortConfigPtr->PortPinsConfig[idx]).PortPinModeChangeable == STD_ON)
            {
                PortLocal_SetPinMode(Pin, Mode);
            }
            else
            {
#ifdef PortDevErrorDetect
                Det_ReportError(PORT_ModuleId, 0x00, 0x04, PORT_E_MODE_UNCHANGEABLE);
#endif
            }
            break;
        }
    }
    return;
}
#endif

#ifdef PortVersionInfoApi
/**
* @name : Port_GetVersionInfo
* @param VersionInfo: Pointer to where to store the version information of this module
* Service ID : 0x03
* Reentrancy: Reentrant
* Sync
* Description: Service to get the version information of this module.
**/
void Port_GetVersionInfo(Std_VersionInfoType *VersionInfo)
{
#if DioDevErrorDetect == TRUE
    if (VersionInfo == NULL_PTR)
    {
        Det_ReportError(DIO_ModuleId, 0, DIO_ApiID_GetVersionInfo, DIO_E_PARAM_POINTER);
        return;
    }
    else
    {
        // Do nothing
    }
#endif
    VersionInfo->moduleID = PORT_ModuleId;
    VersionInfo->sw_major_version = PORT_SW_MAJOR_VERSION;
    VersionInfo->sw_minor_version = PORT_SW_MINOR_VERSION;
    VersionInfo->sw_patch_version = PORT_SW_PATCH_VERSION;
    VersionInfo->vendorID = PORT_VendorId;
    return;
}
#endif

/**
* @name : Port_RefreshPortDirection
* @param void
* Reentrancy: Non Reentrant
Service ID : 0x02
* Sync
* Description: refresh the direction of all configured ports to the configured direction
* exclude those port pins from refreshing that are configured as ‘pin direction changeable during runtime.
**/
void Port_RefreshPortDirection(void)
{
#ifdef PortDevErrorDetect
    if (local_PortConfigPtr == NULL_PTR)
    {
        Det_ReportError(PORT_ModuleId, 0x00, 0x02, PORT_E_UNINIT);
    }
#endif
    uint8 idx = 0;
    for (idx = 0; idx < PORT_MAX_PINS_NUM; idx++)
    {
        if ((local_PortConfigPtr->PortPinsConfig[idx]).PortPinDirectionChangeable == STD_OFF)
        {
            // set port pin direction
            PortLocal_SetPinDirection((local_PortConfigPtr->PortPinsConfig[idx]).PortPinId,
                                      (local_PortConfigPtr->PortPinsConfig[idx]).PortPinDirection);
        }
    }
    return;
}

/************************************************************************/
/*                            Local Functions                           */
/************************************************************************/

/***
 * @name : PortLocal_SetPinDirection
 * @param Pin: Port Pin ID number
 * @param Direction: Port Pin Direction
 * Reentrant
 * Sync
 * Sets the port pin direction
 **/
static void PortLocal_SetPinDirection(Port_PinType Pin, Port_PinDirectionType Direction)
{
    if (Pin >= PORT_A_PIN_0 && Pin < PORT_A_PIN_8)
    {
        Pin -= PORTA_OFFSET;
        GPIOA_CRL &= ~((0b1111) << (Pin * 4)); // Clear configuration bits
        GPIOA_CRL |= (Direction << (Pin * 4)); // Set configuration
    }
    else if (Pin >= PORT_A_PIN_8 && Pin <= PORT_A_PIN_15)
    {
        Pin -= PORTA_OFFSET;
        GPIOA_CRH &= ~((0b1111) << ((Pin - 8) * 4)); // Clear configuration bits
        GPIOA_CRH |= (Direction << ((Pin - 8) * 4)); // Set configuration
    }

    else if (Pin >= PORT_B_PIN_0 && Pin < PORT_B_PIN_8)
    {
        Pin -= PORTB_OFFSET;
        GPIOB_CRL &= ~((0b1111) << (Pin * 4)); // Clear configuration bits
        GPIOB_CRL |= (Direction << (Pin * 4)); // Set configuration
    }
    else if (Pin >= PORT_B_PIN_8 && Pin <= PORT_B_PIN_15)
    {
        Pin -= PORTB_OFFSET;
        GPIOB_CRH &= ~((0b1111) << ((Pin - 8) * 4)); // Clear configuration bits
        GPIOB_CRH |= (Direction << ((Pin - 8) * 4)); // Set configuration
    }
    else if (Pin >= PORT_C_PIN_13 && Pin <= PORT_C_PIN_15)
    {
        Pin -= PORTC_OFFSET;
        GPIOC_CRH &= ~((0b1111) << ((Pin - 8) * 4)); // Clear configuration bits
        GPIOC_CRH |= (Direction << ((Pin - 8) * 4)); // Set configuration
    }
    else
    {
        // invalid Port pin
    }
    return;
}

/***
 * @name : PortLocal_SetPinInitialValue
 * @param Pin: Port Pin ID number
 * @param Pin_Level: Port Pin Level STD_HIGH/ STD_LOW
 * Reentrant
 * Sync
 * Sets the port pin initial level
 **/
static void PortLocal_SetPinInitialValue(Port_PinType Pin, uint8 Pin_Level)
{
    if (Pin < PORTB_OFFSET)
    {
        switch (Pin_Level)
        {
        case STD_HIGH:
            SET_BIT(GPIOA_BSRR, Pin); // Set Channel
            break;
        case STD_LOW:
            SET_BIT(GPIOA_BRR, Pin); // Reset Channel
            break;
        default:
            // shouldn't be here
            break;
        }
    }
    else if (Pin < PORTC_OFFSET)
    {
        Pin -= PORTB_OFFSET;
        switch (Pin_Level)
        {
        case STD_HIGH:
            SET_BIT(GPIOB_BSRR, Pin); // Set Channel
            break;
        case STD_LOW:
            SET_BIT(GPIOB_BRR, Pin); // Reset Channel
            break;
        default:
            // shouldn't be here
            break;
        }
    }
    else
    {
        Pin -= PORTC_OFFSET;
        switch (Pin_Level)
        {
        case STD_HIGH:
            SET_BIT(GPIOC_BSRR, Pin); // Set Channel
            break;
        case STD_LOW:
            SET_BIT(GPIOC_BRR, Pin); // Reset Channel
            break;
        default:
            // shouldn't be here
            break;
        }
    }
    return;
}

/***
 * @name : PorttLocal_ActivateInternalPullUp
 * @param Pin: Port Pin ID number
 * Reentrant
 * Sync
 * Activate the port pin internal pull up 
 **/
static void PorttLocal_ActivateInternalPullUp(Port_PinType Pin)
{
    if (Pin < PORTB_OFFSET)
    {
        SET_BIT(GPIOA_ODR, Pin);
    }
    else if (Pin < PORTC_OFFSET)
    {
        Pin -= PORTB_OFFSET;
        SET_BIT(GPIOB_ODR, Pin);
    }
    else
    {
        Pin -= PORTC_OFFSET;
        SET_BIT(GPIOC_ODR, Pin);
    }
    return;
}

/***
 * @name : PortLocal_SetPinMode
 * @param Pin: Port Pin ID number
 * @param Mode: Port Pin mode (SPI / DIO / ..)
 * Reentrant
 * Sync
 * Configure port registers bits for the required pin mode.  
 **/
static void PortLocal_SetPinMode(Port_PinType Pin, Port_PinModeType Mode)
{
    switch (Mode)
    {
    case PORT_PIN_MODE_DIO:
        // Do nothing
        break;
    case PORT_PIN_MODE_SPI:
        // Do nothing
        break;
    case PORT_PIN_MODE_ADC:
        // TO DO
        break;
    case PORT_PIN_MODE_CAN:
        // TO DO
        break;
    case PORT_PIN_MODE_PWM:
        // TO DO
        break;
    default:
// Shouldn't be here - error
#ifdef PortDevErrorDetect
        // do something
#endif
        break;
    }

    return;
}
