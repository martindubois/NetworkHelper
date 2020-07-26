
// Author     KMS - Martin Dubois, P.Eng.
// Copyrights (C) 2020 KMS. All rights reserved.
// Product    NetworkHelper
// File       NHLib/Check_Public.cpp

// CODE REVIEW 2020-07-26 KMS - Martin Dubois, P.Eng.

// TEST COVERAGE 2020-07-26 KMS - Martin Dubois, P.Eng.

// ===== C ==================================================================
#include <assert.h>

// ===== NHLib ==============================================================
#include "Checks.h"

// Public
/////////////////////////////////////////////////////////////////////////////

// aType    [-K-;R--]
Check_Public::Check_Public(const char * aType, int aCode, const char * aMessage) : Check(aType, aCode, aMessage)
{
}

// ===== Check ==============================================================

Check_Public::~Check_Public()
{
}

unsigned int Check_Public::Verify(const NH::Interface & aElement) const
{
    assert(NULL != &aElement);

    unsigned int lResult = 0;

    if (!aElement.IsPublic())
    {
        lResult += Check::Verify(aElement);
    }

    return lResult;
}
