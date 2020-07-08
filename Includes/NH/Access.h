
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

namespace NH
{

    class SubNet;

    /// \brief NH::Access
    class Access
    {

    public:

        typedef enum
        {
            TYPE_DENY  ,
            TYPE_PERMIT,

            TYPE_QTY
        }
        Type;

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

        Access(Type aType);

        /// \param aOut
        /// \param aOutSize_byte
        void GetDescription(char * aOut, unsigned int aOutSize_byte) const;

        /// \return See Type
        Type GetType() const;

        void SetEstablished();

        /// \param aProtocol See Protocol
        void SetProtocol(Protocol aProtocol);

        /// \param aProtocol  See Protocol
        /// \param aSrcSubNet
        /// \param aSrcPort
        /// \param aDstAddr
        /// \param aDstPort
        /// \retval false
        /// \retval true
        bool Match(Protocol aProtocol, const SubNet & aSrcSubNet, uint16_t aSrcPort, uint32_t aDstAddr, uint16_t aDstPort) const;

        /// \param aProtocol  See Protocol
        /// \retval false
        /// \retval true
        bool Match(Protocol aProtocol) const;

        void Verify() const;

        AccessEnd mDestination;
        AccessEnd mSource     ;

    private:

        void Error(int aCode, const char * aMessage) const;

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
