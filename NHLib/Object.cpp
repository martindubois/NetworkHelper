
// Author     KMS - Martin Dubois, P.Eng.
// Copyrights (C) 2020 KMS. All rights reserved.
// Product    NetworkHelper
// File       NHLib/Object.cpp

// CODE REVIEW 2020-07-15 KMS - Martin Dubois, P.Eng.

// TEST COVERAGE 2020-07-15 KMS - Martin Dubois, P.Eng.

#include "Component.h"

// ===== C ==================================================================
#include <assert.h>
#include <stdio.h>

// ===== Includes ===========================================================
#include <NH/Object.h>

// ===== NHLib ==============================================================
#include "Utilities.h"

namespace NH
{

    // Public
    ////////////////////////////////////////////////////////////////////////

    const char * Object::GetObjectType() const
    {
        assert(NULL != mObjectType);

        return mObjectType;
    }

    // Protected
    ////////////////////////////////////////////////////////////////////////

    // aObjectType [-K-;R--]
    Object::Object(const char * aObjectType) : mObjectType(aObjectType)
    {
        assert(NULL != aObjectType);
    }

    // NOT TESTED NH.Object.DisplayError

    void Object::DisplayError(const char * aErrorType, int aCode, const char * aMessage) const
    {
        assert(NULL != aMessage);

        assert(NULL != mObjectType);

        char lMessage[256];

        int lRet = sprintf_s(lMessage, "%s - %s", mObjectType, aMessage);
        assert(               0 < lRet);
        assert(sizeof(lMessage) > lRet);

        Utl_DisplayError(aErrorType, aCode, lMessage);
    }

    void Object::ThrowError(const char * aErrorType, int aCode, const char * aMessage) const
    {
        assert(NULL != aMessage);

        assert(NULL != mObjectType);

        char lMessage[256];

        int lRet = sprintf_s(lMessage, "%s - %s", mObjectType, aMessage);
        assert(               0 < lRet);
        assert(sizeof(lMessage) > lRet);

        Utl_ThrowError(aErrorType, aCode, lMessage);
    }

    void Object::ThrowErrorIfNeeded(int aCode, unsigned int aErrorCount) const
    {
        assert(NULL != mObjectType);

        Utl_ThrowErrorIfNeeded(aCode, mObjectType, "", aErrorCount);
    }

}
