#include "..\inc\Std_Types.h"
#include "..\inc\crc.h"

/*This service makes a CRC8 calculation on Crc_Length data bytes, with SAE J1850 parameters
Crc_Calculate8() is performed synchrnously. The result is returned within the function call.
[SWS_Crc_00031]
[SWS_Crc_00032]
*/
/*------------------------------------------------------------------------------*/
uint8_t Crc_CalculateCRC8(
    const uint8_t *Crc_DataPtr, /*Pointer to start address of data block to be calculated.*/
    uint32_t Crc_Length,        /*Length of data block to be calculated in bytes.*/
    uint8_t Crc_StartValue8,    /*Start value when the algorithm starts.*/
    bool Crc_IsFirstCall)
{
    uint8_t crc;

    if (Crc_IsFirstCall)    /*TRUE: First call in a sequence or individual CRC calculation; start from initial value, ignore Crc_StartValue8*/
    {
        crc = CRC8_INITIAL_VALUE;
    }
    else                    /*FALSE: Subsequent call in a call sequence; Crc_StartValue8 is interpreted to be the return value of the previous function call.*/
    {
        crc = (CRC8_XOR_VALUE ^ Crc_StartValue8);
    }

    for (size_t i = 0; i < Crc_Length; ++i)
    {
        crc = CRC8_TABLE[crc ^ Crc_DataPtr[i]];
    }

    return (crc ^ CRC8_XOR_VALUE);  /*8 bit result of CRC calculation.*/
}
