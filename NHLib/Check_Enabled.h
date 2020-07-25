
// Author     KMS - Martin Dubois, P.Eng.
// Copyrights (C) 2020 KMS. All rights reserved.
// Product    NetworkHelper
// File       NHLib/Check_Enabled.h

// CODE REVIEW 2020-07-25 KMS - Martin Dubois, P.Eng.

#pragma once

// ===== NHLib ==============================================================
#include "Check.h"

// Class
/////////////////////////////////////////////////////////////////////////////

class Check_Enabled : public Check
{

public:

    Check_Enabled(const char * aType, int aCode, const char * aMessage);

    // ===== Check ==========================================================

    virtual ~Check_Enabled();

    virtual unsigned int Verify(const NH::Interface & aElement) const;

};
