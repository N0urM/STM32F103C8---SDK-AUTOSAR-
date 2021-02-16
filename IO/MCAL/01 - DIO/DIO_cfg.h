#ifndef DIO_CFG_H
#define DIO_CFG_H

// Parent container : DioGeneral
#define DioDevErrorDetect       FALSE
#define DioFlipChannelApi       TRUE
#define DioVersionInfoApi       FALSE

// Parent container : DioPort 
#define DIO_PORTTYPE_PORTA      (0U)
#define DIO_PORTTYPE_PORTB      (1U)
#define DIO_PORTTYPE_PORTC      (2U)

// Parent container: DioChannel 
    // PORTA channels
#define  DIO_CHANNEL_A0         0
#define  DIO_CHANNEL_A1         1
#define  DIO_CHANNEL_A2         2
#define  DIO_CHANNEL_A3         3
#define  DIO_CHANNEL_A4         4
#define  DIO_CHANNEL_A5         5
#define  DIO_CHANNEL_A6         6
#define  DIO_CHANNEL_A7         7
#define  DIO_CHANNEL_A8         8
#define  DIO_CHANNEL_A9         9
#define  DIO_CHANNEL_A10        10
#define  DIO_CHANNEL_A11        11
#define  DIO_CHANNEL_A12        12
#define  DIO_CHANNEL_A15        15
     // PORTB channles 
#define  DIO_CHANNEL_B0         16
#define  DIO_CHANNEL_B1         17
#define  DIO_CHANNEL_B3         19
#define  DIO_CHANNEL_B4         20
#define  DIO_CHANNEL_B5         21
#define  DIO_CHANNEL_B6         22
#define  DIO_CHANNEL_B7         23
#define  DIO_CHANNEL_B8         24
#define  DIO_CHANNEL_B9         25
#define  DIO_CHANNEL_B10        26
#define  DIO_CHANNEL_B11        27
#define  DIO_CHANNEL_B12        28
#define  DIO_CHANNEL_B13        29
#define  DIO_CHANNEL_B14        30
#define  DIO_CHANNEL_B15        31 
    // PORTC channels
#define  DIO_CHANNEL_C13        45
#define  DIO_CHANNEL_C14        46
#define  DIO_CHANNEL_C15        47

#endif      // EOF