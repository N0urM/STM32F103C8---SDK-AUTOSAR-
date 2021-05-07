/************************************************************************/
/* Author    : Nourhan Mansour                                          */
/* Date      : 30/3/2021                                                */
/* Version   : 1.1.0                                                    */
/* File      : Port.h                                                   */
/************************************************************************/
#ifndef PORT_H
#define PORT_H

#include "STD_TYPES.h"
#include "BIT_MATH.h"
#include "Port_cfg.h"


/************************************************************************/
/*                          Published Info                              */
/************************************************************************/

#define PORT_ModuleId                    (124U)
#define PORT_VendorId                    (483U)

#define PORT_AR_RELEASE_MAJOR_VERSION    (1U)
#define PORT_AR_RELEASE_MINOR_VERSION    (1U)
#define PORT_AR_RELEASE_REVISION_VERSION (0U)

#define PORT_SW_MAJOR_VERSION            (4U)
#define PORT_SW_MINOR_VERSION            (3U)
#define PORT_SW_PATCH_VERSION            (1U)

/************************************************************************/
/*                                DET                                   */
/************************************************************************/

#define PORT_E_PARAM_PIN                    0x0A
#define PORT_E_DIRECTION_UNCHANGEABLE       0x0B
#define PORT_E_INIT_FAILED                  0x0C
#define PORT_E_PARAM_INVALID_MODE           0x0D
#define PORT_E_MODE_UNCHANGEABLE            0x0E
#define PORT_E_UNINIT                       0x0F
#define PORT_E_PARAM_POINTER                0x10


/************************************************************************/
/*                            Type Definitons                           */
/************************************************************************/
typedef enum 
{
  PORT_PIN_INPUT_ANALOG    		      =0b0000,
  PORT_PIN_INPUT_FLOATING  		      =0b0100,
  PORT_PIN_INPUT_PULL_UP_DOWN  	    =0b1000,

  PORT_PIN_OUTPUT_PUSH_PULL_10MHZ   =0b0001,
  PORT_PIN_OUTPUT_OPEN_DRAIN_10MHZ  =0b0101,
  PORT_PIN_AF_PUSH_PULL_10MHZ       =0b1001,
  PORT_PIN_AF_OPEN_DRAIN_10MHZ      =0b1101,

  PORT_PIN_OUTPUT_PUSH_PULL_50MHZ   =0b0011,
  PORT_PIN_OUTPUT_OPEN_DRAIN_50MHZ  =0b0111,
  PORT_PIN_AF_PUSH_PULL_50MHZ       =0b1011,
  PORT_PIN_AF_OPEN_DRAIN_50MHZ      =0b1111,

  PORT_PIN_OUTPUT_PUSH_PULL_2MHZ    =0b0010,
  PORT_PIN_OUTPUT_OPEN_DRAIN_2MHZ   =0b0110,
  PORT_PIN_AF_PUSH_PULL_2MHZ        =0b1010,
  PORT_PIN_AF_OPEN_DRAIN_2MHZ       =0b1110,

}Port_PinDirectionType;

typedef uint8 Port_PinType;                 // Symbolic name of a port pin

typedef uint8  Port_PinModeType;            // DIO , SPI, CAN, ... 

typedef struct 
{
  Port_PinType PortPinId;                     // Symbolic name derived from the port pin container
  Port_PinDirectionType PortPinDirection;     // Pin direction (input, output), Mandatory for IO
  boolean PortPinDirectionChangeable;         // STD_ON/STD_OFF
  Port_PinModeType PortPinInitialMode;        // Pin mode (e.g. DIO, SPI …)
  boolean PortPinModeChangeable;              // STD_ON/STD_OFF
  uint8 PortPinLevelValue;                    // Initial value: STD_LOW/STD_HIGH mandatory when pin is used for DIO
  boolean Pin_Internal_PullUp;                // STD_ON/STD_OFF
  
}PortPinConfigType;

typedef struct 
{
  PortPinConfigType PortPinsConfig[PORT_MAX_PINS_NUM];
}Port_ConfigType;


/************************************************************************/
/*                       Functions Definitons                           */
/************************************************************************/

void Port_Init( const Port_ConfigType* ConfigPtr );

void Port_RefreshPortDirection( void );

#ifdef PortSetPinDirectionApi
  void Port_SetPinDirection( Port_PinType Pin, Port_PinDirectionType Direction );
#endif

#ifdef PortSetPinModeApi
  void Port_SetPinMode( Port_PinType Pin, Port_PinModeType Mode );
#endif

#ifdef PortVersionInfoApi
  void Port_GetVersionInfo( Std_VersionInfoType* versioninfo );
#endif

#endif // EOF
