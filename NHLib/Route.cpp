
// Author    KMS - Martin Dubois, P.Eng.
// Copyright (C) 2020 KMS. All rights reserved.
// Product   NetworkHelper
// File      NHLib/Route.cpp

// CODE REVIEW 2020-08-04 KMS - Martin Dubois, P.Eng.

// TEST COVERAGE 2020-08-04 KMS - Martin Dubois, P.Eng.

#include "Component.h"

// ===== Includes ===========================================================
#include <NH/SubNet.h>

#include <NH/Route.h>

// ===== NHLib ==============================================================
#include "Errors.h"
#include "IPv4.h"
#include "Utilities.h"

namespace NH
{

    // Public
    /////////////////////////////////////////////////////////////////////////

    // aSubNet [-K-;---]
    Route::Route(const SubNet * aSubNet, uint32_t aNextRouter) : Object("Route")
    {
        Init(aSubNet, aNextRouter);
    }

    // aSubNet [-K-;---]
    Route::Route(const SubNet * aSubNet, const char * aNextRouter) : Object("Route")
    {
        assert(NULL != aSubNet);

        Init(aSubNet, IPv4_TextToAddress(aNextRouter));
    }

    Route::~Route()
    {
    }

    uint32_t Route::GetNextRouter() const
    {
        return mNextRouter;
    }

    void Route::GetNextRouter(char * aOut, unsigned int aOutSize_byte) const
    {
        IPv4_AddressToText(aOut, aOutSize_byte, mNextRouter);
    }

    void Route::GetDescription(char * aOut, unsigned int aOutSize_byte) const
    {
        assert(NULL != aOut         );
        assert(   0 <  aOutSize_byte);

        char lAddr  [32];
        char lSubNet[64];

        GetNextRouter(lAddr, sizeof(lAddr));

        mSubNet->GetFullName(lSubNet, sizeof(lSubNet));

        int lRet = sprintf_s(aOut, aOutSize_byte, "%s - %s", lSubNet, lAddr);
        assert(            0 <                           lRet );
        assert(aOutSize_byte > static_cast<unsigned int>(lRet));
    }

    const SubNet * Route::GetSubNet() const
    {
        return mSubNet;
    }

    // aSubNet [---;---]
    bool Route::Match(const SubNet & aSubNet) const
    {
        assert(NULL != &aSubNet);

        return mSubNet == &aSubNet;
    }

    // Private
    /////////////////////////////////////////////////////////////////////////

    // aSubNet [-K-;R--]
    void Route::Init(const SubNet * aSubNet, uint32_t aNextRouter)
    {
        assert(NULL != aSubNet);

        switch (IPv4_GetAddressType(aNextRouter))
        {
        case IPv4_PRIVATE:
        case IPv4_PUBLIC:
            break;

        default:
            ThrowError(ERROR_228);
        }

        if (aSubNet->Match(aNextRouter))
        {
            ThrowError(ERROR_256);
        }

        mNextRouter = aNextRouter;
        mSubNet     = aSubNet;
    }

}
