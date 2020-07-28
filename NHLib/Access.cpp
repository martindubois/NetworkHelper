
// Author     KMS - Martin Dubois, P.Eng.
// Copyrights (C) 2020 KMS. All rights reserved.
// Product    NetworkHelper
// File       NHLib/Access.cpp

// CODE REVIEW 2020-07-15 KMS - Martin Dubois, P.Eng.

// TEST COVERAGE 2020-07-15 KMS - Martin Dubois, P.Eng.

#include "Component.h"

// ===== C ==================================================================
#include <stdio.h>

// ===== Includes ===========================================================
#include <NH/Access.h>
#include <NH/SubNet.h>

// ===== NHLib ==============================================================
#include "Errors.h"
#include "IP.h"
#include "Utilities.h"

// Constants
/////////////////////////////////////////////////////////////////////////////

static const char * PROTOCOL_NAMES[NH::Access::PROTOCOL_QTY] = { "ICMP", "IP", "TCP", "UDP" };

namespace NH
{

    // Public
    ////////////////////////////////////////////////////////////////////////

    Access::Access(Type aType) : Object("Access rule"), mProtocol(PROTOCOL_INVALID)
    {
        assert(TYPE_QTY > aType);

        mFlags.mEstablished = false;

        mType = aType;
    }

    void Access::GetDescription(char * aOut, unsigned int aOutSize_byte) const
    {
        assert(NULL != aOut         );
        assert(   0 <  aOutSize_byte);

        assert(PROTOCOL_QTY > mProtocol);

        char lDst[64];
        char lSrc[64];

        mDestination.GetDescription(lDst, sizeof(lDst));
        mSource     .GetDescription(lSrc, sizeof(lSrc));

        sprintf_s(aOut, aOutSize_byte, "%s %s ==> %s", PROTOCOL_NAMES[mProtocol], lSrc, lDst);
    }

    Access::Type Access::GetType() const
    {
        assert(TYPE_QTY > mType);

        return mType;
    }

    void Access::SetEstablished()
    {
        assert(PROTOCOL_QTY > mProtocol);

        if (!mFlags.mEstablished)
        {
            if (PROTOCOL_TCP != mProtocol)
            {
                ThrowError(ERROR_214);
            }

            mFlags.mEstablished = true;
        }
    }

    void Access::SetProtocol(Protocol aProtocol)
    {
        assert(PROTOCOL_QTY > aProtocol);

        mProtocol = aProtocol;
    }

    bool Access::Match(Protocol aProtocol, const SubNet & aSrcSubNet, uint16_t aSrcPort, uint32_t aDstAddr, uint16_t aDstPort) const
    {
        assert((PROTOCOL_TCP == aProtocol) || (PROTOCOL_UDP == aProtocol));

        return Match(aProtocol) & mSource.Match(aSrcSubNet, aSrcPort) && mDestination.Match(aDstAddr, aDstPort);
    }

    bool Access::Match(Protocol aProtocol) const
    {
        assert(PROTOCOL_QTY > aProtocol);

        assert(PROTOCOL_QTY > mProtocol);

        return mProtocol == aProtocol;
    }

    void Access::Verify() const
    {
        ThrowErrorIfNeeded(ERROR_007, Verify_Internal());
    }

    // Internal
    /////////////////////////////////////////////////////////////////////////

    // NOT TESTED NH.Access
    //            FILTER_ANY on source

    unsigned int Access::Verify_Internal() const
    {
        unsigned int lResult = 0;

        lResult += mDestination.Verify_Internal();
        lResult += mSource     .Verify_Internal();

        switch (mDestination.GetFilter())
        {
        case AccessEnd::FILTER_ANY: break;

        case AccessEnd::FILTER_HOST:
            uint32_t lDstH;

            lDstH = mDestination.GetHost();

            switch (mSource.GetFilter())
            {
            case AccessEnd::FILTER_ANY: break;

            case AccessEnd::FILTER_HOST  : if (mSource.GetHost  () ==             lDstH ) { lResult++; DisplayError(ERROR_217); } break;
            case AccessEnd::FILTER_SUBNET: if (mSource.GetSubNet()->VerifyAddress(lDstH)) { lResult++; DisplayError(ERROR_216); } break;

            default: assert(false);
            }
            break;

        case AccessEnd::FILTER_SUBNET:
            const SubNet * lDstSN;

            lDstSN = mDestination.GetSubNet();
            assert(NULL != lDstSN);

            switch (mSource.GetFilter())
            {
            case AccessEnd::FILTER_ANY: break;

            case AccessEnd::FILTER_HOST  : if (lDstSN->VerifyAddress(mSource.GetHost  ())) { lResult++; DisplayError(ERROR_215); } break;
            case AccessEnd::FILTER_SUBNET: if (lDstSN ==             mSource.GetSubNet() ) { lResult++; DisplayError(ERROR_238); } break;

            default: assert(false);
            }
            break;

        default: assert(false);
        }

        return lResult;
    }

    // Protected
    /////////////////////////////////////////////////////////////////////////

    // ===== Object =========================================================

    void Access::DisplayError(const char * aErrorType, int aCode, const char * aMessage) const
    {
        assert(NULL != aMessage);

        char lDesc   [128];
        char lMessage[256];

        GetDescription(lDesc, sizeof(lDesc));

        sprintf_s(lMessage, "%s %s - %s", GetObjectType(), lDesc, aMessage);

        Utl_DisplayError(aErrorType, aCode, lMessage);
    }

}
