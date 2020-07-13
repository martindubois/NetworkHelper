
// Author     KMS - Martin Dubois, P.Eng.
// Copyrights (C) 2020 KMS. All rights reserved.
// Product    NetworkHelper
// File       NHLib/SubNetList.cpp

// CODE REVIEW 2020-07-13 KMS - Martin Dubois, P.Eng.

// TEST COVERAGE 2020-07-13 KMS - Martin Dubois, P.Eng.

#include "Component.h"

// ===== Windows ============================================================
#include <ws2tcpip.h>

// ===== Includes ===========================================================
#include <NH/SubNet.h>

#include <NH/SubNetList.h>

// ===== NHLib ==============================================================
#include "Errors.h"
#include "IPv4.h"
#include "Utilities.h"

// Constants
/////////////////////////////////////////////////////////////////////////////

#define ELEMENT "SubNet list"

namespace NH
{

    // Public
    ////////////////////////////////////////////////////////////////////////

    SubNetList::SubNetList()
    {
    }

    SubNetList::~SubNetList()
    {
        for (InternalList::iterator lIt = mSubNets.begin(); lIt != mSubNets.end(); lIt++)
        {
            assert(NULL != *lIt);

            delete *lIt;
        }
    }

    // NOT TESTED NH.SubNetList.Find
    //            Do not find

    const SubNet * SubNetList::Find(uint32_t aAddr) const
    {
        for (InternalList::const_iterator lSubNet = mSubNets.begin(); lSubNet != mSubNets.end(); lSubNet++)
        {
            const SubNet * lResult = (*lSubNet);
            assert(NULL != lResult);

            if (lResult->VerifyAddress(aAddr))
            {
                return lResult;
            }
        }

        return NULL;
    }

    SubNet * SubNetList::FindOrCreate(uint32_t aAddr, uint32_t aMask)
    {
        SubNet * lSubNet;

        for (InternalList::iterator lIt = mSubNets.begin(); lIt != mSubNets.end(); lIt++)
        {
            lSubNet = *lIt;
            assert(NULL != lSubNet);

            if (lSubNet->Is(aAddr, aMask))
            {
                return lSubNet;
            }
        }

        lSubNet = new SubNet(aAddr, aMask);
        assert(NULL != lSubNet);

        mSubNets.push_back(lSubNet);

        return lSubNet;
    }

    SubNet * SubNetList::FindOrCreate(const char * aAddr, const char * aMask)
    {
        uint32_t lAddr = IPv4_TextToAddress(aAddr);
        uint32_t lMask = IPv4_TextToAddress(aMask);

        return FindOrCreate(lAddr, lMask);
    }

    // NOT TESTED NH.SubNetList.Verify

    void SubNetList::Verify() const
    {
        Utl_ThrowErrorIfNeeded(__LINE__, ELEMENT, "", Verify_Internal());
    }

    // Internal
    ////////////////////////////////////////////////////////////////////////

    void SubNetList::Prepare(HI::Diagram * aDiagram, ShapeMap * aShapeMap)
    {
        for (InternalList::iterator lIt = mSubNets.begin(); lIt != mSubNets.end(); lIt++)
        {
            assert(NULL != *lIt);

            (*lIt)->Prepare(aDiagram, aShapeMap);
        }
    }

    // NOT TESTED NH.SubNetList.Verify.Error

    unsigned int SubNetList::Verify_Internal() const
    {
        unsigned int lResult = 0;

        for (InternalList::const_iterator lIt0 = mSubNets.begin(); lIt0 != mSubNets.end(); lIt0++)
        {
            SubNet * lSN0 = *lIt0;
            assert(NULL != lSN0);

            InternalList::const_iterator lIt1 = lIt0;

            lIt1++;

            for (; lIt1 != mSubNets.end(); lIt1++)
            {
                SubNet * lSN1 = *lIt1;
                assert(NULL != lSN1);

                if (lSN1->VerifyAddress(lSN0->GetAddress()))
                {
                    char lMessage[128];
                    char lName[2][64];

                    lSN0->GetFullName(lName[0], sizeof(lName[0]));
                    lSN1->GetFullName(lName[1], sizeof(lName[1]));

                    int lRet = sprintf_s(lMessage, "SubNet %s and %s overlap", lName[0], lName[1]);
                    assert(               0 < lRet);
                    assert(sizeof(lMessage) > lRet);

                    Utl_DisplayError(ERROR_CONFIG, __LINE__, lMessage);
                    lResult++;
                }
            }
        }

        return lResult;
    }

}
