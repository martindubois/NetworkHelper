
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

        /// \param aOut          The output buffer
        /// \param aOutSize_byte The outbut buffer size
        void GetAddress(char * aOut, unsigned int aOutSize_byte) const;

        /// \param aInterface Is the DHCP server of this subnet binded to this interface?
        /// \retval false No, the DHCP server of this subnet is not binded to this interface.
        /// \retval true  Yes, the DHCP server of this subnet is binded to this interface.
        bool GetDHCP(const Interface * aInterface) const;

        /// \param aOut          The output buffer
        /// \param aOutSize_byte The output buffer size
        void GetFullName(char * aOut, unsigned int aOutSize_byte) const;

        /// \param aOut          The output buffer
        /// \param aOutSize_byte The output buffer size
        void GetMask(char * aOut, unsigned int aOutSize_byte) const;

        /// \param aAddr IPv4 subnet address
        /// \param aMask IPV4 subnet mask
        bool Is(uint32_t aAddr, uint32_t aMask) const;

        /// \retval false No, this subnet is not a private subnet.
        /// \retval true  Yes, this subnetis a private subnet.
        bool IsPrivate() const;

        /// \retval false No, this subnet is not a public subnet.
        /// \retval true  Yes, this subnetis a public subnet.
        bool IsPublic() const;

        /// \param aAddr Is this address on the subnet?
        /// \retval false No, the address is not on the subnet.
        /// \retval true  Yes, the address is on the subnet.
        bool Match(uint32_t aAddr) const;

        /// \param aRouter    This router act as the DHCP server for the subnet.
        /// \param aInterface The DHCP server running on the router is binded to this interface.
        /// \exception std::exception
        void SetDHCP(const Router * aRouter, const Interface * aInterface);

        /// \param aAddr IPv4 address to validate
        /// \exception std::exception
        void ValidateAddress(uint32_t aAddr) const;

    // Internal

        void Prepare(HI::Diagram * aDiagram, ShapeMap * aSubNetMap);

    protected:

        // ===== Object =====================================================
        virtual void ThrowError(const char * aErrorType, int aCode, const char * aMessage) const;

    private:

        uint32_t mAddr;
        uint32_t mMask;

        const Interface * mDHCP_Interface;
        const Router    * mDHCP_Router   ;

    };

}
