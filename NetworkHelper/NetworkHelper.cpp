
// Author    KMS - Martin Dubois, P.Eng.
// Copyright (C) 2020 KMS. All rights reserved.
// Product   NetworkHelper
// File      NetworkHelper/NetworkHelper.cpp

// CODE REVIEW 2020-07-13 KMS - Martin Dubois, P.Eng.

// TEST COVERAGE 2020-07-13 KMS - Martin Dubois, P.Eng.

// ===== C ==================================================================
#include <assert.h>
#include <stdio.h>

// ===== Windows ============================================================
#include <Windows.h>

// ===== Import/Includes ====================================================
#include <HI/Browser.h>
#include <KmsLib/ToolBase.h>
#include <KmsTool.h>
#include <SafeAPI.h>

// ===== Includes ===========================================================
#include <Cisco/Router.h>
#include <NH/Interface.h>
#include <NH/Network.h>
#include <NH/SubNet.h>

// ===== Common =============================================================
#include "../Common/Version.h"

// Commands
/////////////////////////////////////////////////////////////////////////////

static void AddRouter_Cisco  (KmsLib::ToolBase * aToolBase, const char * aArg);
static void AddRouter_Network(KmsLib::ToolBase * aToolBase, const char * aArg);

static const KmsLib::ToolBase::CommandInfo ADD_ROUTER_COMMANDS[] =
{
    { "Cisco"  , AddRouter_Cisco  , "Cisco {Name}"  , NULL },
    { "Network", AddRouter_Network, "Network {Name}", NULL },

    { NULL, NULL, NULL, NULL }
};

static void Cisco_ManyRouters(KmsLib::ToolBase * aToolBase, const char * aArg);
static void Cisco_OneRouter  (KmsLib::ToolBase * aToolBase, const char * aArg);

static const KmsLib::ToolBase::CommandInfo CISCO_COMMANDS[] =
{
    { "ManyRouters", Cisco_ManyRouters, "ManyRouters {Filter}", NULL },
    { "OneRouter"  , Cisco_OneRouter  , "OneRouter {FileName}", NULL },

    { NULL, NULL, NULL, NULL }
};

static void Router_RetrieveInfo_ConfigFile(KmsLib::ToolBase * aToolBase, const char * aArg);

static const KmsLib::ToolBase::CommandInfo ROUTER_RETRIEVE_INFO_COMMANDS[] =
{
    { "ConfigFile", Router_RetrieveInfo_ConfigFile, "ConfigFile {FileName}", NULL },

    { NULL, NULL, NULL, NULL }
};

static void Router_AddInterface(KmsLib::ToolBase * aToolBase, const char * aArg);
static void Router_Verify      (KmsLib::ToolBase * aToolBase, const char * aArg);

static const KmsLib::ToolBase::CommandInfo ROUTER_COMMANDS[] =
{
    { "AddInterface", Router_AddInterface, "AddInterface {Name} [Address]", NULL                          },
    { "RetrieveInfo", NULL               , "RetrieveInfo ..."             , ROUTER_RETRIEVE_INFO_COMMANDS },
    { "Verify"      , Router_Verify      , "Verify"                       , NULL                          },

    { NULL, NULL, NULL, NULL }
};

static void AddDevice      (KmsLib::ToolBase * aToolBase, const char * aArg);
static void AddKnownRouters(KmsLib::ToolBase * aToolBase, const char * aArg);
static void BrowserArgument(KmsLib::ToolBase * aToolBase, const char * aArg);
static void Generate       (KmsLib::ToolBase * aToolBase, const char * aArg);
static void Reset          (KmsLib::ToolBase * aToolBase, const char * aArg);
static void SubNet         (KmsLib::ToolBase * aToolBase, const char * aArg);
static void Verify         (KmsLib::ToolBase * aToolBase, const char * aArg);

static const KmsLib::ToolBase::CommandInfo COMMANDS[] =
{
    { "AddDevice"      , AddDevice      , "AddDevice {Name}"           , NULL                },
    { "AddKnownRouters", AddKnownRouters, "AddKnownRouters"            , NULL                },
    { "AddRouter"      , NULL           , "AddRouter ..."              , ADD_ROUTER_COMMANDS },
    { "BrowserArgument", BrowserArgument, "Browser {Argument}"         , NULL                },
    { "Cisco"          , NULL           , "Cisco ..."                  , CISCO_COMMANDS      },
    { "Generate"       , Generate       , "Generate {FileName} [Title]", NULL                },
    { "Reset"          , Reset          , "Reset"                      , NULL                },
    { "Router"         , NULL           , "Router ..."                 , ROUTER_COMMANDS     },
    { "SubNet"         , SubNet         , "SubNet {Address} {Mask}"    , NULL                },
    { "Verify"         , Verify         , "Verify"                     , NULL                },

    KMS_LIB_TOOL_BASE_FUNCTIONS

    { NULL, NULL, NULL, NULL }
};

// Constants
/////////////////////////////////////////////////////////////////////////////

#define ARG_MAX_SIZE_byte (1024)

// Static variables
/////////////////////////////////////////////////////////////////////////////

static HI::Browser     sBrowser  ;
static NH::Device    * sDevice   ;
static NH::Interface * sInterface;
static NH::Network   * sNetwork  ;
static NH::Router    * sRouter   ;
static NH::SubNet    * sSubNet   ;

// Static function declarations
/////////////////////////////////////////////////////////////////////////////

static void DeleteNetwork();

static void InitRouter(const char * aName);

static bool IsRouterSelected(KmsLib::ToolBase * aToolBase);

static void ParseAddress (NH::Interface * aInterface, const char * aAddr);
static bool ParseArgument(KmsLib::ToolBase * aToolBase, const char * aArg, const char * aFormat, char * aOut, const char * aCommand);
static void ParseOptions (KmsLib::ToolBase * aToolBase, NH::Interface * aInterface, const char aOpt[][ARG_MAX_SIZE_byte], unsigned int aCount);

static void RemoveWildcard(char * aOut, const char * aIn);

static void ReportInvalidCommand(KmsLib::ToolBase * aToolBase, int aCode, const char * aCommand);

// Macros
/////////////////////////////////////////////////////////////////////////////

#define CATCH                                                                 \
    catch (std::exception eE)                                                 \
    {                                                                         \
        char lDesc[128];                                                      \
        sprintf_s(lDesc, "%s (%u)", eE.what(), __LINE__);                     \
        aToolBase->SetError(__LINE__, lDesc, KmsLib::ToolBase::REPORT_ERROR); \
    }

// Entry point
/////////////////////////////////////////////////////////////////////////////

// NOT TESTED NetworkHelper
//            Parse command read from stdin

// NOT TESTED NetworkHelper.Error
//            Catch unexpected exception

int main(int aCount, const char ** aVector)
{
    KMS_TOOL_BANNER("NetworkHelper", "NetworkHelper", VERSION_STR, VERSION_TYPE);

    int lResult = 0;

    sNetwork = new NH::Network();

    KmsLib::ToolBase lToolBase(COMMANDS);

    try
    {
        sBrowser.ParseArguments(aCount, aVector);

        lToolBase.ParseArguments(aCount, aVector);

        lToolBase.ParseCommands();
    }
    catch (std::exception eE)
    {
        char lDesc[ARG_MAX_SIZE_byte];

        int lRet = sprintf_s(lDesc, "Unexpected exception - %s (%u)", eE.what(), __LINE__);
        assert(            0 < lRet);
        assert(sizeof(lDesc) > lRet);

        return lToolBase.SetError(__LINE__, lDesc, KmsLib::ToolBase::REPORT_FATAL_ERROR);
    }
    catch (...)
    {
        return lToolBase.SetError(__LINE__, "Unexpected exception", KmsLib::ToolBase::REPORT_FATAL_ERROR);
    }

    DeleteNetwork();

    return lResult;
}

// Commands
/////////////////////////////////////////////////////////////////////////////

void AddRouter_Cisco(KmsLib::ToolBase * aToolBase, const char * aArg)
{
    char lName[ARG_MAX_SIZE_byte];

    if (ParseArgument(aToolBase, aArg, "%s", lName, "AddRouter Cisco"))
    {
        try
        {
            sRouter = new Cisco::Router();
            assert(NULL != sRouter);

            InitRouter(lName);
        }
        CATCH;
    }
}

void AddRouter_Network(KmsLib::ToolBase * aToolBase, const char * aArg)
{
    char lName[ARG_MAX_SIZE_byte];

    if (ParseArgument(aToolBase, aArg, "%s", lName, "AddRouter Network"))
    {
        try
        {
            sRouter = new NH::Router();
            assert(NULL != sRouter);

            InitRouter(lName);

            sRouter->SetIpRouting();
        }
        CATCH;
    }
}

void Cisco_ManyRouters(KmsLib::ToolBase * aToolBase, const char * aArg)
{
    assert(NULL != aToolBase);

    char lFilter[ARG_MAX_SIZE_byte];

    if (ParseArgument(aToolBase, aArg, "%s", lFilter, "Cisco ManyRouters"))
    {
        WIN32_FIND_DATA lData;
        HANDLE          lHandle;

        lHandle = FindFirstFile(lFilter, &lData);
        if (INVALID_HANDLE_VALUE == lHandle)
        {
            char lMessage[128];

            int lRet = sprintf_s(lMessage, "\"%s\" does not match files", lFilter);
            assert(               0 < lRet);
            assert(sizeof(lMessage) > lRet);

            aToolBase->SetError(__LINE__, lMessage);
            return;
        }

        Reset(aToolBase, NULL);

        do
        {
            AddRouter_Cisco(aToolBase, lData.cAlternateFileName);

            Router_RetrieveInfo_ConfigFile(aToolBase, lData.cFileName);
        }
        while (FindNextFile(lHandle, &lData));

        FindClose(lHandle);

        AddKnownRouters(aToolBase, NULL);

        Verify(aToolBase, NULL);

        char lName[ARG_MAX_SIZE_byte];

        RemoveWildcard(lName, lFilter);

        Generate(aToolBase, lName);
    }
}

void Cisco_OneRouter(KmsLib::ToolBase * aToolBase, const char * aArg)
{
    char lFileName[ARG_MAX_SIZE_byte];

    if (ParseArgument(aToolBase, aArg, "%[^\n\r\t]", lFileName, "Cisco OneRouter"))
    {
        Reset(aToolBase, NULL);

        AddRouter_Cisco(aToolBase, "Router");

        Router_RetrieveInfo_ConfigFile(aToolBase, lFileName);

        AddKnownRouters(aToolBase, NULL);

        Verify(aToolBase, NULL);

        Generate(aToolBase, "Router");
    }
}

void Router_RetrieveInfo_ConfigFile(KmsLib::ToolBase * aToolBase, const char * aArg)
{
    char lData[ARG_MAX_SIZE_byte];

    if (   ParseArgument(aToolBase, aArg, "%[^\n\r\t]", lData, "Router RetrieveInfo ConfigFile")
        && IsRouterSelected(aToolBase))
    {
        assert(NULL != sRouter);

        try
        {
            sRouter->RetrieveInfo(NH::Router::INFO_CONFIG_FILE, lData);
        }
        CATCH;
    }
}

void Router_AddInterface(KmsLib::ToolBase * aToolBase, const char * aArg)
{
    assert(NULL != aArg);

    char lName   [ARG_MAX_SIZE_byte];
    char lAddr   [ARG_MAX_SIZE_byte];
    char lOpts[3][ARG_MAX_SIZE_byte];

    int lRet = sscanf_s(aArg, "%s %s %s %s %s", lName SIZE_INFO(sizeof(lName)), lAddr SIZE_INFO(sizeof(lAddr)), lOpts[0] SIZE_INFO(sizeof(lOpts[0])), lOpts[1] SIZE_INFO(sizeof(lOpts[1])), lOpts[2] SIZE_INFO(sizeof(lOpts[2])));
    if ((1 > lRet) || (5 < lRet))
    {
        ReportInvalidCommand(aToolBase, __LINE__, "Router AddInterface");
        return;
    }

    if (IsRouterSelected(aToolBase))
    {
        assert(NULL != sRouter);

        try
        {
            sInterface = sRouter->mInterfaces.FindOrCreate(lName);
            assert(NULL != sInterface);

            sInterface->SetEnable();

            if (NULL != sSubNet)
            {
                sInterface->SetSubNet(sSubNet);
            }

            if (2 <= lRet)
            {
                ParseAddress(sInterface, lAddr);
            }

            ParseOptions(aToolBase, sInterface, lOpts, lRet - 2);
        }
        CATCH;
    }
}

void Router_Verify(KmsLib::ToolBase * aToolBase, const char * aArg)
{
    if (IsRouterSelected(aToolBase))
    {
        assert(NULL != sRouter);

        try
        {
            sRouter->Verify();
        }
        CATCH;
    }
}

void AddDevice(KmsLib::ToolBase * aToolBase, const char * aArg)
{
    assert(NULL != aArg);

    assert(NULL != sNetwork);

    char lAddr   [ARG_MAX_SIZE_byte];
    char lName   [ARG_MAX_SIZE_byte];
    char lOpts[3][ARG_MAX_SIZE_byte];

    int lRet = sscanf_s(aArg, "%s %s %s %s %s", lName, sizeof(lName), lAddr, sizeof(lAddr), lOpts[0], sizeof(lOpts[0]), lOpts[1], sizeof(lOpts[1]), lOpts[2], sizeof(lOpts[2]));
    if ((1 > lRet) || (5 < lRet))
    {
        ReportInvalidCommand(aToolBase, __LINE__, "AddDevice");
        return;
    }

    try
    {
        sDevice = new NH::Device();
        assert(NULL != sDevice);

        sDevice->SetName(lName);

        if (NULL != sSubNet)
        {
            sDevice->SetSubNet(sSubNet);
        }

        if (2 <= lRet)
        {
            ParseAddress(sDevice, lAddr);
        }

        ParseOptions(aToolBase, sDevice, lOpts, lRet - 2);

        sNetwork->AddDevice(sDevice);
    }
    CATCH;
}

void AddKnownRouters(KmsLib::ToolBase * aToolBase, const char * aArg)
{
    assert(NULL != sNetwork);

    try
    {
        sNetwork->AddKnownRouters();
    }
    CATCH;
}

void BrowserArgument(KmsLib::ToolBase * aToolBase, const char * aArg)
{
    char lArg[ARG_MAX_SIZE_byte];

    if (ParseArgument(aToolBase, aArg, "%[^\n\r\t]", lArg, "BrowserArgument"))
    {
        try
        {
            sBrowser.ParseAssignation(lArg);
        }
        CATCH;
    }
}

void Generate(KmsLib::ToolBase * aToolBase, const char * aArg)
{
    assert(NULL != aArg);

    assert(NULL != sNetwork);

    char lName [ARG_MAX_SIZE_byte];
    char lTitle[ARG_MAX_SIZE_byte];

    switch (sscanf_s(aArg, "%s %s", lName, sizeof(lName), lTitle, sizeof(lTitle)))
    {
    case 1 :
        strcpy_s(lTitle, lName);
        // no break;
    case 2 :
        try
        {
            sNetwork->Generate_HTML(HI::FOLDER_TEMP, lName, lTitle);

            sBrowser.Start(HI::FOLDER_TEMP, lName);
        }
        CATCH;
        break;

    default:
        ReportInvalidCommand(aToolBase, __LINE__, "Generate");
    }
}

void Reset(KmsLib::ToolBase * aToolBase, const char * aArg)
{
    assert(NULL != sNetwork);

    sInterface = NULL;
    sRouter    = NULL;
    sSubNet    = NULL;

    DeleteNetwork();

    sNetwork = new NH::Network;
    assert(NULL != sNetwork);
}

void SubNet(KmsLib::ToolBase * aToolBase, const char * aArg)
{
    assert(NULL != aArg);

    assert(NULL != sNetwork);

    char lAddr[ARG_MAX_SIZE_byte];
    char lMask[ARG_MAX_SIZE_byte];

    if (2 != sscanf_s(aArg, "%s %s", lAddr, sizeof(lAddr), lMask, sizeof(lMask)))
    {
        ReportInvalidCommand(aToolBase, __LINE__, "SubNet");
        return;
    }

    try
    {
        sSubNet = sNetwork->mSubNets.FindOrCreate(lAddr, lMask);
        assert(NULL != sSubNet);
    }
    CATCH;
}

void Verify(KmsLib::ToolBase * aToolBase, const char * aArg)
{
    assert(NULL != sNetwork);

    try
    {
        sNetwork->Verify();
    }
    CATCH
}

// Static function declarations
/////////////////////////////////////////////////////////////////////////////

void DeleteNetwork()
{
    assert(NULL != sNetwork);

    sNetwork->DeleteDevices();
    sNetwork->DeleteRouters();

    delete sNetwork;
    sNetwork = NULL;
}

void InitRouter(const char * aName)
{
    assert(NULL != sNetwork);
    assert(NULL != sRouter );

    sRouter->SetName(aName);

    sRouter->SetSubNetList(&sNetwork->mSubNets);

    sNetwork->AddRouter(sRouter);
}

bool IsRouterSelected(KmsLib::ToolBase * aToolBase)
{
    assert(NULL != aToolBase);

    if (NULL == sRouter)
    {
        aToolBase->SetError(__LINE__, "No router");
        return false;
    }

    return true;
}

void ParseAddress(NH::Interface * aInterface, const char * aAddr)
{
    assert(NULL != aInterface);
    assert(NULL != aAddr     );

    if (0 == _stricmp("DHCP", aAddr))
    {
        aInterface->SetDHCP();
    }
    else
    {
        aInterface->SetAddress(aAddr);
    }
}

bool ParseArgument(KmsLib::ToolBase * aToolBase, const char * aArg, const char * aFormat, char * aOut, const char * aCommand)
{
    assert(NULL != aArg   );
    assert(NULL != aFormat);
    assert(NULL != aOut   );

    if (1 != sscanf_s(aArg, aFormat, aOut, ARG_MAX_SIZE_byte))
    {
        ReportInvalidCommand(aToolBase, __LINE__, aCommand);
        return false;
    }

    return true;
}

void ParseOptions(KmsLib::ToolBase * aToolBase, NH::Interface * aInterface, const char aOpts[][ARG_MAX_SIZE_byte], unsigned int aCount)
{
    assert(NULL != aToolBase );
    assert(NULL != aInterface);
    assert(NULL != aOpts     );

    for (unsigned int i = 0; i < aCount; i++)
    {
        char lMessage[128];
        int  lRet;

        if (0 == _stricmp("DHCP-Server", aOpts[i]))
        {
            if (NULL == sSubNet)
            {
                lRet = sprintf_s(lMessage, "%s must be connected to a subnet to act as a DHCP server", aInterface->GetName());
                assert(               0 < lRet);
                assert(sizeof(lMessage) > lRet);

                aToolBase->SetError(__LINE__, lMessage, KmsLib::ToolBase::REPORT_WARNING);
            }
            else
            {
                sSubNet->SetDHCP(sRouter, aInterface);
            }
        }
        else if (0 == _stricmp("Virtual", aOpts[i])) { aInterface->SetVirtual(); }
        else if (0 == _stricmp("Wifi"   , aOpts[i])) { sInterface->SetWifi   (); }
        else
        {
            lRet = sprintf_s(lMessage, "%s is not a valid option", aOpts[i]);
            assert(               0 < lRet);
            assert(sizeof(lMessage) > lRet);

            aToolBase->SetError(__LINE__, lMessage, KmsLib::ToolBase::REPORT_WARNING);
        }
    }
}

void RemoveWildcard(char * aOut, const char * aIn)
{
    assert(NULL != aOut);
    assert(NULL != aIn );

    unsigned int i = 0;

    while ('\0' != aIn[i])
    {
        switch (aIn[i])
        {
        case '?':
        case '*':
            aOut[i] = '_';
            break;

        default:
            aOut[i] = aIn[i];
        }

        i++;
    }

    aOut[i] = '\0';
}

void ReportInvalidCommand(KmsLib::ToolBase * aToolBase, int aCode, const char * aCommand)
{
    assert(NULL != aToolBase);
    assert(   0 != aCode    );
    assert(NULL != aCommand );

    char lMessage[128];

    int lRet = sprintf_s(lMessage, "Invalid \"%s\" command", aCommand);
    assert(               0 < lRet);
    assert(sizeof(lMessage) > lRet);

    aToolBase->SetError(aCode, lMessage);
}
