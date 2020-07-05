
// Author     KMS - Martin Dubois, P.Eng.
// Copyrights (C) 2020 KMS. All rights reserved.
// Product    NetworkHelper
// File       NHLib/Access.cpp

// CODE REVIEW 2020-07-03 KMS - Martin Dubois, P.Eng.

// TEST COVERAGE 2020-07-03 KMS - Martin Dubois, P.Eng.

// ===== C ==================================================================
#include <assert.h>

// ===== Includes ===========================================================
#include <NH/Access.h>

// ===== NHLib ==============================================================
#include "IP.h"
#include "IPv4.h"
#include "Utilities.h"

namespace NH
{

    // Public
    ////////////////////////////////////////////////////////////////////////

    // NOT TESTED NH.Access.Error

    Access::End::End() : mHost(0), mPort_A(0), mPort_B(0), mPort_Op(OPERATOR_INVALID), mSubNet(NULL)
    {
        mFlags.mAny = false;
    }

    void Access::End::SetAny()
    {
        if (!mFlags.mAny)
        {
            if (0 != mHost)
            {
                Utl_ThrowError("ERROR", __LINE__, "Cannot set to any when a host is already set");
            }

            if (NULL != mSubNet)
            {
                Utl_ThrowError("ERROR", __LINE__, "Cannot set to any when a subnet is already set");
            }

            mFlags.mAny = true;
        }
    }

    void Access::End::SetHost(uint32_t aHost)
    {
        IPv4_Validate(aHost);

        if (mHost != aHost)
        {
            if (mFlags.mAny)
            {
                Utl_ThrowError("ERROR", __LINE__, "Cannot set a host when any is alrady specified");
            }

            if (NULL != mSubNet)
            {
                Utl_ThrowError("ERROR", __LINE__, "Cannot set a host when a subnet is already set");
            }

            mHost = aHost;
        }
    }

    void Access::End::SetHost(const char * aHost)
    {
        assert(NULL != aHost);

        SetHost(IPv4_TextToAddress(aHost));
    }

    // NOT TESTED NH.Access.End.SetPort
    //            OPERATOR_ANY, OPERATOR_RANGE

    void Access::End::SetPort(Operator aOp, uint16_t aPortA, uint16_t aPortB)
    {
        if (mPort_Op != aOp)
        {
            switch (aOp)
            {
            case OPERATOR_ANY:
                if (0 != aPortA)
                {
                    Utl_ThrowError("ERROR", __LINE__, "Unexpected port number");
                }
                // no break;
            case OPERATOR_EQ :
            case OPERATOR_GT :
            case OPERATOR_LT :
            case OPERATOR_NEQ:
                if (0 != aPortB)
                {
                    Utl_ThrowError("ERROR", __LINE__, "Unexpected port number");
                }
                break;

            case OPERATOR_RANGE:
                if (aPortA > aPortB)
                {
                    Utl_ThrowError("ERROR", __LINE__, "Invalid port range");
                }
                break;

            default: assert(false);
            }

            mPort_A  = aPortA;
            mPort_B  = aPortB;
            mPort_Op = aOp   ;
        }
    }

    // aPortA [--O;R--]
    // aPortB [--O;R--]
    void Access::End::SetPort(Operator aOp, const char * aPortA, const char * aPortB)
    {
        assert(NULL != aPortA);

        uint16_t lPortA = (NULL == aPortA) ? 0 : IP_TextToPort(aPortA);
        uint16_t lPortB = (NULL == aPortB) ? 0 : IP_TextToPort(aPortB);

        SetPort(aOp, lPortA, lPortB);
    }

    void Access::End::SetSubNet(const SubNet * aSubNet)
    {
        assert(NULL != aSubNet);

        if (mSubNet != aSubNet)
        {
            if (mFlags.mAny)
            {
                Utl_ThrowError("ERROR", __LINE__, "Cannot set a subnet when any is already specified");
            }

            if (0 != mHost)
            {
                Utl_ThrowError("ERROR", __LINE__, "Cannot set a subnet when a host is already set");
            }

            mSubNet = aSubNet;
        }
    }

    Access::Access(Type aType) : mProtocol(PROTOCOL_INVALID)
    {
        assert(TYPE_QTY > aType);

        mFlags.mEstablished = false;

        mType = aType;
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

}
