
// Author     KMS - Martin Dubois, P.Eng.
// Copyrights (C) 2020 KMS. All rights reserved.
// Product    NetworkHelper
// File       NHLib/Check_Private.cpp

// CODE REVIEW 2020-07-27 KMS - Martin Dubois, P.Eng.

// TEST COVERAGE 2020-07-27 KMS - Martin Dubois, P.Eng.

// ===== C ==================================================================
#include <assert.h>

// ===== NHLib ==============================================================
#include "Checks.h"

// Public
/////////////////////////////////////////////////////////////////////////////

// aType [-K-;R--]
Check_Private::Check_Private(const char * aType, int aCode, const char * aMessage) : Check(aType, aCode, aMessage)
{
}

// ===== Check ==============================================================

Check_Private::~Check_Private()
{
}

unsigned int Check_Private::Verify(const NH::Interface & aElement) const
{
    assert(NULL != &aElement);

    unsigned int lResult = 0;

    if (!aElement.IsPrivate())
    {
        lResult += Check::Verify(aElement);
    }

    return lResult;
}
