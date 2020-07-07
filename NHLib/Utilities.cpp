
// Author     KMS - Martin Dubois, P.Eng.
// Copyrights (C) 2020 KMS. All rights reserved.
// Product    NetworkHelper
// File       NHLib/Utilities.cpp

// CODE REVIEW 2020-07-07 KMS - Martin Dubois, P.Eng.

// TEST COVERAGE 2020-07-07 KMS - Martin Dubois, P.Eng.

// ===== C ==================================================================
#include <assert.h>
#include <stdio.h>

// ===== NHLib ==============================================================
#include "Color.h"

#include "Utilities.h"

// Functions
/////////////////////////////////////////////////////////////////////////////

void Utl_DisplayError(int aCode, const std::exception & aException)
{
    assert(NULL != &aException);

    COLOR(RED);
    {
        fprintf(stderr, "EXCEPTION  %4d  %s\n", aCode, aException.what());
    }
    COLOR(WHITE);
}

void Utl_DisplayError(const char * aErrorType, int aCode, const char * aMessage)
{
    assert(NULL != aErrorType);
    assert(   0 != aCode     );
    assert(NULL != aMessage  );

    COLOR(RED);
    {
        int lRet = fprintf(stderr, "%s  %4d  %s\n", aErrorType, aCode, aMessage);
        assert(0 < lRet);
    }
    COLOR(WHITE);
}

void Utl_ThrowError(const char * aErrorType, int aCode, const char * aMessage, int aData)
{
    assert(NULL != aErrorType);
    assert(   0 != aCode     );
    assert(NULL != aMessage  );

    char lWhat[1024];

    int lRet = sprintf_s(lWhat, "%s  %4d  %s", aErrorType, aCode, aMessage);
    assert(            0 < lRet);
    assert(sizeof(lWhat) > lRet);

    throw std::exception(lWhat, aData);
}

void Utl_ThrowErrorIfNeeded(int aCode, const char * aElement, const char * aName, unsigned int aErrorCount)
{
    assert(   0 != aCode   );
    assert(NULL != aElement);
    assert(NULL != aName   );

    if (0 < aErrorCount)
    {
        char lMessage[128];

        int lRet = sprintf_s(lMessage, "%s %s - %u errors", aElement, aName, aErrorCount);

        Utl_ThrowError(UTL_ERROR, aCode, lMessage);
    }
}
