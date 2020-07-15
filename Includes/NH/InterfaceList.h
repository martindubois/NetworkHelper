
// License https://creativecommons.org/licenses/by-nc-nd/4.0/legalcode
// Product NetworkHelper

/// \author    KMS - Martin Dubois, P.Eng.
/// \copyright Copyright &copy; 2020. All rights reserved.
/// \file      Includes/NH/InterfaceList.h
/// \brief     NH::InterfaceList

#pragma once

// ===== C++ ================================================================
#include <map>
#include <string>

// ===== Import/Includes ====================================================
#include <HI/Diagram.h>

// ===== Includes ===========================================================
#include <NH/Object.h>

// ===== NHLib ==============================================================
class ShapeMap;

namespace NH
{

    class Interface;
    class SubNet   ;

    /// \brief NH::InterfaceList
    class InterfaceList : public Object
    {

    public:

        InterfaceList();

        ~InterfaceList();

        /// \param aAddr
        /// \retval false Cannot reach
        /// \retval true  Can reach
        bool CanReach(uint32_t aAddr) const;

        /// \param aSubNet The SubNet attached to the Interface
        /// \return A pointer to an internal instance
        const Interface * Find(const SubNet * aSubNet) const;

        /// \param aAddr The address of the Interface
        /// \return A pointer to an internal instance
        const Interface * Find(uint32_t aAddr) const;

        /// \param aName The name of the interface to retrieve or to create
        /// \return A pointer to an internal instance
        Interface * FindOrCreate(const char * aName);

        /// \return This method returns the number of interface in the list.
        unsigned int GetCount() const;

        /// \param aName
        /// \return This methode return the zero base index of the interface.
        unsigned int GetIndex(const char * aName) const;

        /// \param aName The name of interface to retrieve
        /// \return This method returns a pointer to an Interface instance or NULL.
        Interface * GetInterface(const char * aName);

        void Verify() const;

    // Internal

        void Prepare(HI::Diagram * aDiagram, HI::CSS_Color aColor, const HI::Shape * aParent, const ShapeMap & aSubNetMap);

        unsigned int Verify_Internal() const;

    private:

        InterfaceList(const InterfaceList &);

        const InterfaceList & operator = (const InterfaceList &);

        void Add(Interface * aInterface);

        typedef std::map<std::string, Interface *> InterfaceMap;

        InterfaceMap mInterfaces;

    };

}
