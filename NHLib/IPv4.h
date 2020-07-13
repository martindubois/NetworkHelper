
// Author     KMS - Martin Dubois, P.Eng.
// Copyrights (C) 2020 KMS. All rights reserved.
// Product    NetworkHelper
// File       NHLib/IPv4.h

// CODE REVIEW 2020-07-08 KMS - Martin Dubois, P.Eng.

#pragma once

// ===== C ==================================================================
#include <stdint.h>

// Data type
/////////////////////////////////////////////////////////////////////////////

typedef enum
{
    IPv4_BROADCAST    ,
    IPv4_DOCUMENTATION,
    IPv4_IETF         ,
    IPv4_INVALID      ,
    IPv4_LINK_LOCAL   ,
    IPv4_LOOPBACK     ,
    IPv4_MULTICAST    ,
    IPv4_PRIVATE      ,
    IPv4_PUBLIC       ,
    IPv4_RELAY        ,
    IPv4_RESERVED     ,
    IPv4_SHARED       ,
    IPv4_SOFTWARE     ,
    IPv4_TEST         ,

    IPv4_QTY
}
IPv4_AddressType;

// Functions
/////////////////////////////////////////////////////////////////////////////

extern void     IPv4_AddressToText       (char * aOut, unsigned int aOutSize_byte, uint32_t aIn);
extern IPv4_AddressType IPv4_GetAddressType (uint32_t aAddr);
extern uint32_t IPv4_TextToAddress       (const char * aIn, bool aValidate = true);
extern uint32_t IPv4_TextToAddress_Invert(const char * aIn);
extern void     IPv4_Validate            (uint32_t aAddr);
extern void     IPv4_Validate            (                uint32_t aSubNet, uint32_t aMask);
extern void     IPv4_Validate            (uint32_t aAddr, uint32_t aSubNet, uint32_t aMask);
