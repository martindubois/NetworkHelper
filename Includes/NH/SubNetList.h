
// License https://creativecommons.org/licenses/by-nc-nd/4.0/legalcode
// Product NetworkHelper

/// \author    KMS - Martin Dubois, P.Eng.
/// \copyright Copyright &copy; 2020 KMS. All rights reserved.
/// \file      Includes/NH/SubNetList.h
/// \brief     NH::SubNetList

#pragma once

// ===== C ==================================================================
#include <stdint.h>

// ===== C++ ================================================================
#include <map>

// ===== Import/Includes ====================================================
#include <HI/Diagram.h>

// ===== NHLib ==============================================================
class ShapeMap;

namespace NH
{

    class SubNet;

    /// \brief NH::SubNetList
    class SubNetList
    {

    public:

        SubNetList();

        ~SubNetList();

        /// \param aAddr IPv4 address
        /// \return This mothod returns the SubNet the address is on or NULL.
        const SubNet * Find(uint32_t aAddr) const;

        /// \param aAddr IPv4 subnet address
        /// \param aMask IPV4 subnet mask
        /// \return A pointer to an internal SubNet instance
        /// \exception std::exception
        SubNet * FindOrCreate(uint32_t aAddr, uint32_t aMask);

        /// \param aAddr IPv4 subnet address
        /// \param aMask IPV4 subnet mask
        /// \return A pointer to an internal SubNet instance
        /// \exception std::exception
        SubNet * FindOrCreate(const char * aAddr, const char * aMask);

        void Verify() const;

    // Internal

        void Prepare(HI::Diagram * aDiagram, ShapeMap * aShapeMap);

        unsigned int Verify_Internal() const;

    private:

        SubNetList(const SubNetList &);

        const SubNetList & operator = (const SubNetList &);

        typedef std::list<SubNet *> InternalList;

        InternalList mSubNets;

    };

}
