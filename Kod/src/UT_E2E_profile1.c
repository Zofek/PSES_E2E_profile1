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
    E2E_P01ConfigType* PointerConfig = &Config;
	uint8_t Counter;
	uint8_t data[8] = {0, 1, 2, 3, 4, 5, 6, 7};
	
	uint8_t crcResult = E2E_P01_CalculateCRC(PointerConfig, 0, data);
	TEST_CHECK(crcResult == 0);
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