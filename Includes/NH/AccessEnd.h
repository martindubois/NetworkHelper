
// License https://creativecommons.org/licenses/by-nc-nd/4.0/legalcode
// Product NetworkHelper

/// \author    KMS - Martin Dubois, P.Eng.
/// \copyright Copyright &copy; 2020 KMS. All rights reserved.
/// \file      Includes/NH/Access.h
/// \brief     NH::AccessEnd

#pragma once

// ===== Windows ============================================================
#undef SetPort

// ===== C ==================================================================
#include <stdint.h>

// ===== Includes ===========================================================
#include <NH/Object.h>

namespace NH
{

    class SubNet;

    /// \brief NH::AccessEnd
    class AccessEnd : public Object
    {

    public:

        typedef enum
        {
            FILTER_ANY   ,
            FILTER_HOST  ,
            FILTER_SUBNET,

            FILTER_QTY,

            FILTER_INVALID
        }
        Filter;

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

        AccessEnd();

        /// \param aOut
        /// \param aOutSize_byte
        void GetDescription(char * aOut, unsigned int aOutSize_byte) const;

        /// \return See Filter
        Filter GetFilter() const;

        /// \return This method returns the host address
        uint32_t GetHost() const;

        /// \param aOut
        /// \param aOutSize_byte
        void GetHost(char * aOut, unsigned int aOutSize_byte) const;

        /// \return See Operator
        Operator GetPortOperator() const;

        /// \return This method returns the subnet
        const SubNet * GetSubNet() const;

        /// \retval false
        /// \retval true
        bool IsInitialized() const;

        void SetAny();

        /// \param aHost
        void SetHost(uint32_t aHost);

        /// \param aHost
        void SetHost(const char * aHost);

        /// \param aOp See Operator
        void SetPort(Operator aOp);

        /// \param aOp    See Operator
        /// \param aPortA
        void SetPort(Operator aOp, uint16_t aPortA);

        /// \param aOp    See Operator
        /// \param aPortA
        /// \param aPortB
        void SetPort(Operator aOp, uint16_t aPortA, uint16_t aPortB);

        /// \param aOp    See Operator
        /// \param aPortA
        void SetPort(Operator aOp, const char * aPortA);

        /// \param aOp    See Operator
        /// \param aPortA
        /// \param aPortB
        void SetPort(Operator aOp, const char * aPortA, const char * aPortB);

        /// \param aSubNet
        void SetSubNet(const SubNet * aSubNet);

        /// \param aSubNet
        /// \param aPort
        /// \retval false
        /// \retval true
        bool Match(const SubNet & aSubNet, uint16_t aPort) const;

        /// \param aAddress
        /// \param aPort
        /// \retval false
        /// \retval true
        bool Match(uint32_t aAddress, uint16_t aPort) const;

        /// \param aSubNet
        /// \retval false
        /// \retval true
        bool Match(const SubNet & aSubNet) const;

        /// \param aAddress
        /// \retval false
        /// \retval true
        bool Match(uint32_t aAddress) const;

        /// \param aPort
        /// \retval false
        /// \retval true
        bool Match(uint16_t aPort) const;

        void Verify() const;

        void VerifyPrivate() const;

    // Internal

        unsigned int Verify_Internal() const;

    private:

        Filter         mFilter ;
        uint32_t       mHost   ;
        uint16_t       mPort_A ;
        uint16_t       mPort_B ;
        Operator       mPort_Op;
        const SubNet * mSubNet ;

    };

}
