
// Author     KMS - Martin Dubois, P.Eng.
// Copyrights (C) 2020 KMS. All rights reserved.
// Product    NetworkHelper
// File       NHLib/AccessListMap.cpp

// CODE REVIEW 2020-07-30 KMS - Martin Dubois, P.Eng.

// TEST COVERAGE 2020-07-30 KMS - Martin Dubois, P.Eng.

#include "Component.h"

// ===== Includes ===========================================================
#include <NH/AccessList.h>

#include <NH/AccessListMap.h>

// ===== NHLib ==============================================================
#include "Errors.h"
#include "Utilities.h"

namespace NH
{

    // Public
    ////////////////////////////////////////////////////////////////////////

    AccessListMap::AccessListMap() : Object("Access lists")
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

    AccessList * AccessListMap::FindOrCreate(const char * aName, unsigned int aListType)
    {
        assert(NULL != aName);

        AccessList * lResult;

        InternalMap::iterator lIt = mAccessLists.find(aName);
        if (mAccessLists.end() == lIt)
        {
            lResult = new AccessList(aName);
            assert(NULL != lResult);

            lResult->SetName    (aName    );
            lResult->SetListType(aListType);

            mAccessLists.insert(InternalMap::value_type(aName, lResult));
        }
        else
        {
            lResult = lIt->second;
            assert(NULL != lResult);

            unsigned int lListType = lResult->GetListType() & aListType;
            if (0 == lListType)
            {
                ThrowError(ERROR_CONFIG, __LINE__, "List of different type with the same name");
            }

            lResult->SetListType(lListType);
        }

        return lResult;
    }

    // Internal
    /////////////////////////////////////////////////////////////////////////

    /// aInterfaces [--O;R--]
    /// aNATs       [--O;R--]
    unsigned int AccessListMap::Verify_Internal(const InterfaceList * aInterfaces, const NATList * aNATs) const
    {
        unsigned int lResult = 0;

        for (InternalMap::const_iterator lIt = mAccessLists.begin(); lIt != mAccessLists.end(); lIt++)
        {
            assert(NULL != lIt->second);

            lResult += lIt->second->Verify_Internal(aInterfaces, aNATs);
        }

        return lResult;
    }

}
