
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

// Static function declarations
/////////////////////////////////////////////////////////////////////////////

static bool Test (unsigned int aTestIndex, unsigned int aRouterIndex);
static bool Tests(unsigned int aTestIndex, unsigned int aRouterQty);

// Tests
/////////////////////////////////////////////////////////////////////////////

KMS_TEST_BEGIN(Router_Base)

    Cisco::Router lCR;

    // ===== RetieveInfo ====================================================

    try
    {
        lCR.RetrieveInfo(NH::Router::INFO_COM_PORT, "");
        KMS_TEST_ASSERT(false);
    }
    catch (std::exception eE)
    {
        KMS_TEST_ERROR_INFO;
        printf("%s\n", eE.what());
    }

    try
    {
        lCR.RetrieveInfo(NH::Router::INFO_CONFIG_FILE, "DoesNotExist.txt");
        KMS_TEST_ASSERT(false);
    }
    catch (std::exception eE)
    {
        KMS_TEST_ERROR_INFO;
        printf("%s\n", eE.what());
    }

    KMS_TEST_ASSERT(Tests(0, 1));
    KMS_TEST_ASSERT(Tests(1, 1));
    KMS_TEST_ASSERT(Tests(2, 2));
    KMS_TEST_ASSERT(Tests(3, 3));
    KMS_TEST_ASSERT(Tests(4, 4));
    KMS_TEST_ASSERT(Tests(5, 6));

    try
    {
        Tests(6, 1);
        KMS_TEST_ASSERT(false);
    }
    catch (std::exception eE)
    {
        KMS_TEST_ERROR_INFO;
        printf("%s\n", eE.what());
    }

KMS_TEST_END

// Static functions
/////////////////////////////////////////////////////////////////////////////

bool Test(unsigned int aTestIndex, unsigned int aRouterIndex)
{
    Cisco::Router lCR;
    char          lFile[64];
    NH::Network   lNN;
    int           lRet;

    lCR.SetSubNetList(&lNN.mSubNets);

    lRet = sprintf_s(lFile, "NHLib_Test/Tests/Test_%u_Router_%u.txt", aTestIndex, aRouterIndex);
    assert(            0 < lRet);
    assert(sizeof(lFile) > lRet);

    lCR.RetrieveInfo(NH::Router::INFO_CONFIG_FILE, lFile);

    lNN.AddRouter(&lCR);

    lNN.AddKnownRouters();

    lNN.Verify();

    char lTitle[64];

    lRet = sprintf_s(lFile , "Test_%u_Router_%u", aTestIndex, aRouterIndex);
    assert(            0 < lRet);
    assert(sizeof(lFile) > lRet);

    lRet = sprintf_s(lTitle, "Test %u - Router %u", aTestIndex, aRouterIndex);
    assert(            0 < lRet);
    assert(sizeof(lFile) > lRet);

    lNN.Generate_HTML(HI::FOLDER_CURRENT, lFile, lTitle);

    return true;
}

bool Tests(unsigned int aTestIndex, unsigned int aRouterQty)
{
    assert(0 < aRouterQty);

    for (unsigned int i = 0; i < aRouterQty; i ++)
    {
        if (!Test(aTestIndex, i))
        {
            return false;
        }
    }

    return true;
}
