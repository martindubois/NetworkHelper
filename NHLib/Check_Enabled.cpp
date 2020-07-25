
// Author     KMS - Martin Dubois, P.Eng.
// Copyrights (C) 2020 KMS. All rights reserved.
// Product    NetworkHelper
// File       NHLib/Check_Enabled.cpp

// CODE REVIEW 2020-07-25 KMS - Martin Dubois, P.Eng.

// TEST COVERAGE 2020-07-25 KMS - Martin Dubois, P.Eng.

// ===== C ==================================================================
#include <assert.h>

// ===== NHLib ==============================================================
#include "Errors.h"

#include "Check_Enabled.h"

// Public
/////////////////////////////////////////////////////////////////////////////

// aType    [-K-;R--]
// aMessage [-K-;R--]
Check_Enabled::Check_Enabled(const char * aType, int aCode, const char * aMessage) : Check(aType, aCode, aMessage)
{
}

// ===== Check ==============================================================

Check_Enabled::~Check_Enabled()
{
}

unsigned int Check_Enabled::Verify(const NH::Interface & aElement) const
{
    assert(NULL != &aElement);

    unsigned int lResult = 0;

    if (!aElement.IsEnabled())
    {
        lResult += Check::Verify(aElement);
    }

    return lResult;
}
