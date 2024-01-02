#include "..\inc\Std_Types.h"
#include "..\inc\crc.h"

/*funkcja pozwalająca na obliczenie CRC8
[PRS_E2E_00070]
*/
/*------------------------------------------------------------------------------*/
uint8_t Crc_CalculateCRC8(
    const uint8_t *Crc_DataPtr,
    uint32_t Crc_Length,
    uint8_t Crc_StartValue8,
    bool Crc_IsFirstCall)
{
    uint8_t crc;

    if (Crc_IsFirstCall)
    {
        crc = CRC8_INITIAL_VALUE;
    }
    else
    {
        crc = (CRC8_XOR_VALUE ^ Crc_StartValue8);
    }

    for (size_t i = 0; i < Crc_Length; ++i)
    {
        crc = CRC8_TABLE[crc ^ Crc_DataPtr[i]];
    }

    return (crc ^ CRC8_XOR_VALUE);
}
