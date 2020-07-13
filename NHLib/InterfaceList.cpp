
// Author    KMS - Martin Dubois, P.Eng.
// Copyright (C) 2020 KMS. All rights reserved.
// Product   NetworkHelper
// File      NHLib/InterfaceList.cpp

// CODE REVIEW 2020-07-13 KMS - Martin Dubois, P.Eng.

// TEST COVERAGE 2020-07-13 KMS - Martin Dubois, P.Eng.

#include "Component.h"

// ===== Import/Includes ====================================================
#include <HI/Line.h>
#include <HI/Link.h>

// ===== Includes ===========================================================
#include <NH/Interface.h>
#include <NH/SubNet.h>

#include <NH/InterfaceList.h>

// ===== NHLib ==============================================================
#include "Color.h"
#include "Errors.h"
#include "Utilities.h"

// Constants
/////////////////////////////////////////////////////////////////////////////

#define ELEMENT "Interfaces"

namespace NH
{

    // Public
    /////////////////////////////////////////////////////////////////////////

    InterfaceList::InterfaceList()
    {
    }

    InterfaceList::~InterfaceList()
    {
        for (InterfaceMap::iterator lIt = mInterfaces.begin(); lIt != mInterfaces.end(); lIt++)
        {
            assert(NULL != lIt->second);

            delete lIt->second;
        }
    }

    bool InterfaceList::CanReach(uint32_t aAddr) const
    {
        for (InterfaceMap::const_iterator lIt = mInterfaces.begin(); lIt != mInterfaces.end(); lIt++)
        {
            assert(NULL != lIt->second);

            const SubNet * lSubNet = lIt->second->GetSubNet();
            if ((NULL != lSubNet) && lSubNet->VerifyAddress(aAddr))
            {
                return true;
            }
        }

        return false;
    }

    // NOT TESTED NH.InterfaceList.Find
    //            Fail to find Interface by SubNet

    // aSubNet [---;---]
    const Interface * InterfaceList::Find(const SubNet * aSubNet) const
    {
        assert(NULL != aSubNet);

        for (InterfaceMap::const_iterator lIt = mInterfaces.begin(); lIt != mInterfaces.end(); lIt++)
        {
            const Interface * lInterface = lIt->second;
            assert(NULL != lInterface);

            if (lInterface->GetSubNet() == aSubNet)
            {
                return lInterface;
            }
        }

        return NULL;
    }

    const Interface * InterfaceList::Find(uint32_t aAddr) const
    {
        for (InterfaceMap::const_iterator lIt = mInterfaces.begin(); lIt != mInterfaces.end(); lIt++)
        {
            const Interface * lInterface = lIt->second;
            assert(NULL != lInterface);

            if (lInterface->GetAddress() == aAddr)
            {
                return lInterface;
            }
        }

        return NULL;
    }

    // NOT TESTED NH.InterfaceList.FindOrCreate
    //            Find an Interface by name

    Interface * InterfaceList::FindOrCreate(const char * aName)
    {
        assert(NULL != aName);

        Interface * lResult;

        InterfaceMap::iterator lIt = mInterfaces.find(aName);
        if (mInterfaces.end() == lIt)
        {
            lResult = new Interface(aName);
            assert(NULL != lResult);

            Add(lResult);
        }
        else
        {
            lResult = lIt->second;
        }

        return lResult;
    }

    unsigned int InterfaceList::GetCount() const
    {
        return mInterfaces.size();
    }

    // NOT TESTED NH.InterfaceList.GetIndex
    //            Do not find

    unsigned int InterfaceList::GetIndex(const char * aName) const
    {
        assert(NULL != aName);

        unsigned int lResult = 0;

        for (InterfaceMap::const_iterator lIt = mInterfaces.begin(); lIt != mInterfaces.end(); lIt++)
        {
            if (0 == strcmp(lIt->second->GetName(), aName))
            {
                return lResult;
            }

            lResult++;
        }

        return 0x7fffffff;
    }

    Interface * InterfaceList::GetInterface(const char * aName)
    {
        assert(NULL != aName);

        InterfaceMap::iterator lIt = mInterfaces.find(aName);
        if (mInterfaces.end() == lIt)
        {
            return false;
        }

        assert(NULL != lIt->second);

        return lIt->second;
    }

    // NOT TESTED NH.AccessList.Verify

    void InterfaceList::Verify() const
    {
        Utl_ThrowErrorIfNeeded(ERROR_003, ELEMENT, "", Verify_Internal());
    }

    // Internal
    /////////////////////////////////////////////////////////////////////////

    void InterfaceList::Prepare(HI::Diagram * aDiagram, HI::CSS_Color aColor, const HI::Shape * aParent, const ShapeMap & aSubNetMap)
    {
        assert(NULL !=  aParent);

        if (0 < mInterfaces.size())
        {
            unsigned int lIndex  = 0;
            HI::Shape ** lShapes = new HI::Shape *[mInterfaces.size()];
            assert(NULL != lShapes);

            for (InterfaceMap::iterator lIt = mInterfaces.begin(); lIt != mInterfaces.end(); lIt++)
            {
                assert(NULL != lIt->second);

                lShapes[lIndex] = lIt->second->PrepareShape(aDiagram, aColor, aSubNetMap);
                assert(NULL != lShapes[lIndex]);

                lIndex++;
            }

            assert(mInterfaces.size() == lIndex);

            lIndex = 0;

            for (InterfaceMap::iterator lIt = mInterfaces.begin(); lIt != mInterfaces.end(); lIt++)
            {
                char              lBaseName[64];
                Interface       * lInterface = lIt->second;
                const HI::Shape * lParent    = aParent;

                if (lInterface->GetBaseName(lBaseName, sizeof(lBaseName)))
                {
                    unsigned int lParentIndex = GetIndex(lBaseName);

                    if (mInterfaces.size() > lParentIndex)
                    {
                        lParent = lShapes[lParentIndex];
                    }
                }

                lInterface->PrepareLink(aDiagram, lShapes[lIndex], lParent);

                lIndex++;
            }

            delete[] lShapes;
        }
    }

    // NOT TESTED NH.InterfaceList.Verify.Error
    //            Two interface connected to the same subnet

    unsigned int InterfaceList::Verify_Internal() const
    {
        unsigned int lResult = 0;

        for (InterfaceMap::const_iterator lIt0 = mInterfaces.begin(); lIt0 != mInterfaces.end(); lIt0++)
        {
            const Interface * lI0 = lIt0->second;
            assert(NULL != lI0);

            lResult += lI0->Verify_Internal();

            InterfaceMap::const_iterator lIt1 = lIt0;

            lIt1++;

            for (; lIt1 != mInterfaces.end(); lIt1++)
            {
                const Interface * lI1 = lIt1->second;
                assert(NULL != lI1);

                if (lI0->GetSubNet() == lI1->GetSubNet())
                {
                    char lMessage[128];

                    int lRet = sprintf_s(lMessage, "Interface %s and %s are connected to the same subnet", lI0->GetName(), lI1->GetName());
                    assert(               0 < lRet);
                    assert(sizeof(lMessage) > lRet);

                    Utl_DisplayError(ERROR_CONFIG, __LINE__, lMessage);
                    lResult++;
                }
            }
        }

        return lResult;
    }

    // Private
    /////////////////////////////////////////////////////////////////////////

    void InterfaceList::Add(Interface * aInterface)
    {
        assert(NULL != aInterface);

        char         lBaseName[64];
        const char * lNewName = aInterface->GetName();

        assert(NULL != lNewName);

        try
        {
            if (aInterface->GetBaseName(lBaseName, sizeof(lBaseName)))
            {
                Interface * lBase = GetInterface(lBaseName);
                if (NULL != lBase)
                {
                    lBase->SetHasSubInterface();
                }
            }
            else
            {
                for (InterfaceMap::iterator lIt = mInterfaces.begin(); lIt != mInterfaces.end(); lIt++)
                {
                    assert(NULL != lIt->second);

                    if (lIt->second->GetBaseName(lBaseName, sizeof(lBaseName)) && (0 == strcmp(lBaseName, lNewName)))
                    {
                        aInterface->SetHasSubInterface();
                        break;
                    }
                }
            }

            mInterfaces.insert(InterfaceMap::value_type(lNewName, aInterface));
        }
        catch (...)
        {
            delete aInterface;
            throw;
        }
    }

}
