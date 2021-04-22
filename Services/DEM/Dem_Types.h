/************************************************************************/
/* Author    : Nourhan Mansour                                          */
/* Date      : 14/4/2021                                                */
/* Version   : 1.1.0                                                    */
/* File      : Dem_Types.h                                              */
/************************************************************************/

#ifndef DEM_TYPES_H
#define DEM_TYPES_H

#include "STD_TYPES.h"

/* dentification of an event by assigned EventId. The EventId is
    assigned by the Dem.
    Example:
    1 refers to monitor x,
    2 refers to monitor y, etc.
*/
typedef uint16 Dem_EventIdType;

/*
This type contains all monitor test result values, which can be
reported via Dem_SetEventStatus().
DEM_EVENT_STATUS_PASSED                 0x00 Monitor reports qualified test result passed.
DEM_EVENT_STATUS_FAILED                 0x01 Monitor reports qualified test result failed.
DEM_EVENT_STATUS_PREPASSED              0x02 Monitor reports non-qualified test result pre-passed (debounced Dem-internally).
DEM_EVENT_STATUS_PREFAILED              0x03 Monitor reports non-qualified test result pre-failed (debounced Dem-internally).
DEM_EVENT_STATUS_FDC_THRESHOLD_REACHED  0x04 Monitor triggers the storage of ExtendedDataRecords andFreeze Frames ON_FDC_THRESHOLD.
*/
typedef uint8 Dem_EventStatusType; 

#endif