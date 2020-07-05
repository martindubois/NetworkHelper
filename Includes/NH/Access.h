
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

namespace NH
{

    class SubNet;

    /// \brief NH::Access
    class Access
    {

    public:

        /// \brief NH::Access::End
        class End
        {

        public:

            /// \brief NH::Access::End::Operator
            typedef enum
            {
                OPERATOR_ANY  ,
                OPERATOR_EQ   ,
                OPERATOR_GT   ,
                OPERATOR_LT   ,
                OPERATOR_NEQ  ,
                OPERATOR_RANGE,

                OPERATOR_QTY,

                OPERATOR_INVALID
            }
            Operator;

            End();

            void SetAny();

            /// \param aHost
            void SetHost(uint32_t aHost);

            /// \param aHost
            void SetHost(const char * aHost);

            /// \param aOp    See Operator
            /// \param aPortA
            /// \param aPortB
            void SetPort(Operator aOp, uint16_t aPortA = 0, uint16_t aPortB = 0);

            /// \param aOp    See Operator
            /// \param aPortA
            /// \param aPortB
            void SetPort(Operator aOp, const char * aPortA = NULL, const char * aPortB = NULL);

            /// \param aSubNet
            void SetSubNet(const SubNet * aSubNet);

        private:

            struct
            {
                unsigned mAny : 1;

                unsigned mReserved0 : 31;
            }
            mFlags;

            uint32_t       mHost   ;
            uint16_t       mPort_A ;
            uint16_t       mPort_B ;
            Operator       mPort_Op;
            const SubNet * mSubNet ;

        };

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

        void SetEstablished();

        /// \param aProtocol See Protocol
        void SetProtocol(Protocol aProtocol);

        void Verify();

        End mDestination;
        End mSource     ;

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
