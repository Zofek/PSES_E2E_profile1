#ifndef E2E_P01_H_
#define E2E_P01_H_

#include "E2E.h"
#include "crc.h"
#include "Std_Types.h"
#include "Platform_Types.h"
#include "MemMap.h"
#include "Modules.h"

typedef enum {
    E2E_P01_DATAID_BOTH = 0x0,
    E2E_P01_DATAID_ALT = 0x1,
    E2E_P01_DATAID_LOW = 0x2,
    E2E_P01_DATAID_NIBBLE = 0x3
} E2E_P01DataIDMode;

typedef struct  {
    uint16_t CounterOffset;
    uint16_t CRCOffset;
    uint16_t DataID;
    E2E_P01DataIDMode DataIDMode;
    uint16_t DataLength;
    uint8_t MaxDeltaCounterInit;
} E2E_P01ConfigType;

typedef struct {
    uint8_t Counter;
} E2E_P01SenderStateType;

typedef enum 
{
        E2E_P01STATUS_OK = 0x0,
        E2E_P01STATUS_NONEWDATA = 0x1,
        E2E_P01STATUS_WRONGCRC = 0x2,
        E2E_P01STATUS_INITAL = 0x4,
        E2E_P01STATUS_REPEATED = 0x8,
        E2E_P01STATUS_OKSOMELOST = 0x20,
        E2E_P01STATUS_WRONGSEQUENCE = 0x40
} E2E_P01ReceiverStatusType;

typedef struct 
{
    uint8_t LastValidCounter;
    uint8_t MaxDeltaCounter;
    boolean WaitForFirstData;
    boolean NewDataAvailable;
    uint8_t LostData;
    E2E_P01ReceiverStatusType Status;
} E2E_P01ReceiverStateType;

uint8_t E2E_UpdateCounter(uint8_t Counter);
uint8_t CalculateDeltaCounter(uint8 receivedCounter, uint8 lastValidCounter);
Std_ReturnType CheckConfig(E2E_P01ConfigType* Config);
uint8_t E2E_P01_CalculateCRC(E2E_P01ConfigType* Config, uint8 Counter, uint8* Data);
Std_ReturnType E2E_P01Protect( E2E_P01ConfigType* Config, E2E_P01SenderStateType* State, uint8* Data);

Std_ReturnType E2E_P01Check( E2E_P01ConfigType* Config, E2E_P01ReceiverStateType* State, uint8* Data);

#endif
