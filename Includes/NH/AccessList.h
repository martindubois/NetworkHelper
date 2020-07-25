
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
#include <NH/NamedObject.h>
#include <NH/Types.h>

namespace NH
{

    class InterfaceList;
    class NATList;

    /// \brief NH::AccessList
    class AccessList : public NamedObject
    {

    public:

        /// \param aName
        AccessList(const char * aName);

        ~AccessList();

        /// \return This method returns the list type set using SetListType.
        unsigned int GetListType() const;

        /// \param aListType
        void SetListType(unsigned int aListType);

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

    // Internal

        unsigned int Verify_Internal(const InterfaceList * aInterfaces, const NATList * aNATs) const;
        unsigned int Verify_Internal(uint32_t       aAddr  , Direction aDirection) const;
        unsigned int Verify_Internal(const SubNet & aSubNet, Direction aDirection) const;

    private:

        AccessList(const AccessList &);

        const AccessList & operator = (const AccessList &);

        void DisplayError(const char * aErrorType, int aCode, const char * aMessage, const Access & aAccess) const;
        void DisplayError(const char * aErrorType, int aCode, const char * aMessage, const Access & aA0, const Access & aA1) const;

        typedef std::list<Access *> InternalList;

        InternalList mAccess;

        unsigned int mListType;

    };

}
