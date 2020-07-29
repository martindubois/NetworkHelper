
// License https://creativecommons.org/licenses/by-nc-nd/4.0/legalcode
// Product NetworkHelper

/// \author    KMS - Martin Dubois, P.Eng.
/// \copyright Copyright &copy; 2020 KMS. All rights reserved.
/// \file      Includes/NH/DHCPList.h
/// \brief     NH::DHCPList

#pragma once

// ===== C++ ================================================================
#include <map>

namespace NH
{

    class DHCP;
    class SubNet;

    /// \brief NH::DHCPList
    class DHCPList
    {

    public:

        DHCPList();

        ~DHCPList();

        /// \param aAddr The address
        /// \return This method returns a pointer to an internal instance or NULL.
        const DHCPList * Find(uint32_t aAddr) const;

        /// \param aSubNet The subnet
        /// \return This method returns a pointer to an internal instance or NULL.
        const DHCP * Find(const SubNet & aSubNet) const;

        /// \param aName The name of the DHCP pool
        /// \return This method returns a pointer to an interna instance.
        DHCP * FindOrCreate(const char * aName);

    // Internal

        unsigned int Verify_Internal() const;

    private:

        DHCPList(const DHCPList &);

        const DHCPList & operator = (const DHCPList &);

        typedef std::map<std::string, DHCP *> InternalMap;

        InternalMap mDHCPs;

    };

}
