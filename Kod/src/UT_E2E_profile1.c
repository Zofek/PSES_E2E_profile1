/** ==================================================================================================================*\
  @file UT_E2E_profile1.c

  @brief Testy jednostkowe do E2E_profile1
\**====================================================================================================================*/
#include "..\inc\Std_Types.h"
#include "..\inc\acutest.h"
#include "E2E_profile1.c"
#include "crc.c"
/**
  @brief Test of compute_p01_crc
**/
void Test_Of_Compute_P01_Crc(void)
{
//sprawdzenie wymagań przez review - spełnione 

    uint8_t retv;
    E2E_P01ConfigType Config;
    E2E_P01ConfigType* PointerConfig = &Config;
    uint8_t Counter;
    uint8_t Data;
    uint8_t* PointerData = &Data;
    retv = compute_p01_crc(PointerConfig,Counter,PointerData);
    TEST_CHECK(retv == 1);
}

/**
  @brief Test of E2E_P01Protect
**/
void Test_Of_E2E_P01Protect(void)
{
//sprawdzenie wymagań przez review - spełnione 

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
//sprawdzenie wymagań przez review - spełnione 

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


/*
  Lista testów 
*/
TEST_LIST = {
    { "Test of Compute_P01_Crc", Test_Of_Compute_P01_Crc },
    { "Test of Test_Of_E2E_P01Protect", Test_Of_E2E_P01Protect },
    { "Test of Test_Of_E2E_P01Check", Test_Of_E2E_P01Check },
    { NULL, NULL }                                      
};