#include "..\inc\Std_Types.h"
#include "..\inc\crc.h"
#include "..\inc\E2E_profile1.h"

#define E2E_P01_DATAID_BOTH 0x0
#define E2E_P01_DATAID_ALT 0x1
#define E2E_P01_DATAID_LOW 0x2
#define E2E_P01_DATAID_NIBBLE 0x3

#define MAX_P01_DATA_LENGTH_IN_BITS    (240)
#define MAX_P01_COUNTER_VALUE          (14)


/*Creation of the E2E header*/
/*--------------------------------------------------------------------------------------------*/

/*----------------------------------------------*/
uint8_t E2E_UpdateCounter(uint8_t Counter) /*increment values in range 0-14 and update counter */
{ 
    return (Counter + 1) % 15; /* Use the Profile 1 value */
}

/*----------------------------------------------*/
uint8_t CalculateDeltaCounter(uint8_t receivedCounter, uint8_t lastValidCounter)
{
    if (receivedCounter >= lastValidCounter) 
    {
        return receivedCounter - lastValidCounter;
    }
    else 
    {
        return MAX_P01_COUNTER_VALUE + 1 + receivedCounter - lastValidCounter;
    }
}

/*----------------------------------------------*/
Std_ReturnType CheckConfig(E2E_P01ConfigType* Config) 
{

    /* Check for NULL pointers */
    if (Config == NULL) 
    {
        return E2E_E_INPUTERR_NULL; /* MISRA exception */
    }

    /* Check input parameters values */
    if ((Config -> DataLength > MAX_P01_DATA_LENGTH_IN_BITS) || (Config -> DataLength % 8 != 0)  ||
        (Config -> CounterOffset % 4 != 0) || Config -> CRCOffset % 8 != 0) 
    {
        return E2E_E_INPUTERR_WRONG; /* MISRA exception */
    }

    /* Check that input parameters do not overlap and fits into the data array. It is enough to
     * check if the CRC and Counter are in the same byte since the CRC is one byte long */
    if ((Config -> CRCOffset + 8 > Config -> DataLength) || (Config -> CounterOffset + 4 > Config -> DataLength) ||
        (Config -> CRCOffset/8 == Config -> CounterOffset/8)) 
    {
        return E2E_E_INPUTERR_WRONG; /* MISRA exception */
    }

    return E2E_E_OK;
	
}


/*----------------------------------------------*/
/*
[PRS_E2E_00163]
[PRS_E2E_00085]
[PRS_E2E_00085]
[PRS_E2E_00306]
[PRS_E2E_00190]
[PRS_E2E_00082]
[PRS_E2E_00640]
*/

uint8_t E2E_P01_CalculateCRC(E2E_P01ConfigType* Config, uint8 Counter, uint8* Data)
{
    uint8_t data_id_lo_byte = (uint8_t)(Config -> DataID);
    uint8_t data_id_hi_byte = (uint8_t)((Config -> DataID)>>8);
    uint8_t crc = 0x00u;

    switch (Config -> DataIDMode)
    {
    case E2E_P01_DATAID_BOTH:

        crc = Crc_CalculateCRC8(&data_id_lo_byte, 1u, crc, false);
        crc = Crc_CalculateCRC8(&data_id_hi_byte, 1u, crc, false);

        break;

    case E2E_P01_DATAID_LOW:

        crc = Crc_CalculateCRC8(&data_id_lo_byte, 1u, crc, false);

        break;

    case E2E_P01_DATAID_ALT:

        if (Counter % 2 == 0)
        {
            crc = Crc_CalculateCRC8(&data_id_lo_byte, 1u, crc, false);
        }
        else
        {
            crc = Crc_CalculateCRC8(&data_id_hi_byte, 1u, crc, false);
        }

        break;

    case E2E_P01_DATAID_NIBBLE:

        crc = Crc_CalculateCRC8(&data_id_lo_byte, 1u, crc, false);

        data_id_hi_byte = 0;

        crc = Crc_CalculateCRC8(&data_id_hi_byte, 1u, crc, false);

        break;
    }

    if (Config -> CRCOffset >= 8)
    {
        // compute crc over data before the crc byte
        crc = Crc_CalculateCRC8(Data, ((Config -> CRCOffset) >> 3), crc, FALSE);
    }

    if (((Config -> CRCOffset) >> 3) < Config -> DataLength) 
    {
        crc = Crc_CalculateCRC8 (Data + ((Config -> CRCOffset) >> 3) + 1, 
                                (Config -> DataLength) - ((Config -> CRCOffset) >> 3),
                                crc, FALSE);
    }

    // write CRC to data
    return crc ^ CRC8_XOR_VALUE;
}

/*----------------------------------------------*/
Std_ReturnType E2E_P01Protect(E2E_P01ConfigType* Config, E2E_P01SenderStateType* State, uint8_t* Data) 
{

    Std_ReturnType returnValue = CheckConfig(Config);

    if (E2E_E_OK != returnValue) 
    {
        return returnValue;
    }

    if ((State == NULL) || (Data == NULL)) 
    {
        return E2E_E_INPUTERR_NULL; /* MISRA exception */
    }

    /* write the counter in data*/
    if (Config -> CounterOffset % 8 == 0) 
    {
        *(Data+(Config -> CounterOffset/8)) = (*(Data+(Config -> CounterOffset/8)) & 0xF0) | (State -> Counter & 0x0F);
    }
    else 
    { /*write DataID nibble in Data, if E2E_P01_DATAID_NIBBLE configuration is used*/
        *(Data+(Config -> CounterOffset/8)) = (*(Data+(Config -> CounterOffset/8)) & 0x0F) | ((State -> Counter<<4) & 0xF0);
    }

    /* compute the CRC and write CRC in Data */
    *(Data+(Config -> CRCOffset/8)) = E2E_P01_CalculateCRC(Config, State -> Counter, Data);
    
    /* increment the Counter (which will be used in the next invocation of E2E_P01Protect()) */
    State -> Counter = E2E_UpdateCounter(State -> Counter); 

    return E2E_E_OK;
}
/*--------------------------------------------------------------------------------------------*/


/*Evaluation of E2E- Header*/
/*--------------------------------------------------------------------------------------------*/

/*----------------------------------------------*/
Std_ReturnType E2E_P01Check(E2E_P01ConfigType* Config, E2E_P01ReceiverStateType* State, uint8_t* Data) 
{

    uint8_t receivedCounter = 0;
    uint8_t receivedCrc = 0;
    uint8_t calculatedCrc = 0;
    uint8_t delta = 0;
    
    Std_ReturnType returnValue = CheckConfig(Config);


    if (E2E_E_OK != returnValue) 
    {
        return returnValue;
    }

    if ((State == NULL) || (Data == NULL)) 
    {
        return E2E_E_INPUTERR_NULL; /* MISRA exception */
    }



    if (State -> MaxDeltaCounter < MAX_P01_COUNTER_VALUE) 
    {
        State -> MaxDeltaCounter++;
    }

    if (State -> NewDataAvailable == FALSE) 
    {
        State -> Status = E2E_P01STATUS_NONEWDATA;
        return E2E_E_OK;  /* MISRA exception */
    }

    /* Counter offset is 4-bit aligned, this check is used to find out if high or low nibble */
    if (Config -> CounterOffset % 8 == 0) 
    {
        receivedCounter = *(Data+(Config -> CounterOffset/8)) & 0x0F;
    }
    else 
    {
        receivedCounter = (*(Data+(Config -> CounterOffset/8)) >> 4) & 0x0F;
    }

    receivedCrc = *(Data+(Config -> CRCOffset/8));
    calculatedCrc = E2E_P01_CalculateCRC(Config, receivedCounter, Data);
    
    // printf("\n received crc  = %d \n", receivedCrc);
    // printf("\n receivedCounter  = %d \n", receivedCounter);
    // printf("\n Data  = %d \n", &Data);
    // printf("\n (Config -> CRCOffset/8)  = %d \n", (Config -> CRCOffset/8));
    // printf("\n calculated crc  = %d \n", calculatedCrc);

    if (receivedCrc != calculatedCrc) 
    {
        State -> Status = E2E_P01STATUS_WRONGCRC;
        return E2E_E_OK;
    }

    /* Check if this is the first data since initialization */
    if (State -> WaitForFirstData == TRUE) 
    {
        State -> WaitForFirstData = FALSE;
        State -> MaxDeltaCounter = Config -> MaxDeltaCounterInit;
        State -> LastValidCounter = receivedCounter;
        State -> Status= E2E_P01STATUS_INITAL;

        return E2E_E_OK;
    }

    /* Check the counter delta */
    delta = CalculateDeltaCounter(receivedCounter, State -> LastValidCounter);

    if (delta == 1)
     {
        State -> MaxDeltaCounter = Config -> MaxDeltaCounterInit;
        State -> LastValidCounter = receivedCounter;
        State -> LostData = 0;
        State -> Status= E2E_P01STATUS_OK;
    }
    else if (delta == 0) 
    {
        State -> Status= E2E_P01STATUS_REPEATED;
    }
    else if (delta <= State -> MaxDeltaCounter) 
    {
        State -> MaxDeltaCounter = Config -> MaxDeltaCounterInit;
        State -> LastValidCounter = receivedCounter;
        State -> LostData = delta - 1;
        State -> Status= E2E_P01STATUS_OKSOMELOST;
    }
    else 
    {
        State -> Status= E2E_P01STATUS_WRONGSEQUENCE;
    }

    return E2E_E_OK;
}
/*--------------------------------------------------------------------------------------------*/
