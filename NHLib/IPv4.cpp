
// Author     KMS - Martin Dubois, P.Eng.
// Copyrights (C) 2020 KMS. All rights reserved.
// Product    NetworkHelper
// File       NHLib/IPv4.cpp

// CODE REVIEW 2020-07-10 KMS - Martin Dubois, P.Eng.

#include "Component.h"

// ===== C ==================================================================
#include <stdio.h>

// ===== Windows ============================================================
#include <WS2tcpip.h>

// ===== NHLib ==============================================================
#include "Errors.h"
#include "Utilities.h"

#include "IPv4.h"

// Data type
/////////////////////////////////////////////////////////////////////////////

typedef struct
{
    uint32_t         mAddr;
    uint32_t         mMask;
    IPv4_AddressType mType;
}
AddressRange;

// Constants
/////////////////////////////////////////////////////////////////////////////

#define MASK_04 (0x000000f0)
#define MASK_08 (0x000000ff)
#define MASK_10 (0x000030ff)
#define MASK_12 (0x0000f0ff)
#define MASK_15 (0x0000feff)
#define MASK_16 (0x0000ffff)
#define MASK_24 (0x00ffffff)
#define MASK_32 (0xffffffff)

static const AddressRange RANGES[] =
{
    { 0xffffffff, MASK_32, IPv4_BROADCAST     },

    { 0x00000000, MASK_08, IPv4_SOFTWARE      },
    { 0x0000000a, MASK_08, IPv4_PRIVATE       },
    { 0x00004064, MASK_10, IPv4_SHARED        },
    { 0x0000007f, MASK_08, IPv4_LOOPBACK      },
    { 0x0000fea9, MASK_16, IPv4_LINK_LOCAL    },
    { 0x000010ac, MASK_12, IPv4_PRIVATE       },
    { 0x000000c0, MASK_24, IPv4_IETF          },
    { 0x000200c0, MASK_24, IPv4_DOCUMENTATION },
    { 0x006358c0, MASK_24, IPv4_RELAY         },
    { 0x0000a8c0, MASK_16, IPv4_PRIVATE       },
    { 0x000012c6, MASK_15, IPv4_TEST          },
    { 0x006433c6, MASK_24, IPv4_DOCUMENTATION },
    { 0x007100cb, MASK_24, IPv4_DOCUMENTATION },
    { 0x000000e0, MASK_04, IPv4_MULTICAST     },
    { 0x000000f0, MASK_04, IPv4_RESERVED      },
};

// --+--------------------------------------------------------------------------------+
//   |  0    1    2    3    4    5    6    7    8    9    a    b    c    d    e    f  |
// --+--------------------------------------------------------------------------------+
// 0 |   0         2                                      10                          |
// 1 |  16        18                                                                  |
// 3 |  48             51                                                             |
// 4 |  64                                                                            |
// 5 |                                          88                                    |
// 6 |                 99  100                                                        |
// 7 |      113                                                                   127 |
// a |                                         168  169            172                |
// c | 192                           198                      203                     |
// e | 224                                                                            |
// f | 240                                                                   254  255 |
// --+--------------------------------------------------------------------------------+

// Functions
/////////////////////////////////////////////////////////////////////////////

void IPv4_AddressToText(char * aOut, unsigned int aOutSize_byte, uint32_t aIn)
{
    assert(NULL != aOut         );
    assert(   0 <  aOutSize_byte);

    int lRet = sprintf_s(aOut, aOutSize_byte, "%u.%u.%u.%u", aIn & 0xff, (aIn >> 8) & 0xff, (aIn >> 16) & 0xff, aIn >> 24);
    assert(            0 <                           lRet );
    assert(aOutSize_byte > static_cast<unsigned int>(lRet));
}

IPv4_AddressType IPv4_GetAddressType(uint32_t aAddr)
{
    for (unsigned int i = 0; i < (sizeof(RANGES) / sizeof(RANGES[0])); i++)
    {
        const AddressRange & lRange = RANGES[i];

        assert(          0 != lRange.mMask);
        assert(IPv4_QTY    >  lRange.mType);
        assert(IPv4_PUBLIC != lRange.mType);

        if (lRange.mAddr == (aAddr & lRange.mMask))
        {
            return lRange.mType;
        }
    }

    return IPv4_PUBLIC;
}

uint32_t IPv4_TextToAddress(const char * aIn, bool aValidate)
{
    assert(NULL != aIn);

    uint32_t lResult;

    int lRet = inet_pton(AF_INET, aIn, &lResult);
    if (1 != lRet)
    {
        Utl_ThrowError(ERROR_404);
    }

    if (aValidate)
    {
        IPv4_Validate(lResult);
    }

    return lResult;
}

uint32_t IPv4_TextToAddress_Invert(const char * aIn)
{
    assert(NULL != aIn);

    uint32_t lResult;

    int lRet = inet_pton(AF_INET, aIn, &lResult);
    if (1 != lRet)
    {
        Utl_ThrowError(ERROR_405);
    }

    lResult = ~lResult;

    IPv4_Validate(lResult);

    return lResult;
}

void IPv4_Validate(uint32_t aAddr)
{
    if ((0 == aAddr) || (0xffffffff == aAddr))
    {
        Utl_ThrowError(ERROR_210);
    }
}

void IPv4_Validate(uint32_t aSubNet, uint32_t aMask)
{
    IPv4_Validate(aSubNet);
    IPv4_Validate(aMask  );

    if (0 == (aSubNet & aMask))
    {
        Utl_ThrowError(ERROR_211);
    }

    if (0 != (aSubNet & ~aMask))
    {
        Utl_ThrowError(ERROR_212);
    }
}

void IPv4_Validate(uint32_t aAddr, uint32_t aSubNet, uint32_t aMask)
{
    IPv4_Validate(aAddr);
    IPv4_Validate(aSubNet, aMask);

    if ((aAddr & aMask) != (aSubNet & aMask))
    {
        Utl_ThrowError(ERROR_CONFIG, __LINE__, "The address is not on the sub net");
    }

    if (0 == (aAddr & ~aMask))
    {
        Utl_ThrowError(ERROR_CONFIG, __LINE__, "The address is not valid on the sub net");
    }
}
