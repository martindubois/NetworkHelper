
// Author     KMS - Martin Dubois, P.Eng.
// Copyrights (C) 2020 KMS. All rights reserved.
// Product    NetworkHelper
// File       NHLib/NATList.cpp

// CODE REVIEW 2020-07-21 KMS - Martin Dubois, P.Eng.

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
        for (InternalList::iterator lIt = mNATs.begin(); lIt != mNATs.end(); lIt++)
        {
            assert(NULL != (*lIt));

            delete (*lIt);
        }
    }

    void NATList::Add(const char * aName, uint32_t aFirst, uint32_t aLast, uint32_t aNetMask)
    {
        NAT * lNAT = new NAT();
        assert(NULL != lNAT);

        lNAT->SetName(aName);

        lNAT->Set(aFirst, aLast, aNetMask);

        mNATs.push_back(lNAT);
    }

    void NATList::Add(const char * aName, const char * aFirst, const char * aLast, const char * aNetMask)
    {
        NAT * lNAT = new NAT();
        assert(NULL != lNAT);

        lNAT->SetName(aName);

        lNAT->Set(aFirst, aLast, aNetMask);

        mNATs.push_back(lNAT);
    }

    const NAT * NATList::Find(uint32_t aAddr) const
    {
        for (InternalList::const_iterator lIt = mNATs.begin(); lIt != mNATs.end(); lIt++)
        {
            const NAT * lNAT = *lIt;
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
        for (InternalList::const_iterator lIt = mNATs.begin(); lIt != mNATs.end(); lIt++)
        {
            const NAT * lNAT = *lIt;
            assert(NULL != lNAT);

            if (lNAT->Match(aSubNet))
            {
                return lNAT;
            }
        }

        return NULL;
    }

}
