
// Author     KMS - Martin Dubois, P.Eng.
// Copyrights (C) 2020 KMS. All rights reserved.
// Product    NetworkHelper
// File       NHLib/Cisco_Parser.h

// CODE REVIEW 2020-07-30 KMS - Martin Dubois, P.Eng.

#pragma once

// ===== Includes ===========================================================
#include <NH/AccessList.h>
#include <NH/Interface.h>

// ===== NHLib ==============================================================
#include "Parser.h"

namespace Cisco
{

    class Parser : public ::Parser
    {

    public:

        Parser();

    protected:

        // ===== ::Parser ===================================================
        virtual unsigned int ParseLine(const char * aLine);
        virtual bool         Process(unsigned int aCode, const char ** aElements, unsigned int aCount);

    private:

        bool         Access          (const char ** aElements, unsigned int aCount,                      NH::Access::Type     aType                          );
        unsigned int Access_End      (const char ** aElements, unsigned int aCount, unsigned int aIndex, NH::Access::Protocol aProtocol, NH::AccessEnd * aEnd);
        unsigned int Access_End_Ports(const char ** aElements, unsigned int aCount, unsigned int aIndex,                                 NH::AccessEnd * aEnd);

        bool Access_Ip        (const char ** aElements, unsigned int aCount, NH::Access    * aAccessd);
        bool Access_Nat       (const char ** aElements, unsigned int aCount, NH::Access    * aAccessd);
        void Access_Nat_Source(const char ** aElements, unsigned int aCount, NH::AccessEnd * aSourced);

        // ===== Commands ===================================================
        bool Cmd_AccessList               (const char ** aElements, unsigned int aCount);
        bool Cmd_DefaultRouter            (const char ** aElements, unsigned int aCount);
        bool Cmd_Deny                     (const char ** aElements, unsigned int aCount);
        bool Cmd_DnsServer                (const char ** aElements, unsigned int aCount);
        bool Cmd_Encapsulation_Dot1Q      (const char ** aElements, unsigned int aCount);
        bool Cmd_Exit                     (const char ** aElements, unsigned int aCount);
        bool Cmd_Hostname                 (const char ** aElements, unsigned int aCount);
        bool Cmd_Interface                (const char ** aElements, unsigned int aCount);
        bool Cmd_Interface_Tunnel         (const char ** aElements, unsigned int aCount);
        bool Cmd_Ip_AccessGroup           (const char ** aElements, unsigned int aCount);
        bool Cmd_Ip_AccessList_Extended   (const char ** aElements, unsigned int aCount);
        bool Cmd_Ip_Address               (const char ** aElements, unsigned int aCount);
        bool Cmd_Ip_Address_Dhcp          (const char ** aElements, unsigned int aCount);
        bool Cmd_Ip_Dhcp_Pool             (const char ** aElements, unsigned int aCount);
        bool Cmd_Ip_Nat_Inside            (const char ** aElements, unsigned int aCount);
        bool Cmd_Ip_Nat_Inside_Source_List(const char ** aElements, unsigned int aCount);
        bool Cmd_Ip_Nat_Outside           (const char ** aElements, unsigned int aCount);
        bool Cmd_Ip_Nat_Pool              (const char ** aElements, unsigned int aCount);
        bool Cmd_Ip_Route                 (const char ** aElements, unsigned int aCount);
        bool Cmd_Ip_Routing               (const char ** aElements, unsigned int aCount);
        bool Cmd_Network                  (const char ** aElements, unsigned int aCount);
        bool Cmd_No_Shutdown              (const char ** aElements, unsigned int aCount);
        bool Cmd_Permit                   (const char ** aElements, unsigned int aCount);
        bool Cmd_Shutdown                 (const char ** aElements, unsigned int aCount);
        bool Cmd_Tunnel_Destination       (const char ** aElements, unsigned int aCount);
        bool Cmd_Tunnel_Source            (const char ** aElements, unsigned int aCount);

    };

}
