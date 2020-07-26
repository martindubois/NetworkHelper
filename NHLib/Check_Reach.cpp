
// Author     KMS - Martin Dubois, P.Eng.
// Copyrights (C) 2020 KMS. All rights reserved.
// Product    NetworkHelper
// File       NHLib/Check_Reach.cpp

// CODE REVIEW 2020-07-26 KMS - Martin Dubois, P.Eng.

// TEST COVERAGE 2020-07-26 KMS - Martin Dubois, P.Eng.

// ===== C ==================================================================
#include <assert.h>

// ===== Includes ===========================================================
#include <NH/SubNet.h>

// ===== NHLib ==============================================================
#include "IPv4.h"

#include "Checks.h"

// Public
/////////////////////////////////////////////////////////////////////////////

// aType    [-K-;R--]
Check_Reach::Check_Reach(const char * aType, int aCode, const char * aMessage, uint32_t aAddr) : Check(aType, aCode), mAddr(aAddr), mSubNet(NULL)
{
    assert(NULL != aMessage);

    char lAddr   [ 32];
    char lMessage[256];

    IPv4_AddressToText(lAddr, sizeof(lAddr), aAddr);

    int lRet = sprintf_s(lMessage, "%s - %s", aMessage, lAddr);
    assert(               0 < lRet);
    assert(sizeof(lMessage) > lRet);

    SetMessage(lMessage);
}

// aType    [-K-;R--]
// aSubNet  [-K-;R--]
Check_Reach::Check_Reach(const char * aType, int aCode, const char * aMessage, const NH::SubNet * aSubNet) : Check(aType, aCode), mAddr(0), mSubNet(aSubNet)
{
    assert(NULL != aMessage);
    assert(NULL != aSubNet);

    char lFullName[128];
    char lMessage [256];

    aSubNet->GetFullName(lFullName, sizeof(lFullName));

    int lRet = sprintf_s(lMessage, "%s - %s", aMessage, lFullName);
    assert(               0 < lRet);
    assert(sizeof(lMessage) > lRet);

    SetMessage(lMessage);
}

// ===== Check ==============================================================

Check_Reach::~Check_Reach()
{
}

unsigned int Check_Reach::Verify(const NH::Router & aElement) const
{
    assert(NULL != &aElement);

    unsigned int lResult = 0;

    if (0 == mAddr)
    {
        assert(NULL != mSubNet);

        if (!aElement.CanReach(*mSubNet))
        {
            lResult += Check::Verify(aElement);
        }
    }
    else
    {
        if (!aElement.CanReach(mAddr))
        {
            lResult += Check::Verify(aElement);
        }
    }

    return lResult;
}
