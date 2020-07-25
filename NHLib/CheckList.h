
// Author     KMS - Martin Dubois, P.Eng.
// Copyrights (C) 2020 KMS. All rights reserved.
// Product    NetworkHelper
// File       NHLib/CheckList.h

// CODE REVIEW 2020-07-25 KMS - Martin Dubois, P.Eng.

#pragma once

// ===== C++ ================================================================
#include <list>

// ===== Includes ===========================================================
#include <NH/Interface.h>

// ===== NHLib ==============================================================
class Check;

// Class
/////////////////////////////////////////////////////////////////////////////

class CheckList
{

public:

    CheckList();

    ~CheckList();

    void Add(Check * aCheck);

    unsigned int Verify(const NH::Interface & aElement) const;

private:

    typedef std::list<Check *> InternalList;

    InternalList mChecks;

};
