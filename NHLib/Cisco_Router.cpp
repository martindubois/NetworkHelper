
// Author     KMS - Martin Dubois, P.Eng.
// Copyrights (C) 2020 KMS. All rights reserved.
// Product    NetworkHelper
// File       NHLib/Cisco_Router.cpp

// CODE REVIEW 2020-07-10 KMS - Martin Dubois, P.Eng.

// TEST COVERAGE 2020-07-10 KMS - Martin Dubois, P.Eng.

#include "Component.h"

// ===== Includes ===========================================================
#include <Cisco/Router.h>

// ===== NHLib ==============================================================
#include "Errors.h"
#include "Utilities.h"

#include "Cisco_Parser.h"

namespace Cisco
{

    // Public
    /////////////////////////////////////////////////////////////////////////

    Router::Router()
    {
    }

    // ===== Network/Router =================================================

    Router::~Router()
    {
    }

    void Router::RetrieveInfo(InfoType aType, const char * aData, unsigned int aFlags)
    {
        assert(INFO_QTY > aType);

        NH::Router::RetrieveInfo(aType, aData, aFlags);

        switch (aType)
        {
        case INFO_CONFIG_FILE: ParseConfigFile(aData, aFlags); break;

        default:
            Utl_ThrowError(ERROR_CALLER, __LINE__, "The specified information type is not supported");
        }
    }

    // Private
    ////////////////////////////////////////////////////////////////////////

    void Router::ParseConfigFile(const char * aFileName, unsigned int aFlags)
    {
        assert(NULL != aFileName);

        Cisco::Parser lParser;

        lParser.SetRouter(this);

        lParser.Parse(aFileName);
    }

}
