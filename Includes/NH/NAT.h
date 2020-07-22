
// License https://creativecommons.org/licenses/by-nc-nd/4.0/legalcode
// Product NetworkHelper

/// \author    KMS - Martin Dubois, P.Eng.
/// \copyright Copyright &copy; 2020 KMS. All rights reserved.
/// \file      Includes/NH/NAT.h
/// \brief     NH::NAT

#pragma once

// ===== Includes ===========================================================
#include <NH/NamedObject.h>

namespace NH
{

    class SubNet;

    /// \brief NH::NAT
    class NAT : public NamedObject
    {

    public:

        NAT();

        /// \param aAddr
        /// \retval false
        /// \retval true
        bool Match(uint32_t aAddr) const;

        /// \param aSubNet
        /// \retval false
        /// \retval true
        bool Match(const SubNet & aSubNet) const;

        /// \param aFirst
        /// \param aLast
        /// \param aNetMask
        /// \exception std::exception
        void Set(uint32_t aFirst, uint32_t aLast, uint32_t aNetMask);

        /// \param aFirst
        /// \param aLast
        /// \param aNetMask
        /// \exception std::exception
        void Set(const char * aFirst, const char * aLast, const char * aNetMask);

    private:

        uint32_t mFirst  ;
        uint32_t mLast   ;
        uint32_t mNetMask;

    };

}
