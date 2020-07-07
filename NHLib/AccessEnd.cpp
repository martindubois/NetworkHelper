
// Author     KMS - Martin Dubois, P.Eng.
// Copyrights (C) 2020 KMS. All rights reserved.
// Product    NetworkHelper
// File       NHLib/AccessEnd.cpp

// CODE REVIEW 2020-07-07 KMS - Martin Dubois, P.Eng.

// TEST COVERAGE 2020-07-07 KMS - Martin Dubois, P.Eng.

// ===== C ==================================================================
#include <assert.h>
#include <memory.h>

// ===== Includes ===========================================================
#include <NH/SubNet.h>

#include <NH/AccessEnd.h>

// ===== NHLib ==============================================================
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

    AccessEnd::AccessEnd() : mHost(0), mPort_A(0), mPort_B(0), mPort_Op(OPERATOR_INVALID), mSubNet(NULL)
    {
        mFlags.mAny = false;
    }

    // NOT TESTED NH.Access.End.SetPort
    //            OPERATOR_RANGE

    void AccessEnd::GetDescription(char * aOut, unsigned int aOutSize_byte) const
    {
        assert(NULL != aOut         );
        assert(   0 <  aOutSize_byte);

        memset(aOut, 0, aOutSize_byte);

        if (0 != mHost)
        {
            GetHost(aOut, aOutSize_byte);
        }
        else if (NULL != mSubNet)
        {
            mSubNet->GetFullName(aOut, aOutSize_byte);
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

    // NOT TESTED NH.AccessEnd.Error
    //            Not initialized

    AccessEnd::Filter AccessEnd::GetFilter() const
    {
        if (mFlags.mAny)
        {
            return FILTER_ANY;
        }

        if (0 != mHost)
        {
            return FILTER_HOST;
        }

        if (NULL != mSubNet)
        {
            return FILTER_SUBNET;
        }

        return FILTER_INVALID;
    }

    uint32_t AccessEnd::GetHost() const
    {
        assert(0 != mHost);

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
        return mFlags.mAny || (0 != mHost) || (NULL != mSubNet);
    }

    // NOT TESTED NH.AccessEnd.Error
    //            Already initialized

    void AccessEnd::SetAny()
    {
        if (IsInitialized())
        {
            Utl_ThrowError(UTL_CALLER_ERROR, __LINE__, "Access end already initialized");
        }

        assert(!mFlags.mAny);

        mFlags.mAny = true;
    }

    void AccessEnd::SetHost(uint32_t aHost)
    {
        IPv4_Validate(aHost);

        if (IsInitialized())
        {
            Utl_ThrowError(UTL_CALLER_ERROR, __LINE__, "Access end already initialized");
        }

        assert(0 == mHost);

        mHost = aHost;
    }

    void AccessEnd::SetHost(const char * aHost)
    {
        assert(NULL != aHost);

        SetHost(IPv4_TextToAddress(aHost));
    }

    // NOT TESTED NH.AccessEnd.Error
    //            Port alread initialized

    void AccessEnd::SetPort(Operator aOp)
    {
        if (OPERATOR_INVALID != mPort_Op)
        {
            Utl_ThrowError(UTL_CALLER_ERROR, __LINE__, "Port already initialized");
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
            Utl_ThrowError(UTL_CALLER_ERROR, __LINE__, "Port already initialized");
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
            Utl_ThrowError(UTL_CALLER_ERROR, __LINE__, "Port already initialized");
        }

        assert(0 == mPort_A);
        assert(0 == mPort_B);

        switch (aOp)
        {
        case OPERATOR_RANGE:
            if (aPortA > aPortB)
            {
                Utl_ThrowError(UTL_CONFIG_ERROR, __LINE__, "Invalid port range");
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
            Utl_ThrowError(UTL_CALLER_ERROR, __LINE__, "Access end already initialized");
        }

        assert(NULL == mSubNet);

        mSubNet = aSubNet;
    }

    void AccessEnd::Verify() const
    {
        if (!IsInitialized())
        {
            Utl_ThrowError(UTL_CALLER_ERROR, __LINE__, "Access end is not correctly initialized");
        }
    }

    bool AccessEnd::VerifyAddress(uint32_t aAddr) const
    {
        if (0 != mHost)
        {
            return mHost == aAddr;
        }
        else if (NULL != mSubNet)
        {
            return mSubNet->VerifyAddress(aAddr);
        }

        return false;
    }

    // NOT TESTED NH.AccessEnd.VerifySubNet
    //            FILTER_ANY

    bool AccessEnd::VerifySubNet(const SubNet * aSubNet) const
    {
        assert(NULL != aSubNet);

        if (0 != mHost)
        {
            return aSubNet->VerifyAddress(mHost);
        }
        else if (NULL != mSubNet)
        {
            return mSubNet == aSubNet;
        }

        return false;
    }

}
