/************************************************************************/
/* Author    : Nourhan Mansour                                          */
/* Date      : 4/5/2021                                                 */
/* Version   : 1.1.2                                                    */
/* File      : spi.h                                                    */
/* Note      : Current implementation only supports LEVEL 0 (SYNC) com. */
/************************************************************************/

#ifndef SPI_H
#define SPI_H

#include "STD_TYPES.h"
#include "Dem_Types.h"
#include "spi_Cfg.h"


/************************************************************************/
/*                          Published Info                              */
/************************************************************************/

#define SPI_ModuleId                    (38U)
#define SPI_VendorId                    (483U)

#define SPI_AR_RELEASE_MAJOR_VERSION    (1U)
#define SPI_AR_RELEASE_MINOR_VERSION    (1U)
#define SPI_AR_RELEASE_REVISION_VERSION (2U)

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
/*                                DEM                                   */
/************************************************************************/
#define SPI_E_HARDWARE_ERROR            0xFF

/************************************************************************/
/*                          RUN TIME ERRORS                             */
/************************************************************************/
#define SPI_E_SEQ_PENDING               0x2A
#define SPI_E_SEQ_IN_PROCESS            0x3A


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
#define SPI_SEQ_OK                  0x00

// The SPI Handler/Driver is performing a SPI Sequence.. The mean-ing of this status is equal to SPI_BUSY.
#define SPI_SEQ_PENDING             0x01

//The last transmission of the Sequence has failed.
#define SPI_SEQ_FAILED              0x02

//An asynchronous transmit Job has been cancelled by user
#define SPI_SEQ_CANCELLED           0x03
////////////////////////////////////////////////////////////////////

// SPI ASYNC MODE
////////////////////////////////////////////////////////////////////
#define SPI_POLLING_MODE            0x00 
#define SPI_INTERRUPT_MODE          0x01        
////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////
#define SPI1_HW_UNIT                    0x01
#define SPI2_HW_UNIT                    0x02      

/* Master Slave Selection */
#define SPI_MASTER_MODE                 1U
#define SPI_NOTUSED_MODE                0U

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
typedef boolean SpiTransferStartType; 

// SPIBAUD_RATE_CLK_DIVx
typedef uint8 Spi_BaudRateType; 

// SPI1_HW_UNIT / SPI2_HW_UNIT
typedef uint8 Spi_HWunitType; 

// DIO_CHANNEL_xx
typedef uint8 Spi_CS_Pin;

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
// Should be of type EcucIntegerParamDef
typedef uint8 Spi_DataBufferType; 

// Type for defining the number of data elements of the type 
// Spi_DataBufferType to send and / or receive by Channel
typedef uint16 Spi_NumberOfDataType; 

// Specifies the identification (ID) for a Channel.
typedef uint8 Spi_ChannelType;

// Specifies the identification (ID) for a Job.
typedef uint16 Spi_JobType; 

// Specifies the identification (ID) for a sequence of jobs.
typedef uint8 Spi_SequenceType; 

typedef struct 
{
    // channel config 
    Spi_ChannelType SpiChannelId; 
    Spi_DffType SpiDataWidth;                       // DFF_MODE_8Bit / DFF_MODE_16Bit
    SpiTransferStartType SpiTransferStart;          // LSB_FIRST / LSB_LAST
		uint8 ChannelType;                               // Specify EB/IB type      
		/* For IB - contains number of IB data elements,
    For EB - contains maximum data elements          */
		uint16 NoOfDataElements;
    Spi_DataBufferType SpiDefaultData;                          // default Transmit Value
}Spi_ChannelConfigType;

typedef struct 
{
    Spi_JobType SpiJobId;                           // Job ID used with APIs
    uint8 JobPriority;                              // Job Priority ranging from 0 (Lowest) to 3 (Highest) 
		uint8 No_Channel;																// No. of channels associated with the job
    Spi_ChannelType *ChnlLinkPtrPhysical;           // Ptr to channels asscociated with the job 
		Spi_HWunitType  SpiHwUnit;                      // SP1 / SPI2 HW unit
    Spi_ClkPolType SpiClkPol;                       // SPI_CLK_POL_LOW / SPI_CLK_POL_HIGH
    Spi_ClkPhaseType SpiClkPhase;                   // SPI_CLK_PHASE_FIRST / SPI_CLK_PHASE_SECOND
    Spi_BaudRateType SpiBaudRate;                   // SPIBAUD_RATE_CLK_DIVx
    Spi_CS_Pin SpiCSPin;                            // DIO_CHANNEL_xx
    boolean SpiCsOn;                                // TRUE = Chip Select Functionality ON (HW handling)
 //   void (*SpiEndJobNotification_ptr)(void);        // Ptr to call back function       
}Spi_JobConfigType;

typedef struct 
{
    Spi_JobType * JobLinkPtr;                       // Ptr to jobs IDs asscociated with the  seq
    Spi_JobType NoOfJobs;														// Number of Jobs configured 
    Spi_SequenceType SpiSeqId;                      // Sequence ID used with APIs
}Spi_SeqConfigType;

typedef struct Spi_ConfigType
{
		// The SPI Handler/Driver only supports full-duplex mode

		// MODE: SPI_NOTUSED_MODE / SPI_MASTER_MODE
		// [SWS_Spi_00040] The SPI Handler/Driver handles only the Master mode.
    Spi_ModeType Spi1Mode;
		Spi_ModeType Spi2Mode;	
    
		// Number of Jobs configured 
     Spi_JobType NoOfJobs;

    // Number of Channels configured 
     Spi_ChannelType NoOfChannels;
    
		// Number of Sequnces 
    Spi_SequenceType SpiSequence;  
		
		// Pointer to Sequence configutration 
    Spi_SeqConfigType * Spi_SeqConfigPtr;
  
		// pointer to job configuration 
    Spi_JobConfigType * Spi_JobConfigPtr; 
   
    // Pointer to channel configuration 
    Spi_ChannelConfigType * Spi_ChannelConfigPtr ;   
	              

}Spi_ConfigType;

/************************************************************************/
/*                      Functions Declaration                           */
/************************************************************************/

void Spi_Init( const Spi_ConfigType* ConfigPtr );

Std_ReturnType Spi_DeInit( void ); 

Std_ReturnType Spi_WriteIB( Spi_ChannelType Channel, const Spi_DataBufferType* DataBufferPtr );

// Std_ReturnType Spi_AsyncTransmit( Spi_SequenceType Sequence );

Std_ReturnType Spi_ReadIB( Spi_ChannelType Channel, Spi_DataBufferType* DataBufferPointer );

Std_ReturnType Spi_SetupEB( Spi_ChannelType Channel, const Spi_DataBufferType* SrcDataBufferPtr, Spi_DataBufferType* DesDataBufferPtr, Spi_NumberOfDataType Length );

Spi_StatusType Spi_GetStatus( void );

Spi_JobResultType Spi_GetJobResult( Spi_JobType Job ); 

Spi_SeqResultType Spi_GetSequenceResult( Spi_SequenceType Sequence );

void Spi_GetVersionInfo( Std_VersionInfoType* versioninfo );

Std_ReturnType Spi_SyncTransmit( Spi_SequenceType Sequence );

Spi_StatusType Spi_GetHWUnitStatus( Spi_HWunitType HWUnit );

// void Spi_Cancel( Spi_SequenceType Sequence );

// Std_ReturnType Spi_SetAsyncMode( Spi_AsyncModeType Mode );

// void Spi_MainFunction_Handling(void);

#endif
