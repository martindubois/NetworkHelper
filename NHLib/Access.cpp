
// Author     KMS - Martin Dubois, P.Eng.
// Copyrights (C) 2020 KMS. All rights reserved.
// Product    NetworkHelper
// File       NHLib/Access.cpp

// CODE REVIEW 2020-07-10 KMS - Martin Dubois, P.Eng.

// TEST COVERAGE 2020-07-10 KMS - Martin Dubois, P.Eng.

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

#define ELEMENT "Access rule"

static const char * PROTOCOL_NAMES[NH::Access::PROTOCOL_QTY] = { "ICMP", "IP", "TCP", "UDP" };

namespace NH
{

    // Public
    ////////////////////////////////////////////////////////////////////////

    Access::Access(Type aType) : mProtocol(PROTOCOL_INVALID)
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
                Utl_ThrowError(ERROR_214);
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

    // NOT TESTED NH.Access
    //            FILTER_ANY on source

    void Access::Verify() const
    {
        mDestination.Verify();
        mSource     .Verify();

        switch (mDestination.GetFilter())
        {
        case AccessEnd::FILTER_ANY: break;

        case AccessEnd::FILTER_HOST:
            uint32_t lDstH;

            lDstH = mDestination.GetHost();

            switch (mSource.GetFilter())
            {
            case AccessEnd::FILTER_ANY: break;

            case AccessEnd::FILTER_HOST  : if (mSource.GetHost  () ==             lDstH ) { Error(ERROR_217); } break;
            case AccessEnd::FILTER_SUBNET: if (mSource.GetSubNet()->VerifyAddress(lDstH)) { Error(ERROR_216); } break;

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

            case AccessEnd::FILTER_HOST  : if (lDstSN->VerifyAddress(mSource.GetHost  ())) { Error(ERROR_215); } break;
            case AccessEnd::FILTER_SUBNET: if (lDstSN ==             mSource.GetSubNet() ) { Error(__LINE__, "Describes traffic not going through the router"); } break;

            default: assert(false);
            }
            break;

        default: assert(false);
        }
    }

    // Private
    /////////////////////////////////////////////////////////////////////////

    void Access::Error(int aCode, const char * aMessage) const
    {
        assert(NULL != aMessage);

        char lDesc   [128];
        char lMessage[128];

        GetDescription(lDesc, sizeof(lDesc));

        sprintf_s(lMessage, ELEMENT " %s - %s", lDesc, aMessage);

        Utl_ThrowError(ERROR_ERROR, aCode, lMessage);
    }

}
