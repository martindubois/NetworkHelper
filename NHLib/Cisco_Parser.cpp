
// Author     KMS - Martin Dubois, P.Eng.
// Copyrights (C) 2020 KMS. All rights reserved.
// Product    NetworkHelper
// File       NHLib/Cisco_Parser.cpp

// CODE REVIEW 2020-07-30 KMS - Martin Dubois, P.Eng.

// TEST COVERAGE 2020-07-30 KMS - Martin Dubois, P.Eng.

#include "Component.h"

// ===== Includes ===========================================================
#include <NH/DHCP.h>
#include <NH/NAT.h>
#include <NH/Router.h>
#include <NH/SubNet.h>
#include <NH/SubNetList.h>

// ===== NHLib ==============================================================
#include "CheckList.h"
#include "Checks.h"
#include "Errors.h"
#include "IPv4.h"
#include "Utilities.h"

#include "Cisco_Parser.h"

// Constants
/////////////////////////////////////////////////////////////////////////////

#define ACCESS_LIST_IP_EXTENDED (0x00000001)
#define ACCESS_LIST_IP_STANDARD (0x00000002)
#define ACCESS_LIST_IP          (ACCESS_LIST_IP_EXTENDED|ACCESS_LIST_IP_STANDARD)
#define ACCESS_LIST_NAT         (0x00000004)

#define SECTION_ACCESS_LIST (0)
#define SECTION_DHCP        (1)
#define SECTION_INTERFACE   (2)

#define SECTION_QTY (3)

static const char * SECTION_NAMES[SECTION_QTY] =
{
    "access-list or ip access-list",
    "ip dhcp pool",
    "interface",
};

// Enumerations
/////////////////////////////////////////////////////////////////////////////

static const Parser::Node ENUM_ACCESS_PROTOCOL[] =
{
    { NH::Access::PROTOCOL_ICMP, "ICMP", NULL },
    { NH::Access::PROTOCOL_IP  , "IP"  , NULL },
    { NH::Access::PROTOCOL_TCP , "TCP" , NULL },
    { NH::Access::PROTOCOL_UDP , "UDP" , NULL },

    { Parser::CODE_ERROR, NULL, NULL }
};

static const Parser::Node ENUM_ACCESS_END_FILTER[] =
{
    { NH::AccessEnd::FILTER_ANY , "any" , NULL },
    { NH::AccessEnd::FILTER_HOST, "host", NULL },

    { Parser::CODE_NO_MATCH, NULL, NULL }
};

static const Parser::Node ENUM_ACCESS_END_OPERATOR[] =
{
    { NH::AccessEnd::OPERATOR_EQ   , "eq"   , NULL },
    { NH::AccessEnd::OPERATOR_GT   , "gt"   , NULL },
    { NH::AccessEnd::OPERATOR_LT   , "lt"   , NULL },
    { NH::AccessEnd::OPERATOR_NEQ  , "neq"  , NULL },
    { NH::AccessEnd::OPERATOR_RANGE, "range", NULL },

    { NH::AccessEnd::OPERATOR_ANY, NULL, NULL },
};

static const Parser::Node ENUM_DIRECTION[] =
{
    { NH::DIRECTION_IN , "in" , NULL },
    { NH::DIRECTION_OUT, "out", NULL },

    { Parser::CODE_ERROR, NULL, NULL }
};

// Commands
/////////////////////////////////////////////////////////////////////////////

#define CMD_ACCESS_LIST               ( 1)
#define CMD_DEFAULT_ROUTER            ( 2)
#define CMD_DENY                      ( 3)
#define CMD_DNS_SERVER                ( 4)
#define CMD_ENCAPSULATION_DOT1Q       ( 5)
#define CMD_EXIT                      ( 6)
#define CMD_HOSTNAME                  ( 7)
#define CMD_INTERFACE                 ( 8)
#define CMD_INTERFACE_TUNNEL          ( 9)
#define CMD_IP_ACCESS_GROUP           (10)
#define CMD_IP_ACCESS_LIST_EXTENDED   (11)
#define CMD_IP_ADDRESS                (12)
#define CMD_IP_ADDRESS_DHCP           (13)
#define CMD_IP_DHCP_POOL              (14)
#define CMD_IP_NAT_INSIDE             (15)
#define CMD_IP_NAT_INSIDE_SOURCE_LIST (16)
#define CMD_IP_NAT_OUTSIDE            (17)
#define CMD_IP_NAT_POOL               (18)
#define CMD_IP_ROUTE                  (19)
#define CMD_IP_ROUTING                (20)
#define CMD_NETWORK                   (21)
#define CMD_NO_SHUTDOWN               (22)
#define CMD_PERMIT                    (23)
#define CMD_SHUTDOWN                  (24)
#define CMD_TUNNEL_DESTINATION        (25)
#define CMD_TUNNEL_SOURCE             (26)

static const Parser::Node CMDS_ACCESS[] =
{
    { CMD_DENY  , "deny"  , NULL },
    { CMD_PERMIT, "permit", NULL },

    { Parser::CODE_ERROR, NULL, NULL }
};

static const Parser::Node CMDS_ENCAPSULATION[] =
{
    { CMD_ENCAPSULATION_DOT1Q, "dot1q", NULL },

    { Parser::CODE_IGNORE, NULL, NULL }
};

static const Parser::Node CMDS_INTERFACE[] =
{
    { CMD_INTERFACE_TUNNEL, "tunnel", NULL },

    { Parser::CODE_NO_MATCH, NULL, NULL }
};

static const Parser::Node CMDS_IP_ACCESS_LIST[] =
{
    { CMD_IP_ACCESS_LIST_EXTENDED, "extended", NULL },

    { Parser::CODE_IGNORE, NULL, NULL }
};

static const Parser::Node CMDS_IP_ADDRESS[] =
{
    { CMD_IP_ADDRESS_DHCP, "dhcp", NULL },

    { Parser::CODE_NO_MATCH, NULL, NULL }
};

static const Parser::Node CMDS_IP_DHCP[] =
{
    { CMD_IP_DHCP_POOL, "pool", NULL },

    { Parser::CODE_NO_MATCH, NULL, NULL }
};

static const Parser::Node CMDS_IP_NAT_INSIDE_SOURCE[] =
{
    { CMD_IP_NAT_INSIDE_SOURCE_LIST, "list", NULL },

    { Parser::CODE_ERROR, NULL, NULL }
};

static const Parser::Node CMDS_IP_NAT_INSIDE[] =
{
    { Parser::CODE_ERROR, "source", CMDS_IP_NAT_INSIDE_SOURCE },

    { Parser::CODE_NO_MATCH, NULL, NULL }
};

static const Parser::Node CMDS_IP_NAT[] =
{
    { CMD_IP_NAT_INSIDE , "inside" , CMDS_IP_NAT_INSIDE },
    { CMD_IP_NAT_OUTSIDE, "outside", NULL },
    { CMD_IP_NAT_POOL   , "pool"   , NULL },

    { Parser::CODE_IGNORE, NULL, NULL }
};

static const Parser::Node CMDS_IP[] =
{
    { CMD_IP_ACCESS_GROUP, "access-group", NULL                },
    { Parser::CODE_IGNORE, "access-list" , CMDS_IP_ACCESS_LIST },
    { CMD_IP_ADDRESS     , "address"     , CMDS_IP_ADDRESS     },
    { Parser::CODE_IGNORE, "dhcp"        , CMDS_IP_DHCP        },
    { Parser::CODE_IGNORE, "nat"         , CMDS_IP_NAT         },
    { CMD_IP_ROUTE       , "route"       , NULL                },
    { CMD_IP_ROUTING     , "routing"     , NULL                },

    { Parser::CODE_IGNORE, NULL, NULL }
};

static const Parser::Node CMDS_NO[] =
{
    { CMD_NO_SHUTDOWN, "shutdown", NULL },

    { Parser::CODE_IGNORE, NULL, NULL }
};

static const Parser::Node CMDS_TUNNEL[] =
{
    { CMD_TUNNEL_DESTINATION, "destination", NULL },
    { CMD_TUNNEL_SOURCE     , "source"     , NULL },

    { Parser::CODE_IGNORE, NULL, NULL }
};

static const Parser::Node COMMANDS[] =
{
    { CMD_ACCESS_LIST    , "access-list"   , NULL               },
    { CMD_DEFAULT_ROUTER , "default-router", NULL               },
    { CMD_DENY           , "deny"          , NULL               },
    { CMD_DNS_SERVER     , "dns-server"    , NULL               },
    { Parser::CODE_IGNORE, "encapsulation" , CMDS_ENCAPSULATION },
    { CMD_EXIT           , "exit"          , NULL               },
    { CMD_HOSTNAME       , "hostname"      , NULL               },
    { CMD_INTERFACE      , "interface"     , CMDS_INTERFACE     },
    { Parser::CODE_IGNORE, "ip"            , CMDS_IP            },
    { CMD_NETWORK        , "network"       , NULL               },
    { Parser::CODE_IGNORE, "no"            , CMDS_NO            },
    { CMD_PERMIT         , "permit"        , NULL               },
    { CMD_SHUTDOWN       , "shutdown"      , NULL               },
    { Parser::CODE_IGNORE, "tunnel"        , CMDS_TUNNEL        },

    { Parser::CODE_IGNORE, NULL, NULL }
};

// Static function declarations
/////////////////////////////////////////////////////////////////////////////

static unsigned int Access_End_Ports(const char ** aElements, unsigned int aCount, unsigned int aIndex, NH::AccessEnd * aEnd, const char * aCommand);

namespace Cisco
{

    // Public
    /////////////////////////////////////////////////////////////////////////

    Parser::Parser() : ::Parser(COMMANDS, SECTION_NAMES)
    {
    }

    // Protected
    /////////////////////////////////////////////////////////////////////////

    // ===== ::Parser =======================================================

    unsigned int Parser::ParseLine(const char * aLine)
    {
        assert(NULL != aLine);

        const char * lLine = aLine;

        for (;; )
        {
            switch (*lLine)
            {
            case ' ':
            case '\t':
                lLine++;
                break;

            case '\0':
            case '\n':
            case '\r':
            case '!':
                Display_Comment(aLine);
                return 0;

            default:
                return ::Parser::ParseLine(lLine);
            }
        }
    }

    bool Parser::Process(unsigned int aCode, const char ** aElements, unsigned int aCount)
    {
        switch (aCode)
        {
        case CMD_ACCESS_LIST              : return Cmd_AccessList               (aElements, aCount);
        case CMD_DEFAULT_ROUTER           : return Cmd_DefaultRouter            (aElements, aCount);
        case CMD_DENY                     : return Cmd_Deny                     (aElements, aCount);
        case CMD_DNS_SERVER               : return Cmd_DnsServer                (aElements, aCount);
        case CMD_ENCAPSULATION_DOT1Q      : return Cmd_Encapsulation_Dot1Q      (aElements, aCount);
        case CMD_EXIT                     : return Cmd_Exit                     (aElements, aCount);
        case CMD_HOSTNAME                 : return Cmd_Hostname                 (aElements, aCount);
        case CMD_INTERFACE                : return Cmd_Interface                (aElements, aCount);
        case CMD_INTERFACE_TUNNEL         : return Cmd_Interface_Tunnel         (aElements, aCount);
        case CMD_IP_ACCESS_GROUP          : return Cmd_Ip_AccessGroup           (aElements, aCount);
        case CMD_IP_ACCESS_LIST_EXTENDED  : return Cmd_Ip_AccessList_Extended   (aElements, aCount);
        case CMD_IP_ADDRESS               : return Cmd_Ip_Address               (aElements, aCount);
        case CMD_IP_ADDRESS_DHCP          : return Cmd_Ip_Address_Dhcp          (aElements, aCount);
        case CMD_IP_DHCP_POOL             : return Cmd_Ip_Dhcp_Pool             (aElements, aCount);
        case CMD_IP_NAT_INSIDE            : return Cmd_Ip_Nat_Inside            (aElements, aCount);
        case CMD_IP_NAT_INSIDE_SOURCE_LIST: return Cmd_Ip_Nat_Inside_Source_List(aElements, aCount);
        case CMD_IP_NAT_OUTSIDE           : return Cmd_Ip_Nat_Outside           (aElements, aCount);
        case CMD_IP_NAT_POOL              : return Cmd_Ip_Nat_Pool              (aElements, aCount);
        case CMD_IP_ROUTE                 : return Cmd_Ip_Route                 (aElements, aCount);
        case CMD_IP_ROUTING               : return Cmd_Ip_Routing               (aElements, aCount);
        case CMD_NETWORK                  : return Cmd_Network                  (aElements, aCount);
        case CMD_NO_SHUTDOWN              : return Cmd_No_Shutdown              (aElements, aCount);
        case CMD_PERMIT                   : return Cmd_Permit                   (aElements, aCount);
        case CMD_SHUTDOWN                 : return Cmd_Shutdown                 (aElements, aCount);
        case CMD_TUNNEL_DESTINATION       : return Cmd_Tunnel_Destination       (aElements, aCount);
        case CMD_TUNNEL_SOURCE            : return Cmd_Tunnel_Source            (aElements, aCount);

        default: assert(false);
        }

        return false;
    }

    // Private
    /////////////////////////////////////////////////////////////////////////

    bool Parser::Access(const char ** aElements, unsigned int aCount, NH::Access::Type aType)
    {
        ValidateCount(aCount, 2);

        NH::AccessList * lContext = dynamic_cast<NH::AccessList *>(Section_GetContext(SECTION_ACCESS_LIST));
        assert(NULL != lContext);

        NH::Access * lAccess = lContext->Add(aType);
        assert(NULL != lAccess);

        try
        {
            switch (lContext->GetListType())
            {
            case ACCESS_LIST_IP_EXTENDED: return Access_Ip (aElements, aCount, lAccess);
            case ACCESS_LIST_NAT        : return Access_Nat(aElements, aCount, lAccess);

            default: assert(false);
            }
        }
        catch (...)
        {
            lContext->Undo();
            throw;
        }

        return false;
    }

    unsigned int Parser::Access_End(const char ** aElements, unsigned int aCount, unsigned int aIndex, NH::Access::Protocol aProtocol, NH::AccessEnd * aEnd)
    {
        assert(NULL   != aElements);
        assert(aCount >= aIndex   );
        assert(NH::Access::PROTOCOL_QTY > aProtocol);
        assert(NULL   != aEnd     );

        unsigned int lIndex = aIndex;

        ValidateCount(aCount, lIndex + 1);

        NH::Router * lRouter = GetRouter();
        assert(NULL != lRouter);
        assert(NULL != lRouter->mCheckList);

        switch (Walk(aElements + lIndex, 1, ENUM_ACCESS_END_FILTER))
        {
        case NH::AccessEnd::FILTER_ANY:
            aEnd->SetAny();
            break;

        case NH::AccessEnd::FILTER_HOST:
            lIndex++;

            ValidateCount(aCount, lIndex + 1);

            uint32_t lHost;

            lHost = IPv4_TextToAddress(aElements[lIndex]);

            aEnd->SetHost(lHost);

            lRouter->mCheckList->Add(new Check_Reach(ERROR_235, lHost));
            break;

        case Parser::CODE_NO_MATCH:
            ValidateCount(aCount, lIndex + 2);

            uint32_t lAddr;
            uint32_t lMask;

            lAddr = IPv4_TextToAddress       (aElements[lIndex    ]);
            lMask = IPv4_TextToAddress_Invert(aElements[lIndex + 1]);

            NH::SubNet * lSubNet;

            lSubNet = GetRouter()->GetSubNetList()->FindOrCreate(lAddr, lMask);

            aEnd->SetSubNet(lSubNet);

            lRouter->mCheckList->Add(new Check_Reach(ERROR_234, lSubNet));

            lIndex += 1;
            break;

        default: assert(false);
        }

        lIndex++;

        switch (aProtocol)
        {
        case NH::Access::PROTOCOL_TCP:
        case NH::Access::PROTOCOL_UDP:
            lIndex = Access_End_Ports(aElements, aCount, lIndex, aEnd);
            break;
        }

        return lIndex;
    }

    unsigned int Parser::Access_End_Ports(const char ** aElements, unsigned int aCount, unsigned int aIndex, NH::AccessEnd * aEnd)
    {
        assert(NULL   != aElements);
        assert(     0 <  aCount   );
        assert(aCount >= aIndex   );
        assert(NULL   != aEnd     );

        unsigned int lIndex = aIndex;

        if (aCount > lIndex)
        {
            assert(NULL != aElements[lIndex]);

            NH::AccessEnd::Operator lOperator = static_cast<NH::AccessEnd::Operator>(Parser::Walk(aElements + lIndex, 1, ENUM_ACCESS_END_OPERATOR));
            switch (lOperator)
            {
            case NH::AccessEnd::OPERATOR_ANY:
                aEnd->SetPort(lOperator);
                break;

            case NH::AccessEnd::OPERATOR_EQ :
            case NH::AccessEnd::OPERATOR_GT :
            case NH::AccessEnd::OPERATOR_LT :
            case NH::AccessEnd::OPERATOR_NEQ:
                lIndex++;

                ValidateCount(aCount, lIndex + 1);
                assert(NULL != aElements[lIndex]);

                aEnd->SetPort(lOperator, aElements[lIndex]);

                lIndex++;
                break;

            case NH::AccessEnd::OPERATOR_RANGE:
                lIndex++;

                ValidateCount(aCount, lIndex + 2);
                assert(NULL != aElements[lIndex]);
                assert(NULL != aElements[lIndex + 1]);

                aEnd->SetPort(lOperator, aElements[lIndex], aElements[lIndex + 1]);

                lIndex += 2;
                break;

            default: assert(false);
            }
        }
        else
        {
            aEnd->SetPort(NH::AccessEnd::OPERATOR_ANY);
        }

        return lIndex;
    }

    bool Parser::Access_Ip(const char ** aElements, unsigned int aCount, NH::Access * aAccess)
    {
        assert(NULL != aElements);
        assert(NULL != aAccess);

        NH::Access::Protocol lProtocol = static_cast<NH::Access::Protocol>(Walk(aElements + 1, 1, ENUM_ACCESS_PROTOCOL));

        aAccess->SetProtocol(lProtocol);

        unsigned int lIndex = 2;

        lIndex = Access_End(aElements, aCount, lIndex, lProtocol, &aAccess->mSource     );
        lIndex = Access_End(aElements, aCount, lIndex, lProtocol, &aAccess->mDestination);

        if (aCount > lIndex)
        {
            assert(NULL != aElements[lIndex]);

            if (0 == _stricmp("established", aElements[lIndex]))
            {
                aAccess->SetEstablished();
            }
            else
            {
                Utl_ThrowError(ERROR_406);
            }
        }

        aAccess->Verify();

        // TODO Cisco.ip.access-list
        //      Add a Check_Allowed to verify if the answer is allowed it other direction

        return true;
    }

    bool Parser::Access_Nat(const char ** aElements, unsigned int aCount, NH::Access * aAccess)
    {
        assert(NULL != aElements);
        assert(NULL != aAccess);

        aAccess->mDestination.SetAny();

        Access_Nat_Source(aElements, aCount, &aAccess->mSource);

        aAccess->Verify();

        return true;
    }

    void Parser::Access_Nat_Source(const char ** aElements, unsigned int aCount, NH::AccessEnd * aSource)
    {
        assert(NULL != aElements);
        assert(NULL != aSource);

        ValidateCount(aCount, 2, 3);

        NH::Router * lRouter = GetRouter();
        assert(NULL != lRouter);
        assert(NULL != lRouter->mCheckList);

        switch (aCount)
        {
        case 2:
            uint32_t lHost;

            lHost = IPv4_TextToAddress(aElements[1]);

            aSource->SetHost(lHost);

            lRouter->mCheckList->Add(new Check_Reach(ERROR_CONFIG, __LINE__, "Cannot reach a host configured in a NAT access list", lHost));
            break;

        case 3:
            ValidateCount(aCount, 3, 3);

            uint32_t lAddr;
            uint32_t lMask;

            lAddr = IPv4_TextToAddress       (aElements[1]);
            lMask = IPv4_TextToAddress_Invert(aElements[2]);

            NH::SubNet * lSubNet;

            lSubNet = lRouter->GetSubNetList()->FindOrCreate(lAddr, lMask);

            aSource->SetSubNet(lSubNet);

            lRouter->mCheckList->Add(new Check_Reach(ERROR_CONFIG, __LINE__, "Cannot reach a host configured in a NAT access list", lSubNet));
            break;

        default: assert(false);
        }

        aSource->VerifyPrivate();
    }

    // ===== Commands =======================================================

    bool Parser::Cmd_AccessList(const char ** aElements, unsigned int aCount)
    {
        assert(NULL != aElements);

        ValidateCount("access-list", aCount, 2);

        NH::AccessList * lContext = GetRouter()->mAccessLists.FindOrCreate(aElements[1], ACCESS_LIST_NAT);
        assert(NULL != lContext);

        Section_Enter(SECTION_ACCESS_LIST, lContext);

        bool lResult = true;

        if (3 <= aCount)
        {
            switch (Walk(aElements + 2, 1, CMDS_ACCESS))
            {
            case CMD_DENY  : lResult = Cmd_Deny  (aElements + 2, aCount - 2); break;
            case CMD_PERMIT: lResult = Cmd_Permit(aElements + 2, aCount - 2); break;

            default: assert(false);
            }

            Section_Exit();
        }

        return lResult;
    }

    bool Parser::Cmd_DefaultRouter(const char ** aElements, unsigned int aCount)
    {
        ValidateCount("default-router", aCount, 2, 2);

        NH::DHCP * lContext = dynamic_cast<NH::DHCP *>(Section_GetContext(SECTION_DHCP));
        assert(NULL != lContext);

        uint32_t lAddr = IPv4_TextToAddress(aElements[1]);

        lContext->SetDefaultRouter(lAddr);

        NH::Router * lRouter = GetRouter();
        assert(NULL != lRouter);
        assert(NULL != lRouter->mCheckList);

        lRouter->mCheckList->Add(new Check_Address(ERROR_WARNING, __LINE__, "The router is not the default router it advertises", lAddr));
        lRouter->mCheckList->Add(new Check_Reach(ERROR_CONFIG, __LINE__, "Cannot reach a default router", lAddr));

        return true;
    }

    bool Parser::Cmd_Deny(const char ** aElements, unsigned int aCount)
    {
        SetCommand("deny");

        return Access(aElements, aCount, NH::Access::TYPE_DENY);
    }

    bool Parser::Cmd_DnsServer(const char ** aElements, unsigned int aCount)
    {
        assert(NULL != aElements);

        ValidateCount("dns-server", aCount, 2, 2);

        NH::DHCP * lContext = dynamic_cast<NH::DHCP *>(Section_GetContext(SECTION_DHCP));
        assert(NULL != lContext);

        uint32_t lAddr = IPv4_TextToAddress(aElements[1]);

        lContext->SetDnsServer(lAddr);

        NH::Router * lRouter = GetRouter();
        assert(NULL != lRouter);
        assert(NULL != lRouter->mCheckList);

        lRouter->mCheckList->Add(new Check_Reach(ERROR_CONFIG, __LINE__, "Cannot reach a configured DNS server", lAddr));

        // TODO Cisco.dns-server
        //      Verify the DNS request are allowed (in) on the interface - Check_Allowed

        return true;
    }

    bool Parser::Cmd_Encapsulation_Dot1Q(const char ** aElements, unsigned int aCount)
    {
        assert(NULL != aElements);

        ValidateCount("encapsulation dot1q", aCount, 3, 3);

        NH::Interface * lContext = dynamic_cast<NH::Interface *>(Section_GetContext(SECTION_INTERFACE));
        assert(NULL != lContext);

        lContext->SetVLAN(aElements[2]);

        char lBaseName[64];

        bool lRetB = lContext->GetBaseName(lBaseName, sizeof(lBaseName));
        assert(lRetB);

        NH::Interface * lBase = GetRouter()->mInterfaces.FindOrCreate(lBaseName);
        assert(NULL != lBase);
        assert(NULL != lBase->mCheckList);

        lBase->mCheckList->Add(new Check_Enabled(ERROR_CONFIG, __LINE__, "Must be enabled because it as sub-interface"));

        return true;
    }

    bool Parser::Cmd_Exit(const char ** aElements, unsigned int aCount)
    {
        ValidateCount("exit", aCount, 1, 1);

        Section_Exit();

        return true;
    }

    bool Parser::Cmd_Hostname(const char ** aElements, unsigned int aCount)
    {
        assert(NULL != aElements);

        ValidateCount("hostname", aCount, 2, 2);

        GetRouter()->SetName(aElements[1]);

        return true;
    }

    bool Parser::Cmd_Interface(const char ** aElements, unsigned int aCount)
    {
        assert(NULL != aElements);

        ValidateCount("interface", aCount, 2, 2);

        NH::Interface * lContext = GetRouter()->mInterfaces.FindOrCreate(aElements[1]);
        assert(NULL != lContext);

        Section_Enter(SECTION_INTERFACE, lContext);

        return true;
    }

    bool Parser::Cmd_Interface_Tunnel(const char ** aElements, unsigned int aCount)
    {
        assert(NULL != aElements);

        ValidateCount("interface tunel", aCount, 3, 3);
        assert(NULL != aElements[2]);

        char lName[64];

        int lRet = sprintf_s(lName, "Tunnel %s", aElements[2]);
        assert(            0 < lRet);
        assert(sizeof(lName) > lRet);

        NH::Interface * lContext = GetRouter()->mInterfaces.FindOrCreate(lName);
        assert(NULL != lContext);

        lContext->SetVirtual();

        Section_Enter(SECTION_INTERFACE, lContext);

        return true;
    }

    bool Parser::Cmd_Ip_AccessGroup(const char ** aElements, unsigned int aCount)
    {
        assert(NULL != aElements);

        ValidateCount("ip access-group", aCount, 4, 4);

        NH::Interface * lContext = dynamic_cast<NH::Interface *>(Section_GetContext(SECTION_INTERFACE));
        assert(NULL != lContext);

        NH::Direction lDirection = static_cast<NH::Direction>(Walk(aElements + 3, 1, ENUM_DIRECTION));

        NH::AccessList * lAccessList = GetRouter()->mAccessLists.FindOrCreate(aElements[2], ACCESS_LIST_IP);
        assert(NULL != lAccessList);

        lContext->SetAccessList(lDirection, lAccessList);

        return true;
    }

    bool Parser::Cmd_Ip_AccessList_Extended(const char ** aElements, unsigned int aCount)
    {
        assert(NULL != aElements);

        ValidateCount("ip access-list extended", aCount, 4);

        NH::AccessList * lContext = GetRouter()->mAccessLists.FindOrCreate(aElements[3], ACCESS_LIST_IP_EXTENDED);
        assert(NULL != lContext);

        Section_Enter(SECTION_ACCESS_LIST, lContext);

        bool lResult = true;

        if (5 <= aCount)
        {
            switch (Walk(aElements + 4, 1, CMDS_ACCESS))
            {
            case CMD_DENY  : lResult = Cmd_Deny  (aElements + 4, aCount - 4); break;
            case CMD_PERMIT: lResult = Cmd_Permit(aElements + 4, aCount - 4); break;

            default: assert(false);
            }

            Section_Exit();
        }

        return lResult;
    }

    bool Parser::Cmd_Ip_Address(const char ** aElements, unsigned int aCount)
    {
        assert(NULL != aElements);

        ValidateCount("ip address", aCount, 4, 4);

        NH::Interface * lContext = dynamic_cast<NH::Interface *>(Section_GetContext(SECTION_INTERFACE));
        assert(NULL != lContext);

        uint32_t lAddr = IPv4_TextToAddress(aElements[2]);
        uint32_t lMask = IPv4_TextToAddress(aElements[3]);

        lContext->SetAddress(lAddr);
        lContext->SetSubNet (GetRouter()->GetSubNetList()->FindOrCreate(lAddr & lMask, lMask));

        return true;
    }

    bool Parser::Cmd_Ip_Address_Dhcp(const char ** aElements, unsigned int aCount)
    {
        ValidateCount("ip address dhcp", aCount, 3, 3);

        NH::Interface * lContext = dynamic_cast<NH::Interface *>(Section_GetContext(SECTION_INTERFACE));
        assert(NULL != lContext) ;

        lContext->SetDHCP();

        return true;
    }

    bool Parser::Cmd_Ip_Dhcp_Pool(const char ** aElements, unsigned int aCount)
    {
        assert(NULL != aElements);

        ValidateCount("ip dhcp pool", aCount, 4, 4);

        NH::DHCP * lContext = GetRouter()->mDHCPs.FindOrCreate(aElements[3]);
        assert(NULL != lContext);

        Section_Enter(SECTION_DHCP, lContext);

        return true;
    }

    bool Parser::Cmd_Ip_Nat_Inside(const char ** aElements, unsigned int aCount)
    {
        SetCommand("ip nat inside");

        if (3 < aCount)
        {
            return false;
        }

        NH::Interface * lContext = dynamic_cast<NH::Interface *>(Section_GetContext(SECTION_INTERFACE));
        assert(NULL != lContext);

        lContext->SetNAT_Inside();

        return true;
    }

    bool Parser::Cmd_Ip_Nat_Inside_Source_List(const char ** aElements, unsigned int aCount)
    {
        assert(NULL != aElements);

        ValidateCount("ip nat inside source list", aCount, 8, 9);
        assert(NULL != aElements[6]);

        if (0 != _stricmp("pool", aElements[6]))
        {
            Utl_ThrowError(ERROR_PARSE, __LINE__, "Invalid \"ip nat inside source list\" command");
        }

        if (9 <= aCount)
        {
            assert(NULL != aElements[8]);

            if (0 != _stricmp("overload", aElements[8]))
            {
                Utl_ThrowError(ERROR_PARSE, __LINE__, "Invalid \"ip nat inside source list\" command");
            }
        }

        NH::Router * lRouter = GetRouter();
        assert(NULL != lRouter);

        NH::AccessList * lAccessList = lRouter->mAccessLists.FindOrCreate(aElements[5], ACCESS_LIST_NAT);
        assert(NULL != lAccessList);

        NH::NAT * lNAT = lRouter->mNATs.FindOrCreate(aElements[7]);
        assert(NULL != lNAT);

        lNAT->SetAccessList(lAccessList);

        return true;
    }

    bool Parser::Cmd_Ip_Nat_Outside(const char ** aElements, unsigned int aCount)
    {
        ValidateCount("ip nat outside", aCount, 3, 3);

        NH::Interface * lContext = dynamic_cast<NH::Interface *>(Section_GetContext(SECTION_INTERFACE));
        assert(NULL != lContext);

        lContext->SetNAT_Outside();

        return true;
    }

    bool Parser::Cmd_Ip_Nat_Pool(const char ** aElements, unsigned int aCount)
    {
        assert(NULL != aElements);

        ValidateCount("ip nat pool", aCount, 8, 8);

        if (0 != _stricmp("netmask", aElements[6]))
        {
            Utl_ThrowError(ERROR_PARSE, __LINE__, "Invalid \"ip nat pool\" command");
        }

        NH::NAT * lNAT = GetRouter()->mNATs.FindOrCreate(aElements[3]);
        assert(NULL != lNAT);

        lNAT->Set(aElements[4], aElements[5], aElements[7]);

        return true;
    }

    bool Parser::Cmd_Ip_Route(const char ** aElements, unsigned int aCount)
    {
        assert(NULL != aElements);

        ValidateCount("ip route", aCount, 5, 5);

        NH::Router * lRouter = GetRouter();
        assert(NULL != lRouter);

        NH::SubNet * lSubNet = lRouter->GetSubNetList()->FindOrCreate(aElements[2], aElements[3]);
        assert(NULL != lSubNet);

        lRouter->AddRoute(lSubNet, aElements[4]);

        return true;
    }

    bool Parser::Cmd_Ip_Routing(const char ** aElements, unsigned int aCount)
    {
        ValidateCount("ip routing", aCount, 2, 2);

        GetRouter()->SetIpRouting();

        return true;
    }

    bool Parser::Cmd_Network(const char ** aElements, unsigned int aCount)
    {
        assert(NULL != aElements);

        ValidateCount("network", aCount, 3, 3);

        NH::DHCP * lContext = dynamic_cast<NH::DHCP *>(Section_GetContext(SECTION_DHCP));
        assert(NULL != lContext);

        NH::Router * lRouter = GetRouter();
        assert(NULL != lRouter);

        NH::SubNet * lSubNet = lRouter->GetSubNetList()->FindOrCreate(aElements[1], aElements[2]);
        assert(NULL != lSubNet);

        lContext->SetSubNet(lSubNet);

        const NH::Interface * lInterface = lRouter->mInterfaces.Find(lSubNet);

        lSubNet->SetDHCP(lRouter, lInterface);

        if (NULL != lInterface)
        {
            assert(NULL != lInterface->mCheckList);

            lInterface->mCheckList->Add(new Check_Enabled(ERROR_237));
            lInterface->mCheckList->Add(new Check_NoDHCP(ERROR_CONFIG, __LINE__, "A same interface cannot be DHCP client and server"));

            // TODO Cisco.network
            //      Add a Check_StaticAddress
        }

        return true;
    }

    bool Parser::Cmd_No_Shutdown(const char ** aElements, unsigned int aCount)
    {
        ValidateCount("no shutdown", 2, 2);

        NH::Interface * lContext = dynamic_cast<NH::Interface *>(Section_GetContext(SECTION_INTERFACE));
        assert(NULL != lContext);

        lContext->SetEnable();

        return true;
    }

    bool Parser::Cmd_Permit(const char ** aElements, unsigned int aCount)
    {
        SetCommand("permit");

        return Access(aElements, aCount, NH::Access::TYPE_PERMIT);
    }

    bool Parser::Cmd_Shutdown(const char ** aElements, unsigned int aCount)
    {
        ValidateCount("shutdown", aCount, 1, 1);

        NH::Interface * lContext = dynamic_cast<NH::Interface *>(Section_GetContext(SECTION_INTERFACE));
        assert(NULL != lContext);

        lContext->SetEnable(false);

        return true;
    }

    bool Parser::Cmd_Tunnel_Destination(const char ** aElements, unsigned int aCount)
    {
        assert(NULL != aElements);

        ValidateCount("tunnel destination", aCount, 3, 3);

        NH::Interface * lContext = dynamic_cast<NH::Interface *>(Section_GetContext(SECTION_INTERFACE));
        assert(NULL != lContext);

        uint32_t lAddr = IPv4_TextToAddress(aElements[2]);

        switch (IPv4_GetAddressType(lAddr))
        {
        case IPv4_PRIVATE: Utl_ThrowError(ERROR_WARNING, __LINE__, "The tunnel destination is a private address");
        case IPv4_PUBLIC : break;

        default: Utl_ThrowError(ERROR_CONFIG, __LINE__, "The tunnel destination is not a valid IPv4 address");
        }

        NH::Router * lRouter = GetRouter();
        assert(NULL != lRouter);
        assert(NULL != lRouter->mCheckList);

        lRouter->mCheckList->Add(new Check_Reach(ERROR_CONFIG, __LINE__, "Cannot reach a tunnel destination", lAddr));

        return true;
    }

    bool Parser::Cmd_Tunnel_Source(const char ** aElements, unsigned int aCount)
    {
        assert(NULL != aElements);

        ValidateCount("tunnel source", 3, 3);

        NH::Interface * lContext = dynamic_cast<NH::Interface *>(Section_GetContext(SECTION_INTERFACE));
        assert(NULL != lContext);

        NH::Interface * lInterface = GetRouter()->mInterfaces.FindOrCreate(aElements[2]);
        assert(NULL != lInterface);
        assert(NULL != lInterface->mCheckList);

        lInterface->mCheckList->Add(new Check_Enabled(ERROR_CONFIG, __LINE__, "Must be enabled because it is a tunnel source"));
        lInterface->mCheckList->Add(new Check_Public(ERROR_WARNING, __LINE__, "Should be connected to a public subnet because it is a tunnel source"));

        return true;
    }

}
