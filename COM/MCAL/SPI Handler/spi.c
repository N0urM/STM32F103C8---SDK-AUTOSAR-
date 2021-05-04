/************************************************************************/
/* Author    : Nourhan Mansour                                          */
/* Date      : 5/5/2021                                                 */
/* Version   : 1.1.2                                                    */
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
#define Spi_ApiID_Spi_Init 0x00
#define Spi_ApiID_Spi_DeInit
#define Spi_ApiID_Spi_WriteIB 0x02
#define Spi_ApiID_Spi_AsyncTransmit
#define Spi_ApiID_Spi_ReadIB 0x04
#define Spi_ApiID_Spi_SetupEB 0x05
#define Spi_ApiID_Spi_GetStatus 0x06
#define Spi_ApiID_Spi_GetJobResult 0x07
#define Spi_ApiID_Spi_GetSequenceResult 0x08
#define Spi_ApiID_Spi_GetVersionInfo 0x09
#define Spi_ApiID_Spi_SyncTransmit 0x0A
#define Spi_ApiID_Spi_GetHWUnitStatus 0x0b
#define Spi_ApiID_Spi_Cancel
#define Spi_ApiID_Spi_SetAsyncMode
#define Spi_ApiID_Spi_MainFunction_Handling

#define PORTA_CHANNEL_OFFEST (0U)
#define PORTB_CHANNEL_OFFEST (16U)
#define PORTC_CHANNEL_OFFEST (32U)
#define PORTx_CHANNEL_OFFSET (48U)

/************************************************************************/
/*                          Type Definition                             */
/************************************************************************/
typedef struct TransmitionType
{
    volatile uint32 *SPI_DR;
    volatile uint32 *SPI_SR;
    const Spi_DataBufferType *Srcdata;
    Spi_DataBufferType *Desdata;
    Spi_NumberOfDataType Length;

} TransmitionType;

/************************************************************************/
/*                          Local functions                             */
/************************************************************************/
static void SPI_StaticStartTransmission(TransmitionType TransmitStrcut);
static void Spi_StaticHandleJob(Spi_JobType SpiJobId);
static void Spi_StaticInitHWUnits(void);
static void Dio_WriteChannel(Spi_CS_Pin ChannelId, Std_ReturnType Level);

/************************************************************************/
/*                         Global variables                             */
/************************************************************************/
/* ptr to a copy of the configutation struct confgured by the user*/
static const Spi_ConfigType *Spi_ConfigPtr = (NULL_PTR);

/* Define the current status of the SPI module 
    Shared variable between APIs 
*/
static Spi_StatusType Spi1_Status = SPI_UNINIT;
static Spi_StatusType Spi2_Status = SPI_UNINIT;

// Array to hold all Jobs' results
static Spi_JobResultType Spi_JobResult[SpiMaxJob];

// Array to hold all Sequences' results
static Spi_SeqResultType Spi_SequenceResult[SpiMaxSequence];

// Internal Data buffer array
static Spi_DataBufferType Spi_IB[SpiMaxChannel][SpiIbNBuffers];

// External Data Buffer pointers
static struct Spi_EB
{
    const Spi_DataBufferType *srcDataPtr;
    Spi_DataBufferType *DestDataPtr;
    Spi_NumberOfDataType Length;
};

static struct Spi_EB Spi_EBInstance[SpiMaxChannel];
/************************************************************************/
/*                         APIS definitions                             */
/************************************************************************/
/**
 * @name : Spi_Init
 * @param: ConfigPtr: Pointer to configuration set.
 * Service ID : 0x00 
 * Non Reentrant
 * Sync
 * Service for SPI initialization.
 * Note: 
 **/
void Spi_Init(const Spi_ConfigType *ConfigPtr)
{

#ifdef SpiDevErrorDetect
    if (ConfigPtr == NULL_PTR)
    {
        Det_ReportError(SPI_ModuleId, 0x00, Spi_ApiID_Spi_Init, SPI_E_PARAM_POINTER);
    }
    else
    {
        // ok proceed
        uint16 JobIdx;
        uint8 SeqIdx;
        uint8 ChIdx;

        // Copy Config ptr
        Spi_ConfigPtr = ConfigPtr;

        // Initialize HW units
        Spi_StaticInitHWUnits();
        Spi1_Status = SPI_IDLE;
        Spi2_Status = SPI_IDLE;

        // Initialize all jobs status
        for (JobIdx = 0; JobIdx < SpiMaxJob; JobIdx++)
        {
            Spi_JobResult[JobIdx] = SPI_JOB_OK;
        }

        // Initialize all sequences status
        for (SeqIdx = 0; SeqIdx < SpiMaxSequence; SeqIdx++)
        {
            Spi_SequenceResult[SeqIdx] = SPI_SEQ_OK;
        }

        // Setup buffers
#if (SpiChannelBuffersAllowed == SpiChannelBufferIB || SpiChannelBuffersAllowed == SpiChannelBufferIB_EB)
        for (ChIdx = 0; ChIdx < SpiMaxChannel; ChIdx++)
        {
            *Spi_IB[ChIdx] = (uint32)NULL_PTR;
        }
#endif

#if (SpiChannelBuffersAllowed == SpiChannelBufferEB || SpiChannelBuffersAllowed == SpiChannelBufferIB_EB)
        for (ChIdx = 0; ChIdx < SpiMaxChannel; ChIdx++)
        {
            Spi_EBInstance[ChIdx].DestDataPtr = NULL_PTR;
            Spi_EBInstance[ChIdx].srcDataPtr = NULL_PTR;
        }
#endif
    }
#else
//		Spi_ConfigPtr = ConfigPtr;
#endif

    return;
}

#if SpiChannelBuffersAllowed == SpiChannelBufferIB || SpiChannelBuffersAllowed == SpiChannelBufferIB_EB
/**
    * @name : Spi_WriteIB
	* @param: Channel: Channel ID.
	* 		DataBufferPtr: Pointer to source data buffer.
	* Note: If this pointer is null, default transmit value of this channel will be used instead.
    * Reentrancy: Reentrant
    * @Return:  Std_ReturnType:
	*          E_OK: Write command has been accepted 
    *          E_NOT_OK: Write command has not been accepted
    * Sync
    * @Description: Service for writing one or more data to an IB SPI Handler/Driver Channel specified by parameter.
**/
Std_ReturnType Spi_WriteIB(Spi_ChannelType Channel, const Spi_DataBufferType *DataBufferPtr)
{
    Std_ReturnType retStatus = E_OK;
    uint8 BufferIdx = 0;
#ifdef SpiDevErrorDetect
    // Check if module is initialized
    // Channel is valid
    // Channel buffer type is internal
    if (Spi_ConfigPtr == NULL_PTR ||
        Spi_ConfigPtr->Spi_ChannelConfigPtr[Channel].ChannelType != SpiChannelBufferIB)
    {
        retStatus = E_NOT_OK;
        Det_ReportError(SPI_ModuleId, 0, Spi_ApiID_Spi_WriteIB, SPI_E_UNINIT);
    }
    else if (Channel > SpiMaxChannel)
    {
        Det_ReportError(SPI_ModuleId, 0, Spi_ApiID_Spi_WriteIB, SPI_E_PARAM_CHANNEL);
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
            for (BufferIdx = 0; BufferIdx < Spi_ConfigPtr->Spi_ChannelConfigPtr[Channel].NoOfDataElements; BufferIdx++)
            {
                Spi_IB[Channel][BufferIdx] = (DataBufferPtr[BufferIdx]);
            }
        }
    }
#else
    // Store data in internal buffer
    for (BufferIdx = 0; BufferIdx < Spi_ConfigPtr->Spi_ChannelConfigPtr[Channel].NoOfDataElements; BufferIdx++)
    {
        Spi_IB[Channel][BufferIdx] = (DataBufferPtr[BufferIdx]);
    }
#endif

    return retStatus;
}

#endif

#if SpiChannelBuffersAllowed == SpiChannelBufferIB || SpiChannelBuffersAllowed == SpiChannelBufferIB_EB
/**
* @name : Spi_ReadIB
* @param: Channel: Channel ID.
* 			 DataBufferPtr: Pointer to source data buffer.
* @Reentrancy: Reentrant
* @Return:  Std_ReturnType:
*          E_OK: Write command has been accepted 
*          E_NOT_OK: Write command has not been accepted
* Sync
* @Description: Service for reading synchronously one or more data 
* from an IB SPI Handler/Driver Channel specified by parameter.
**/
Std_ReturnType Spi_ReadIB(Spi_ChannelType Channel, Spi_DataBufferType *DataBufferPointer)
{

    Std_ReturnType retStatus = E_OK;
#ifdef SpiDevErrorDetect

    // Check if module is initialized
    // Channel is valid
    // Channel buffer type is internal
    if (Spi_ConfigPtr == NULL_PTR || (Spi_ConfigPtr->Spi_ChannelConfigPtr[Channel].ChannelType != SpiChannelBufferIB))
    {
        retStatus = E_NOT_OK;
        Det_ReportError(SPI_ModuleId, 0, Spi_ApiID_Spi_ReadIB, SPI_E_UNINIT);
    }
    else if (Channel > SpiMaxChannel)
    {
        retStatus = E_NOT_OK;
        Det_ReportError(SPI_ModuleId, 0, Spi_ApiID_Spi_ReadIB, SPI_E_PARAM_CHANNEL);
    }
    else
    {
        DataBufferPointer = Spi_IB[Channel];
    }
#else

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
Std_ReturnType Spi_SetupEB(Spi_ChannelType Channel,
                           const Spi_DataBufferType *SrcDataBufferPtr,
                           Spi_DataBufferType *DesDataBufferPtr,
                           Spi_NumberOfDataType Length)
{
    Std_ReturnType retStatus = E_OK;
#ifdef SpiDevErrorDetect
    // Check if module is initialized
    // Channel is valid
    // Channel buffer type is internal
    if (Spi_ConfigPtr == NULL_PTR || (Spi_ConfigPtr->Spi_ChannelConfigPtr[Channel].ChannelType != SpiChannelBufferEB))
    {
        retStatus = E_NOT_OK;
        Det_ReportError(SPI_ModuleId, 0, Spi_ApiID_Spi_SetupEB, SPI_E_UNINIT);
    }
    else if (Channel > SpiMaxChannel)
    {
        retStatus = E_NOT_OK;
        Det_ReportError(SPI_ModuleId, 0, Spi_ApiID_Spi_SetupEB, SPI_E_PARAM_CHANNEL);
    }
    else if (Length == 0 || Length > SpiEbMaxLength)
    {
        retStatus = E_NOT_OK;
        Det_ReportError(SPI_ModuleId, 0, Spi_ApiID_Spi_SetupEB, SPI_E_PARAM_LENGTH);
    }
    else
    {
        // OK
        Spi_EBInstance[Channel].DestDataPtr = DesDataBufferPtr;
        Spi_EBInstance[Channel].srcDataPtr = SrcDataBufferPtr;
        Spi_EBInstance[Channel].Length = Length;
    }

#else
    Spi_EB[Channel].DestDataPtr = DesDataBufferPtr;
    Spi_EB[Channel].srcDataPtr = SrcDataBufferPtr;
    Spi_EB[Channel].Length = Length;
#endif

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
Spi_StatusType Spi_GetStatus(void)
{
    Spi_StatusType Spi_Status = SPI_UNINIT;
    if (Spi1_Status == SPI_BUSY || Spi2_Status == SPI_BUSY)
    {
        Spi_Status = SPI_BUSY;
    }
    else if (Spi1_Status == SPI_IDLE || Spi2_Status == SPI_IDLE)
    {
        Spi_Status = SPI_IDLE;
    }
    else
    {
        Spi_Status = SPI_UNINIT;
    }

    return Spi_Status;
}

#if SpiLevelDelivered == 0 || SpiLevelDelivered == 2
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
Std_ReturnType Spi_SyncTransmit(Spi_SequenceType Sequence)
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
            // Set Sequence status to pending
            Spi_SequenceResult[Sequence] = SPI_SEQ_PENDING;
            // Create an instance of the sequence
            Spi_SeqConfigType Spi_Seq = Spi_ConfigPtr->Spi_SeqConfigPtr[Sequence];
            // Job instance
            Spi_JobType Spi_CurrentJob;
            Spi_JobConfigType Spi_CurrentJobPtr;
            uint16 JobIdx = 0;
            for (JobIdx = 0; JobIdx < Spi_Seq.NoOfJobs; JobIdx++)
            {
                // Get the curretn job information in the sequence
                // Note: Assuming Job idx represented by its ID
                Spi_CurrentJob = Spi_Seq.JobLinkPtr[JobIdx];

                Spi_CurrentJobPtr = Spi_ConfigPtr->Spi_JobConfigPtr[Spi_CurrentJob];

                // Check which HW unit the job is assigned to and perform Sync send operation.
                if (Spi_CurrentJobPtr.SpiHwUnit == SPI1_HW_UNIT)
                {
                    // UPDATE SPI STATUS
                    Spi1_Status = SPI_BUSY;
                }
                else if (Spi_CurrentJobPtr.SpiHwUnit == SPI2_HW_UNIT)
                {
                    // UPDATE SPI STATUS
                    Spi2_Status = SPI_BUSY;
                }
                else
                {
                    // Shouldn't be here
                }
                // UPDATE SEQ Status
                Spi_SequenceResult[Sequence] = SPI_SEQ_PENDING;

                // UPDATE JOB Status
                Spi_JobResult[Spi_CurrentJobPtr.SpiJobId] = SPI_JOB_PENDING;

                // Handle current Job
                Spi_StaticHandleJob(Spi_CurrentJob);

                // Update Job status
                Spi_JobResult[Spi_CurrentJobPtr.SpiJobId] = SPI_JOB_OK;

                // Release HW unit
                if (Spi_CurrentJobPtr.SpiHwUnit == SPI1_HW_UNIT)
                {
                    // UPDATE SPI STATUS
                    Spi1_Status = SPI_IDLE;
                }
                else if (Spi_CurrentJobPtr.SpiHwUnit == SPI2_HW_UNIT)
                {
                    // UPDATE SPI STATUS
                    Spi2_Status = SPI_IDLE;
                }
                else
                {
                    // Shouldn't be here
                }
            }
            // Update Sequence Status
            Spi_SequenceResult[Sequence] = SPI_SEQ_OK;
        }
    }
#else
    // TODO
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
Spi_StatusType Spi_GetHWUnitStatus(Spi_HWunitType HWUnit)
{
    Spi_StatusType ret_Status = SPI_IDLE;
    switch (HWUnit)
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
Spi_JobResultType Spi_GetJobResult(Spi_JobType Job)
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
Spi_SeqResultType Spi_GetSequenceResult(Spi_SequenceType Sequence)
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
    return;
}
#endif
/************************************************************************/
/*                    Local functions Definitions                       */
/************************************************************************/

/**
* @name : SPI_StaticStartTransmission
* @param: HW_Unit: Associated HW unit 
* 			  Srcdata: source data buffer 
* 			  Destdata: destination data buffer
*					Length: Length of data buffer
* @Description: local function to start transmition of array of data and store the result.
**/
static void SPI_StaticStartTransmission(TransmitionType TransmitStrcut)
{
    uint16 idx = 0;

    // Send Data
    for (idx = 0; idx < TransmitStrcut.Length; idx++)
    {
        *(TransmitStrcut.SPI_DR) = TransmitStrcut.Srcdata[idx];
        // *(TransmitStrcut.SPI_DR) = *((const uint32 *)((TransmitStrcut.Srcdata) + idx));
        // Wait for Busy Flag
        while (GET_BIT(*(TransmitStrcut.SPI_SR), SPI_SR_BSY) != 0)
            ;
        TransmitStrcut.Desdata[idx] = (((*TransmitStrcut.SPI_DR) >> 24) & 0xFF);
    }
    return;
}

/**
* @name : Spi_StaticHandleJob
* @param: SpiJobId : Job ID
* @Description: Handle Job transmition
**/
static void Spi_StaticHandleJob(Spi_JobType SpiJobId)
{

    volatile uint32 *SPI_CR1;
    // Channel instance
    Spi_ChannelConfigType Spi_CurrentChConfig;
    Spi_ChannelType Spi_CurrentCh;
    uint8 SpiChIdx = 0;

    // Job instance
    Spi_JobConfigType Spi_CurrentJobConfig = Spi_ConfigPtr->Spi_JobConfigPtr[SpiJobId];
    Spi_HWunitType HW_Unit = Spi_CurrentJobConfig.SpiHwUnit;

    // transmission Struct
    TransmitionType Transmit_Struct;

    // Identify used HW unit
    switch (HW_Unit)
    {
    case SPI1_HW_UNIT:
        Transmit_Struct.SPI_DR = &SPI1_DR;
        Transmit_Struct.SPI_SR = &SPI1_SR;
        SPI_CR1 = &SPI1_CR1;
        break;
    case SPI2_HW_UNIT:
        Transmit_Struct.SPI_DR = &SPI2_DR;
        Transmit_Struct.SPI_SR = &SPI2_SR;
        SPI_CR1 = &SPI2_CR1;
        break;
    default:
        // Shoulnd't be here
        Transmit_Struct.SPI_DR = NULL_PTR;
        Transmit_Struct.SPI_SR = NULL_PTR;
        break;
    }

    // Clk polarity select
    switch (Spi_CurrentJobConfig.SpiClkPol)
    {
    case SPI_CLK_POL_HIGH:
        SET_BIT(*SPI_CR1, SPI_CR1_CPOL); //idle = 1
        break;
    case SPI_CLK_POL_LOW:
        CLR_BIT(*SPI_CR1, SPI_CR1_CPOL); //idle = 0
        break;
    default:
        // ERROR
        break;
    }

    // Clk phase select
    switch (Spi_CurrentJobConfig.SpiClkPhase)
    {
    case SPI_CLK_PHASE_FIRST:
        CLR_BIT(*SPI_CR1, SPI_CR1_CPHA); // Data on First Clock
        break;
    case SPI_CLK_PHASE_SECOND:
        SET_BIT(*SPI_CR1, SPI_CR1_CPHA); // Data on Second Clock
        break;
    default:
        // ERROR
        break;
    }

    // Baud Rate Select
    *(SPI_CR1) &= (~((uint32)Spi_CurrentJobConfig.SpiBaudRate << SPI_CR1_BR0));
    *(SPI_CR1) |= ((uint32)Spi_CurrentJobConfig.SpiBaudRate << SPI_CR1_BR0);

    // Clear ss pin (Active low)
    if (Spi_CurrentJobConfig.SpiCsOn == TRUE)
    {
        // HW handle of SS bit
        CLR_BIT(*SPI_CR1, SPI_CR1_SSM);
        CLR_BIT(*SPI_CR1, SPI_CR1_SSI);
    }
    else
    {
        // SW handle of SS bit
        SET_BIT(*SPI_CR1, SPI_CR1_SSI);
        SET_BIT(*SPI_CR1, SPI_CR1_SSM);
        Dio_WriteChannel(Spi_CurrentJobConfig.SpiCSPin, STD_LOW);
    }

    // Scan all channels in the job availabe to send
    for (SpiChIdx = 0; SpiChIdx < Spi_CurrentJobConfig.No_Channel; SpiChIdx++)
    {

        // Access channel with ID ChnlLinkPtrPhysical[SpiChIdx] from the main config struct
        Spi_CurrentCh = Spi_CurrentJobConfig.ChnlLinkPtrPhysical[SpiChIdx];
        Spi_CurrentChConfig = Spi_ConfigPtr->Spi_ChannelConfigPtr[Spi_CurrentCh];

        // Config LSB / MSB
        switch (Spi_CurrentChConfig.SpiTransferStart)
        {
        case LSB_FIRST:
            SET_BIT(*SPI_CR1, SPI_CR1_LSBF); // LSB sent first
            break;
        case MSB_FIRST:
            CLR_BIT(*SPI_CR1, SPI_CR1_LSBF); // MSB sent first
            break;
        default:
            break;
        }

        if (Spi_CurrentChConfig.ChannelType == SpiChannelBufferIB)
        {
            // Check if IB is null
            if (Spi_IB[Spi_CurrentCh] == NULL_PTR)
            {
                Transmit_Struct.Srcdata = &Spi_CurrentChConfig.SpiDefaultData;
            }
            else
            {
                Transmit_Struct.Srcdata = Spi_IB[Spi_CurrentCh];
            }
            Transmit_Struct.Desdata = Spi_IB[Spi_CurrentCh];
            Transmit_Struct.Length = Spi_CurrentChConfig.NoOfDataElements;
        }
        else
        {
            if (Spi_EBInstance[Spi_CurrentCh].srcDataPtr == NULL_PTR)
            {
                Transmit_Struct.Srcdata = &Spi_CurrentChConfig.SpiDefaultData;
            }
            else
            {
                Transmit_Struct.Srcdata = Spi_EBInstance[Spi_CurrentCh].srcDataPtr;
            }
            Transmit_Struct.Desdata = Spi_EBInstance[Spi_CurrentCh].DestDataPtr;
            Transmit_Struct.Length = Spi_EBInstance[Spi_CurrentCh].Length;
        }

        // Select Data Frame Format
        switch (Spi_CurrentChConfig.SpiDataWidth)
        {
        case SPI_DFF_MODE_8Bit:
            CLR_BIT(*SPI_CR1, SPI_CR1_DFF); // 8-Bit data Selected
            break;
        case SPI_DFF_MODE_16Bit:
            SET_BIT(*SPI_CR1, SPI_CR1_DFF); // 16-Bit data Selected
            break;
        default:
            break;
        }

        // Send Data
        SPI_StaticStartTransmission(Transmit_Struct);
    } // End of channels in the Job

    // set SS pin
    if (Spi_CurrentJobConfig.SpiCsOn == TRUE)
    {
        // HW handle of SS bit
        SET_BIT(*SPI_CR1, SPI_CR1_SSI);
    }
    else
    {
        // SW handle of SS bit
        Dio_WriteChannel(Spi_CurrentJobConfig.SpiCSPin, STD_HIGH);
    }
}

/**
 * name : Dio_WriteChannel
 * param: ChannelId: ID of DIO channel 
 *            Level: Value to be written
 * Reentrancy: Reentrant
 * Sync
 * Description: Service to set a level of a channel.
 **/
static void Dio_WriteChannel(Spi_CS_Pin ChannelId, Std_ReturnType Level)
{
    if (ChannelId >= PORTA_CHANNEL_OFFEST && ChannelId < PORTx_CHANNEL_OFFSET)
    {
        // OK proceed
        /* 
        * USE BSRR and BRR registers to provide atomic channel access 
        */
        if (ChannelId >= PORTA_CHANNEL_OFFEST &&
            ChannelId < PORTB_CHANNEL_OFFEST)
        {
            switch (Level)
            {
            case STD_HIGH:
                SET_BIT(GPIOA_BSRR, ChannelId); // Set Channel
                break;
            case STD_LOW:
                SET_BIT(GPIOA_BRR, ChannelId); // Reset Channel
                break;
            default:
                // shouldn't be here
                break;
            }
        }
        else if (ChannelId >= PORTB_CHANNEL_OFFEST &&
                 ChannelId < PORTC_CHANNEL_OFFEST)
        {
            ChannelId -= PORTB_CHANNEL_OFFEST;
            switch (Level)
            {
            case STD_HIGH:
                SET_BIT(GPIOB_BSRR, ChannelId); // Set Channel
                break;
            case STD_LOW:
                SET_BIT(GPIOB_BRR, ChannelId); // Reset Channel
                break;
            default:
                // shouldn't be here
                break;
            }
        }
        else
        {
            ChannelId -= PORTC_CHANNEL_OFFEST;
            switch (Level)
            {
            case STD_HIGH:
                SET_BIT(GPIOC_BSRR, ChannelId); // Set Channel
                break;
            case STD_LOW:
                SET_BIT(GPIOC_BRR, ChannelId); // Reset Channel
                break;
            default:
                // shouldn't be here
                break;
            }
        }
    }
    else
    {
        // Shouldn't be here
    }

    return;
}

/**
    * @name : Spi_StaticInitHWUnits
		* @param: NONE
    * @Description: initialize all HW units. 
**/
static void Spi_StaticInitHWUnits(void)

{
    // [SWS_Spi_00040] The SPI Handler/Driver handles only the Master mode.

    SET_BIT(SPI1_CR1, SPI_CR1_MSTR);
    SET_BIT(SPI2_CR1, SPI_CR1_MSTR);

    return;
}
