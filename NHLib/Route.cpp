
// Author    KMS - Martin Dubois, P.Eng.
// Copyright (C) 2020 KMS. All rights reserved.
// Product   NetworkHelper
// File      NHLib/Route.cpp

// CODE REVIEW 2020-07-10 KMS - Martin Dubois, P.Eng.

// TEST COVERAGE 2020-07-10 KMS - Martin Dubois, P.Eng.

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

    // NOT TESTED NH.Route.Error
    //            The next router is on the destination network.

    Route::Route(const SubNet * aSubNet, const char * aAddr) : mSubNet(aSubNet)
    {
        assert(NULL != aSubNet);

        mAddr = IPv4_TextToAddress(aAddr);

        if (aSubNet->VerifyAddress(mAddr))
        {
            Utl_ThrowError(ERROR_CONFIG, __LINE__, "The next router cannot be on the destination subnet");
        }
    }

    Route::~Route()
    {
    }

    uint32_t Route::GetAddress() const
    {
        return mAddr;
    }

    void Route::GetAddress(char * aOut, unsigned int aOutSize_byte) const
    {
        IPv4_AddressToText(aOut, aOutSize_byte, mAddr);
    }

    void Route::GetDescription(char * aOut, unsigned int aOutSize_byte) const
    {
        assert(NULL != aOut         );
        assert(   0 <  aOutSize_byte);

        char lAddr  [32];
        char lSubNet[64];

        GetAddress(lAddr, sizeof(lAddr));

        mSubNet->GetFullName(lSubNet, sizeof(lSubNet));

        int lRet = sprintf_s(aOut, aOutSize_byte, "%s - %s", lSubNet, lAddr);
        assert(            0 <                           lRet );
        assert(aOutSize_byte > static_cast<unsigned int>(lRet));
    }

    const SubNet * Route::GetSubNet() const
    {
        return mSubNet;
    }

}
