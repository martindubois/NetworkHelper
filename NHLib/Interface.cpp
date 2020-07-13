
// Author    KMS - Martin Dubois, P.Eng.
// Copyright (C) 2020 KMS. All rights reserved.
// Product   NetworkHelper
// File      NHLib/Interface.cpp

// CODE REVIEW 2020-07-13 KMS - Martin Dubois, P.Eng

// TEST COVERAGE 2020-07-13 KMS - Martin Dubois, P.Eng

#include "Component.h"

// ===== Import/Includes ====================================================
#include <HI/Line.h>
#include <HI/Link.h>

// ===== Includes ===========================================================
#include <NH/AccessList.h>
#include <NH/SubNet.h>

#include <NH/Interface.h>

// ===== NHLib ==============================================================
#include "Errors.h"
#include "IPv4.h"
#include "ShapeMap.h"
#include "Utilities.h"

// Constants
/////////////////////////////////////////////////////////////////////////////

#define ELEMENT "Interface"

#define EOL "\n"

namespace NH
{

    // Public
    /////////////////////////////////////////////////////////////////////////

    Interface::Interface(const char * aName)
    {
        Init();

        SetName(aName);
    }

    Interface::~Interface()
    {
    }

    uint32_t Interface::GetAddress() const
    {
        return mAddr;
    }

    void Interface::GetAddress(char * aOut, unsigned int aOutSize_byte) const
    {
        IPv4_AddressToText(aOut, aOutSize_byte, mAddr);
    }

    bool Interface::GetBaseName(char * aOut, unsigned int aOutSize_byte)
    {
        assert(NULL != aOut         );
        assert(   0 <  aOutSize_byte);

        memset(aOut, 0, aOutSize_byte);

        if (!mFlags.mHasSubInterface)
        {
            for (unsigned int i = 0; i < mName.size(); i++)
            {
                char lC = mName[i];

                if ('.' == lC)
                {
                    return true;
                }

                aOut[i] = lC;
            }
        }

        return false;
    }

    const char   * Interface::GetName  () const { return mName.c_str(); }
    const SubNet * Interface::GetSubNet() const { return mSubNet; }

    bool Interface::IsDHCPServer() const
    {
        return (NULL != mSubNet) && mSubNet->GetDHCP(this);
    }

    void Interface::SetAccessList(Direction aDirection, const AccessList * aAccessList)
    {
        assert(DIRECTION_QTY >  aDirection );
        assert(NULL          != aAccessList);

        if (NULL != mAccessLists[aDirection])
        {
            Utl_ThrowError(ERROR_209);
        }

        mAccessLists[aDirection] = aAccessList;
    }

    // TODO NH.Interface.SetAddress
    //      Report error if the address is a multicast or broadcast address.
    //      Report error if the address is already set

    // NOT TESTED NH.Interface.SetAddress
    //            Set address after the subnet

    void Interface::SetAddress(uint32_t aAddr)
    {
        switch (IPv4_GetAddressType(aAddr))
        {
        case IPv4_PRIVATE:
        case IPv4_PUBLIC:
            break;

        default:
            Utl_ThrowError(ERROR_227, ELEMENT " - " ERROR_227_MSG);
        }

        if (mFlags.mDHCP           ) { Utl_ThrowError(ERROR_224); }
        if (mFlags.mHasSubInterface) { Utl_ThrowError(ERROR_220); }

        if (NULL != mSubNet)
        {
            mSubNet->ValidateAddress(aAddr);
        }

        mAddr = aAddr;
    }

    void Interface::SetAddress(const char * aAddr)
    {
        SetAddress(IPv4_TextToAddress(aAddr));
    }

    // TODO NH.Interface.SetDHCP.Error
    //      DHCP already set

    // NOT TESTED NH.Interface.SetDHCP.Error
    //            Same interface as DHCP client and server

    void Interface::SetDHCP()
    {
        if (!mFlags.mDHCP)
        {
            if (mFlags.mHasSubInterface)
            {
                Utl_ThrowError(ERROR_219);
            }

            if (0 != mAddr)
            {
                Utl_ThrowError(ERROR_223);
            }

            if (IsDHCPServer())
            {
                Utl_ThrowError(ERROR_CONFIG, __LINE__, "Do not enable DHCP client on an interface acting as DHCP server");
            }

            mFlags.mDHCP = true;
        }
    }

    // NOT TESTED NH.Interface.SetHasSubInterface.Error
    //            Sub-sub-interface, Sub-interface connected to a subnet

    void Interface::SetHasSubInterface()
    {
        if (!mFlags.mHasSubInterface)
        {
            if (mFlags.mDHCP   ) { Utl_ThrowError(ERROR_218); }
            if (mFlags.mSub    ) { Utl_ThrowError(ERROR_CONFIG, __LINE__, "Do not create sub-interface for a sub-interface"                      ); }
            if (mFlags.mVirtual) { Utl_ThrowError(ERROR_CONFIG, __LINE__, "Do not create sub-interface for a virtual interface"                  ); }

            if (   0 != mAddr  ) { Utl_ThrowError(ERROR_222); }
            if (NULL != mSubNet) { Utl_ThrowError(ERROR_CONFIG, __LINE__, "Do not create sub-interface for an interface connected to a subnet"         ); }

            mFlags.mHasSubInterface = true;
        }
    }

    // TODO NH.Interface.SetName.Error
    //      Name already set

    void Interface::SetName(const char * aName)
    {
        assert(NULL != aName);

        if (NULL != strchr(aName, '.'))
        {
            mFlags.mSub     = true;
            mFlags.mVirtual = true;
        }

        mName = aName;
    }

    // TODO NH.Interface.SetNAT_Inside.Error
    //      NAT inside already set

    void Interface::SetNAT_Inside()
    {
        if (!mFlags.mNAT_Inside)
        {
            if (mFlags.mNAT_Outside)
            {
                Utl_ThrowError(ERROR_203);
            }

            mFlags.mNAT_Inside = true;
        }
    }

    // TODO NH.Interface.SetNAT_Inside.Error
    //      NAT outside already set

    void Interface::SetNAT_Outside()
    {
        if (!mFlags.mNAT_Outside)
        {
            if (mFlags.mNAT_Inside)
            {
                Utl_ThrowError(ERROR_204);
            }

            mFlags.mNAT_Outside = true;
        }
    }

    // TODO NH.Interface.SetSubNet.Error
    //      SubNet already set

    // NOT TESTED NH.Interface.SetSubNet.Error
    //            DHCP client and server on the same interface

    void Interface::SetSubNet(const SubNet * aSubNet)
    {
        assert(NULL != aSubNet);

        if (mSubNet != aSubNet)
        {
            if (mFlags.mHasSubInterface)
            {
                Utl_ThrowError(ERROR_CONFIG, __LINE__, "Do not connect a subnet to and interface with sub-interfaces");
            }

            if (0 != mAddr)
            {
                aSubNet->ValidateAddress(mAddr);
            }

            if (mFlags.mDHCP && aSubNet->GetDHCP(this))
            {
                Utl_ThrowError(ERROR_CONFIG, __LINE__, "Do not enable DHCP client on an interface acting as DHCP server");
            }

            mSubNet = aSubNet;
        }
    }

    // TODO NH.Interface.SetVLAN.Error
    //      VLAN already set

    void Interface::SetVLAN(const char * aVLAN)
    {
        assert(NULL != aVLAN);

        unsigned int lVLAN;

        int lRet = sscanf_s(aVLAN, "%u", &lVLAN);
        if (1 != lRet)
        {
            Utl_ThrowError(ERROR_403);
        }

        if (4096 <= lVLAN)
        {
            Utl_ThrowError(ERROR_201);
        }

        if (mVLAN != lVLAN)
        {
            if ( mFlags.mHasSubInterface) { Utl_ThrowError(ERROR_221); }
            if (!mFlags.mSub            ) { Utl_ThrowError(ERROR_202); }

            mVLAN = lVLAN;
        }
    }

    void Interface::SetVirtual() { mFlags.mVirtual = true; }
    void Interface::SetWifi   () { mFlags.mWifi    = true; }

    // NOT TESTED NH.Interface.Verify.Error
    //            Not configured interface

    // NOT TESTED NH.Interface.Verify
    //            Interface configured using DHCP with an access list.

    void Interface::Verify() const
    {
        unsigned int lErrorCount = 0;
        char         lMessage[128];
        int          lRet;

        if ((0 == mAddr) && (!mFlags.mDHCP) && (!mFlags.mHasSubInterface))
        {
            lRet = sprintf_s(lMessage, ELEMENT " %s - No IPv4 address set and DHCP client not enabled", mName.c_str());
            assert(               0 < lRet);
            assert(sizeof(lMessage) > lRet);

            Utl_DisplayError(ERROR_CONFIG, __LINE__, lMessage);
            lErrorCount++;
        }

        for(unsigned int i = 0; i < DIRECTION_QTY; i ++)
        {
            if (NULL != mAccessLists[i])
            {
                try
                {
                    if (0 != mAddr)
                    {
                        mAccessLists[i]->Verify(mAddr, static_cast<NH::Direction>(i));
                    }
                    else if (NULL != mSubNet)
                    {
                        mAccessLists[i]->Verify(*mSubNet, static_cast<NH::Direction>(i));
                    }
                }
                catch (std::exception eE)
                {
                    Utl_DisplayError(__LINE__, eE);
                    lErrorCount++;
                }
            }
        }

        if (IsDHCPServer() && (NULL != mAccessLists[DIRECTION_IN]))
        {
            assert(NULL != mSubNet);

            if (!mAccessLists[DIRECTION_IN]->IsAllowed(Access::PROTOCOL_UDP, *mSubNet, 68, 0, 67))
            {
                lRet = sprintf_s(lMessage, ELEMENT " %s - " ERROR_205_MSG, mName.c_str());
                assert(               0 < lRet);
                assert(sizeof(lMessage) > lRet);

                Utl_DisplayError(ERROR_205, lMessage);
                lErrorCount++;
            }
        }

        Utl_ThrowErrorIfNeeded(ERROR_004, ELEMENT, mName.c_str(), lErrorCount);
    }

    // Internal
    /////////////////////////////////////////////////////////////////////////

    void Interface::PrepareLink(HI::Diagram * aDiagram, HI::Shape * aShape, const HI::Shape * aParent)
    {
        assert(NULL != aDiagram);
        assert(NULL != aShape  );
        assert(NULL != aParent );

        HI::Link * lLink = aDiagram->mLinks.AddLink(aParent, aShape);
        assert(NULL != lLink);

        lLink->SetWeightFactor(2);
        lLink->SetWidth       (3);

        if (mFlags.mVirtual)
        {
            lLink->SetDashArray("9, 3");
        }
    }

    HI::Shape * Interface::PrepareShape(HI::Diagram * aDiagram, HI::CSS_Color aColor, const ShapeMap & aSubNetMap)
    {
        HI::Shape * lResult = new HI::Shape("Interface", mName.c_str());
        assert(NULL != lResult);

        lResult->SetFillColor(aColor);

        Prepare(aDiagram, lResult, aSubNetMap);

        return lResult;
    }

    // Protected
    /////////////////////////////////////////////////////////////////////////

    // TODO NH.Interface
    //      Use images

    void Interface::Prepare(HI::Diagram * aDiagram, HI::Shape * aShape, const ShapeMap & aSubNetMap)
    {
        assert(NULL != aDiagram);
        assert(NULL != aShape  );

        aShape->SetAutoDelete();

        Prepare_Title(aShape);

        if (NULL != mSubNet)
        {
            Prepare_Link_SubNet(aShape, aDiagram, aSubNetMap);
        }

        aDiagram->mShapes.AddShape(aShape);
    }

    // Private
    /////////////////////////////////////////////////////////////////////////

    void Interface::Init()
    {
        mAddr   =    0;
        mSubNet = NULL;
        mVLAN   =    0;

        memset(&mFlags      , 0, sizeof(mFlags      ));
        memset(&mAccessLists, 0, sizeof(mAccessLists));
    }

    void Interface::Prepare_Link_SubNet(HI::Shape * aShape, HI::Diagram * aDiagram, const ShapeMap & aSubNetMap)
    {
        assert(NULL != aShape  );
        assert(NULL != aDiagram);

        assert(NULL != mSubNet);

        HI::Link * lLink = aDiagram->mLinks.AddLink(aShape, aSubNetMap.Get(mSubNet));
        assert(NULL != lLink);

        if (mFlags.mWifi)
        {
            lLink->SetDashArray("3, 3");
        }
        else
        {
            lLink->SetColor(HI::COLOR_BLUE);
            lLink->SetWidth(2);

            if (mFlags.mVirtual)
            {
                lLink->SetDashArray("6, 2");
            }
        }
    }

    // NOT TESTED NH.Inteface.Title.Warning

    void Interface::Prepare_Title(HI::Shape * aShape)
    {
        assert(NULL != aShape);

        char        lStr[32];
        std::string lTitle;

        GetAddress(lStr, sizeof(lStr));

        if (mFlags.mDHCP)
        {
            assert(0 == mAddr);

            lTitle += "DHCP Client";
        }
        else
        {
            if (0 == mAddr)
            {
                if (mFlags.mNAT_Inside || mFlags.mNAT_Outside || mFlags.mSub || mFlags.mVirtual || mFlags.mWifi || (NULL != mSubNet) || (0 != mVLAN))
                {
                    lTitle += "No IPv4 address <== WARNING!";
                }
            }
            else
            {
                lTitle += lStr;
            }
        }

        if (0 != mVLAN)
        {
            int lRet = sprintf_s(lStr, EOL "VLAN %u", mVLAN); lTitle += lStr;
            assert(           0 < lRet);
            assert(sizeof(lStr) > lRet);
        }

        if (IsDHCPServer())
        {
            assert(!mFlags.mDHCP); lTitle += EOL "DHCP Server";
        }

        if (mFlags.mNAT_Inside)
        {
            assert(!mFlags.mNAT_Outside);

            lTitle += EOL "NAT Indisde";
        }
        else
        {
            if (mFlags.mNAT_Outside) { assert(!mFlags.mNAT_Inside); lTitle += EOL "NAT Outside"; }
        }

        aShape->SetTitle(lTitle.c_str());
    }

}
