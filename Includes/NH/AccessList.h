
// License https://creativecommons.org/licenses/by-nc-nd/4.0/legalcode
// Product NetworkHelper

/// \author    KMS - Martin Dubois, P.Eng.
/// \copyright Copyright &copy; 2020 KMS. All rights reserved.
/// \file      Includes/NH/AccessList.h
/// \brief     NH::AccessList

#pragma once

// ===== C++ ================================================================
#include <list>
#include <string>

// ===== Includes ===========================================================
#include <NH/Access.h>

namespace NH
{

    /// \brief NH::AccessList
    class AccessList
    {

    public:

        /// \param aName
        AccessList(const char * aName);

        ~AccessList();

        /// \param aType
        Access * Add(Access::Type aType);

        void Undo();

        void Verify() const;

    private:

        AccessList(const AccessList &);

        const AccessList & operator = (const AccessList &);

        void Error(int aCode, const char * aMessage) const;
        void Error(int aCode, const char * aMessage, const Access * aA0, const Access * aA1) const;

        typedef std::list<Access *> InternalList;

        InternalList mAccess;
        std::string  mName  ;

    };

}
