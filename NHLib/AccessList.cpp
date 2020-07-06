
// Author     KMS - Martin Dubois, P.Eng.
// Copyrights (C) 2020 KMS. All rights reserved.
// Product    NetworkHelper
// File       NHLib/AccessList.cpp

// CODE REVIEW 2020-07-06 KMS - Martin Dubois, P.Eng.

// TEST COVERAGE 2020-07-06 KMS - Martin Dubois, P.Eng.

// ===== C ==================================================================
#include <assert.h>

// ===== Includes ===========================================================
#include <NH/Access.h>

#include <NH/AccessList.h>

// ===== NHLib ==============================================================
#include "Color.h"
#include "Utilities.h"

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
        unsigned int lErrorCount = 0;

        for (InternalList::const_iterator lIt0 = mAccess.begin(); lIt0 != mAccess.end(); lIt0++)
        {
            try
            {
                Access * lA0 = (*lIt0);
                assert(NULL != lA0);

                lA0->Verify();

                AccessEnd::Filter lSF0 = lA0->mSource.GetFilter();
                if (AccessEnd::FILTER_ANY != lSF0)
                {
                    InternalList::const_iterator lIt1 = lIt0;

                    for (lIt1++; lIt1 != mAccess.end(); lIt1++)
                    {
                        Access * lA1 = (*lIt1);
                        assert(NULL != lA1);

                        switch (lSF0)
                        {
                        case AccessEnd::FILTER_HOST  :
                            if (lA1->mDestination.VerifyAddress(lA0->mSource.GetHost()))
                            {
                                Error(__LINE__, "describe opposed trafics", lA0, lA1);
                            }
                            break;
                        case AccessEnd::FILTER_SUBNET:
                            if (lA1->mDestination.VerifySubNet(lA0->mSource.GetSubNet()))
                            {
                                Error(__LINE__, "describe opposed trafics", lA0, lA1);
                            }
                            break;

                        default: assert(false);
                        }
                    }
                }
            }
            catch (std::exception eE)
            {
                COLOR(RED);
                {
                    fprintf(stderr, "EXCEPTION  %03u  %s\n", __LINE__, eE.what());
                }
                COLOR(WHITE);
                lErrorCount++;
            }
        }

        if (0 < lErrorCount)
        {
            Error(__LINE__, "contains at least one error");
        }
    }

    // Private
    /////////////////////////////////////////////////////////////////////////

    void AccessList::Error(int aCode, const char * aMessage) const
    {
        assert(NULL != aMessage);

        char lMessage[128];

        sprintf_s(lMessage, "The %s access-list %s", mName.c_str(), aMessage);

        Utl_ThrowError("ERROR", aCode, lMessage);
    }

    void AccessList::Error(int aCode, const char * aMessage, const Access * aA0, const Access * aA1) const
    {
        assert(NULL != aMessage);
        assert(NULL != aA0);
        assert(NULL != aA1);

        char lMessage[256];
        char lDesc0  [ 64];
        char lDesc1  [ 64];

        aA0->GetDescription(lDesc0, sizeof(lDesc0));
        aA1->GetDescription(lDesc1, sizeof(lDesc1));

        sprintf_s(lMessage, "In %s access-list, %s and %s %s", mName.c_str(), lDesc0, lDesc1, aMessage);

        Utl_ThrowError("ERROR", aCode, lMessage);
    }

}
