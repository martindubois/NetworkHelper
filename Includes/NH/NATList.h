
// License https://creativecommons.org/licenses/by-nc-nd/4.0/legalcode
// Product NetworkHelper

/// \author    KMS - Martin Dubois, P.Eng.
/// \copyright Copyright &copy; 2020 KMS. All rights reserved.
/// \file      Includes/NH/NAT.h
/// \brief     NH::NATList

#pragma once

// ===== C++ ================================================================
#include <map>

namespace NH
{

    class AccessList;
    class NAT;
    class SubNet;

    /// \brief NH::NATList
    class NATList
    {

    public:

        NATList();

        ~NATList();

        /// \param aAddr
        /// \return This method returns a pointer to an internal instance or NULL.
        const NAT * Find(uint32_t aAddr) const;

        /// \param aAccessList
        /// \return This method return a pointer to an internal NAT instance or NULL.
        const NAT * Find(const AccessList & aAccessList) const;

        /// \param aSubNet 
        /// \return This method returns a pointer to an internal instance or NULL.
        const NAT * Find(const SubNet & aSubNet) const;

        /// \param aName
        /// \return This method returns a pointer to an interna instance.
        NAT * FindOrCreate(const char * aName);

    // Internal

        unsigned int Verify_Internal() const;

    private:

        NATList(const NATList &);

        const NATList & operator = (const NATList &);

        typedef std::map<std::string, NAT *> InternalMap;

        InternalMap mNATs;

    };

}
