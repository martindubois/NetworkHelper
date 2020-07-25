
// Author     KMS - Martin Dubois, P.Eng.
// Copyrights (C) 2020 KMS. All rights reserved.
// Product    NetworkHelper
// File       NHLib/AccessList.cpp

// CODE REVIEW 2020-07-24 KMS - Martin Dubois, P.Eng.

// TEST COVERAGE 2020-07-24 KMS - Martin Dubois, P.Eng.

#include "Component.h"

// ===== Includes ===========================================================
#include <NH/Access.h>
#include <NH/InterfaceList.h>
#include <NH/NATList.h>

#include <NH/AccessList.h>

// ===== NHLib ==============================================================
#include "Color.h"
#include "Errors.h"
#include "Utilities.h"

namespace NH
{

    // Public
    ////////////////////////////////////////////////////////////////////////

    AccessList::AccessList(const char * aName) : NamedObject("Access list")
    {
        assert(NULL != aName);

        SetName(aName);
    }

    AccessList::~AccessList()
    {
        for (InternalList::iterator lIt = mAccess.begin(); lIt != mAccess.end(); lIt++)
        {
            assert(NULL != (*lIt));

            delete (*lIt);
        }
    }

    unsigned int AccessList::GetListType() const
    {
        return mListType;
    }

    void AccessList::SetListType(unsigned int aListType)
    {
        mListType = aListType;
    }

    Access * AccessList::Add(Access::Type aType)
    {
        Access * lResult = new Access(aType);
        assert(NULL != lResult);

        mAccess.push_back(lResult);

        return lResult;
    }

    bool AccessList::IsAllowed(Access::Protocol aProtocol, const SubNet & aSrcSubNet, uint16_t aSrcPort, uint32_t aDstAddr, uint16_t aDstPort) const
    {
        for (InternalList::const_iterator lIt = mAccess.begin(); lIt != mAccess.end(); lIt++)
        {
            Access * lAccess = (*lIt);
            assert(NULL != lAccess);

            if (lAccess->Match(aProtocol, aSrcSubNet, aSrcPort, aDstAddr, aDstPort))
            {
                switch (lAccess->GetType())
                {
                case Access::TYPE_DENY  : return false;
                case Access::TYPE_PERMIT: return true ;

                default: assert(false);
                }
            }
        }

        return false;
    }

    void AccessList::Undo()
    {
        assert(NULL != mAccess.back());

        delete mAccess.back();

        mAccess.pop_back();
    }

    // NOT TESTED NH.AccessList.Verify

    void AccessList::Verify() const
    {
        ThrowErrorIfNeeded(ERROR_006, Verify_Internal(NULL, NULL));
    }

    // Internal
    /////////////////////////////////////////////////////////////////////////

    // aInterfaces [--O;R--]
    // aNATs       [--O;R--]
    unsigned int AccessList::Verify_Internal(const InterfaceList * aInterfaces, const NATList * aNATs) const
    {
        unsigned int lResult = 0;

        if (   (NULL != aInterfaces) && (NULL == aInterfaces->Find(*this))
            && (NULL != aNATs      ) && (NULL == aNATs      ->Find(*this)))
        {
            NamedObject::DisplayError(ERROR_233);
            lResult++;
        }

        if (mAccess.empty())
        {
            NamedObject::DisplayError(ERROR_232);
            lResult++;
        }
        else
        {
            for (InternalList::const_iterator lIt0 = mAccess.begin(); lIt0 != mAccess.end(); lIt0++)
            {
                Access * lA0 = (*lIt0);
                assert(NULL != lA0);

                lResult += lA0->Verify_Internal();

                AccessEnd::Filter lSF0 = lA0->mSource.GetFilter();
                if (AccessEnd::FILTER_ANY != lSF0)
                {
                    InternalList::const_iterator lIt1 = lIt0;

                    for (lIt1++; lIt1 != mAccess.end(); lIt1++)
                    {
                        Access * lA1 = (*lIt1);
                        assert(NULL != lA1);

                        AccessEnd::Filter lDF1 = lA1->mDestination.GetFilter();

                        switch (lSF0)
                        {
                        case AccessEnd::FILTER_HOST:
                            if ((AccessEnd::FILTER_ANY != lDF1) && lA1->mDestination.Match(lA0->mSource.GetHost()))
                            {
                                DisplayError(ERROR_CONFIG, __LINE__, "describe opposed traffics", *lA0, *lA1);
                                lResult++;
                            }
                            break;
                        case AccessEnd::FILTER_SUBNET:
                            if ((AccessEnd::FILTER_ANY != lDF1) && lA1->mDestination.Match(*lA0->mSource.GetSubNet()))
                            {
                                DisplayError(ERROR_208, *lA0, *lA1);
                                lResult++;
                            }
                            break;

                        default: assert(false);
                        }
                    }
                }
            }
        }

        return lResult;
    }

    // NOT TESTED NH.AccessList.Verify
    //            Some case of inverted data stream

    unsigned int AccessList::Verify_Internal(uint32_t aAddr, Direction aDirection) const
    {
        unsigned int lResult = 0;

        for (InternalList::const_iterator lIt = mAccess.begin(); lIt != mAccess.end(); lIt++)
        {
            Access * lAccess = (*lIt);
            assert(NULL != lAccess);

            AccessEnd::Filter lDF = lAccess->mDestination.GetFilter();
            AccessEnd::Filter lSF = lAccess->mSource     .GetFilter();

            switch (aDirection)
            {
            case DIRECTION_IN:
                if ((AccessEnd::FILTER_HOST == lSF) && lAccess->mSource.Match(aAddr))
                {
                    DisplayError(ERROR_CONFIG, __LINE__, " describe traffic going out from the interace and the access list is used as \"in\"", *lAccess);
                    lResult++;
                }

                if ((AccessEnd::FILTER_SUBNET == lDF) && lAccess->mDestination.Match(aAddr))
                {
                    DisplayError(ERROR_207, *lAccess);
                    lResult++;
                }
                break;

            case DIRECTION_OUT:
                if ((AccessEnd::FILTER_SUBNET == lSF) && lAccess->mSource.Match(aAddr))
                {
                    DisplayError(ERROR_206, *lAccess);
                    lResult++;
                }

                if ((AccessEnd::FILTER_HOST == lDF) && lAccess->mDestination.Match(aAddr))
                {
                    DisplayError(ERROR_CONFIG, __LINE__, " describe traffic going into the interace and the access list is used as \"out\"", *lAccess);
                    lResult++;
                }
                break;

            default: assert(false);
            }
        }

        return lResult;
    }

    // NOT TESTED NH.AccessList.Verify
    //            Access associated to an interface using DHCP.

    unsigned int AccessList::Verify_Internal(const SubNet & aSubNet, Direction aDirection) const
    {
        assert(NULL != &aSubNet);

        unsigned int lResult = 0;

        for (InternalList::const_iterator lIt = mAccess.begin(); lIt != mAccess.end(); lIt++)
        {
            Access * lAccess = (*lIt);
            assert(NULL != lAccess);

            switch (aDirection)
            {
            case DIRECTION_IN:
                if (lAccess->mSource.Match(aSubNet))
                {
                    DisplayError(ERROR_CONFIG, __LINE__, " describe traffic going out from the interace and the access list is used as \"in\"", *lAccess);
                    lResult++;
                }
                break;

            case DIRECTION_OUT:
                if (lAccess->mDestination.Match(aSubNet))
                {
                    DisplayError(ERROR_CONFIG, __LINE__, " describe traffic going into the interace and the access list is used as \"out\"", *lAccess);
                    lResult++;
                }
                break;

            default: assert(false);
            }
        }

        return lResult;
    }

    // Private
    /////////////////////////////////////////////////////////////////////////

    void AccessList::DisplayError(const char * aErrorType, int aCode, const char * aMessage, const Access & aAccess) const
    {
        assert(NULL != aMessage);
        assert(NULL != &aAccess);

        char lMessage[256];
        char lDesc   [ 64];

        aAccess.GetDescription(lDesc, sizeof(lDesc));

        sprintf_s(lMessage, "%s %s", lDesc, aMessage);

        NamedObject::DisplayError(aErrorType, aCode, lMessage);
    }

    void AccessList::DisplayError(const char * aErrorType, int aCode, const char * aMessage, const Access & aA0, const Access & aA1) const
    {
        assert(NULL != aMessage);
        assert(NULL != &aA0);
        assert(NULL != &aA1);

        char lMessage[256];
        char lDesc0  [ 64];
        char lDesc1  [ 64];

        aA0.GetDescription(lDesc0, sizeof(lDesc0));
        aA1.GetDescription(lDesc1, sizeof(lDesc1));

        sprintf_s(lMessage, "%s and %s %s", lDesc0, lDesc1, aMessage);

        NamedObject::DisplayError(aErrorType, aCode, lMessage);
    }

}
