
// License https://creativecommons.org/licenses/by-nc-nd/4.0/legalcode
// Product NetworkHelper

/// \author    KMS - Martin Dubois, P.Eng.
/// \copyright Copyright &copy; 2020. All rights reserved.
/// \file      Includes/NH/Interface.h
/// \brief     NH::Interface

#pragma once

// ===== C++ ================================================================
#include <string>

// ===== Import/Includes ====================================================
#include <HI/Diagram.h>
#include <HI/Shape.h>

// ===== Includes ===========================================================
#include <NH/NamedObject.h>
#include <NH/Types.h>

// ===== NHLib ==============================================================
class CheckList;
class ShapeMap;

namespace NH
{

    class AccessList;
    class NATList;
    class SubNet;

    /// \brief NH::Interface
    class Interface : public NamedObject
    {

    public:

        /// \param aName The interface's name
        Interface(const char * aName);

        ~Interface();

        /// \return This method return the configured address
        uint32_t GetAddress() const;

        /// \param aOut
        /// \param aOutSize_byte
        void GetAddress(char * aOut, unsigned int aOutSize_byte) const;

        /// \param aOut
        /// \param aOutSize_byte
        /// \retval false This interface is not a sub-interface
        /// \retval true  This interface is a sub-interface
        bool GetBaseName(char * aOut, unsigned int aOutSize_byte);

        /// \retval No, the DHCP client is not enabled.
        /// \retval Yes, the DHCP client is enabled.
        bool GetDHCP() const;

        /// \return This methode returns the pointer to an internal stored into the SubNetList instance associated to the Network.
        const SubNet * GetSubNet() const;

        /// \retval false
        /// \retval true
        bool IsDHCPServer() const;

        /// \retval false
        /// \retval true
        bool IsEnabled() const;

        /// \retval false
        /// \retval true
        bool IsPrivate() const;

        /// \retval false
        /// \retval true
        bool IsPublic() const;

        /// \param aAccessList
        /// \retval false
        /// \retval true
        bool Match(const AccessList & aAccessList) const;

        /// \param aDirection  See Direction
        /// \param aAccessList
        void SetAccessList(Direction aDirection, const AccessList * aAccessList);

        /// \param aAddr The address
        void SetAddress(uint32_t aAddr);

        /// \param aAddr The address
        void SetAddress(const char * aAddr);

        void SetDHCP();

        /// \param aEnable
        void SetEnable(bool aEnable = true);

        void SetHasSubInterface();

        void SetNAT_Inside ();
        void SetNAT_Outside();

        /// \param aSubNet The SubNet
        void SetSubNet(const SubNet * aSubNet);

        /// \param aVLAN
        void SetVLAN(const char * aVLAN);

        void SetVirtual();

        void SetWifi();

        void Verify() const;

        // ===== NamedObject ================================================

        virtual void SetName(const char * aName);

    // Internal

        void        PrepareLink (HI::Diagram * aDiagram, HI::Shape * aShape, const HI::Shape * aParent);
        HI::Shape * PrepareShape(HI::Diagram * aDiagram, HI::CSS_Color aColor, const ShapeMap & aSubNetMap);

        unsigned int Verify_Internal(const NATList * aNATs) const;

        CheckList * mCheckList;

    protected:

        void Prepare(HI::Diagram * aDiagram, HI::Shape * aShape, const ShapeMap & aSubNetMap);

    private:

        Interface(const Interface &);

        const Interface & operator = (const Interface &);

        void Init();

        void Prepare_Link_SubNet(HI::Shape * aShape, HI::Diagram * aDiagram, const ShapeMap & aSubNetMap);
        void Prepare_Title      (HI::Shape * aShape);

        unsigned int Verify_Flags() const;
        unsigned int Verify_WithAddress   (const NATList * aNATs) const;
        unsigned int Verify_WithoutAddress(const NATList * aNATs) const;

        struct
        {
            unsigned mDHCP            : 1;
            unsigned mEnable          : 1;
            unsigned mHasSubInterface : 1;
            unsigned mNAT_Inside      : 1;
            unsigned mNAT_Outside     : 1;
            unsigned mSub             : 1;
            unsigned mVirtual         : 1;
            unsigned mWifi            : 1;

            unsigned mReserved0 : 25;
        }
        mFlags;

        uint32_t       mAddr  ;
        const SubNet * mSubNet;
        uint16_t       mVLAN  ;

        const NH::AccessList * mAccessLists[DIRECTION_QTY];

    };

}
