
// Author     KMS - Martin Dubois, P.Eng.
// Copyrights (C) 2020 KMS. All rights reserved.
// Product    NetworkHelper
// File       NHLib/NamedObject.cpp

// CODE REVIEW 2020-07-25 KMS - Martin Dubois, P.Eng.

// TEST COVERAGE 2020-07-25 KMS - Martin Dubois, P.Eng.

#include "Component.h"

// ===== C ==================================================================
#include <assert.h>
#include <stdio.h>

// ===== Includes ===========================================================
#include <NH/NamedObject.h>

// ===== NHLib ==============================================================
#include "Utilities.h"

namespace NH
{

    // Public
    ////////////////////////////////////////////////////////////////////////

    NamedObject::NamedObject(const char * aObjectType) : Object(aObjectType)
    {
    }

    const char * NamedObject::GetName() const
    {
        return mName.c_str();
    }

    void NamedObject::GetName(char * aOut, unsigned int aOutSize_byte) const
    {
        assert(NULL != aOut         );
        assert(   0 <  aOutSize_byte);

        memset(aOut, 0, aOutSize_byte);

        strncpy_s(aOut, aOutSize_byte, mName.c_str(), aOutSize_byte - 1);
    }

    void NamedObject::SetName(const char * aName)
    {
        assert(NULL != aName);

        mName = aName;
    }

    // Internal
    ////////////////////////////////////////////////////////////////////////

    // ===== Object ========================================================

    void NamedObject::DisplayError(const char * aErrorType, int aCode, const char * aMessage) const
    {
        assert(NULL != aMessage);

        char lMessage[256];

        int lRet = sprintf_s(lMessage, "%s %s - %s", GetObjectType(), mName.c_str(), aMessage);
        assert(               0 < lRet);
        assert(sizeof(lMessage) > lRet);

        Utl_DisplayError(aErrorType, aCode, lMessage);
    }

    // Protected
    ////////////////////////////////////////////////////////////////////////

    // ===== Object ========================================================

    void NamedObject::ThrowError(const char * aErrorType, int aCode, const char * aMessage) const
    {
        assert(NULL != aMessage);

        char lMessage[256];

        int lRet = sprintf_s(lMessage, "%s %s - %s", GetObjectType(), mName.c_str(), aMessage);
        assert(               0 < lRet);
        assert(sizeof(lMessage) > lRet);

        Utl_ThrowError(aErrorType, aCode, lMessage);
    }

    // NOT TESTED NH.NamedObject.ThrowErrorIfNeeded

    void NamedObject::ThrowErrorIfNeeded(int aCode, unsigned int aErrorCount) const
    {
        Utl_ThrowErrorIfNeeded(aCode, GetObjectType(), mName.c_str(), aErrorCount);
    }
}
