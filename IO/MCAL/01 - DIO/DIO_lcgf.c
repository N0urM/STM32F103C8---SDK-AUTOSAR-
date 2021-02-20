/******************************************************/
/* Author    : Nourhan Mansour                        */
/* Date      : 20/2/2021                              */
/* Version   : 1.1.0                                  */
/* File      : DIO_lcfg.h                             */
/* Target    : STM32F103C8                            */
/* AUTOSAR Version : 4.3.1                            */
/******************************************************/

#ifndef DIO_LCFG_H
#define DIO_LCFG_H

#include "DIO.h"

Dio_ChannelGroupType MyDioGroupArray[Dio_GroupArraySize] = 
{
    {0x0011 , 0x00  , DIO_PORTTYPE_PORTA },
    {0x0010 , 0x02  , DIO_PORTTYPE_PORTB },
    {0x0028 , 0x01  , DIO_PORTTYPE_PORTA },
    {0x0005 , 0x03  , DIO_PORTTYPE_PORTC }
}
#endif
