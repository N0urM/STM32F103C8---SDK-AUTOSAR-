/************************************************************************/
/* Author    : Nourhan Mansour                                          */
/* Date      : 13/4/2021                                                */
/* Version   : 1.1.0                                                    */
/* File      : spi.c                                                    */
/************************************************************************/

// The SPI Handler/Driver handles only the Master mode
// The SPI Handler/Driver only supports full-duplex mode.

#include "det.h"
#include "BIT_MATH.h"
#include "spi.h"
#include "spi_Cfg.h"
#include "hw_reg.h"
#include "compiler.h"

/************************************************************************/
/*                          Global Constants                            */
/************************************************************************/
#define Spi_ApiID_Spi_Init                      0x00
#define Spi_ApiID_Spi_DeInit
#define Spi_ApiID_Spi_WriteIB										0x02
#define Spi_ApiID_Spi_AsyncTransmit
#define Spi_ApiID_Spi_ReadIB										0x04
#define Spi_ApiID_Spi_SetupEB
#define Spi_ApiID_Spi_GetStatus                 0x06
#define Spi_ApiID_Spi_GetJobResult							0x07
#define Spi_ApiID_Spi_GetSequenceResult					0x08	
#define Spi_ApiID_Spi_GetVersionInfo        		0x09
#define Spi_ApiID_Spi_SyncTransmit          		0x0A
#define Spi_ApiID_Spi_GetHWUnitStatus						0x0b
#define Spi_ApiID_Spi_Cancel
#define Spi_ApiID_Spi_SetAsyncMode
#define Spi_ApiID_Spi_MainFunction_Handling
/************************************************************************/
/*                          Local functions                             */
/************************************************************************/
static void SPI_StaticStartTransmission (Spi_HWunitType HW_Unit , uint32 data);

/************************************************************************/
/*                         Global variables                             */
/************************************************************************/
/* ptr to a copy of the configutation struct confgured by the user*/
static Spi_ConfigType * Spi_ConfigPtr = (NULL_PTR) ; 

/* Define the current status of the SPI module 
    Shared variable between APIs 
*/
static Spi_StatusType Spi1_Status = SPI_UNINIT; 
static Spi_StatusType Spi2_Status = SPI_UNINIT; 

// Array to hold all Jobs' results 
static Spi_JobResultType Spi_JobResult [SpiMaxJob] = {SPI_JOB_OK};

// Array to hold all Sequences' results 
static Spi_SeqResultType Spi_SequenceResult [SpiMaxSequence] = {SPI_SEQ_OK};

// Internal Data buffer array 
static Spi_DataBufferType  Spi_IB[SpiMaxChannel][SpiIbNBuffers];

// External Data Buffer array
// Spi_DataBufferType  Spi_EB[SpiMaxChannel];
/************************************************************************/
/*                         APIS definitions                             */
/************************************************************************/
/**
 * @@name : Spi_Init
 * @@param: ConfigPtr: Pointer to configuration set.
 * Service ID : 0x00 
 * Non Reentrant
 * Sync
 * Service for SPI initialization.
 * Note: 
 **/
void Spi_Init( const Spi_ConfigType* ConfigPtr )
{

#ifdef SpiDevErrorDetect
    if (ConfigPtr == NULL_PTR)
    {
        Det_ReportError(SPI_ModuleId, 0x00, Spi_ApiID_Spi_Init, SPI_E_PARAM_POINTER);
    }
    else
    {
        // ok
    }    

#endif

    Spi_ConfigPtr =  ConfigPtr ; 
    uint16 Jobidx = 0 ;
    uint8 ChannelIdx = 0 ;

}

#if SpiChannelBuffersAllowed == SpiChannelBufferIB || SpiChannelBuffersAllowed == SpiChannelBufferIB_EB
/**
    * @name : Spi_WriteIB
		* @param: Channel: Channel ID.
		* 			 DataBufferPtr: Pointer to source data buffer.
		• 				Note: If this pointer is null, default transmit value of this channel will be used instead.
    * Reentrancy: Reentrant
    * @Return:  Std_ReturnType:
	  *          E_OK: Write command has been accepted 
    *          E_NOT_OK: Write command has not been accepted
    * Sync
    * @Description: Service for writing one or more data to an IB SPI Handler/Driver Channel specified by parameter.
**/
Std_ReturnType Spi_WriteIB( Spi_ChannelType Channel, const Spi_DataBufferType* DataBufferPtr )
{
		Std_ReturnType retStatus = E_OK;
		uint8 BufferIdx = 0 ; 
	
		// Check if module is initialized
		// Channel is valid 
		// Channel buffer type is internal 
		if (Spi_ConfigPtr == NULL_PTR || 
				Channel > SpiMaxChannel   ||
				Spi_ConfigPtr->Spi_ChannelConfigPtr[Channel].ChannelType != SpiChannelBufferIB
				)
		{
			retStatus = E_NOT_OK;
		}
		else 
		{
			if (DataBufferPtr == NULL_PTR)
			{
					// Channel should transmit the default value
			}
			else 
			{
				// Store data in internal buffer
					for (BufferIdx=0 ; BufferIdx < Spi_ConfigPtr->Spi_ChannelConfigPtr[Channel].NoOfDataElements ; BufferIdx++)
					{
							Spi_IB[Channel][BufferIdx] = (DataBufferPtr[BufferIdx]);
					}
			}
		}
	
		return retStatus;
}

#endif

#if SpiChannelBuffersAllowed == SpiChannelBufferIB || SpiChannelBuffersAllowed == SpiChannelBufferIB_EB
/**
    * @name : Spi_ReadIB
		* @param: Channel: Channel ID.
		* 			 DataBufferPtr: Pointer to source data buffer.
    * Reentrancy: Reentrant
    * @Return:  Std_ReturnType:
	  *          E_OK: Write command has been accepted 
    *          E_NOT_OK: Write command has not been accepted
    * Sync
    * @Description: Service for reading synchronously one or more data 
		* from an IB SPI Handler/Driver Channel specified by parameter.
**/
Std_ReturnType Spi_ReadIB
				( Spi_ChannelType Channel, Spi_DataBufferType* DataBufferPointer )
{
	
		Std_ReturnType retStatus = E_OK;
	
		// Check if module is initialized
		// Channel is valid 
		// Channel buffer type is internal 
#ifdef SpiDevErrorDetect	
	if (Spi_ConfigPtr == NULL_PTR
			|| (Spi_ConfigPtr->Spi_ChannelConfigPtr[Channel].ChannelType != SpiChannelBufferIB) ) 
		{
			retStatus = E_NOT_OK;
			Det_ReportError(SPI_ModuleId , 0 , Spi_ApiID_Spi_ReadIB , SPI_E_UNINIT );
		}
		else if (Channel > SpiMaxChannel) 
		{
			retStatus = E_NOT_OK;
			Det_ReportError(SPI_ModuleId , 0 , Spi_ApiID_Spi_ReadIB , SPI_E_PARAM_CHANNEL );
		}
		else 
		{
					DataBufferPointer = Spi_IB[Channel];
		}
#else 
				DataBufferPointer = Spi_IB[Channel];
#endif
	return retStatus;
}
#endif

#if SpiChannelBuffersAllowed == SpiChannelBufferEB || SpiChannelBuffersAllowed == SpiChannelBufferIB_EB
/**
    * @name : Spi_SetupEB
		* @param: @Channel: Channel ID.
		* 			  @SrcDataBufferPtr: Pointer to source data buffer.
		*				  @DesDataBufferPtr: Pointer to destination data buffer in RAM.
		* 			  @Length: number of data elements) of the data to be transmitted 
		*							from SrcDataBufferPtr and/or received from DesDataBufferPtr. 
    * Reentrancy: Reentrant
    * @Return:  Std_ReturnType:
	  *          E_OK: Write command has been accepted 
    *          E_NOT_OK: Write command has not been accepted
    * Sync
    * @Description: Service to setup the buffers and the length of data 
		*						 for the EB SPI Handler/Driver Channel specified.
**/
Std_ReturnType Spi_SetupEB( Spi_ChannelType Channel, 
														const Spi_DataBufferType* SrcDataBufferPtr, 
														Spi_DataBufferType* DesDataBufferPtr, 
														Spi_NumberOfDataType Length )
{
	Std_ReturnType retStatus =  E_OK;
	
	
	return retStatus;	
}
#endif
/**
    * @name : Spi_GetStatus
    * @param: NONE
    * Reentrancy: Reentrant
    * @Return:  Spi_StatusType:
    *           SPI_UNINIT: The SPI Handler/Driver is not initialized or not usable.
    *           SPI_IDLE: The SPI Handler/Driver is not currently transmitting any Job.
    *           SPI_BUSY: The SPI Handler/Driver is performing a SPI Job (trans-mit)
    * Sync
    * @Description: Service returns the SPI Handler/Driver software module status. 
**/
Spi_StatusType Spi_GetStatus( void )
{
    Spi_StatusType Spi_Status =  SPI_UNINIT;
    if (Spi1_Status == SPI_BUSY || Spi2_Status == SPI_BUSY){
        Spi_Status = SPI_BUSY;
    }
    else if (Spi1_Status == SPI_IDLE || Spi2_Status == SPI_IDLE )
    {
        Spi_Status = SPI_IDLE; 
    }
    else {
        Spi_Status = SPI_UNINIT ; 
    }

    return Spi_Status ;
}

#if  SpiLevelDelivered == 0 || SpiLevelDelivered == 2 
/**
    * @name : Spi_SyncTransmit
    * @param: Sequence: Sequence ID.
    * Reentrancy: Reentrant
    * @Return:  Std_ReturnType:
    *          E_OK: Transmission command has been accepted 
    *          E_NOT_OK: Transmission command has not been accepted
    * Async
    * @Description: Service to transmit data on the SPI bus. 
**/
Std_ReturnType Spi_SyncTransmit( Spi_SequenceType Sequence )
{
    Std_ReturnType ret = E_OK;
    // Concurrent Send is not allowed 
#ifndef SpiSupportConcurrentSyncTransmit
    if (Spi_GetStatus() != SPI_IDLE)
    {
        ret = E_NOT_OK; 
    }
    else 
    {
			 // NOT ALLOWED Seq ID
			if (Sequence > SpiMaxSequence)
			{
					ret = E_NOT_OK; 
			
			}
			// OK Proceed 
			else 
			{
				Spi_SequenceResult[Sequence] = SPI_SEQ_PENDING;
				Spi_SeqConfigType * Spi_Seq = Spi_ConfigPtr->Spi_SeqConfigPtr;
				Spi_JobType Spi_CurrentJob; 
				Spi_JobConfigType Spi_CurrentJobPtr;
				Spi_ChannelConfigType Spi_CurrentChPtr; 
				uint16 JobIdx = 0 ; 
				uint16 SpiAllJobIdIdx = 0 ;
				uint8 SpiChIdx=0; 
				for (JobIdx=0 ; JobIdx < Spi_Seq->NoOfJobs ; JobIdx++)
				{
						// Get the curretn job information in the sequence 
						Spi_CurrentJob = ( * (Spi_Seq->JobLinkPtr)+JobIdx );
					
						// Find a job match in the configuration structure
						for(SpiAllJobIdIdx=0; SpiAllJobIdIdx < Spi_ConfigPtr->NoOfJobs; SpiAllJobIdIdx++)
						{
							// TRUE == Match is found
							if(Spi_ConfigPtr->Spi_JobConfigPtr[SpiAllJobIdIdx].SpiJobId == Spi_CurrentJob)
							{
									
									Spi_CurrentJobPtr = Spi_ConfigPtr->Spi_JobConfigPtr[SpiAllJobIdIdx];
									
								// Check which HW unit the job is assigned to and perform Sync send operation. 
									if (Spi_CurrentJobPtr.SpiHwUnit == SPI1_HW_UNIT)
									{
										// UPDATE SPI STATUS
										Spi1_Status = SPI_BUSY; 
										
										// UPDATE SEQ Status 
										Spi_SequenceResult[Sequence] = SPI_SEQ_PENDING;
										
										// UPDATE JOB Status 
										Spi_JobResult[Spi_CurrentJobPtr.SpiJobId] = SPI_JOB_PENDING;
										
										// Scan all channels in the job availabe to send 
										for (SpiChIdx=0 ; SpiChIdx < Spi_CurrentJobPtr.No_Channel ; SpiChIdx++)
										{
											// Access channel with ID ChnlLinkPtrPhysical[SpiChIdx] from the main config struct
												Spi_CurrentChPtr =  Spi_ConfigPtr->Spi_ChannelConfigPtr[ Spi_CurrentJobPtr.ChnlLinkPtrPhysical[SpiChIdx]];
											// Start transmitting Channel data
												SPI_StaticStartTransmission(SPI1_HW_UNIT ,  );
												
										}

									}
									else if (Spi_CurrentJobPtr.SpiHwUnit == SPI2_HW_UNIT)
									{
										// UPDATE SPI STATUS
											Spi2_Status = SPI_BUSY; 
										
									}
									else 
									{
										// Error Shouldn't be here
									}
									break;
							}
						}
				}
			}
    }
#endif
    return ret; 
}
#endif

#ifdef SpiHwStatusApi
/**
    * @name : Spi_GetHWUnitStatus
		* @param: HWUnit: SPI Hardware microcontroller peripheral (unit) ID.
    * Reentrancy: Reentrant
		* @Return: Spi_StatusType:
								SPI_UNINIT
								SPI_IDLE
								SPI_BUSY
    * Sync
    * @Description: return the status of the specified SPI Hardware microcontroller pe-ripheral.
**/
Spi_StatusType Spi_GetHWUnitStatus( Spi_HWunitType HWUnit )
{
	Spi_StatusType ret_Status = SPI_IDLE; 
	switch(HWUnit)
		{
			case SPI1_HW_UNIT:
			  ret_Status = Spi1_Status;
				break; 
			case SPI2_HW_UNIT:
				ret_Status = Spi2_Status;
				break;
			default:
					// Shoulnd't be here
				break;
		}
		return ret_Status;
}
#endif

/**
    * @name : Spi_GetJobResult
		* @param: Job: Job ID. An invalid job ID will return an undefined result.
    * Reentrancy: Reentrant
		* @Return : Spi_JobResultType:
					SPI_JOB_OK
					SPI_JOB_PENDING
					SPI_JOB_FAILED
					SPI_JOB_QUEUED
    * Sync
    * @Description: This service returns the last transmission result of the specified Job.
**/
Spi_JobResultType Spi_GetJobResult( Spi_JobType Job )
{
	return Spi_JobResult[Job];
}

/**
    * @name : Spi_GetSequenceResult
		* @param: Sequence: Sequence ID. An invalid seq ID will return an undefined result.
    * Reentrancy: Reentrant
		* @Return : Spi_SeqResultType:
					SPI_SEQ_OK
					SPI_SEQ_PENDING
					SPI_SEQ_FAILED
					SPI_SEQ_QUEUED
    * Sync
    * @Description: This service returns the last transmission result of the specified Sequence.
**/
Spi_SeqResultType Spi_GetSequenceResult( Spi_SequenceType Sequence )
{
	return Spi_SequenceResult[Sequence];
}


#if SpiVersionInfoApi == TRUE
/**
    * @name : Spi_GetVersionInfo
    * @param: VersionInfo: Pointer to where to store the version information of this module
    * Reentrancy: Reentrant
    * Sync
    * @Description: Service to get the version information of this module.
**/
void Spi_GetVersionInfo(Std_VersionInfoType *VersionInfo)
{
#ifdef SpiDevErrorDetect
    if (VersionInfo == NULL_PTR)
    {
        Det_ReportError(SPI_ModuleId, 0, Spi_ApiID_Spi_GetVersionInfo, SPI_E_PARAM_POINTER);
    }
    else
    {
        VersionInfo->moduleID = SPI_ModuleId;
        VersionInfo->sw_major_version = SPI_SW_MAJOR_VERSION;
        VersionInfo->sw_minor_version = SPI_SW_MINOR_VERSION;
        VersionInfo->sw_patch_version = SPI_SW_PATCH_VERSION;
        VersionInfo->vendorID = SPI_VendorId;
    }
#else 
    VersionInfo->moduleID = SPI_ModuleId;
    VersionInfo->sw_major_version = SPI_SW_MAJOR_VERSION;
    VersionInfo->sw_minor_version = SPI_SW_MINOR_VERSION;
    VersionInfo->sw_patch_version = SPI_SW_PATCH_VERSION;
    VersionInfo->vendorID = SPI_VendorId;
#endif
    return ; 
}
#endif
/************************************************************************/
/*                    Local functions Definitions                       */
/************************************************************************/
static void SPI_StaticStartTransmission (Spi_HWunitType HW_Unit , uint32 data)
{
		// Clear ss pin (Active low)
		/* To Be handles in main by the GPIO Driver */
		switch(HW_Unit)
		{
			case SPI1_HW_UNIT:
			  // Send Data 
        SPI1_DR = data;
				// Wait for Busy Flag 
        while (GET_BIT(SPI1_SR , SPI_SR_BSY ) != 0);
				break; 
			case SPI2_HW_UNIT:
				 // Send Data 
        SPI2_DR = data;
				// Wait for Busy Flag 
        while (GET_BIT(SPI2_SR , SPI_SR_BSY ) != 0);
				break;
			default:
					// Shoulnd't be here
				break;
		}
		    
    // set SS pin
    /* To be handled in main by GPIO DRIVER */
  
	return ;
}
