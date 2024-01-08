/** ==================================================================================================================*\
  @file UT_E2E_profile1.c

  @brief Testy jednostkowe do E2E_profile1
\**====================================================================================================================*/
#include "..\inc\Std_Types.h"
#include "..\inc\acutest.h"
#include "E2E_profile1.c"
#include "crc.c"

typedef struct {
    E2E_P01ConfigType config;
    uint8_t data[8];
    uint8_t expectedCRC;
} TestCase;

uint8_t Crc_CalculateCRC8Mock(uint8_t* data, uint16_t length, uint8_t seed, bool reflected)
{
    return 0;
}

/**
  @brief Test of E2E_P01Protect
**/
void Test_Of_E2E_P01Protect(void)
{

    Std_ReturnType retv;
    E2E_P01ConfigType Config;
    E2E_P01ConfigType* PointerConfig = &Config;
    E2E_P01SenderStateType State;
    E2E_P01SenderStateType* PointerState = &State;
    uint8_t Data;
    uint8_t* PointerData = &Data;

    retv = E2E_P01Protect(PointerConfig,PointerState,PointerData);
    TEST_CHECK(retv == E_OK);
}


/**
  @brief Test of E2E_P01Check
**/
void Test_Of_E2E_P01Check(void)
{

    Std_ReturnType retv;
    E2E_P01ConfigType Config;
    E2E_P01ConfigType* PointerConfig = &Config;
    E2E_P01ReceiverStateType State;
    E2E_P01ReceiverStateType* PointerState = &State;
    uint8_t Data;
    uint8_t* PointerData = &Data;

    retv = E2E_P01Check(PointerConfig,PointerState,PointerData);
    TEST_CHECK(retv == E_OK);
}

/**
  @brief Test of E2E_P01_CalculateCRC
**/

void Test_Of_E2E_P01_CalculateCRC(void)
{
	
	E2E_P01ConfigType Config;
    Config.CounterOffset = 3 * 8;
	Config.CRCOffset = 0;
	Config.DataIDMode = E2E_P01_DATAID_BOTH;
	Config.DataLength = 2 * 8;
	Config.MaxDeltaCounterInit = 3;
	uint8_t data[8] = {0, 1, 2, 3, 4, 5, 6, 7};
	uint8_t crcResult = E2E_P01_CalculateCRC(&Config, 0, data);
	TEST_CHECK(crcResult == 228);

	Config.DataIDMode = E2E_P01_DATAID_LOW;
	crcResult = E2E_P01_CalculateCRC(&Config, 0, data);
	TEST_CHECK(crcResult == 131);
	
	Config.DataIDMode = E2E_P01_DATAID_ALT;
	crcResult = E2E_P01_CalculateCRC(&Config, 0, data);
	TEST_CHECK(crcResult == 131);
	
	Config.DataIDMode = E2E_P01_DATAID_ALT;
	crcResult = E2E_P01_CalculateCRC(&Config, 1, data);
	TEST_CHECK(crcResult == 232);
	
	Config.DataIDMode = E2E_P01_DATAID_NIBBLE;
	crcResult = E2E_P01_CalculateCRC(&Config, 0, data);
	TEST_CHECK(crcResult == 213);
	
	Config.CRCOffset = 8;
	crcResult = E2E_P01_CalculateCRC(&Config, 0, data);
	TEST_CHECK(crcResult == 200);
	
}

/*
  Lista testów 
*/
TEST_LIST = 
{
    { "Test of E2E_P01Protect", Test_Of_E2E_P01Protect },
    { "Test of E2E_P01Check", Test_Of_E2E_P01Check },
	{ "Test of Test_Of_E2E_P01_CalculateCRC", Test_Of_E2E_P01_CalculateCRC },
    { NULL, NULL }                                      
};