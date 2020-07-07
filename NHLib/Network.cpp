
// Author     KMS - Martin Dubois, P.Eng.
// Copyrights (C) 2020 KMS. All rights reserved.
// Product    NetworkHelper
// File       NHLib/Network.cpp

// CODE REVIEW 2020-07-07 KMS - Martin Dubois, P.Eng.

// CODE COVERAGE 2020-07-07 KMS - Martin Dubois, P.Eng.

// ===== C ==================================================================
#include <assert.h>

// ===== Import/Includes ====================================================
#include <HI/Diagram.h>
#include <HI/SVG_Document.h>

// ===== Includes ===========================================================
#include <NH/Router.h>

#include <NH/Network.h>

// ===== NHLib ==============================================================
#include "IPv4.h"
#include "ShapeMap.h"
#include "Utilities.h"

// Constants
/////////////////////////////////////////////////////////////////////////////

#define ROUTER_COLOR_QTY (6)

static const HI::CSS_Color ROUTER_COLORS[ROUTER_COLOR_QTY] =
{
    HI::COLOR_AQUAMARINE    ,
    HI::COLOR_BURLY_WOOD    ,
    HI::COLOR_CYAN          ,
    HI::COLOR_LIGHT_GRAY    ,
    HI::COLOR_DARK_SEA_GREEN,
    HI::COLOR_LIGHT_CORAL   ,
};

namespace NH
{

    // Public
    ////////////////////////////////////////////////////////////////////////

    const unsigned int Network::FLAG_DEBUG = 0x00000001;

    Network::Network()
    {
    }

    Network::~Network()
    {
    }

    // NOT TESTED NH.Network.AddDevice.Error
    //            Add the same Device twice.

    // aDevice [-K-;R--]
    void Network::AddDevice(Device * aDevice)
    {
        assert(NULL != aDevice);

        for (DeviceList::iterator lIt = mDevices.begin(); lIt != mDevices.end(); lIt++)
        {
            assert(NULL != *lIt);

            if (*lIt == aDevice)
            {
                Utl_ThrowError(UTL_CALLER_ERROR, __LINE__, "Device already added");
            }
        }

        mDevices.push_back(aDevice);
    }

    void Network::AddKnownRouters()
    {
        for (RouterList::iterator lRouter = mRouters.begin(); lRouter != mRouters.end(); lRouter++)
        {
            const Router::RouteList * lRoutes = (*lRouter)->GetRoutes();
            assert(NULL != lRoutes);

            for (Router::RouteList::const_iterator lRoute = lRoutes->begin(); lRoute != lRoutes->end(); lRoute++)
            {
                uint32_t lAddr = lRoute->GetAddress();

                const Router * lExisting = FindRouter(lAddr);
                if (NULL == lExisting)
                {
                    char lName[32];

                    lRoute->GetAddress(lName, sizeof(lName));

                    Router * lNew = new Router();
                    assert(NULL != lNew);

                    lNew->SetName(lName);

                    Interface * lInterface = lNew->mInterfaces.FindOrCreate("1");
                    assert(NULL != lInterface);

                    lInterface->SetAddress(lAddr);

                    const SubNet * lSubNet = mSubNets.Find(lAddr);
                    if (NULL != lSubNet)
                    {
                        lInterface->SetSubNet(lSubNet);
                    }

                    AddRouter(lNew);
                }
            }
        }
    }

    // NOT TESTED NH.Network.AddRouter.Error
    //            Add the same Router twice.

    // aRouter [-K-;R--]
    void Network::AddRouter(Router * aRouter)
    {
        assert(NULL != aRouter);

        for (RouterList::iterator lIt = mRouters.begin(); lIt != mRouters.end(); lIt++)
        {
            assert(NULL != *lIt);

            if (*lIt == aRouter)
            {
                Utl_ThrowError(UTL_CALLER_ERROR, __LINE__, "Router already added");
            }
        }

        mRouters.push_back(aRouter);
    }

    void Network::DeleteDevices()
    {
        for (DeviceList::iterator lIt = mDevices.begin(); lIt != mDevices.end(); lIt++)
        {
            assert(NULL != *lIt);

            delete *lIt;
        }

        mDevices.clear();
    }

    void Network::DeleteRouters()
    {
        for (RouterList::iterator lIt = mRouters.begin(); lIt != mRouters.end(); lIt++)
        {
            assert(NULL != *lIt);

            delete *lIt;
        }

        mRouters.clear();
    }

    void Network::Generate_HTML(HI::FolderId aFolder, const char * aName, const char * aTitle, unsigned int aFlags)
    {
        HI::Diagram lDiagram;
        unsigned int lIndex = 0;

        if (0 != (aFlags & FLAG_DEBUG))
        {
            lDiagram.SetDebug();
        }

        ShapeMap lSubNetMap;

        mSubNets.Prepare(&lDiagram, &lSubNetMap);

        for (DeviceList::iterator lIt = mDevices.begin(); lIt != mDevices.end(); lIt++)
        {
            assert(NULL != *lIt);

            (*lIt)->Prepare(&lDiagram, lSubNetMap);
        }

        for (RouterList::iterator lIt = mRouters.begin(); lIt != mRouters.end(); lIt++)
        {
            Router * lRouter = *lIt;
            assert(NULL != lRouter);

            lRouter->Prepare(&lDiagram, ROUTER_COLORS[lIndex % ROUTER_COLOR_QTY], lSubNetMap);

            lIndex++;
        }

        lDiagram.PositionShapes();

        if (0 != (aFlags & FLAG_DEBUG))
        {
            lDiagram.Generate_CPP(HI::FOLDER_TEMP, aName);
        }

        lDiagram.Generate_HTML(aFolder, aName, aTitle);
    }

    void Network::Verify() const
    {
        for (RouterList::const_iterator lIt = mRouters.begin(); lIt != mRouters.end(); lIt++)
        {
            assert(NULL != *lIt);

            (*lIt)->Verify();
        }

        mSubNets.Verify();
    }

    // Private
    /////////////////////////////////////////////////////////////////////////

    const Router * Network::FindRouter(uint32_t aAddr) const
    {
        for (RouterList::const_iterator lRouter = mRouters.begin(); lRouter != mRouters.end(); lRouter++)
        {
            const Router * lResult = (*lRouter);
            assert(NULL != lResult);

            if (NULL != lResult->mInterfaces.Find(aAddr))
            {
                return lResult;
            }
        }

        return NULL;
    }
}
