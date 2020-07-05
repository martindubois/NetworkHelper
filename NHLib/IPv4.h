
// Author     KMS - Martin Dubois, P.Eng.
// Copyrights (C) 2020 KMS. All rights reserved.
// Product    NetworkHelper
// File       NHLib/IPv4.h

#pragma once

// ===== C ==================================================================
#include <stdint.h>

// Functions
/////////////////////////////////////////////////////////////////////////////

extern void     IPv4_AddressToText       (char * aOut, unsigned int aOutSize_byte, uint32_t aIn);
extern uint32_t IPv4_TextToAddress       (const char * aIn);
extern uint32_t IPv4_TextToAddress_Invert(const char * aIn);
extern void     IPv4_Validate            (uint32_t aAddr);
extern void     IPv4_Validate            (                uint32_t aSubNet, uint32_t aMask);
extern void     IPv4_Validate            (uint32_t aAddr, uint32_t aSubNet, uint32_t aMask);
