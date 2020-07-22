
// Author     KMS - Martin Dubois, P.Eng.
// Copyrights (C) 2020 KMS. All rights reserved.
// Product    NetworkHelper
// File       NHLib/NAT.cpp

// CODE REVIEW 2020-07-21 KMS - Martin Dubois, P.Eng.

#include "Component.h"

// ===== Includes ===========================================================
#include <NH/SubNet.h>

#include <NH/NAT.h>

// ===== NHLib ==============================================================
#include "Errors.h"
#include "IPv4.h"

namespace NH
{

    // Public
    ////////////////////////////////////////////////////////////////////////

    NAT::NAT() : NamedObject("NAT pool"), mFirst(0), mLast(0), mNetMask(0)
    {
    }

    bool NAT::Match(uint32_t aAddr) const
    {
        return IPv4_IsAddressOnSubNet(aAddr, mFirst & mNetMask, mNetMask);
    }

    bool NAT::Match(const SubNet & aSubNet) const
    {
        assert(NULL != &aSubNet);

        return aSubNet.VerifyAddress(mFirst);
    }

    void NAT::Set(uint32_t aFirst, uint32_t aLast, uint32_t aNetMask)
    {
        switch (IPv4_GetAddressType(aFirst))
        {
        case IPv4_PUBLIC: break;

        default: ThrowError(ERROR_CONFIG, __LINE__, "The first address is not valid");
        }

        IPv4_Validate(aNetMask);

        IPv4_Validate(aLast, aFirst & aNetMask, aNetMask);

        mFirst   = aFirst  ;
        mLast    = aLast   ;
        mNetMask = aNetMask;
    }

    void NAT::Set(const char * aFirst, const char * aLast, const char * aNetMask)
    {
        Set(IPv4_TextToAddress(aFirst), IPv4_TextToAddress(aLast), IPv4_TextToAddress(aNetMask));
    }

}
