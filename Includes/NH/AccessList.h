
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
#include <NH/Types.h>

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

        /// \param aProtocol
        /// \param aSrcSubNet
        /// \param aSrcPort
        /// \param aDstAddr
        /// \param aDstPort
        /// \retval false
        /// \retval true
        bool IsAllowed(Access::Protocol aProtocol, const SubNet & aSrcSubNet, uint16_t aSrcPort, uint32_t aDstAddr, uint16_t aDstPort) const;

        void Undo();

        void Verify() const;

        void Verify(uint32_t aAddr, Direction aDirection) const;

        void Verify(const SubNet & aSubNet, Direction aDirection) const;

    private:

        AccessList(const AccessList &);

        const AccessList & operator = (const AccessList &);

        void DisplayError(int aCode, const char * aMessage, const Access & aAccess) const;
        void DisplayError(int aCode, const char * aMessage, const Access & aA0, const Access & aA1) const;

        typedef std::list<Access *> InternalList;

        InternalList mAccess;
        std::string  mName  ;

    };

}
