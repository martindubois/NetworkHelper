
// Author     KMS - Martin Dubois, P.Eng.
// Copyrights (C) 2020 KMS. All rights reserved.
// Product    NetworkHelper
// File       NHLib/Access.cpp

// CODE REVIEW 2020-07-05 KMS - Martin Dubois, P.Eng.

// TEST COVERAGE 2020-07-05 KMS - Martin Dubois, P.Eng.

// ===== C ==================================================================
#include <assert.h>
#include <stdio.h>

// ===== Includes ===========================================================
#include <NH/Access.h>
#include <NH/SubNet.h>

// ===== NHLib ==============================================================
#include "IP.h"
#include "Utilities.h"

namespace NH
{

    // Public
    ////////////////////////////////////////////////////////////////////////

    // NOT TESTED NH.Access.Error

    // NOT TESTED NH.Access.GetDescription

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

        char lDst[64];
        char lSrc[64];

        mDestination.GetDescription(lDst, sizeof(lDst));
        mSource     .GetDescription(lSrc, sizeof(lSrc));

        sprintf_s(aOut, aOutSize_byte, "%s ==> %s", lSrc, lDst);
    }

    void Access::SetEstablished()
    {
        assert(PROTOCOL_QTY > mProtocol);

        if (!mFlags.mEstablished)
        {
            if (PROTOCOL_TCP != mProtocol)
            {
                Utl_ThrowError("ERROR", __LINE__, "establish is valid with tcp only");
            }

            mFlags.mEstablished = true;
        }
    }

    void Access::SetProtocol(Protocol aProtocol)
    {
        assert(PROTOCOL_QTY > aProtocol);

        mProtocol = aProtocol;
    }

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

            case AccessEnd::FILTER_HOST  : if (mSource.GetHost  () ==             lDstH ) { Error(__LINE__, "describes trafic not going through the network"); } break;
            case AccessEnd::FILTER_SUBNET: if (mSource.GetSubNet()->VerifyAddress(lDstH)) { Error(__LINE__, "describes trafic not going through the router" ); } break;

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

            case AccessEnd::FILTER_HOST  : if (lDstSN->VerifyAddress(mSource.GetHost  ())) { Error(__LINE__, "describes trafic not going through the router"); } break;
            case AccessEnd::FILTER_SUBNET: if (lDstSN ==             mSource.GetSubNet() ) { Error(__LINE__, "describes trafic not going through the router"); } break;

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

        sprintf_s(lMessage, "The access rules %s %s", lDesc, aMessage);

        Utl_ThrowError("ERROR", __LINE__, lMessage);
    }

}
