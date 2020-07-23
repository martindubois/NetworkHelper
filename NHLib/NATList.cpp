
// Author     KMS - Martin Dubois, P.Eng.
// Copyrights (C) 2020 KMS. All rights reserved.
// Product    NetworkHelper
// File       NHLib/NATList.cpp

// CODE REVIEW 2020-07-23 KMS - Martin Dubois, P.Eng.

#include "Component.h"

// ===== Includes ===========================================================
#include <NH/NAT.h>

#include <NH/NATList.h>

namespace NH
{

    // Public
    /////////////////////////////////////////////////////////////////////////

    NATList::NATList()
    {
    }

    NATList::~NATList()
    {
        for (InternalMap::iterator lIt = mNATs.begin(); lIt != mNATs.end(); lIt++)
        {
            assert(NULL != lIt->second);

            delete lIt->second;
        }
    }

    const NAT * NATList::Find(uint32_t aAddr) const
    {
        for (InternalMap::const_iterator lIt = mNATs.begin(); lIt != mNATs.end(); lIt++)
        {
            const NAT * lNAT = lIt->second;
            assert(NULL != lNAT);

            if (lNAT->Match(aAddr))
            {
                return lNAT;
            }
        }

        return NULL;
    }

    const NAT * NATList::Find(const SubNet & aSubNet) const
    {
        for (InternalMap::const_iterator lIt = mNATs.begin(); lIt != mNATs.end(); lIt++)
        {
            const NAT * lNAT = lIt->second;
            assert(NULL != lNAT);

            if (lNAT->Match(aSubNet))
            {
                return lNAT;
            }
        }

        return NULL;
    }

    NAT * NATList::FindOrCreate(const char * aName)
    {
        assert(NULL != aName);

        NAT * lResult;

        InternalMap::iterator lIt = mNATs.find(aName);
        if (mNATs.end() == lIt)
        {
            lResult = new NAT();
            assert(NULL != lResult);

            lResult->SetName(aName);

            mNATs.insert(InternalMap::value_type(aName, lResult));
        }
        else
        {
            lResult = lIt->second;
            assert(NULL != lResult);
        }

        return lResult;
    }

}
