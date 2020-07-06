
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

// ===== NHLib ==============================================================
class ShapeMap;

namespace NH
{

    class AccessList;
    class SubNet;

    /// \brief NH::Interface
    class Interface
    {

    public:

        typedef enum
        {
            DIRECTION_IN ,
            DIRECTION_OUT,

            DIRECTION_QTY
        }
        Direction;

        /// \param aName The interface's name
        Interface(const char * aName);

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

        /// \return This method returns the address of an internal buffer.
        const char * GetName() const;

        /// \return This methode returns the pointer to an internal stored into the SubNetList instance associated to the Network.
        const SubNet * GetSubNet() const;

        /// \aDirection  See Direction
        /// \aAccessList
        void SetAccessList(Direction aDirection, const AccessList * aAccessList);

        /// \param aAddr The address
        void SetAddress(uint32_t aAddr);

        /// \param aAddr The address
        void SetAddress(const char * aAddr);

        void SetDHCP           ();
        void SetHasSubInterface();

        /// \param aName The name
        void SetName(const char * aName);

        void SetNAT_Inside ();
        void SetNAT_Outside();

        /// \param aSubNet The SubNet
        void SetSubNet(const SubNet * aSubNet);

        /// \param aVLAN
        void SetVLAN(const char * aVLAN);

        void SetVirtual();

        void SetWifi();

        void Verify() const;

        // ===== HI::Shape ==================================================

        ~Interface();

    // Internal

        void        PrepareLink (HI::Diagram * aDiagram, HI::Shape * aShape, const HI::Shape * aParent);
        HI::Shape * PrepareShape(HI::Diagram * aDiagram, HI::CSS_Color aColor, const ShapeMap & aSubNetMap);

    protected:

        void Prepare(HI::Diagram * aDiagram, HI::Shape * aShape, const ShapeMap & aSubNetMap);

    private:

        Interface(const Interface &);

        const Interface & operator = (const Interface &);

        void Init();

        void Prepare_Link_SubNet(HI::Shape * aShape, HI::Diagram * aDiagram, const ShapeMap & aSubNetMap);
        void Prepare_Title      (HI::Shape * aShape);

        struct
        {
            unsigned mDHCP            : 1;
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
        std::string    mName  ;
        const SubNet * mSubNet;
        uint16_t       mVLAN  ;

        const NH::AccessList * mAccessLists[DIRECTION_QTY];

    };

}
