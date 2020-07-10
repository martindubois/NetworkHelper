
// Author     KMS - Martin Dubois, P.Eng.
// Copyrights (C) 2020 KMS. All rights reserved.
// Product    NetworkHelper
// File       NHLib/AccessListMap.cpp

// CODE REVIEW 2020-07-10 KMS - Martin Dubois, P.Eng.

// TEST COVERAGE 2020-07-10 KMS - Martin Dubois, P.Eng.

#include "Component.h"

// ===== Includes ===========================================================
#include <NH/AccessList.h>

#include <NH/AccessListMap.h>

namespace NH
{

    // Public
    ////////////////////////////////////////////////////////////////////////

    AccessListMap::AccessListMap()
    {
    }

    AccessListMap::~AccessListMap()
    {
        for (InternalMap::iterator lIt = mAccessLists.begin(); lIt != mAccessLists.end(); lIt++)
        {
            assert(NULL != lIt->second);

            delete lIt->second;
        }
    }

    AccessList * AccessListMap::FindOrCreate(const char * aName)
    {
        assert(NULL != aName);

        AccessList * lResult;

        InternalMap::iterator lIt = mAccessLists.find(aName);
        if (mAccessLists.end() == lIt)
        {
            lResult = new AccessList(aName);

            mAccessLists.insert(InternalMap::value_type(aName, lResult));
        }
        else
        {
            lResult = lIt->second;
        }

        assert(NULL != lResult);

        return lResult;
    }

    void AccessListMap::Verify() const
    {
        for (InternalMap::const_iterator lIt = mAccessLists.begin(); lIt != mAccessLists.end(); lIt++)
        {
            assert(NULL != lIt->second);

            lIt->second->Verify();
        }
    }

}
