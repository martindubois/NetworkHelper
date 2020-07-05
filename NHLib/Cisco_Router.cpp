
// Author     KMS - Martin Dubois, P.Eng.
// Copyrights (C) 2020 KMS. All rights reserved.
// Product    NetworkHelper
// File       NHLib/Cisco_Router.cpp

// CODE REVIEW 2020-07-04 KMS - Martin Dubois, P.Eng.

// TEST COVERAGE 2020-07-04 KMS - Martin Dubois, P.Eng.

// ===== C ==================================================================
#include <assert.h>

// ===== Includes ===========================================================
#include <Cisco/Router.h>

// ===== NHLib ==============================================================
#include "Cisco_Parser.h"
#include "Utilities.h"

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
            Utl_ThrowError("ERROR", __LINE__, "The specified information type is not supported", aType);
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
