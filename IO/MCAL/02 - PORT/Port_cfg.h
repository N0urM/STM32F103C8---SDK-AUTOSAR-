/******************************************************/
/* Author    : Nourhan Mansour                        */
/* Date      : 31/3/2021                              */
/* Version   : 1.1.1                                  */
/* File      : Port_cfg.h                             */
/******************************************************/
#ifndef PORT_CFG_H
#define PORT_CFG_H

// PORTS OFFSETS 
#define PORTA_OFFSET                   ( 0U)
#define PORTB_OFFSET                   (16U)
#define PORTC_OFFSET                   (32U)

// Parent Conatiner: Port_PinModeType
#define PORT_PIN_MODE_DIO               0x00
#define PORT_PIN_MODE_ADC               0x01
#define PORT_PIN_MODE_PWM               0x02
#define PORT_PIN_MODE_SPI               0x03
#define PORT_PIN_MODE_CAN               0x04
#define PORT_PIN_MODE_DIO_GPT           0x05
#define PORT_PIN_MODE_DIO_WDG           0x06

#define PORT_MAX_PINS_NUM               (48U)

// Port Pins symbolic names 
#define PORT_A_PIN_0                    (0U)
#define PORT_A_PIN_1                    (1U)
#define PORT_A_PIN_2                    (2U)
#define PORT_A_PIN_3                    (3U)
#define PORT_A_PIN_4                    (4U)
#define PORT_A_PIN_5                    (5U)
#define PORT_A_PIN_6                    (6U)
#define PORT_A_PIN_7                    (7U)
#define PORT_A_PIN_8                    (8U)
#define PORT_A_PIN_9                    (9U)
#define PORT_A_PIN_10                   (10U)
#define PORT_A_PIN_11                   (11U)
#define PORT_A_PIN_12                   (12U)
#define PORT_A_PIN_15                   (15U)

#define  PORT_B_PIN_0                    (16U)
#define  PORT_B_PIN_1                    (17U)
#define  PORT_B_PIN_3                    (19U)
#define  PORT_B_PIN_4                    (20U)
#define  PORT_B_PIN_5                    (21U)
#define  PORT_B_PIN_6                    (22U)
#define  PORT_B_PIN_7                    (23U)
#define  PORT_B_PIN_8                    (24U)
#define  PORT_B_PIN_9                    (25U)
#define  PORT_B_PIN_10                   (26U)
#define  PORT_B_PIN_11                   (27U)
#define  PORT_B_PIN_12                   (28U)
#define  PORT_B_PIN_13                   (29U)
#define  PORT_B_PIN_14                   (30U)
#define  PORT_B_PIN_15                   (31U) 

#define  PORT_C_PIN_13                   (45U)
#define  PORT_C_PIN_14                   (46U)
#define  PORT_C_PIN_15                   (47U)

/*** Configutration Params ***/
#define  PortSetPinDirectionApi          
#define  PortDevErrorDetect              
#define  PortSetPinModeApi               
#define  PortVersionInfoApi           


#endif 