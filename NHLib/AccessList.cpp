
// Author     KMS - Martin Dubois, P.Eng.
// Copyrights (C) 2020 KMS. All rights reserved.
// Product    NetworkHelper
// File       NHLib/AccessList.cpp

// CODE REVIEW 2020-07-07 KMS - Martin Dubois, P.Eng.

// TEST COVERAGE 2020-07-07 KMS - Martin Dubois, P.Eng.

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
                            DisplayError(__LINE__, "describe opposed trafics", lA0, lA1);
                            lErrorCount++;
                        }
                        break;
                    case AccessEnd::FILTER_SUBNET:
                        if (lA1->mDestination.VerifySubNet(lA0->mSource.GetSubNet()))
                        {
                            DisplayError(__LINE__, "describe opposed trafics", lA0, lA1);
                            lErrorCount++;
                        }
                        break;

                    default: assert(false);
                    }
                }
            }
        }

        Utl_ThrowErrorIfNeeded(__LINE__, "access-list", mName.c_str(), lErrorCount);
    }

    // NOT TESTED NH.AccessList.Verify
    //            Some case of inverted data stream

    void AccessList::Verify(uint32_t aAddr, Direction aDirection) const
    {
        unsigned int lErrorCount = 0;

        for (InternalList::const_iterator lIt = mAccess.begin(); lIt != mAccess.end(); lIt++)
        {
            Access * lAccess = (*lIt);
            assert(NULL != lAccess);

            switch (aDirection)
            {
            case DIRECTION_IN:
                if ((AccessEnd::FILTER_HOST == lAccess->mSource.GetFilter()) && lAccess->mSource.VerifyAddress(aAddr))
                {
                    DisplayError(__LINE__, " describe trafic going out from the interace and the access list is using as \"in\"", lAccess);
                    lErrorCount++;
                }

                if ((AccessEnd::FILTER_SUBNET == lAccess->mDestination.GetFilter()) && lAccess->mDestination.VerifyAddress(aAddr))
                {
                    DisplayError(__LINE__, " describe trafic going out from the interace and the access list is using as \"in\"", lAccess);
                    lErrorCount++;
                }
                break;

            case DIRECTION_OUT:
                if ((AccessEnd::FILTER_SUBNET == lAccess->mSource.GetFilter()) && lAccess->mSource.VerifyAddress(aAddr))
                {
                    DisplayError(__LINE__, " describe trafic going in to the interace and the access list is using as \"out\"", lAccess);
                    lErrorCount++;
                }

                if ((AccessEnd::FILTER_HOST == lAccess->mDestination.GetFilter()) && lAccess->mDestination.VerifyAddress(aAddr))
                {
                    DisplayError(__LINE__, " describe trafic going in to the interace and the access list is using as \"out\"", lAccess);
                    lErrorCount++;
                }
                break;

            default: assert(false);
            }
        }

        Utl_ThrowErrorIfNeeded(__LINE__, "access-list", mName.c_str(), lErrorCount);
    }

    // NOT TESTED NH.AccessList.Verify
    //            Access associated to an interface using DHCP.

    void AccessList::Verify(const SubNet * aSubNet, Direction aDirection) const
    {
        assert(NULL != aSubNet);

        unsigned int lErrorCount = 0;

        for (InternalList::const_iterator lIt = mAccess.begin(); lIt != mAccess.end(); lIt++)
        {
            Access * lAccess = (*lIt);
            assert(NULL != lAccess);

            switch (aDirection)
            {
            case DIRECTION_IN:
                if (lAccess->mSource.VerifySubNet(aSubNet))
                {
                    DisplayError(__LINE__, " describe trafic going out from the interace and the access list is using as \"in\"", lAccess);
                    lErrorCount++;
                }
                break;

            case DIRECTION_OUT:
                if (lAccess->mDestination.VerifySubNet(aSubNet))
                {
                    DisplayError(__LINE__, " describe trafic going in to the interace and the access list is using as \"out\"", lAccess);
                    lErrorCount++;
                }
                break;

            default: assert(false);
            }
        }

        Utl_ThrowErrorIfNeeded(__LINE__, "access-list", mName.c_str(), lErrorCount);
    }

    // Private
    /////////////////////////////////////////////////////////////////////////

    void AccessList::DisplayError(int aCode, const char * aMessage, const Access * aAccess) const
    {
        assert(NULL != aMessage);
        assert(NULL != aAccess);

        char lMessage[256];
        char lDesc   [ 64];

        aAccess->GetDescription(lDesc, sizeof(lDesc));

        sprintf_s(lMessage, "In %s access-list, %s %s", mName.c_str(), lDesc, aMessage);

        Utl_DisplayError("ERROR", aCode, lMessage);
    }

    void AccessList::DisplayError(int aCode, const char * aMessage, const Access * aA0, const Access * aA1) const
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

        Utl_DisplayError("ERROR", aCode, lMessage);
    }

}
