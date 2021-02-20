/******************************************************/
/* Author    : Nourhan Mansour                        */
/* Date      : 18/2/2021                              */
/* Version   : 1.0                                    */
/* File      : det.h                                  */
/******************************************************/

#ifndef DET_H
#define DET_H

#include "STD_TYPES.h"

#define DET_MAX_ERROR_BUFFER        (5U)

/******************************************************/
/*               Functions Definitons                 */
/******************************************************/
void Det_Init (void);

Std_ReturnType Det_ReportError( uint16 ModuleId, uint8 InstanceId, uint8 ApiId, uint8 ErrorId);

void Det_Start( void );

#ifdef DetVersionInfoApi
    void Det_GetVersionInfo( Std_VersionInfoType* versioninfo);
#endif

#endif      // EOF
