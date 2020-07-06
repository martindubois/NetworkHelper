
// Author     KMS - Martin Dubois, P.Eng.
// Copyrights (C) 2020 KMS. All rights reserved.
// Product    NetworkHelper
// File       NHLib/AccessList.cpp

// CODE REVIEW 2020-07-03 KMS - Martin Dubois, P.Eng.

// TEST COVERAGE 2020-07-03 KMS - Martin Dubois, P.Eng.

// ===== C ==================================================================
#include <assert.h>

// ===== Includes ===========================================================
#include <NH/Access.h>

#include <NH/AccessList.h>

namespace NH
{

    // Public
    ////////////////////////////////////////////////////////////////////////

    AccessList::AccessList(const char * aName)
    {
        assert(NULL != aName);

        mName = aName;
    }

    AccessList::~AccessList()
    {
        for (InternalList::iterator lIt = mAccess.begin(); lIt != mAccess.end(); lIt++)
        {
            assert(NULL != (*lIt));

            delete (*lIt);
        }
    }

    Access * AccessList::Add(Access::Type aType)
    {
        Access * lResult = new Access(aType);
        assert(NULL != lResult);

        mAccess.push_front(lResult);

        return lResult;
    }

    // NOT TESTED NH.AccessList.Undo

    void AccessList::Undo()
    {
        assert(NULL != mAccess.back());

        delete mAccess.back();

        mAccess.pop_back();
    }

    void AccessList::Verify() const
    {
        for (InternalList::const_iterator lIt = mAccess.begin(); lIt != mAccess.end(); lIt++)
        {
            assert(NULL != (*lIt));

            (*lIt)->Verify();
        }
    }

}
