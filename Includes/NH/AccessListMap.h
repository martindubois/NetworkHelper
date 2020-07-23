
// License https://creativecommons.org/licenses/by-nc-nd/4.0/legalcode
// Product NetworkHelper

/// \author    KMS - Martin Dubois, P.Eng.
/// \copyright Copyright &copy; 2020 KMS. All rights reserved.
/// \file      Includes/NH/AccessListMap.h
/// \brief     NH::AccessListMap

#pragma once

// ===== C++ ================================================================
#include <Map>

// ===== Includes ===========================================================
#include <NH/Object.h>

namespace NH
{

    class AccessList;

    /// \brief NH::AccessListMap
    class AccessListMap : public Object
    {

    public:

        AccessListMap();

        ~AccessListMap();

        /// \param aName
        /// \param aListType
        /// \return This method return a pointer to a AccessList instance.
        AccessList * FindOrCreate(const char * aName, unsigned int aListType);

        void Verify() const;

    // Internal

        unsigned int Verify_Internal() const;

    private:

        AccessListMap(const AccessListMap &);

        const AccessListMap & operator = (const AccessListMap &);

        typedef std::map<std::string, AccessList *> InternalMap;

        InternalMap mAccessLists;

    };

}
