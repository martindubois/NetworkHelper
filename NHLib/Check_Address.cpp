
// Author     KMS - Martin Dubois, P.Eng.
// Copyrights (C) 2020 KMS. All rights reserved.
// Product    NetworkHelper
// File       NHLib/Check_Address.cpp

// CODE REVIEW 2020-07-30 KMS - Martin Dubois, P.Eng.

// TEST COVERAGE 2020-07-30 KMS - Martin Dubois, P.Eng.

// ===== C ==================================================================
#include <assert.h>

// ===== NHLib ==============================================================
#include "IPv4.h"

#include "Checks.h"

// Public
/////////////////////////////////////////////////////////////////////////////

// aType [-K-;R--]
Check_Address::Check_Address(const char * aType, int aCode, const char * aMessage, uint32_t aAddress) : Check(aType, aCode), mAddress(aAddress)
{
    assert(NULL != aMessage);

    char lAddress[32];
    char lMessage[256];

    IPv4_AddressToText(lAddress, sizeof(lAddress), aAddress);

    int lRet = sprintf_s(lMessage, "%s - %s", aMessage, lAddress);
    assert(lRet > 0);
    assert(lRet < sizeof(lMessage));

    SetMessage(lMessage);
}

// ===== Check ==============================================================

Check_Address::~Check_Address()
{
}

unsigned int Check_Address::Verify(const NH::Router & aElement) const
{
    assert(NULL != &aElement);

    unsigned int lResult = 0;

    if (!aElement.mInterfaces.Find(mAddress))
    {
        lResult += Check::Verify(aElement);
    }

    return lResult;
}
