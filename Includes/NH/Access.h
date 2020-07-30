
// License https://creativecommons.org/licenses/by-nc-nd/4.0/legalcode
// Product NetworkHelper

/// \author    KMS - Martin Dubois, P.Eng.
/// \copyright Copyright &copy; 2020 KMS. All rights reserved.
/// \file      Includes/NH/Access.h
/// \brief     NH::Access

#pragma once

// ===== Windows ============================================================
#undef SetPort

// ===== C ==================================================================
#include <stdint.h>

// ===== Includes ===========================================================
#include <NH/AccessEnd.h>
#include <NH/Object.h>

namespace NH
{

    class SubNet;

    /// \brief NH::Access
    class Access : public Object
    {

    public:

        /// \brief NH::Access::Type
        typedef enum
        {
            TYPE_DENY  ,
            TYPE_PERMIT,

            TYPE_QTY
        }
        Type;

        /// \brief NH::Acces::Protocol
        typedef enum
        {
            PROTOCOL_ICMP,
            PROTOCOL_IP  ,
            PROTOCOL_TCP ,
            PROTOCOL_UDP ,

            PROTOCOL_QTY,

            PROTOCOL_INVALID
        }
        Protocol;

        /// \param aType See NH::Access::Type
        Access(Type aType);

        /// \param aOut          Output buffer
        /// \param aOutSize_byte Output buffer size
        void GetDescription(char * aOut, unsigned int aOutSize_byte) const;

        /// \return See NH::Access::Type
        Type GetType() const;

        void SetEstablished();

        /// \param aProtocol See NH::Access::Protocol
        void SetProtocol(Protocol aProtocol);

        /// \param aProtocol  See NH::Access::Protocol
        /// \param aSrcSubNet The source subnet
        /// \param aSrcPort   The source port
        /// \param aDstAddr   The destination IPv4 address
        /// \param aDstPort   The destination port
        /// \retval false No, this access rule does not match the arguments.
        /// \retval true  Yes, this access rule matches the arguments.
        bool Match(Protocol aProtocol, const SubNet & aSrcSubNet, uint16_t aSrcPort, uint32_t aDstAddr, uint16_t aDstPort) const;

        /// \param aProtocol See NH::Access::Protocol
        /// \retval false No, the access rule is not for the specified protocol.
        /// \retval true  Yes, the access rule is for the specified protocol.
        bool Match(Protocol aProtocol) const;

        void Verify() const;

        AccessEnd mDestination;
        AccessEnd mSource     ;

    // Internal

        unsigned int Verify_Internal() const;

    protected:

        // ===== Object =====================================================
        virtual void DisplayError(const char * aErrorType, int aCode, const char * aMessage) const;

    private:

        struct
        {
            unsigned mEstablished : 1;

            unsigned mReserved0 : 31;
        }
        mFlags;

        Protocol mProtocol;
        Type     mType    ;

    };

}
