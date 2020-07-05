
// License https://creativecommons.org/licenses/by-nc-nd/4.0/legalcode
// Product NetworkHelper

/// \author    KMS - Martin Dubois, P.Eng.
/// \copyright Copyright &copy; 2020 KMS. All rights reserved.
/// \file      Includes/NH/Route.h
/// \brief     NH::Route

#pragma once

// ===== C ==================================================================
#include <stdint.h>

namespace NH
{

    class SubNet;

    /// \brief NH::Route
    class Route
    {

    public:

        /// \param aSubNet
        /// \param aAddr
        Route(const SubNet * aSubNet, uint32_t aAddr);

        /// \param aSubNet
        /// \param aAddr
        Route(const SubNet * aSubNet, const char * aAddr);

        ~Route();

        /// \return This method return the address of the router to use to reach the SubNet.
        uint32_t GetAddress() const;

        /// \param aOut
        /// \param aOutSize_byte
        void GetAddress(char * aOut, unsigned int aOutSize_byte) const;

        /// \param aOut
        /// \param aOutSize_byte
        void GetDescription(char * aOut, unsigned int aOutSize_byte) const;

        /// \return This method return the destination SubNet
        const SubNet * GetSubNet() const;

    private:

        uint32_t mAddr;

        const SubNet * mSubNet;

    };

}
