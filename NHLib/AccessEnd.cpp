
// Author     KMS - Martin Dubois, P.Eng.
// Copyrights (C) 2020 KMS. All rights reserved.
// Product    NetworkHelper
// File       NHLib/AccessEnd.cpp

// CODE REVIEW 2020-07-23 KMS - Martin Dubois, P.Eng.

// TEST COVERAGE 2020-07-23 KMS - Martin Dubois, P.Eng.

#include "Component.h"

// ===== C ==================================================================
#include <memory.h>

// ===== Includes ===========================================================
#include <NH/SubNet.h>

#include <NH/AccessEnd.h>

// ===== NHLib ==============================================================
#include "Errors.h"
#include "IP.h"
#include "IPv4.h"
#include "Utilities.h"

// Constants
/////////////////////////////////////////////////////////////////////////////

static const char * OPERATORS[NH::AccessEnd::OPERATOR_QTY - 1] = { "", "==", ">", "<", "!=" };

namespace NH
{

    // Public
    ////////////////////////////////////////////////////////////////////////

    AccessEnd::AccessEnd() : Object("Access end"), mFilter(FILTER_INVALID), mHost(0), mPort_A(0), mPort_B(0), mPort_Op(OPERATOR_INVALID), mSubNet(NULL)
    {
    }

    // NOT TESTED NH.Access.End.SetPort
    //            OPERATOR_RANGE

    void AccessEnd::GetDescription(char * aOut, unsigned int aOutSize_byte) const
    {
        assert(NULL != aOut         );
        assert(   0 <  aOutSize_byte);

        memset(aOut, 0, aOutSize_byte);

        switch (mFilter)
        {
        case FILTER_ANY   : break;
        case FILTER_HOST  : GetHost             (aOut, aOutSize_byte); break;
        case FILTER_SUBNET: mSubNet->GetFullName(aOut, aOutSize_byte); break;

        default: assert(false);
        }

        unsigned int lLen = strlen(aOut);

        switch (mPort_Op)
        {
        case OPERATOR_ANY    :
        case OPERATOR_INVALID:
            break;

        case OPERATOR_EQ :
        case OPERATOR_GT :
        case OPERATOR_LT :
        case OPERATOR_NEQ:
            sprintf_s(aOut + lLen, aOutSize_byte - lLen, " %s %u", OPERATORS[mPort_Op], mPort_A);
            break;

        case OPERATOR_RANGE:
            sprintf_s(aOut + lLen, aOutSize_byte - lLen, " %u - %u", mPort_A, mPort_B);
            break;

        default: assert(false);
        }
    }

    AccessEnd::Filter AccessEnd::GetFilter() const
    {
        return mFilter;
    }

    uint32_t AccessEnd::GetHost() const
    {
        return mHost;
    }

    void AccessEnd::GetHost(char * aOut, unsigned int aOutSize_byte) const
    {
        IPv4_AddressToText(aOut, aOutSize_byte, mHost);
    }

    // NOT TESTED NH.AccessEnd.GetPortOperator

    AccessEnd::Operator AccessEnd::GetPortOperator() const
    {
        return mPort_Op;
    }

    const SubNet * AccessEnd::GetSubNet() const
    {
        assert(NULL != mSubNet);

        return mSubNet;
    }

    bool AccessEnd::IsInitialized() const
    {
        return FILTER_INVALID != mFilter;
    }

    void AccessEnd::SetAny()
    {
        if (IsInitialized())
        {
            ThrowError(ERROR_CALLER, __LINE__, "Filter already initialized");
        }

        assert(FILTER_INVALID == mFilter);

        mFilter = FILTER_ANY;
    }

    void AccessEnd::SetHost(uint32_t aHost)
    {
        switch (IPv4_GetAddressType(aHost))
        {
        case IPv4_PRIVATE:
        case IPv4_PUBLIC:
            break;

        default:
            ThrowError(ERROR_226);
        }

        if (IsInitialized())
        {
            ThrowError(ERROR_CALLER, __LINE__, "Filter already initialized");
        }

        assert(FILTER_INVALID == mFilter);
        assert(             0 == mHost  );

        mFilter = FILTER_HOST;
        mHost   = aHost;
    }

    void AccessEnd::SetHost(const char * aHost)
    {
        assert(NULL != aHost);

        SetHost(IPv4_TextToAddress(aHost, false));
    }

    void AccessEnd::SetPort(Operator aOp)
    {
        if (OPERATOR_INVALID != mPort_Op)
        {
            ThrowError(ERROR_CALLER, __LINE__, "Port already initialized");
        }

        switch (aOp)
        {
        case OPERATOR_ANY:
            mPort_Op = aOp;
            break;

        default: assert(false);
        }
    }

    void AccessEnd::SetPort(Operator aOp, uint16_t aPortA)
    {
        if (OPERATOR_INVALID != mPort_Op)
        {
            ThrowError(ERROR_CALLER, __LINE__, "Port already initialized");
        }

        assert(0 == mPort_A);

        switch (aOp)
        {
        case OPERATOR_EQ:
        case OPERATOR_GT:
        case OPERATOR_LT:
        case OPERATOR_NEQ:
            mPort_A  = aPortA;
            mPort_Op = aOp;
            break;

        default: assert(false);
        }
    }

    void AccessEnd::SetPort(Operator aOp, uint16_t aPortA, uint16_t aPortB)
    {
        if (OPERATOR_INVALID != mPort_Op)
        {
            ThrowError(ERROR_CALLER, __LINE__, "Port already initialized");
        }

        assert(0 == mPort_A);
        assert(0 == mPort_B);

        switch (aOp)
        {
        case OPERATOR_RANGE:
            if (aPortA > aPortB)
            {
                ThrowError(ERROR_CONFIG, __LINE__, "Invalid port range");
            }
            mPort_A  = aPortA;
            mPort_B  = aPortB;
            mPort_Op = aOp;
            break;

        default: assert(false);
        }
    }

    void AccessEnd::SetPort(Operator aOp, const char * aPortA)
    {
        SetPort(aOp, IP_TextToPort(aPortA));
    }

    void AccessEnd::SetPort(Operator aOp, const char * aPortA, const char * aPortB)
    {
        SetPort(aOp, IP_TextToPort(aPortA), IP_TextToPort(aPortB));
    }

    void AccessEnd::SetSubNet(const SubNet * aSubNet)
    {
        assert(NULL != aSubNet);

        if (IsInitialized())
        {
            ThrowError(ERROR_CALLER, __LINE__, "Filter already initialized");
        }

        assert(FILTER_INVALID == mFilter);
        assert(NULL           == mSubNet);

        mFilter = FILTER_SUBNET;
        mSubNet = aSubNet;
    }

    bool AccessEnd::Match(const SubNet & aSubNet, unsigned short aPort) const
    {
        return Match(aSubNet) && Match(aPort);
    }

    bool AccessEnd::Match(uint32_t aAddress, uint16_t aPort) const
    {
        return Match(aAddress) && Match(aPort);
    }

    bool AccessEnd::Match(const SubNet & aSubNet) const
    {
        assert(NULL != &aSubNet);

        switch (mFilter)
        {
        case FILTER_ANY   : return true;
        case FILTER_HOST  : return aSubNet.VerifyAddress(mHost);
        case FILTER_SUBNET: return mSubNet == &aSubNet;

        default: assert(false);
        }

        return false;
    }

    bool AccessEnd::Match(uint32_t aAddress) const
    {
        switch (mFilter)
        {
        case FILTER_ANY   : return true;
        case FILTER_HOST  : return mHost ==               aAddress ;
        case FILTER_SUBNET: return mSubNet->VerifyAddress(aAddress);

        default: assert(false);
        }

        return false;
    }

    bool AccessEnd::Match(uint16_t aPort) const
    {
        assert(0 != aPort);

        switch (mPort_Op)
        {
        case OPERATOR_ANY  : return true;
        case OPERATOR_EQ   : return  mPort_A == aPort;
        case OPERATOR_GT   : return  mPort_A <  aPort;
        case OPERATOR_LT   : return  mPort_A >  aPort;
        case OPERATOR_NEQ  : return  mPort_A != aPort;
        case OPERATOR_RANGE: return (mPort_A <= aPort) && (mPort_B >= aPort);

        default: assert(false);
        }

        return false;
    }

    void AccessEnd::Verify() const
    {
        ThrowErrorIfNeeded(__LINE__, Verify_Internal());
    }

    // NOT TESTED NH.AccessEnd.VerifyPrivate
    //            FILTER_ANY and FILTER_HOST

    void AccessEnd::VerifyPrivate() const
    {
        switch (mFilter)
        {
        case FILTER_ANY: break;

        case FILTER_HOST:
            if (IPv4_PRIVATE != IPv4_GetAddressType(mHost))
            {
                ThrowError(ERROR_CONFIG, __LINE__, "The access rule source is not a private address");
            }
            break;

        case FILTER_SUBNET:
            assert(NULL != mSubNet);

            if (!mSubNet->IsPrivate())
            {
                ThrowError(ERROR_CONFIG, __LINE__, "The access rule source subnet is not private");
            }
            break;

        default: assert(false);
        }
    }

    // Internal
    /////////////////////////////////////////////////////////////////////////

    unsigned int AccessEnd::Verify_Internal() const
    {
        unsigned int lResult = 0;

        if (!IsInitialized())
        {
            DisplayError(ERROR_CALLER, __LINE__, "Is not correctly initialized");
            lResult++;
        }

        return lResult;
    }

}
