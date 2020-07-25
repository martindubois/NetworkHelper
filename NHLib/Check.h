
// Author     KMS - Martin Dubois, P.Eng.
// Copyrights (C) 2020 KMS. All rights reserved.
// Product    NetworkHelper
// File       NHLib/Check.h

// CODE REVIEW 2020-07-25 KMS - Martin Dubois, P.Eng.

#pragma once

// ===== Includes ===========================================================
#include <NH/Interface.h>

// Class
/////////////////////////////////////////////////////////////////////////////

class Check
{

public:

    Check(const char * aType, int aCode, const char * aMessage);

    virtual ~Check();

    virtual unsigned int Verify(const NH::Interface & aElement) const;

private:

    int          mCode   ;
    const char * mMessage;
    const char * mType   ;

};
