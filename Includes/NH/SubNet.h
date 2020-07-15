
// License https://creativecommons.org/licenses/by-nc-nd/4.0/legalcode
// Product NetworkHelper

/// \author    KMS - Martin Dubois, P.Eng.
/// \copyright Copyright &copy; 2020 KMS. All rights reserved.
/// \file      Includes/NH/SubNet.h
/// \brief     NH::Subnet

#pragma once

// ===== Import/Includes ====================================================
#include <HI/Diagram.h>
#include <HI/Shape.h>

// ===== Includes ===========================================================
#include <NH/Object.h>

// ===== NHLib ==============================================================
class ShapeMap;

namespace NH
{

    class Interface;
    class Router   ;

    /// \brief NH::SubNet
    class SubNet : public Object
    {

    public:

        /// \param aAddr IPv4 subnet address
        /// \param aMask IPV4 subnet mask
        /// \exception std::exception
        SubNet(uint32_t aAddr, uint32_t aMask);

        ~SubNet();

        /// \return This method returns the subnet address.
        uint32_t GetAddress() const;

        /// \param aOut
        /// \param aOutSize_byte
        void GetAddress(char * aOut, unsigned int aOutSize_byte) const;

        /// \param aInterface
        /// \retval false
        /// \retval true
        bool GetDHCP(const Interface * aInterface) const;

        /// \param aOut
        /// \param aOutSize_byte
        void GetFullName(char * aOut, unsigned int aOutSize_byte) const;

        /// \param aOut
        /// \param aOutSize_byte
        void GetMask(char * aOut, unsigned int aOutSize_byte) const;

        /// \param aAddr IPv4 subnet address
        /// \param aMask IPV4 subnet mask
        bool Is(uint32_t aAddr, uint32_t aMask) const;

        /// \param aRouter
        /// \param aInterface
        /// \exception std::exception
        void SetDHCP(const Router * aRouter, const Interface * aInterface);

        /// \param aAddr IPv4 address to validate
        /// \exception std::exception
        void ValidateAddress(uint32_t aAddr) const;

        /// \param aAddr IPv4 address to validate
        bool VerifyAddress(uint32_t aAddr) const;

    // Internal

        void Prepare(HI::Diagram * aDiagram, ShapeMap * aSubNetMap);

    private:

        uint32_t mAddr;
        uint32_t mMask;

        const Interface * mDHCP_Interface;
        const Router    * mDHCP_Router   ;

    };

}
