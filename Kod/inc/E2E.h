#ifndef E2E_H_
#define E2E_H_

#include "crc.h"
#include "Std_Types.h"
#include "Platform_Types.h"
#include "MemMap.h"
#include "Modules.h"

#define E2E_E_INPUTERR_NULL     0x13
#define E2E_E_INPUTERR_WRONG    0x17
#define E2E_E_INTERR            0x19
#define E2E_E_OK                0x00
#define E2E_E_INVALID           0xFF


uint8 E2E_UpdateCounter(uint8 Counter);

#define E2E_VENDOR_ID               60
#define E2E_MODULE_ID               MODULE_ID_E2E
#define E2E_AR_MAJOR_VERSION        4
#define E2E_AR_MINOR_VERSION        0
#define E2E_AR_PATCH_VERSION        3

#define E2E_SW_MAJOR_VERSION        1
#define E2E_SW_MINOR_VERSION        0
#define E2E_SW_PATCH_VERSION        0

#define E2E_GetVersionInfo(_vi) STD_GET_VERSION_INFO(_vi,E2E)


#endif
