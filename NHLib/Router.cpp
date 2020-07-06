
// Author     KMS - Martin Dubois, P.Eng.
// Copyrights (C) 2020 KMS. All rights reserved.
// Product    NetworkHelper
// File       NHLib/Router.cpp

// CODE REVIEW 2020-06-30 KMS - Martin Dubois, P.Eng.

// TEST COVERAGE 2020-06-30 KMS - Martin Dubois, P.Eng.

// ===== C ==================================================================
#include <assert.h>

// ===== Includes ===========================================================
#include <NH/Route.h>
#include <NH/SubNet.h>

#include <NH/Router.h>

// ===== NHLib ==============================================================
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

    const unsigned int Router::FLAG_NO_ECHO = 0x00000001;

    Router::Router() : mName(NAME_DEFAULT)
    {
    }

    Router::~Router()
    {
    }

    void Router::AddRoute(const SubNet * aSubNet, const char * aAddr)
    {
        mRoutes.push_back(Route(aSubNet, aAddr));
    }

    bool Router::CanReach(uint32_t aAddr) const
    {
        if (mInterfaces.CanReach(aAddr))
        {
            return true;
        }

        for (RouteList::const_iterator lIt = mRoutes.begin(); lIt != mRoutes.end(); lIt++)
        {
            if (lIt->GetSubNet()->VerifyAddress(aAddr))
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

    void Router::SetName(const char * aName)
    {
        assert(NULL != aName);

        mName = aName;
    }

    void Router::SetSubNetList(SubNetList * aSubNets)
    {
        assert(NULL != aSubNets);

        mSubNets = aSubNets;
    }

    // NOT TESTED NH.Router.RetrieveInfo
    //            Retrieve Router information through Telenet connection

    // NOT TESTED NH.Router.RetrieveInfo.Error
    //            Report an error when the InfoType is not valid

    void Router::RetrieveInfo(InfoType aType, const char * aData, unsigned int aFlags)
    {
        assert(NULL != aData);

        if (0 == strcmp(NAME_DEFAULT, mName.c_str()))
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
                Utl_ThrowError("ERROR", __LINE__, "Invalid information type", aType);
            }

            SetName(lName);
        }
    }

    void Router::Verify() const
    {
        mAccessLists.Verify();
        mInterfaces .Verify();

        for (RouteList::const_iterator lIt = mRoutes.begin(); lIt != mRoutes.end(); lIt++)
        {
            char lMessage[128];
            int  lRet;
            char lSubNet [ 64];

            const SubNet * lDestSubNet = lIt->GetSubNet();
            assert(NULL != lDestSubNet);

            if (NULL != mInterfaces.Find(lDestSubNet))
            {
                lDestSubNet->GetFullName(lSubNet, sizeof(lSubNet));

                lRet = sprintf_s(lMessage, "Useless route because the %s subnet is directly connected", lSubNet);
                assert(               0 < lRet);
                assert(sizeof(lMessage) > lRet);

                Utl_ThrowError("ERROR", __LINE__, lMessage);
            }

            if (!CanReach(lIt->GetAddress()))
            {
                char lAddr   [ 32];

                lIt->GetAddress(lAddr, sizeof(lAddr));

                lDestSubNet->GetFullName(lSubNet, sizeof(lSubNet));

                lRet = sprintf_s(lMessage, "Cannot reach %s, the next router for %s", lAddr, lSubNet);
                assert(               0 < lRet);
                assert(sizeof(lMessage) > lRet);

                Utl_ThrowError("ERROR", __LINE__, lMessage);
            }
        }
    }

    // Internal
    /////////////////////////////////////////////////////////////////////////

    const Router::RouteList * Router::GetRoutes() const
    {
        return &mRoutes;
    }

    void Router::Prepare(HI::Diagram * aDiagram, HI::CSS_Color aColor, const ShapeMap & aSubNetMap)
    {
        assert(NULL != aDiagram);

        HI::Shape * lShape = aDiagram->mShapes.AddShape("Router", mName.c_str());
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
