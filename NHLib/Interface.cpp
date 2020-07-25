
// Author    KMS - Martin Dubois, P.Eng.
// Copyright (C) 2020 KMS. All rights reserved.
// Product   NetworkHelper
// File      NHLib/Interface.cpp

// CODE REVIEW 2020-07-24 KMS - Martin Dubois, P.Eng

// TEST COVERAGE 2020-07-24 KMS - Martin Dubois, P.Eng

#include "Component.h"

// ===== Import/Includes ====================================================
#include <HI/Line.h>
#include <HI/Link.h>

// ===== Includes ===========================================================
#include <NH/AccessList.h>
#include <NH/NATList.h>
#include <NH/SubNet.h>

#include <NH/Interface.h>

// ===== NHLib ==============================================================
#include "Errors.h"
#include "IPv4.h"
#include "ShapeMap.h"
#include "Utilities.h"

// Constants
/////////////////////////////////////////////////////////////////////////////

#define EOL "\n"

namespace NH
{

    // Public
    /////////////////////////////////////////////////////////////////////////

    Interface::Interface(const char * aName) : NamedObject("Interface")
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
            const char * lName = GetName();

            for (unsigned int i = 0; '\0' != lName[i]; i++)
            {
                char lC = lName[i];

                if ('.' == lC)
                {
                    return true;
                }

                aOut[i] = lC;
            }
        }

        return false;
    }

    const SubNet * Interface::GetSubNet() const { return mSubNet; }

    bool Interface::IsDHCPServer() const
    {
        return (NULL != mSubNet) && mSubNet->GetDHCP(this);
    }

    // aAccessList [---;---]
    bool Interface::Match(const AccessList & aAccessList) const
    {
        for (unsigned int i = 0; i < DIRECTION_QTY; i++)
        {
            if (mAccessLists[i] == &aAccessList)
            {
                return true;
            }
        }

        return false;
    }

    void Interface::SetAccessList(Direction aDirection, const AccessList * aAccessList)
    {
        assert(DIRECTION_QTY >  aDirection );
        assert(NULL          != aAccessList);

        if (NULL != mAccessLists[aDirection])
        {
            ThrowError(ERROR_209);
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
            ThrowError(ERROR_227);
        }

        if (mFlags.mDHCP)
        {
            ThrowError(ERROR_224);
        }

        if (mFlags.mHasSubInterface)
        {
            ThrowError(ERROR_220);
        }

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

    void Interface::SetDHCP()
    {
        if (!mFlags.mDHCP)
        {
            if (mFlags.mHasSubInterface)
            {
                ThrowError(ERROR_219);
            }

            if (0 != mAddr)
            {
                ThrowError(ERROR_223);
            }

            if (IsDHCPServer())
            {
                ThrowError(ERROR_CONFIG, __LINE__, "Do not enable DHCP client on an interface acting as DHCP server");
            }

            mFlags.mDHCP = true;
        }
    }

    void Interface::SetEnable(bool aEnable)
    {
        mFlags.mEnable = aEnable;
    }

    void Interface::SetHasSubInterface()
    {
        if (!mFlags.mHasSubInterface)
        {
            if (mFlags.mDHCP   ) { ThrowError(ERROR_218); }
            if (mFlags.mSub    ) { ThrowError(ERROR_CONFIG, __LINE__, "Do not create sub-interface for a sub-interface"    ); }
            if (mFlags.mVirtual) { ThrowError(ERROR_CONFIG, __LINE__, "Do not create sub-interface for a virtual interface"); }

            if (   0 != mAddr  ) { ThrowError(ERROR_222); }
            if (NULL != mSubNet) { ThrowError(ERROR_CONFIG, __LINE__, "Do not create sub-interface for an interface connected to a subnet"); }

            mFlags.mHasSubInterface = true;
        }
    }

    // TODO NH.Interface.SetNAT_Inside.Error
    //      NAT inside already set

    void Interface::SetNAT_Inside()
    {
        if (!mFlags.mNAT_Inside)
        {
            if (mFlags.mNAT_Outside)
            {
                ThrowError(ERROR_203);
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
                ThrowError(ERROR_204);
            }

            mFlags.mNAT_Outside = true;
        }
    }

    // TODO NH.Interface.SetSubNet.Error
    //      SubNet already set

    void Interface::SetSubNet(const SubNet * aSubNet)
    {
        assert(NULL != aSubNet);

        if (mSubNet != aSubNet)
        {
            if (mFlags.mHasSubInterface)
            {
                ThrowError(ERROR_CONFIG, __LINE__, "Do not connect a subnet to and interface with sub-interfaces");
            }

            if (0 != mAddr)
            {
                aSubNet->ValidateAddress(mAddr);
            }

            if (mFlags.mDHCP && aSubNet->GetDHCP(this))
            {
                ThrowError(ERROR_CONFIG, __LINE__, "Do not enable DHCP client on an interface acting as DHCP server");
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
            ThrowError(ERROR_403);
        }

        if (4096 <= lVLAN)
        {
            ThrowError(ERROR_201);
        }

        if (mVLAN != lVLAN)
        {
            if ( mFlags.mHasSubInterface)
            {
                ThrowError(ERROR_221);
            }

            if (!mFlags.mSub)
            {
                ThrowError(ERROR_202);
            }

            mVLAN = lVLAN;
        }
    }

    void Interface::SetVirtual() { mFlags.mVirtual = true; }
    void Interface::SetWifi   () { mFlags.mWifi    = true; }

    void Interface::Verify() const
    {
        ThrowErrorIfNeeded(ERROR_004, Verify_Internal(NULL));
    }

    // ===== NamedObject ====================================================

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

        NamedObject::SetName(aName);
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
        HI::Shape * lResult = new HI::Shape("Interface", GetName());
        assert(NULL != lResult);

        lResult->SetFillColor(aColor);

        Prepare(aDiagram, lResult, aSubNetMap);

        return lResult;
    }

    // aNATs [--O;R--]
    unsigned int Interface::Verify_Internal(const NATList * aNATs) const
    {
        unsigned int lResult = 0;

        lResult += Verify_Flags();

        if (0 == mAddr)
        {
            lResult += Verify_WithoutAddress(aNATs);
        }
        else
        {
            lResult += Verify_WithAddress(aNATs);
        }

        if (IsDHCPServer() && (NULL != mAccessLists[DIRECTION_IN]))
        {
            assert(NULL != mSubNet);

            if (!mAccessLists[DIRECTION_IN]->IsAllowed(Access::PROTOCOL_UDP, *mSubNet, 68, 0, 67))
            {
                DisplayError(ERROR_205);
                lResult++;
            }
        }

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

    unsigned int Interface::Verify_Flags() const
    {
        unsigned int lResult = 0;

        if (!mFlags.mEnable)
        {
            if (mFlags.mDHCP)
            {
                DisplayError(ERROR_WARNING, __LINE__, "Disabled but configured using DHCP");
                lResult++;
            }

            if (mFlags.mHasSubInterface)
            {
                DisplayError(ERROR_WARNING, __LINE__, "Disabled but has sub-interface");
                lResult++;
            }

            if (mFlags.mNAT_Inside)
            {
                DisplayError(ERROR_WARNING, __LINE__, "Disabled but configured as NAT inside");
                lResult++;
            }

            if (mFlags.mNAT_Outside)
            {
                DisplayError(ERROR_WARNING, __LINE__, "Disabled but configured as NAT outside");
                lResult++;
            }

            if (mFlags.mSub)
            {
                DisplayError(ERROR_WARNING, __LINE__, "Disabled sub-interface");
                lResult++;
            }

            if (mFlags.mVirtual)
            {
                DisplayError(ERROR_WARNING, __LINE__, "Disabled virtual interface");
                lResult++;
            }
        }

        return lResult;
    }

    // aNATs [--O;R--]
    unsigned int Interface::Verify_WithAddress(const NATList * aNATs) const
    {
        unsigned int lResult = 0;

        if (!mFlags.mEnable)
        {
            DisplayError(ERROR_501);
            lResult++;
        }

        if (mFlags.mNAT_Outside && (NULL != aNATs))
        {
            const NAT * lNAT = aNATs->Find(mAddr);
            if (NULL == lNAT)
            {
                DisplayError(ERROR_CONFIG, __LINE__, "Configured as NAT outside but do not match any NAT pool");
                lResult++;
            }
        }

        for (unsigned int i = 0; i < DIRECTION_QTY; i++)
        {
            if (NULL != mAccessLists[i])
            {
                lResult += mAccessLists[i]->Verify_Internal(mAddr, static_cast<NH::Direction>(i));
            }
        }

        return lResult;
    }

    // NOT TESTED NH.Interface.Verify
    //            Interface configured using DHCP with an access list.
    //            Interface used as NAT outside but without static address.

    // aNATs [--O;R--]
    unsigned int Interface::Verify_WithoutAddress(const NATList * aNATs) const
    {
        unsigned int lResult = 0;

        if ((!mFlags.mDHCP) && (!mFlags.mHasSubInterface))
        {
            DisplayError(ERROR_CONFIG, __LINE__, "No IPv4 address set and DHCP client not enabled");
            lResult++;
        }

        if (mFlags.mNAT_Outside && (NULL != mSubNet) && (NULL != aNATs))
        {
            const NAT * lNAT = aNATs->Find(*mSubNet);
            if (NULL != lNAT)
            {
                DisplayError(ERROR_CONFIG, __LINE__, "Configured as NAT outside but do not match any NAT pool");
                lResult++;
            }
        }

        if (NULL != mSubNet)
        {
            for (unsigned int i = 0; i < DIRECTION_QTY; i++)
            {
                if (NULL != mAccessLists[i])
                {
                    lResult += mAccessLists[i]->Verify_Internal(*mSubNet, static_cast<NH::Direction>(i));
                }
            }
        }

        return lResult;
    }

}
