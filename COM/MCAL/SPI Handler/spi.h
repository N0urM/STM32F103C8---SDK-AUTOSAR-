/************************************************************************/
/* Author    : Nourhan Mansour                                          */
/* Date      : 23/4/2021                                                */
/* Version   : 1.1.1                                                    */
/* File      : spi.h                                                    */
/************************************************************************/

#ifndef SPI_H
#define SPI_H

#include "STD_TYPES.h"
#include "Dem_Types.h"
#include "spi_Cfg.h"


/************************************************************************/
/*                          Published Info                              */
/************************************************************************/

#define SPI_ModuleId                    (038U)
#define SPI_VendorId                    (483U)

#define SPI_AR_RELEASE_MAJOR_VERSION    (1U)
#define SPI_AR_RELEASE_MINOR_VERSION    (1U)
#define SPI_AR_RELEASE_REVISION_VERSION (0U)

#define SPI_SW_MAJOR_VERSION            (4U)
#define SPI_SW_MINOR_VERSION            (3U)
#define SPI_SW_PATCH_VERSION            (1U)

/************************************************************************/
/*                                DET                                   */
/************************************************************************/
#define SPI_E_PARAM_CHANNEL             0x0A
#define SPI_E_PARAM_JOB                 0x0B
#define SPI_E_PARAM_SEQ                 0x0C
#define SPI_E_PARAM_LENGTH              0x0D
#define SPI_E_PARAM_UNIT                0x0E
#define SPI_E_PARAM_POINTER             0x10    
#define SPI_E_UNINIT                    0x1A
#define SPI_E_ALREADY_INITIALIZED       0x4A


/************************************************************************/
/*                          RUN TIME ERRORS                             */
/************************************************************************/
#define SPI_E_SEQ_PENDING               0x2A
#define SPI_E_SEQ_IN_PROCESS            0x3A

/************************************************************************/
/*                         EXTENDED ERRORS                              */
/************************************************************************/

// This Extended Production Error shall be issued when any error bit 
// inside the SPI hardware transmit status register is raised
#define SPI_E_HARDWARE_ERROR            

/************************************************************************/
/*                         Module Constants                             */
/************************************************************************/

// SPI Status
//////////////////////////////////////////////////////////////////
// The SPI Handler/Driver is not initialized or not usable.
#define  SPI_UNINIT         0x00

// The SPI Handler/Driver is not currently transmitting any Job.
#define  SPI_IDLE           0x01

//The SPI Handler/Driver is performing a SPI Job (trans-mit).
#define SPI_BUSY            0x02
////////////////////////////////////////////////////////////////////

// Spi Job Result 
////////////////////////////////////////////////////////////////////
// The last transmission of the Job has been finished success-fully.
#define SPI_JOB_OK          0x00

// The SPI Handler/Driver is performing a SPI Job. The mean-ing of this status is equal to SPI_BUSY.
#define SPI_JOB_PENDING     0x01

//The last transmission of the Job has failed.
#define SPI_JOB_FAILED      0x02

//An asynchronous transmit Job has been accepted, while actual transmission for this Job has not started yet.
#define SPI_JOB_QUEUED      0x03
////////////////////////////////////////////////////////////////////

// Spi Sequence Result 
////////////////////////////////////////////////////////////////////
// The last transmission of the sequence has been finished success-fully.
#define SPI_SEQ_OK          0x00

// The SPI Handler/Driver is performing a SPI Sequence.. The mean-ing of this status is equal to SPI_BUSY.
#define SPI_SEQ_PENDING     0x01

//The last transmission of the Sequence has failed.
#define SPI_SEQ_FAILED      0x02

//An asynchronous transmit Job has been cancelled by user
#define SPI_SEQ_CANCELLED      0x03
////////////////////////////////////////////////////////////////////

// SPI ASYNC MODE
////////////////////////////////////////////////////////////////////
#define SPI_POLLING_MODE            0x00 
#define SPI_INTERRUPT_MODE          0x01        
////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////
#define SPI_CHANNEL_MAX_DATA_BUFFER     0x05     // Min Value = 1 (0 makes no sense)

#define SPI1_HW_UNIT                    0x01
#define SPI2_HW_UNIT                    0x02      

/* Master Slave Selection */
#define SPI_MASTER_MODE                 1U
#define SPI_SLAVE_MODE                  0U

/* Clk Polarity & phase  for either master or slave*/
/*
    Logic 0 when Idle = 0
    Logic 1 when Idle = 1
*/
#define SPI_CLK_POL_LOW             0U
#define SPI_CLK_POL_HIGH            1U
/*
    First data Transition in first clock  = 0
    First data Transition in second clock = 1 
*/
#define SPI_CLK_PHASE_FIRST         0U 
#define SPI_CLK_PHASE_SECOND        1U

/* LSB FIRST = 1 
   MSB FIRST = 0
*/
#define SPI_LSB_FIRST                   0U
#define SPI_LSB_LAST                    1U

/* 8  Bit Data Frame  = 0 
   16 Bit Data Frame  = 1
*/
#define SPI_DFF_MODE_8Bit               0U
#define SPI_DFF_MODE_16Bit              1U

// Baud Rate 
#define SPI_BAUD_RATE_CLK_DIV2       0U
#define SPI_BAUD_RATE_CLK_DIV4       1U
#define SPI_BAUD_RATE_CLK_DIV8       2U
#define SPI_BAUD_RATE_CLK_DIV16      3U
#define SPI_BAUD_RATE_CLK_DIV32      4U
#define SPI_BAUD_RATE_CLK_DIV64      5U  
#define SPI_BAUD_RATE_CLK_DIV128     6U
#define SPI_BAUD_RATE_CLK_DIV256     7U

// Piriority 
#define SPi_JobPiriority0           0U
#define SPi_JobPiriority1           1U
#define SPi_JobPiriority2           2U
#define SPi_JobPiriority3           3U


///////////////////////////////////////
// CS PINS
///////////////////////////////////////
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

/************************************************************************/
/*                            Type Definitons                           */
/************************************************************************/

// SPI_MASTER_MODE / SPI_SLAVE_MODE
typedef uint8 Spi_ModeType;

// SPI_CLK_POL_LOW / SPI_CLK_POL_HIGH
typedef boolean Spi_ClkPolType; 

// SPI_CLK_PHASE_FIRST / SPI_CLK_PHASE_Second
typedef boolean Spi_ClkPhaseType; 

// SPI_DFF_MODE_8Bit / SPI_DFF_MODE_16Bit
typedef uint8 Spi_DffType; 

// SPI_LSB_FIRST / SPI_LSB_LAST
typedef boolean Spi_LSBFirstType; 

// SPIBAUD_RATE_CLK_DIVx
typedef uint8 Spi_BaudRateType; 

// SPI1_HW_UNIT / SPI2_HW_UNIT
typedef uint8 Spi_HWunitType; 

// DIO_CHANNEL_xx
typedef uint8 SPi_CS_Pin;

// Range : SPi_JobPiriority0 (lowest) to SPi_JobPiriority3 (Highest) 
typedef uint8 Spi_JobPiriority; 

/* Enumeration
    Range: 
    SPI_UNINIT      (Default)
    SPI_IDLE
    SPI_BUSY
*/ 
typedef uint8 Spi_StatusType; 

/* Specifies the asynchronous mechanism mode for 
     SPI busses handled asynchro-nously in LEVEL 2.
    Enumeration Range: 
    SPI_POLLING_MODE
    SPI_INTERRUPT_MODE
*/
typedef uint8 Spi_AsyncModeType; 

/*
Enumerationo Range:
    SPI_JOB_OK      (Default)
    SPI_JOB_PENDING
    SPI_JOB_FAILED
    SPI_JOB_QUEUED
*/
typedef uint8 Spi_JobResultType;

/*
Enumerationo Range:
    SPI_SEQ_OK      (Default)
    SPI_SEQ_PENDING
    SPI_SEQ_FAILED
    SPI_SEQ_CANCELLED
*/
typedef uint8 Spi_SeqResultType; 

// Type of application data buffer elements.
typedef uint8 Spi_DataBufferType; 

// Type for defining the number of data elements of the type 
// Spi_DataBufferType to send and / or receive by Channel
typedef uint16 Spi_NumberOfDataType; 


/*
    # StackOverFlow: 
    A Channel is a software exchange medium for data that are defined with the same criteria: 
    Configuration Parameters, Number of Data elements with the same size and data pointers 
    (Source & Destination) or location.

    A Job is composed of one or several Channels. it's considered atomic and therefore cannot 
    be interrupted by another job. it's a basic SPI command.

    A sequence is a general routine like reading, erase, write. it contains a set of jobs that 
    should be executed sequentially. A Sequence communication is interruptible 
    (by another Sequence of communication).

    for example, to write, you need the job
    command job : to set the address
    command addr data : to write the data
*/
// Specifies the identification (ID) for a Channel.
typedef uint8 Spi_ChannelType;

// Specifies the identification (ID) for a Job.
typedef uint16 Spi_JobType; 

// Specifies the identification (ID) for a sequence of jobs.
typedef uint8 Spi_SequenceType; 

typedef struct Spi_ConfigType
{
    Spi_ModeType SpiMode;               // Master / Slave

    // channel config 
    Spi_DffType Spi_ChannelDff;                     // DFF_MODE_8Bit / DFF_MODE_16Bit
    Spi_LSBFirstType Spi_ChannelLSB;                // LSB_FIRST / LSB_LAST
    uint16 defaultTransmitValue;                    // default Transmit Value 
    Spi_DataBufferType DataBuffer [SPI_CHANNEL_MAX_DATA_BUFFER];// Internal Buffer Array 
    Spi_ChannelType SpiChannel; 
    
    // Spi job config 
    Spi_HWunitType  SpiHwUnit;                      // SP1 / SPI2 HW unit
    Spi_ClkPolType SpiClkPol;                   // SPI_CLK_POL_LOW / SPI_CLK_POL_HIGH
    Spi_ClkPhaseType SpiClkPhase;               // SPI_CLK_PHASE_FIRST / SPI_CLK_PHASE_SECOND
    Spi_BaudRateType SpiBaudRate;               // SPIBAUD_RATE_CLK_DIVx
    SPi_CS_Pin SpiCSPin;                        // DIO_CHANNEL_xx
    boolean SpiCsOn;                            // TRUE = Chip Select Functionality ON
    void (*SpiEndJobNotification_ptr)(void);    // Ptr to call back function       
    Spi_JobType SpiJob; 
    /* Collection of Jobs (at least one)
        Interruptible or not interruptible after each Job
        Sequence finish end notification function 
    */
    Spi_SequenceType SpiSequence;

}Spi_ConfigType;

/************************************************************************/
/*                      Functions Declaration                           */
/************************************************************************/

void Spi_Init( const Spi_ConfigType* ConfigPtr );
Std_ReturnType Spi_DeInit( void ); 
Std_ReturnType Spi_WriteIB( Spi_ChannelType Channel, const Spi_DataBufferType* DataBufferPtr );
Std_ReturnType Spi_AsyncTransmit( Spi_SequenceType Sequence );
Std_ReturnType Spi_ReadIB( Spi_ChannelType Channel, Spi_DataBufferType* DataBufferPointer );
Std_ReturnType Spi_SetupEB( Spi_ChannelType Channel, const Spi_DataBufferType* SrcDataBufferPtr, Spi_DataBufferType* DesDataBufferPtr, Spi_NumberOfDataType Length );
Spi_StatusType Spi_GetStatus( void );
Spi_JobResultType Spi_GetJobResult( Spi_JobType Job ); 
Spi_SeqResultType Spi_GetSequenceResult( Spi_SequenceType Sequence );
void Spi_GetVersionInfo( Std_VersionInfoType* versioninfo );
Std_ReturnType Spi_SyncTransmit( Spi_SequenceType Sequence );
Spi_StatusType Spi_GetHWUnitStatus( Spi_HWunitType HWUnit );
void Spi_Cancel( Spi_SequenceType Sequence );
Std_ReturnType Spi_SetAsyncMode( Spi_AsyncModeType Mode );
void Spi_MainFunction_Handling(void);

#endif
