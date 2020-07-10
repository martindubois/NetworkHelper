
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
