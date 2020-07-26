
// Author     KMS - Martin Dubois, P.Eng.
// Copyrights (C) 2020 KMS. All rights reserved.
// Product    NetworkHelper
// File       NHLib/Check.cpp

// CODE REVIEW 2020-07-26 KMS - Martin Dubois, P.Eng.

// TEST COVERAGE 2020-07-26 KMS - Martin Dubois, P.Eng.

// ===== C ==================================================================
#include <assert.h>

// ===== NHLib ==============================================================
#include "Check.h"

// Public
/////////////////////////////////////////////////////////////////////////////

Check::~Check()
{
}

unsigned int Check::Verify(const NH::Interface & aElement) const
{
    assert(NULL != &aElement);

    aElement.DisplayError(mType, mCode, mMessage.c_str());

    return 1;
}

unsigned int Check::Verify(const NH::Router & aElement) const
{
    assert(NULL != &aElement);

    aElement.DisplayError(mType, mCode, mMessage.c_str());

    return 1;
}

// Protected
/////////////////////////////////////////////////////////////////////////////

// aType [-K-;R--]
Check::Check(const char * aType, int aCode) : mCode(aCode), mType(aType)
{
    assert(NULL != aType);
}

// aType [-K-;R--]
Check::Check(const char * aType, int aCode, const char * aMessage) : mCode(aCode), mMessage(aMessage), mType(aType)
{
    assert(NULL != aType);
    assert(NULL != aMessage);
}

void Check::SetMessage(const char * aMessage)
{
    assert(NULL != aMessage);

    mMessage = aMessage;
}
