/** ==================================================================================================================*\
  @file UT_E2E_profile1.c

  @brief Testy jednostkowe do E2E_profile1
\**====================================================================================================================*/
#include "..\inc\Std_Types.h"
#include "..\inc\acutest.h"
#include "E2E_profile1.c"
#include "crc.c"

/**
  @brief Test of E2E_P01Protect
**/
void Test_Of_E2E_P01Protect(void)
{
    E2E_P01ConfigType Config;
    E2E_P01SenderStateType State;
	uint8_t dataOK[8] = {0, 1, 2, 3, 4, 5, 6, 7};
	uint8_t data[] = {};
	
    TEST_CHECK(E2E_P01Protect(NULL, &State, data) == E2E_E_INPUTERR_NULL);
	
	E2E_P01ConfigType ConfigOK1 = {
        .DataLength = 16,
        .CounterOffset = 4,
        .CRCOffset = 8
    };
    TEST_CHECK(E2E_P01Protect(&ConfigOK1, NULL, data) == E2E_E_INPUTERR_NULL);
    TEST_CHECK(E2E_P01Protect(&ConfigOK1, &State, NULL) == E2E_E_INPUTERR_NULL);
	
	
	Std_ReturnType result = E2E_P01Protect(&ConfigOK1, &State, dataOK);
	
	TEST_CHECK(result == E2E_E_OK);
	TEST_CHECK(dataOK[ConfigOK1.CounterOffset / 8] == 0);
	TEST_CHECK(dataOK[ConfigOK1.CRCOffset / 8] == 196);
	
	E2E_P01ConfigType ConfigOK2 = {
        .DataLength = 32,
        .CounterOffset = 16,
        .CRCOffset = 24
    };
	
	result = E2E_P01Protect(&ConfigOK2, &State, dataOK);
	
	TEST_CHECK(result == E2E_E_OK);
	TEST_CHECK(dataOK[ConfigOK2.CounterOffset / 8] == 1);
	TEST_CHECK(dataOK[ConfigOK2.CRCOffset / 8] == 29);
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

/**
  @brief Test of E2E_UpdateCounter
**/

void Test_Of_E2E_UpdateCounter(void)
{
    /* Test for valid range 0-14 */
    TEST_CHECK(E2E_UpdateCounter(0) == 1);
    TEST_CHECK(E2E_UpdateCounter(7) == 8);
    TEST_CHECK(E2E_UpdateCounter(14) == 0);

    /* Test for invalid input (greater than 14) */
    TEST_CHECK(E2E_UpdateCounter(15) == 1);
    TEST_CHECK(E2E_UpdateCounter(20) == 6);
}

/**
  @brief Test of CalculateDeltaCounter
**/

void Test_Of_CalculateDeltaCounter(void)
{
    /* Test for receivedCounter >= lastValidCounter */
    TEST_CHECK(CalculateDeltaCounter(8, 5) == 3);
    TEST_CHECK(CalculateDeltaCounter(5, 5) == 0);
    TEST_CHECK(CalculateDeltaCounter(0, 14) == 1);

    /* Test for receivedCounter < lastValidCounter */
    TEST_CHECK(CalculateDeltaCounter(5, 8) == 12);
    TEST_CHECK(CalculateDeltaCounter(14, 0) == 14);
}

/* Test for CheckConfig with NULL Config */
void Test_CheckConfig(void) {
    Std_ReturnType result = CheckConfig(NULL);
    TEST_CHECK(result == E2E_E_INPUTERR_NULL);

    E2E_P01ConfigType ConfigOK = {
        .DataLength = 16,
        .CounterOffset = 4,
        .CRCOffset = 8
    };

    result = CheckConfig(&ConfigOK);
    TEST_CHECK(result == E2E_E_OK);

    E2E_P01ConfigType ConfigInvalidDataLength = {
        .DataLength = 17,
        .CounterOffset = 4,
        .CRCOffset = 8
    };

    result = CheckConfig(&ConfigInvalidDataLength);
    TEST_CHECK(result == E2E_E_INPUTERR_WRONG);

    E2E_P01ConfigType ConfigInvalidCounterOffset = {
        .DataLength = 16,
        .CounterOffset = 3,
        .CRCOffset = 8
    };

    result = CheckConfig(&ConfigInvalidCounterOffset);
    TEST_CHECK(result == E2E_E_INPUTERR_WRONG);

    E2E_P01ConfigType ConfigOverlappingOffsets = {
        .DataLength = 0,
        .CounterOffset = 16,
        .CRCOffset = 16
    };

    result = CheckConfig(&ConfigOverlappingOffsets);
    TEST_CHECK(result == E2E_E_INPUTERR_WRONG);
}

/*
  Lista testów 
*/
TEST_LIST = 
{
    { "Test of E2E_P01Protect", Test_Of_E2E_P01Protect },
    { "Test of E2E_P01Check", Test_Of_E2E_P01Check },
	{ "Test of Test_Of_E2E_P01_CalculateCRC", Test_Of_E2E_P01_CalculateCRC },
	{ "Test of E2E_UpdateCounter", Test_Of_E2E_UpdateCounter },
	{ "Test of CalculateDeltaCounter", Test_Of_CalculateDeltaCounter },
	{ "Test of CheckConfig", Test_CheckConfig },
    { NULL, NULL }                                      
};