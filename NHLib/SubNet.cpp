
// Author    KMS - Martin Dubois, P.Eng.
// Copyright (C) 2020 KMS. All rights reserved.
// Product   NetworkHelper
// File      NHLib/SubNet.cpp

// CODE REVIEW 2020-07-23 KMS - Martin Dubois, P.Eng.

// TEST COVERAGE 2020-07-23 KMS - Martin Dubois, P.Eng.

#include "Component.h"

// ===== Includes ===========================================================
#include <NH/SubNet.h>

// ===== NHLib ==============================================================
#include "Errors.h"
#include "IPv4.h"
#include "ShapeMap.h"
#include "Utilities.h"

namespace NH
{

    // Public
    /////////////////////////////////////////////////////////////////////////

    SubNet::SubNet(uint32_t aAddr, uint32_t aMask)
        : Object("SubNet")
        , mAddr(aAddr)
        , mMask(aMask)
        , mDHCP_Interface(NULL)
        , mDHCP_Router   (NULL)
    {
        IPv4_Validate(aAddr, aMask);

        switch (IPv4_GetAddressType(aAddr))
        {
        case IPv4_PRIVATE:
        case IPv4_PUBLIC:
            break;

        default:
            ThrowError(ERROR_CONFIG, __LINE__, "Invalid IPv4 address");
        }
    }

    SubNet::~SubNet()
    {
    }

    uint32_t SubNet::GetAddress() const
    {
        return mAddr;
    }

    void SubNet::GetAddress(char * aOut, unsigned int aOutSize_byte) const
    {
        IPv4_AddressToText(aOut, aOutSize_byte, mAddr);
    }

    // aInterface [---;---]
    bool SubNet::GetDHCP(const Interface * aInterface) const
    {
        assert(NULL != aInterface);

        return (mDHCP_Interface == aInterface);
    }

    void SubNet::GetFullName(char * aOut, unsigned int aOutSize_byte) const
    {
        assert(NULL != aOut         );
        assert(   0 <  aOutSize_byte);

        char lAddr[32];
        char lMask[32];

        GetAddress(lAddr, sizeof(lAddr));
        GetMask   (lMask, sizeof(lMask));

        int lRet = sprintf_s(aOut, aOutSize_byte, "%s/%s", lAddr, lMask);
        assert(            0 <                           lRet );
        assert(aOutSize_byte > static_cast<unsigned int>(lRet));
    }

    void SubNet::GetMask(char * aOut, unsigned int aOutSize_byte) const
    {
        IPv4_AddressToText(aOut, aOutSize_byte, mMask);
    }

    bool SubNet::Is(uint32_t aAddr, uint32_t aMask) const
    {
        return (mAddr == aAddr) && (mMask == aMask);
    }

    bool SubNet::IsPrivate() const
    {
        return IPv4_PRIVATE == IPv4_GetAddressType(mAddr);
    }

    // TODO NH.SubNet.SetDHCP
    //      DHCP set twice

    // aRouter    [-K-;---]
    // aInterface [-KO;---]
    void SubNet::SetDHCP(const Router * aRouter, const Interface * aInterface)
    {
        assert(NULL != aRouter);

        if ((NULL != mDHCP_Router) && (mDHCP_Router != aRouter))
        {
            ThrowError(ERROR_CONFIG, __LINE__, "Two DHCP server for the same SubNet");
        }

        if (NULL != aInterface)
        {
            if ((NULL != mDHCP_Interface) && (mDHCP_Interface != aInterface))
            {
                ThrowError(ERROR_CONFIG, __LINE__, "Two DHCP server for the same SubNet");
            }

            mDHCP_Interface = aInterface;
        }

        mDHCP_Router = aRouter;
    }

    void SubNet::ValidateAddress(uint32_t aAddr) const
    {
        IPv4_Validate(aAddr, mAddr, mMask);
    }

    bool SubNet::VerifyAddress(uint32_t aAddr) const
    {
        return ((mAddr & mMask) == (aAddr & mMask));
    }

    // Internal
    /////////////////////////////////////////////////////////////////////////

    // TODO NH.SubNet
    //      Make private and public subnet look differently

    void SubNet::Prepare(HI::Diagram * aDiagram, ShapeMap * aSubNetMap)
    {
        assert(NULL != aDiagram  );
        assert(NULL != aSubNetMap);

        char lStr[32];

        GetAddress(lStr, sizeof(lStr));

        HI::Shape * lShape = aDiagram->mShapes.AddShape("SubNet", lStr, HI::Shape::TYPE_ELLIPSE);

        GetMask(lStr, sizeof(lStr));

        lShape->SetTitle(lStr);

        aSubNetMap->Add(this, lShape);
    }

}
