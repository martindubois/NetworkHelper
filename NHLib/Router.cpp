
// Author     KMS - Martin Dubois, P.Eng.
// Copyrights (C) 2020 KMS. All rights reserved.
// Product    NetworkHelper
// File       NHLib/Router.cpp

// CODE REVIEW 2020-08-05 KMS - Martin Dubois, P.Eng.

// TEST COVERAGE 2020-08-04 KMS - Martin Dubois, P.Eng.

#include "Component.h"

// ===== Includes ===========================================================
#include <NH/NAT.h>
#include <NH/Route.h>
#include <NH/SubNet.h>

#include <NH/Router.h>

// ===== NHLib ==============================================================
#include "CheckList.h"
#include "Checks.h"
#include "Errors.h"
#include "IPv4.h"
#include "Utilities.h"

// Constants
/////////////////////////////////////////////////////////////////////////////

#define NAME_DEFAULT "Router"

// Static function declaration
/////////////////////////////////////////////////////////////////////////////

static void ExtractFileName(char * aOut, unsigned int aOutSize_byte, const char * aIn);

namespace NH
{

    // Public
    ////////////////////////////////////////////////////////////////////////

    Router::Router() : NamedObject("Router"), mCheckList(new CheckList), mSubNets(NULL)
    {
        assert(NULL != mCheckList);

        mFlags.mIpRouting = false;

        SetName(NAME_DEFAULT);
    }

    Router::~Router()
    {
        assert(NULL != mCheckList);

        delete mCheckList;
    }

    void Router::AddRoute(const SubNet * aSubNet, uint32_t aNextRouter)
    {
        assert(NULL != mCheckList);

        mRoutes.push_back(Route(aSubNet, aNextRouter));

        mCheckList->Add(new Check_Reach(ERROR_243, aNextRouter));
    }

    void Router::AddRoute(const SubNet * aSubNet, const char * aNextRouter)
    {
        AddRoute(aSubNet, IPv4_TextToAddress(aNextRouter));
    }

    bool Router::CanReach(uint32_t aAddr) const
    {
        if (mInterfaces.CanReach(aAddr))
        {
            return true;
        }

        for (RouteList::const_iterator lIt = mRoutes.begin(); lIt != mRoutes.end(); lIt++)
        {
            if (lIt->GetSubNet()->Match(aAddr))
            {
                return true;
            }
        }

        return false;
    }

    /// aSubNet [---;---]
    bool Router::CanReach(const SubNet & aSubNet) const
    {
        if (NULL != mInterfaces.Find(&aSubNet))
        {
            return true;
        }

        for (RouteList::const_iterator lIt = mRoutes.begin(); lIt != mRoutes.end(); lIt++)
        {
            if (lIt->Match(aSubNet))
            {
                return true;
            }
        }

        return false;
    }

    SubNetList * Router::GetSubNetList()
    {
        assert(NULL != mSubNets);

        return mSubNets;
    }

    void Router::SetIpRouting()
    {
        if (mFlags.mIpRouting)
        {
            ThrowError(ERROR_248);
        }

        mFlags.mIpRouting = true;
    }

    void Router::SetSubNetList(SubNetList * aSubNets)
    {
        assert(NULL != aSubNets);

        mSubNets = aSubNets;
    }

    // NOT TESTED NH.Router.RetrieveInfo
    //            Retrieve Router information through Telenet connection

    void Router::RetrieveInfo(InfoType aType, const char * aData, unsigned int aFlags)
    {
        assert(NULL != aData);

        if (0 == strcmp(NAME_DEFAULT, GetName()))
        {
            char lName[64];

            memset(&lName, 0, sizeof(lName));

            switch (aType)
            {
            case INFO_COM_PORT:
                int lRet;
                
                lRet = sprintf_s(lName, "On %s", aData);
                assert(            0 < lRet);
                assert(sizeof(lName) > lRet);
                break;

            case INFO_CONFIG_FILE:
            case INFO_LOG_FILE   :
                ExtractFileName(lName, sizeof(lName), aData);
                break;

            case INFO_TELNET:
                strncpy_s(lName, aData, sizeof(lName) - 1);
                break;

            default:
                ThrowError(ERROR_CALLER, __LINE__, "Invalid information type");
            }

            SetName(lName);
        }
    }

    // NOT TESTED NH.Router.Verify

    void Router::Verify() const
    {
        ThrowErrorIfNeeded(ERROR_002, Verify_Internal());
    }

    // Internal
    /////////////////////////////////////////////////////////////////////////

    const Router::RouteList * Router::GetRoutes() const
    {
        return &mRoutes;
    }

    // TODO NH.Router
    //      Use images

    void Router::Prepare(HI::Diagram * aDiagram, HI::CSS_Color aColor, const ShapeMap & aSubNetMap)
    {
        assert(NULL != aDiagram);

        HI::Shape * lShape = aDiagram->mShapes.AddShape("Router", GetName());
        assert(NULL != lShape);

        lShape->SetFillColor (aColor);

        unsigned int lIndex = 0;
        std::string  lTitle;

        for (RouteList::const_iterator lIt = mRoutes.begin(); lIt != mRoutes.end(); lIt++)
        {
            char lDesc[64];

            lIt->GetDescription(lDesc, sizeof(lDesc));

            if (0 != lIndex)
            {
                lTitle += "\n";
            }

            lIndex++;
            lTitle += lDesc;
        }

        lShape->SetTitle(lTitle.c_str());

        mInterfaces.Prepare(aDiagram, aColor, lShape, aSubNetMap);
    }

    // TODO NH.Router.Verify
    //      Does NAT pool access list match the nat inside interface?

    unsigned int Router::Verify_Internal() const
    {
        assert(NULL != mCheckList);

        unsigned int lResult = 0;

        lResult += mCheckList->Verify(*this);

        lResult += mAccessLists.Verify_Internal(&mInterfaces, &mNATs);
        lResult += mDHCPs      .Verify_Internal();
        lResult += mInterfaces .Verify_Internal(              &mNATs);
        lResult += mNATs       .Verify_Internal();

        lResult += Verify_Routes();

        if (!mFlags.mIpRouting)
        {
            if (1 < mInterfaces.GetCount())
            {
                DisplayError(ERROR_229);
                lResult++;
            }

            if (!mRoutes.empty())
            {
                DisplayError(ERROR_242);
                lResult++;
            }
        }

        return lResult;
    }

    // Privates
    /////////////////////////////////////////////////////////////////////////

    unsigned int Router::Verify_Routes() const
    {
        unsigned int lResult = 0;

        for (RouteList::const_iterator lIt = mRoutes.begin(); lIt != mRoutes.end(); lIt++)
        {
            char lMessage[128];
            int  lRet;
            char lSubNet[64];

            const SubNet * lDestSubNet = lIt->GetSubNet();
            assert(NULL != lDestSubNet);

            if (NULL != mInterfaces.Find(lDestSubNet))
            {
                lDestSubNet->GetFullName(lSubNet, sizeof(lSubNet));

                lRet = sprintf_s(lMessage, ERROR_255_FMT, lSubNet);
                assert(               0 < lRet);
                assert(sizeof(lMessage) > lRet);

                DisplayError(ERROR_255, lMessage);
                lResult++;
            }
        }

        return lResult;
    }

}

// Static functions
/////////////////////////////////////////////////////////////////////////////

void ExtractFileName(char * aOut, unsigned int aOutSize_byte, const char * aIn)
{
    assert(NULL != aOut         );
    assert(   0 <  aOutSize_byte);
    assert(NULL != aIn          );

    memset(aOut, 0, aOutSize_byte);

    unsigned int lInLen = static_cast<unsigned int>(strlen(aIn));
    assert(0 < lInLen);

    unsigned int lBegin = lInLen - 1;
    unsigned int lEnd   = lInLen;

    while (0 < lBegin)
    {
        switch (aIn[lBegin])
        {
        case '.': lEnd = lBegin; break;

        case '/' :
        case '\\':
            lBegin++;
            goto End;
        }

        lBegin--;
    }

End:
    assert(aOutSize_byte > lEnd - lBegin);
    memcpy(aOut, aIn + lBegin, lEnd - lBegin);
}
