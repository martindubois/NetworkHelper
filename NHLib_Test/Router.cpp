
// Author    KMS - Martin Dubois, P.Eng.
// Copyright (C) 2020 KMS. All rights reserved.
// Product   NetworkHelper
// File      NHLib_Test/Router.h

// ===== Import/Includes ====================================================
#include <KmsTest.h>

// ===== Import/Includes ====================================================
#include <HI/HTML_Document.h>

// ===== Includes ===========================================================
#include <Cisco/Router.h>
#include <NH/Network.h>

// Constants
/////////////////////////////////////////////////////////////////////////////

static const char * ERRORS[]
{
    "Test_interface",
    "Test_ip_access-list",
    "Test_ip_dhcp_pool",
    "Test_ip_route",

    NULL
};

// Static function declarations
/////////////////////////////////////////////////////////////////////////////

static void Test (unsigned int aTestIndex, unsigned int aRouterIndex);
static void Test (const char * aFileName);
static void Tests(unsigned int aTestIndex, unsigned int aRouterQty);

static bool Test_Error(const char * aFileName);

// Macro
/////////////////////////////////////////////////////////////////////////////

#define CATCH                      \
    catch (std::exception eE)      \
    {                              \
        KMS_TEST_ERROR_INFO;       \
        printf("%s\n", eE.what()); \
    }

// Tests
/////////////////////////////////////////////////////////////////////////////

KMS_TEST_BEGIN(Router_Base)
{
    Tests(0, 1);
    Tests(1, 1);
    Tests(2, 2);
    Tests(3, 3);
    Tests(4, 4);
    Tests(5, 6);
}
KMS_TEST_END

KMS_TEST_BEGIN(Router_Error)
{
    Cisco::Router lCR;

    // ===== RetieveInfo ====================================================

    try
    {
        lCR.RetrieveInfo(NH::Router::INFO_COM_PORT, "");
        KMS_TEST_ASSERT(false);
    }
    CATCH

    try
    {
        lCR.RetrieveInfo(NH::Router::INFO_CONFIG_FILE, "DoesNotExist.txt");
        KMS_TEST_ASSERT(false);
    }
    CATCH

    try
    {
        Tests(6, 1);
        KMS_TEST_ASSERT(false);
    }
    CATCH

    try
    {
        Tests(7, 1);
        KMS_TEST_ASSERT(false);
    }
    CATCH

    try
    {
        Test("Test_ip_routing");
        KMS_TEST_ASSERT(false);
    }
    CATCH

    unsigned int lIndex = 0;
    while (NULL != ERRORS[lIndex])
    {
        KMS_TEST_ASSERT(Test_Error(ERRORS[lIndex]));

        lIndex++;
    }

}
KMS_TEST_END

// Static functions
/////////////////////////////////////////////////////////////////////////////

void Test(unsigned int aTestIndex, unsigned int aRouterIndex)
{
    char lFileName[64];

    int lRet = sprintf_s(lFileName, "Test_%u_Router_%u", aTestIndex, aRouterIndex);
    assert(lRet > 0);
    assert(lRet < sizeof(lFileName));

    Test(lFileName);
}

void Test(const char * aFileName)
{
    Cisco::Router lCR;
    char          lFileName[256];
    NH::Network   lNN;

    lCR.SetSubNetList(&lNN.mSubNets);

    int lRet = sprintf_s(lFileName, "NHLib_Test/Tests/%s.txt", aFileName);
    assert(lRet > 0);
    assert(lRet < sizeof(lFileName));

    lCR.RetrieveInfo(NH::Router::INFO_CONFIG_FILE, lFileName);

    lNN.AddRouter(&lCR);

    lNN.AddKnownRouters();

    lNN.Verify();

    lNN.Generate_HTML(HI::FOLDER_CURRENT, aFileName, aFileName);
}

void Tests(unsigned int aTestIndex, unsigned int aRouterQty)
{
    assert(0 < aRouterQty);

    for (unsigned int i = 0; i < aRouterQty; i ++)
    {
        Test(aTestIndex, i);
    }
}

bool Test_Error(const char * aFileName)
{
    Cisco::Router lCR;
    char          lFileName[256];
    NH::Network   lNN;
    bool          lResult = true;

    lCR.SetSubNetList(&lNN.mSubNets);

    int lRet = sprintf_s(lFileName, "NHLib_Test/Tests/%s.txt", aFileName);
    assert(lRet > 0);
    assert(lRet < sizeof(lFileName));

    try
    {
        lCR.RetrieveInfo(NH::Router::INFO_CONFIG_FILE, lFileName);
        lResult = false;
    }
    CATCH

    lNN.AddRouter(&lCR);

    lNN.AddKnownRouters();

    try
    {
        lNN.Verify();
        lResult = false;
    }
    CATCH

    lNN.Generate_HTML(HI::FOLDER_CURRENT, aFileName, aFileName);

    return lResult;
}
