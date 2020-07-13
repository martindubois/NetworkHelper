
// Author     KMS - Martin Dubois, P.Eng.
// Copyrights (C) 2020 KMS. All rights reserved.
// Product    NetworkHelper
// File       NHLib/Cisco_Parser.cpp

// CODE REVIEW 2020-07-10 KMS - Martin Dubois, P.Eng.

// TEST COVERAGE 2020-07-10 KMS - Martin Dubois, P.Eng.

#include "Component.h"

// ===== Includes ===========================================================
#include <NH/Router.h>
#include <NH/SubNet.h>
#include <NH/SubNetList.h>

// ===== NHLib ==============================================================
#include "Errors.h"
#include "IPv4.h"
#include "Utilities.h"

#include "Cisco_Parser.h"

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

#define CMD_ACCESS_LIST             ( 1)
#define CMD_DENY                    ( 2)
#define CMD_ENCAPSULATION_DOT1Q     ( 3)
#define CMD_HOSTNAME                ( 4)
#define CMD_INTERFACE               ( 5)
#define CMD_INTERFACE_TUNNEL        ( 6)
#define CMD_IP_ACCESS_GROUP         ( 7)
#define CMD_IP_ACCESS_LIST_EXTENDED ( 8)
#define CMD_IP_ADDRESS              ( 9)
#define CMD_IP_ADDRESS_DHCP         (10)
#define CMD_IP_NAT_INSIDE           (11)
#define CMD_IP_NAT_OUTSIDE          (12)
#define CMD_IP_NAT_POOL             (13)
#define CMD_IP_ROUTE                (14)
#define CMD_NETWORK                 (15)
#define CMD_PERMIT                  (16)
#define CMD_TUNNEL_DESTINATION      (17)
#define CMD_TUNNEL_SOURCE           (18)

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

static const Parser::Node CMDS_IP_NAT[] =
{
    { CMD_IP_NAT_INSIDE , "inside" , NULL },
    { CMD_IP_NAT_OUTSIDE, "outside", NULL },
    { CMD_IP_NAT_POOL   , "pool"   , NULL },

    { Parser::CODE_IGNORE, NULL, NULL }
};

static const Parser::Node CMDS_IP[] =
{
    { CMD_IP_ACCESS_GROUP, "access-group", NULL                },
    { Parser::CODE_IGNORE, "access-list" , CMDS_IP_ACCESS_LIST },
    { CMD_IP_ADDRESS     , "address"     , CMDS_IP_ADDRESS     },
    { Parser::CODE_IGNORE, "nat"         , CMDS_IP_NAT         },
    { CMD_IP_ROUTE       , "route"       , NULL                },

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
    { CMD_ACCESS_LIST    , "access-list"  , NULL               },
    { CMD_DENY           , "deny"         , NULL               },
    { Parser::CODE_IGNORE, "encapsulation", CMDS_ENCAPSULATION },
    { CMD_HOSTNAME       , "hostname"     , NULL               },
    { CMD_INTERFACE      , "interface"    , CMDS_INTERFACE     },
    { Parser::CODE_IGNORE, "ip"           , CMDS_IP            },
    { CMD_NETWORK        , "network"      , NULL               },
    { CMD_PERMIT         , "permit"       , NULL               },
    { Parser::CODE_IGNORE, "tunnel"       , CMDS_TUNNEL        },

    { Parser::CODE_IGNORE, NULL, NULL }
};

// Static function declarations
/////////////////////////////////////////////////////////////////////////////

static unsigned int Access_End_Ports(const char ** aElements, unsigned int aCount, unsigned int aIndex, NH::AccessEnd * aEnd, const char * aCommand);

namespace Cisco
{

    // Public
    /////////////////////////////////////////////////////////////////////////

    Parser::Parser() : ::Parser(COMMANDS), mAccessList(NULL), mInterface(NULL)
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
        case CMD_DENY                   : return Cmd_Deny                  (aElements, aCount);
        case CMD_ENCAPSULATION_DOT1Q    : return Cmd_Encapsulation_Dot1Q   (aElements, aCount);
        case CMD_HOSTNAME               : return Cmd_Hostname              (aElements, aCount);
        case CMD_INTERFACE              : return Cmd_Interface             (aElements, aCount);
        case CMD_INTERFACE_TUNNEL       : return Cmd_Interface_Tunnel      (aElements, aCount);
        case CMD_IP_ACCESS_GROUP        : return Cmd_Ip_AccessGroup        (aElements, aCount);
        case CMD_IP_ACCESS_LIST_EXTENDED: return Cmd_Ip_AccessList_Extended(aElements, aCount);
        case CMD_IP_ADDRESS             : return Cmd_Ip_Address            (aElements, aCount);
        case CMD_IP_ADDRESS_DHCP        : return Cmd_Ip_Address_Dhcp       (aElements, aCount);
        case CMD_IP_NAT_INSIDE          : return Cmd_Ip_Nat_Inside         (aElements, aCount);
        case CMD_IP_NAT_OUTSIDE         : return Cmd_Ip_Nat_Outside        (aElements, aCount);
        case CMD_IP_ROUTE               : return Cmd_Ip_Route              (aElements, aCount);
        case CMD_NETWORK                : return Cmd_Network               (aElements, aCount);
        case CMD_PERMIT                 : return Cmd_Permit                (aElements, aCount);

        case CMD_ACCESS_LIST       :
        case CMD_IP_NAT_POOL       :
        case CMD_TUNNEL_DESTINATION:
        case CMD_TUNNEL_SOURCE     :
            break;

        default: assert(false);
        }

        return false;
    }

    // Private
    /////////////////////////////////////////////////////////////////////////

    // NOT TESTED Cisco.Router.Error
    //            Invalid final element for deny or permit command

    bool Parser::Access(const char ** aElements, unsigned int aCount, NH::Access::Type aType, const char * aCommand)
    {
        assert(NULL                 != aElements);
        assert(                   1 <= aCount   );
        assert(NH::Access::TYPE_QTY >  aType    );

        ValidateCount(aCommand, aCount, 2);
        assert(NULL != aElements[1]);

        Section_AccessList(aCommand, "ip access-list");
        assert(NULL != mAccessList);

        NH::Access * lAccess = mAccessList->Add(NH::Access::TYPE_DENY);
        assert(NULL != lAccess);

        try
        {
            NH::Access::Protocol lProtocol = static_cast<NH::Access::Protocol>(Walk(aElements + 1, 1, ENUM_ACCESS_PROTOCOL));

            lAccess->SetProtocol(lProtocol);

            unsigned int lIndex = 2;

            lIndex = Access_End(aElements, aCount, lIndex, lProtocol, &lAccess->mSource     , aCommand);
            lIndex = Access_End(aElements, aCount, lIndex, lProtocol, &lAccess->mDestination, aCommand);

            if (aCount > lIndex)
            {
                assert(NULL != aElements[lIndex]);

                if (0 == _stricmp("established", aElements[lIndex]))
                {
                    lAccess->SetEstablished();
                }
                else
                {
                    Utl_ThrowError(ERROR_PARSE, __LINE__, "Unexexpected command element");
                }
            }

            lAccess->Verify();
        }
        catch (...)
        {
            mAccessList->Undo();
            throw;
        }

        return true;
    }

    unsigned int Parser::Access_End(const char ** aElements, unsigned int aCount, unsigned int aIndex, NH::Access::Protocol aProtocol, NH::AccessEnd * aEnd, const char * aCommand)
    {
        assert(NULL   != aElements);
        assert(     0 <  aCount   );
        assert(aCount >= aIndex   );
        assert(NH::Access::PROTOCOL_QTY > aProtocol);
        assert(NULL   != aEnd     );

        unsigned int lIndex = aIndex;

        ValidateCount(aCommand, aCount, lIndex + 1);
        assert(NULL != aElements[lIndex]);

        switch (Walk(aElements + lIndex, 1, ENUM_ACCESS_END_FILTER))
        {
        case NH::AccessEnd::FILTER_ANY:
            aEnd->SetAny();
            break;

        case NH::AccessEnd::FILTER_HOST:
            lIndex++;

            ValidateCount(aCommand, aCount, lIndex + 1);
            assert(NULL != aElements[lIndex]);

            aEnd->SetHost(aElements[lIndex]);
            break;

        case Parser::CODE_NO_MATCH:
            ValidateCount(aCommand, aCount, lIndex + 2);
            assert(NULL != aElements[lIndex    ]);
            assert(NULL != aElements[lIndex + 1]);

            uint32_t lAddr;
            uint32_t lMask;

            lAddr = IPv4_TextToAddress       (aElements[lIndex    ]);
            lMask = IPv4_TextToAddress_Invert(aElements[lIndex + 1]);

            aEnd->SetSubNet(GetRouter()->GetSubNetList()->FindOrCreate(lAddr, lMask));

            lIndex += 1;
            break;

        default: assert(false);
        }

        lIndex++;

        switch (aProtocol)
        {
        case NH::Access::PROTOCOL_TCP:
        case NH::Access::PROTOCOL_UDP:
            lIndex = Access_End_Ports(aElements, aCount, lIndex, aEnd, aCommand);
            break;
        }

        return lIndex;
    }

    // ===== Commands =======================================================

    // NOT TESTED Cisco.Router
    //            deny command

    bool Parser::Cmd_Deny(const char ** aElements, unsigned int aCount)
    {
        return Access(aElements, aCount, NH::Access::TYPE_DENY, "deny");
    }

    bool Parser::Cmd_Encapsulation_Dot1Q(const char ** aElements, unsigned int aCount)
    {
        static const char * COMMAND = "encapsulation dot1q";

        assert(NULL != aElements);
        assert(   2 <= aCount   );

        ValidateCount(COMMAND, aCount, 3, 3);
        assert(NULL != aElements[2]);

        Section_Interface(COMMAND);
        assert(NULL != mInterface);

        mInterface->SetVLAN(aElements[2]);

        return true;
    }

    bool Parser::Cmd_Hostname(const char ** aElements, unsigned int aCount)
    {
        assert(NULL != aElements);
        assert(   1 <= aCount   );

        ValidateCount("hostname", aCount, 2, 2);
        assert(NULL != aElements[1]);

        GetRouter()->SetName(aElements[1]);

        return true;
    }

    bool Parser::Cmd_Interface(const char ** aElements, unsigned int aCount)
    {
        assert(NULL != aElements);
        assert(   1 <= aCount   );

        ValidateCount("interface", aCount, 2, 2);
        assert(NULL != aElements[1]);

        mInterface = GetRouter()->mInterfaces.FindOrCreate(aElements[1]);
        assert(NULL != mInterface);

        return true;
    }

    bool Parser::Cmd_Interface_Tunnel(const char ** aElements, unsigned int aCount)
    {
        assert(NULL != aElements);
        assert(   2 <= aCount   );

        ValidateCount("interface tunnel", aCount, 3, 3);
        assert(NULL != aElements[2]);

        char lName[64];

        int lRet = sprintf_s(lName, "Tunnel %s", aElements[2]);
        assert(            0 < lRet);
        assert(sizeof(lName) > lRet);

        mInterface = GetRouter()->mInterfaces.FindOrCreate(lName);
        assert(NULL != mInterface);

        mInterface->SetVirtual();
        return true;
    }

    bool Parser::Cmd_Ip_AccessGroup(const char ** aElements, unsigned int aCount)
    {
        static const char * COMMAND = "ip access-group";

        assert(NULL != aElements);
        assert(   2 <= aCount   );

        ValidateCount(COMMAND, aCount, 4, 4);
        assert(NULL != aElements[2]);
        assert(NULL != aElements[3]);

        Section_Interface(COMMAND);
        assert(NULL != mInterface);

        NH::Direction lDirection = static_cast<NH::Direction>(Walk(aElements + 3, 1, ENUM_DIRECTION));

        NH::AccessList * lAccessList = GetRouter()->mAccessLists.FindOrCreate(aElements[2]);
        assert(NULL != lAccessList);

        mInterface->SetAccessList(lDirection, lAccessList);

        return true;
    }

    // NOT TESTED Cisco.Router
    //            deny or permit at the end of an access-list command

    bool Parser::Cmd_Ip_AccessList_Extended(const char ** aElements, unsigned int aCount)
    {
        assert(NULL != aElements);
        assert(   3 <= aCount   );

        ValidateCount("ip access-list extended", aCount, 4);
        assert(NULL != aElements[3]);

        mAccessList = GetRouter()->mAccessLists.FindOrCreate(aElements[3]);
        assert(NULL != mAccessList);

        if (5 <= aCount)
        {
            switch (Walk(aElements + 4, 1, CMDS_ACCESS))
            {
            case CMD_DENY  : return Cmd_Deny  (aElements + 4, aCount - 4);
            case CMD_PERMIT: return Cmd_Permit(aElements + 4, aCount - 4);

            default: assert(false);
            }
        }

        return true;
    }

    bool Parser::Cmd_Ip_Address(const char ** aElements, unsigned int aCount)
    {
        static const char * COMMAND = "ip address";

        assert(NULL != aElements);
        assert(   2 <= aCount   );

        ValidateCount(COMMAND, aCount, 4, 4);
        assert(NULL != aElements[2]);
        assert(NULL != aElements[3]);

        Section_Interface(COMMAND);
        assert(NULL != mInterface);

        uint32_t lAddr = IPv4_TextToAddress(aElements[2]);
        uint32_t lMask = IPv4_TextToAddress(aElements[3]);

        mInterface->SetAddress(lAddr);
        mInterface->SetSubNet (GetRouter()->GetSubNetList()->FindOrCreate(lAddr & lMask, lMask));

        return true;
    }

    bool Parser::Cmd_Ip_Address_Dhcp(const char ** aElements, unsigned int aCount)
    {
        static const char * COMMAND = "ip address dhcp";

        assert(NULL != aElements);
        assert(   3 <= aCount   );

        ValidateCount(COMMAND, aCount, 3, 3);

        Section_Interface(COMMAND);
        assert(NULL != mInterface);

        mInterface->SetDHCP();

        return true;
    }

    bool Parser::Cmd_Ip_Nat_Inside(const char ** aElements, unsigned int aCount)
    {
        assert(NULL != aElements);
        assert(   3 <= aCount   );

        if (3 < aCount)
        {
            return false;
        }

        Section_Interface("ip nat inside");
        assert(NULL != mInterface);

        mInterface->SetNAT_Inside();

        return true;
    }

    bool Parser::Cmd_Ip_Nat_Outside(const char ** aElements, unsigned int aCount)
    {
        static const char * COMMAND = "ip nat outside";

        assert(NULL != aElements);
        assert(   3 <= aCount   );

        ValidateCount(COMMAND, aCount, 3, 3);

        Section_Interface(COMMAND);
        assert(NULL != mInterface);

        mInterface->SetNAT_Outside();

        return true;
    }

    bool Parser::Cmd_Ip_Route(const char ** aElements, unsigned int aCount)
    {
        assert(NULL != aElements);
        assert(   2 <= aCount   );

        ValidateCount("ip route", aCount, 5, 5);
        assert(NULL != aElements[2]);
        assert(NULL != aElements[3]);
        assert(NULL != aElements[4]);

        NH::Router * lRouter = GetRouter();
        assert(NULL != lRouter);

        NH::SubNet * lSubNet = lRouter->GetSubNetList()->FindOrCreate(aElements[2], aElements[3]);
        assert(NULL != lSubNet);

        lRouter->AddRoute(lSubNet, aElements[4]);

        return true;
    }

    bool Parser::Cmd_Network(const char ** aElements, unsigned int aCount)
    {
        assert(NULL != aElements);
        assert(   1 <= aCount   );

        ValidateCount("network", aCount, 3, 3);
        assert(NULL != aElements[1]);
        assert(NULL != aElements[2]);

        NH::Router * lRouter = GetRouter();
        assert(NULL != lRouter);

        NH::SubNet * lSubNet = lRouter->GetSubNetList()->FindOrCreate(aElements[1], aElements[2]);
        assert(NULL != lSubNet);

        const NH::Interface * lInterface = lRouter->mInterfaces.Find(lSubNet);

        lSubNet->SetDHCP(lRouter, lInterface);

        return true;
    }

    bool Parser::Cmd_Permit(const char ** aElements, unsigned int aCount)
    {
        return Access(aElements, aCount, NH::Access::TYPE_PERMIT, "permit");
    }

    // ===== Sections =======================================================

    // NOT TESTED Cisco.Router.Error
    //            Access list entry outside of an access list section

    void Parser::Section_AccessList(const char * aCommand, const char * aSection)
    {
        if (NULL == mAccessList)
        {
            Section_Error(aCommand, aSection);
        }
    }

    void Parser::Section_Error(const char * aCommand, const char * aSection)
    {
        assert(NULL != aCommand);
        assert(NULL != aSection);

        char lMessage[128];

        int lRet = sprintf_s(lMessage, ERROR_225_FMT, aCommand, aSection);
        assert(0 < lRet);
        assert(sizeof(lMessage) > lRet);

        Utl_ThrowError(ERROR_225, lMessage);
    }

    void Parser::Section_Interface(const char * aCommand)
    {
        if (NULL == mInterface)
        {
            Section_Error(aCommand, "interface");
        }
    }

}

// Static functions
/////////////////////////////////////////////////////////////////////////////

// NOT TESTED Cisco.Router
//            Access list entry with port range;
//            Access list entry without destination port;

unsigned int Access_End_Ports(const char ** aElements, unsigned int aCount, unsigned int aIndex, NH::AccessEnd * aEnd, const char * aCommand)
{
    assert(NULL   != aElements);
    assert(     0 <  aCount   );
    assert(aCount >= aIndex   );
    assert(NULL   != aEnd     );
    assert(NULL   != aCommand );

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

            Parser::ValidateCount(aCommand, aCount, lIndex + 1);
            assert(NULL != aElements[lIndex]);

            aEnd->SetPort(lOperator, aElements[lIndex]);

            lIndex++;
            break;

        case NH::AccessEnd::OPERATOR_RANGE:
            lIndex++;

            Parser::ValidateCount(aCommand, aCount, lIndex + 2);
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
