
// Author     KMS - Martin Dubois, P.Eng.
// Copyrights (C) 2020 KMS. All rights reserved.
// Product    NetworkHelper
// File       NHLib/DHCP.cpp

// CODE REVIEW 2020-07-29 KMS - Martin Dubois, P.Eng.

// TEST COVERAGE 2020-07-29 KMS - Martin Dubois, P.Eng.

#include "Component.h"

// ===== Includes ===========================================================
#include <NH/SubNet.h>

#include <NH/DHCP.h>

// ===== NHLib ==============================================================
#include "Errors.h"
#include "IPv4.h"

namespace NH
{

    // Public
    ////////////////////////////////////////////////////////////////////////

    DHCP::DHCP() : NamedObject("DHCP pool"), mDefaultRouter(0), mDnsServer(0), mSubNet(NULL)
    {
    }

    void DHCP::SetDefaultRouter(uint32_t aDefaultRouter)
    {
        if (0 != mDefaultRouter)
        {
            ThrowError(ERROR_CALLER, __LINE__, "Default router already set");
        }

        switch (IPv4_GetAddressType(aDefaultRouter))
        {
        case IPv4_PRIVATE:
        case IPv4_PUBLIC:
            break;

        default: ThrowError(ERROR_CONFIG, __LINE__, "The default router address is not valid");
        }

        mDefaultRouter = aDefaultRouter;
    }

    void DHCP::SetDnsServer(uint32_t aDnsServer)
    {
        switch (IPv4_GetAddressType(aDnsServer))
        {
        case IPv4_PRIVATE:
        case IPv4_PUBLIC:
            break;

        default: ThrowError(ERROR_CONFIG, __LINE__, "The DNS server address is not valid");
        }

        mDnsServer = aDnsServer;
    }

    // aSubNet [-K-;R--]
    void DHCP::SetSubNet(const SubNet * aSubNet)
    {
        assert(NULL != aSubNet);

        if (NULL != mSubNet)
        {
            ThrowError(ERROR_CALLER, __LINE__, "Subnet already set");
        }

        mSubNet = aSubNet;
    }

    // Internal
    ////////////////////////////////////////////////////////////////////////

    unsigned int DHCP::Verify_Internal() const
    {
        unsigned int lResult = 0;

        if (0 == mDefaultRouter)
        {
            DisplayError(ERROR_240);
            lResult++;
        }

        if (0 == mDnsServer)
        {
            DisplayError(ERROR_502);
            lResult++;
        }

        if (NULL == mSubNet)
        {
            DisplayError(ERROR_244);
            lResult++;
        }
        else
        {
            if ((0 != mDefaultRouter) && (!mSubNet->Match(mDefaultRouter)))
            {
                DisplayError(ERROR_CONFIG, __LINE__, "The default-router is not on the subnet");
                lResult++;
            }
        }

        return lResult;
    }

}
