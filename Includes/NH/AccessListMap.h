
// License https://creativecommons.org/licenses/by-nc-nd/4.0/legalcode
// Product NetworkHelper

/// \author    KMS - Martin Dubois, P.Eng.
/// \copyright Copyright &copy; 2020 KMS. All rights reserved.
/// \file      Includes/NH/AccessListMap.h
/// \brief     NH::AccessListMap

#pragma once

// ===== C++ ================================================================
#include <Map>

namespace NH
{

    class AccessList;

    /// \brief NH::AccessListMap
    class AccessListMap
    {

    public:

        AccessListMap();

        ~AccessListMap();

        /// \param aName
        /// \return This method return a pointer to a AccessList instance.
        AccessList * FindOrCreate(const char * aName);

        void Verify() const;

    private:

        AccessListMap(const AccessListMap &);

        const AccessListMap & operator = (const AccessListMap &);

        typedef std::map<std::string, AccessList *> InternalMap;

        InternalMap mAccessLists;

    };

}
