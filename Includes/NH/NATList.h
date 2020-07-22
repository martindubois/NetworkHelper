
// License https://creativecommons.org/licenses/by-nc-nd/4.0/legalcode
// Product NetworkHelper

/// \author    KMS - Martin Dubois, P.Eng.
/// \copyright Copyright &copy; 2020 KMS. All rights reserved.
/// \file      Includes/NH/NAT.h
/// \brief     NH::NATList

#pragma once

// ===== C++ ================================================================
#include <list>

namespace NH
{

    class NAT;
    class SubNet;

    /// \brief NH::NATList
    class NATList
    {

    public:

        NATList();

        ~NATList();

        /// \param aName
        /// \param aFirst
        /// \param aLast
        /// \param aNetMask
        /// \exception std::exception
        void Add(const char * aName, uint32_t aFirst, uint32_t aLast, uint32_t aNetMask);

        /// \param aName
        /// \param aFirst
        /// \param aLast
        /// \param aNetMask
        /// \exception std::exception
        void Add(const char * aName, const char * aFirst, const char * aLast, const char * aNetMask);

        /// \param aAddr
        /// \return This method returns a pointer to an internal instance or NULL.
        const NAT * Find(uint32_t aAddr) const;

        /// \param aSubNet 
        /// \return This method returns a pointer to an internal instance or NULL.
        const NAT * Find(const SubNet & aSubNet) const;

    private:

        NATList(const NATList &);

        const NATList & operator = (const NATList &);

        typedef std::list<NAT *> InternalList;

        InternalList mNATs;

    };

}
