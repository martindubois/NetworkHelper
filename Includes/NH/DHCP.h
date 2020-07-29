
// License https://creativecommons.org/licenses/by-nc-nd/4.0/legalcode
// Product NetworkHelper

/// \author    KMS - Martin Dubois, P.Eng.
/// \copyright Copyright &copy; 2020 KMS. All rights reserved.
/// \file      Includes/NH/DHCP.h
/// \brief     NH::DHCP

#pragma once

// ===== Includes ===========================================================
#include <NH/NamedObject.h>

namespace NH
{

    class SubNet;

    /// \brief NH::DHCP
    class DHCP : public NamedObject
    {

    public:

        DHCP();

        /// \param aDefaultRouter The default router address
        /// \exception std::exception
        void SetDefaultRouter(uint32_t aDefaultRouter);

        /// \param aDnsServer The DNS server address
        /// \exception std::exception
        void SetDnsServer(uint32_t aDnsServer);

        /// \param aSubNet The subnet the DHCP server provide address to
        /// \exception std::exception
        void SetSubNet(const SubNet * aSubNet);

    // Internal

        unsigned int Verify_Internal() const;

    private:

        uint32_t mDefaultRouter;
        uint32_t mDnsServer;

        const SubNet * mSubNet;

    };

}
