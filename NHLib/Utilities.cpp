
// Author     KMS - Martin Dubois, P.Eng.
// Copyrights (C) 2020 KMS. All rights reserved.
// Product    NetworkHelper
// File       NHLib/Utilities.cpp

// CODE REVIEW 2020-06-15 KMS - Martin Dubois, P.Eng.

// TEST COVERAGE 2020-06-15 KMS - Martin Dubois, P.Eng.

// ===== C ==================================================================
#include <assert.h>
#include <stdio.h>

// ===== C++ ================================================================
#include <exception>

// ===== NHLib ==============================================================
#include "Utilities.h"

// Functions
/////////////////////////////////////////////////////////////////////////////

void Utl_ThrowError(const char * aErrorType, int aCode, const char * aMessage, int aData)
{
    assert(NULL != aErrorType);
    assert(   0 != aCode     );
    assert(NULL != aMessage  );

    char lWhat[1024];

    int lRet = sprintf_s(lWhat, "%s  %3d  %s", aErrorType, aCode, aMessage);
    assert(            0 < lRet);
    assert(sizeof(lWhat) > lRet);

    throw std::exception(lWhat, aData);
}
