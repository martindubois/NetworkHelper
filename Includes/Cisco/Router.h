
// License https://creativecommons.org/licenses/by-nc-nd/4.0/legalcode
// Product NetworkHelper

/// \author    KMS - Martin Dubois, P.Eng.
/// \copyright Copyright &copy; 2020 KMS. All rights reserved.
/// \file      Includes/Cisco/Router.h
/// \brief     Cisco::Router

#pragma once

// ===== Includes ===========================================================
#include <NH/Access.h>
#include <NH/Router.h>

namespace Cisco
{

    /// \brief Cisco::Router
    class Router : public NH::Router
    {

    public:

        Router();

        // ====== NH::Router ================================================

        virtual ~Router();

        virtual void RetrieveInfo(InfoType aType, const char * aData, unsigned int aFlags = 0);

    private:

        enum
        {
            CES = 64,
        };

        Router(const Router &);

        const Router & operator = (const Router &);


        void ParseConfigFile(const char * aFileName, unsigned int aFlags);

        unsigned int ParseConfigLine(const char * aLine, unsigned int aFlags);

        bool ParseRealConfigLine(const char * aLine);

    };

}
