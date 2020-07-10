
// Author     KMS - Martin Dubois, P.Eng.
// Copyrights (C) 2020 KMS. All rights reserved.
// Product    NetworkHelper
// File       NHLib/IP.cpp

// CODE REVIEW 2020-07-10 KMS - Martin Dubois, P.Eng.

// CODE COVERAGE 2020-07-10 KMS - Martin Dubois, P.Eng.

#include "Component.h"

// ===== C ==================================================================
#include <stdlib.h>

// ===== NHLib ==============================================================
#include "Errors.h"
#include "Utilities.h"

#include "IP.h"

// Functions
/////////////////////////////////////////////////////////////////////////////

uint16_t IP_TextToPort(const char * aIn)
{
    assert(NULL != aIn);

    unsigned long lResult = strtoul(aIn, NULL, 10);
    if (0xffff < lResult)
    {
        Utl_ThrowError(ERROR_213);
    }

    return static_cast<uint16_t>(lResult);
}
