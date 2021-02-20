/******************************************************/
/* Author    : Nourhan Mansour                        */
/* Date      : 18/2/2021                              */
/* Version   : 1.0                                    */
/* File      : det.c                                  */
/******************************************************/
#include "det.h"

/*****************************************/
/*            Type Definitions           */
/*****************************************/
typedef struct 
{
    uint16 ModuleId;
    uint8 InstanceId;
    uint8 ApiId;
    uint8 ErrorId;
}DET_ErrorBufferType;

/*****************************************/
/*            Local Variables            */
/*****************************************/
static boolean Det_initFlag = FALSE;
DET_ErrorBufferType DetErrorBuffer[DET_MAX_ERROR_BUFFER];
static uint8 DetErrorBuffer_idx = 0;


/*****************************************/
/*         Functions definitions         */
/*****************************************/
/**
 * @name Det_Init
 * @param none
 * Service to initialize the Development Error Tracer.
 * non-reeenttrant
 * Sync
 */ 
void Det_Init (void)
{
    Det_initFlag = TRUE;
}

/**
 * @name Det_Start
 * @param none
 * Optional Service to start the Development Error Tracer.
 * non-reeenttrant
 * Sync
 */ 
void Det_Start( void )
{
    // Empty
}


/**
 * @name Det_ReportError
 * @param ModuleId: Module ID of calling module.
 * @param InstanceId:The identifier of the index based instance of a module
 * @param ApiId:ID of API service in which error is detected
 * @param ErrorId:ID of detected development error
 * Reeentrant
 * Sync
 * returns always E_OK 
 */ 
Std_ReturnType Det_ReportError( uint16 ModuleId, uint8 InstanceId, uint8 ApiId, uint8 ErrorId)
{
    /* Det initialized */
    if (Det_initFlag)
    {   
        /* There is more space in the buffer */
        if (DetErrorBuffer_idx < DET_MAX_ERROR_BUFFER)
        {
            DetErrorBuffer->ApiId = ApiId;
            DetErrorBuffer->ErrorId=ErrorId;
            DetErrorBuffer->InstanceId=InstanceId;
            DetErrorBuffer->ModuleId =ModuleId;
        }
        else
        {
            /* Not enough buffer space */
            return E_NOT_OK;
        }

    }
    else 
    {
        /* DET not initialized */
        return E_NOT_OK;
    }    
    /* Error data stored in buffer */
    return E_OK;
}
