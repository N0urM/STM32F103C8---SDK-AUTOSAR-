/************************************************************************/
/* Author    : Nourhan Mansour                                          */
/* Date      : 13/4/2021                                                */
/* Version   : 1.1.0                                                    */
/* File      : spi_cfg.h                                                */
/************************************************************************/

#ifndef SPI_CFG_H
#define SPI_CFG_H

// SpiDemEventParameterRefs Container

// Reference to configured DEM event to report "Hardware failure". 
// If the reference is not configured the error shall not be reported.
#define SPI_E_HARDWARE_ERROR

///////////////////////////////////////////////////////////////////////////////////////////////////

// SPI GENERAL Container 

// Switches the Spi_Cancel function ON or OFF.
#define SpiCancelApi

// Selects the SPI Handler/Driver Channel Buffers usage allowed and delivered. 
// IB = 0; EB = 1; IB/EB = 2;
#define SpiChannelBufferIB          0U
#define SpiChannelBufferEB          1U
#define SpiChannelBufferIB_EB       2U
#define SpiChannelBuffersAllowed    SpiChannelBufferIB_EB

// Switches the development error detection and notification on or off.
#define SpiDevErrorDetect

// Switches the Spi_GetHWUnitStatus function ON or OFF.
#define SpiHwStatusApi

/*  Switches the Interruptible Sequences handling functionality ON or OFF.
    Dependency: dependency: This parameter depends on SPI_LEVEL_DELIVERED value. 
    It is only used for SPI_LEVEL_DELIVERED configured to 1 or 2. 
*/
#define SpiInterruptibleSeqAllowed

/*
    Selects the SPI Handler/Driver level of scalable functionality that is
    available and delivered.
    Range: 0 ~ 2
*/
#define SpiLevelDelivered           0U
/*
    This parameter defines the cycle time of the function Spi_MainFunction_Handling in seconds. 
    The parameter is not used by the driver it self, but it is used by upper layer.
    Type: EcucFloatParamDef
*/
#define SpiMainFunctionPeriod       0.5

// Specifies whether concurrent Spi_SyncTransmit() calls for different se-quences shall be configurable.
#undef SpiSupportConcurrentSyncTransmit
/*
    Header file name which will be included by the Spi.
    The parameter value MUST NOT represent a path
    Type: EcucStringParamDef
*/
#define SpiUserCallbackHeaderFile       "PATH.h"

// Switches the Spi_GetVersionInfo function ON or OFF.
#define SpiVersionInfoApi								TRUE

///////////////////////////////////////////////////////////////////////////////////////////////////

// Spi Sequence Container

// This parameter allows or not this Sequence to be suspended by another one.
#undef SpiInterruptibleSequence

// This parameter is a reference to a notification function.
#define SpiSeqEndNotification

// SPI Sequence ID, used as parameter in SPI API functions.
// 0 ~ 255
#define SpiSequenceId           1U

// A sequence references several jobs, which are executed during a commu-nication sequence
// Type: Reference to [ SpiJob ]
#define SpiJobAssignment

///////////////////////////////////////////////////////////////////////////////////////////////////

// SpiChannel Container 

//the maximum size (number of data elements) of data buffers in case of EB Channels.
#define SpiEbMaxLength      10U

// the maximum number of data buffers in case of IB Channels and only.
#define SpiIbNBuffers       1U

// Spi Driver Container

// No. of configured channels 
// Range: 0 - 255
#define SpiMaxChannel       10U

// No. of configured jobs 
// Range: 0 - 65535
#define SpiMaxJob           1U

// No. of configured sequnces 
// Range: 0 - 255
#define SpiMaxSequence       1U

#endif
