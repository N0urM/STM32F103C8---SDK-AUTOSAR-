/******************************************************/
/* Author    : Nourhan Mansour                        */
/* Date      : 20/2/2021                              */
/* Version   : 1.1.0                                  */
/* File      : DIO_cfg.h                              */
/* Target    : STM32F103C8                            */
/* AUTOSAR Version : 4.3.1                            */
/******************************************************/

#ifndef DIO_CFG_H
#define DIO_CFG_H


#define Dio_GroupArraySize      (4U)

// Parent container : DioGeneral
#define DioDevErrorDetect       TRUE
#define DioFlipChannelApi       TRUE
#define DioVersionInfoApi       TRUE

// Parent container : DioPort 
#define DIO_PORTTYPE_PORTA      (0U)
#define DIO_PORTTYPE_PORTB      (1U)
#define DIO_PORTTYPE_PORTC      (2U)

// Parent container: DioChannel 
    // PORTA channels
#define  DIO_CHANNEL_A0         (0U)
#define  DIO_CHANNEL_A1         (1U)
#define  DIO_CHANNEL_A2         (2U)
#define  DIO_CHANNEL_A3         (3U)
#define  DIO_CHANNEL_A4         (4U)
#define  DIO_CHANNEL_A5         (5U)
#define  DIO_CHANNEL_A6         (6U)
#define  DIO_CHANNEL_A7         (7U)
#define  DIO_CHANNEL_A8         (8U)
#define  DIO_CHANNEL_A9         (9U)
#define  DIO_CHANNEL_A10        (10U)
#define  DIO_CHANNEL_A11        (11U)
#define  DIO_CHANNEL_A12        (12U)
#define  DIO_CHANNEL_A15        (15U)
     // PORTB channles 
#define  DIO_CHANNEL_B0         (16U)
#define  DIO_CHANNEL_B1         (17U)
#define  DIO_CHANNEL_B3         (19U)
#define  DIO_CHANNEL_B4         (20U)
#define  DIO_CHANNEL_B5         (21U)
#define  DIO_CHANNEL_B6         (22U)
#define  DIO_CHANNEL_B7         (23U)
#define  DIO_CHANNEL_B8         (24U)
#define  DIO_CHANNEL_B9         (25U)
#define  DIO_CHANNEL_B10        (26U)
#define  DIO_CHANNEL_B11        (27U)
#define  DIO_CHANNEL_B12        (28U)
#define  DIO_CHANNEL_B13        (29U)
#define  DIO_CHANNEL_B14        (30U)
#define  DIO_CHANNEL_B15        (31U) 
    // PORTC channels
#define  DIO_CHANNEL_C13        (45U)
#define  DIO_CHANNEL_C14        (46U)
#define  DIO_CHANNEL_C15        (47U)

#endif      // EOF