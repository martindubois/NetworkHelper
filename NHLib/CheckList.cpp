
// Author     KMS - Martin Dubois, P.Eng.
// Copyrights (C) 2020 KMS. All rights reserved.
// Product    NetworkHelper
// File       NHLib/CheckList.cpp

// CODE REVIEW 2020-07-25 KMS - Martin Dubois, P.Eng.

// TEST COVERAGE 2020-07-25 KMS - Martin Dubois, P.Eng.

// ===== C ==================================================================
#include <assert.h>

// ===== NHLib ==============================================================
#include "Check.h"

#include "CheckList.h"

// Public
/////////////////////////////////////////////////////////////////////////////

CheckList::CheckList()
{
}

CheckList::~CheckList()
{
    for (InternalList::iterator lIt = mChecks.begin(); lIt != mChecks.end(); lIt++)
    {
        assert(NULL != (*lIt));

        delete *lIt;
    }
}

// [DK-;RW-]
void CheckList::Add(Check * aCheck)
{
    assert(NULL != aCheck);

    mChecks.push_back(aCheck);
}

unsigned int CheckList::Verify(const NH::Interface & aElement) const
{
    unsigned int lResult = 0;

    for (InternalList::const_iterator lIt = mChecks.begin(); lIt != mChecks.end(); lIt++)
    {
        assert(NULL != (*lIt));

        lResult += (*lIt)->Verify(aElement);
    }

    return lResult;
}
