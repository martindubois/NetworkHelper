
// Author     KMS - Martin Dubois, P.Eng.
// Copyrights (C) 2020 KMS. All rights reserved.
// Product    NetworkHelper
// File       NHLib/DHCPList.cpp

// CODE REVIEW 2020-07-28 KMS - Martin Dubois, P.Eng.

// TEST COVERAGE 2020-07-28 KMS - Martin Dubois, P.Eng.

#include "Component.h"

// ===== C ==================================================================
#include <assert.h>

// ===== Includes ===========================================================
#include <NH/DHCP.h>

#include <NH/DHCPList.h>

namespace NH
{

    // Public
    /////////////////////////////////////////////////////////////////////////

    DHCPList::DHCPList()
    {
    }

    DHCPList::~DHCPList()
    {
        for (InternalMap::iterator lIt = mDHCPs.begin(); lIt != mDHCPs.end(); lIt++)
        {
            assert(NULL != lIt->second);

            delete lIt->second;
        }
    }

    DHCP * DHCPList::FindOrCreate(const char * aName)
    {
        assert(NULL != aName);

        DHCP * lResult;

        InternalMap::iterator lIt = mDHCPs.find(aName);
        if (mDHCPs.end() == lIt)
        {
            lResult = new DHCP();
            assert(NULL != lResult);

            lResult->SetName(aName);

            mDHCPs.insert(InternalMap::value_type(aName, lResult));
        }
        else
        {
            lResult = lIt->second;
            assert(NULL != lResult);
        }

        return lResult;
    }

    // Internal
    /////////////////////////////////////////////////////////////////////////

    unsigned int DHCPList::Verify_Internal() const
    {
        unsigned int lResult = 0;

        for (InternalMap::const_iterator lIt = mDHCPs.begin(); lIt != mDHCPs.end(); lIt++)
        {
            assert(NULL != lIt->second);

            lResult += lIt->second->Verify_Internal();
        }

        return lResult;
    }

}
