
// Author     KMS - Martin Dubois, P.Eng.
// Copyrights (C) 2020 KMS. All rights reserved.
// Product    NetworkHelper
// File       NHLib/Utilities.h

// CODE REVIEW 2020-07-07 KMS - Martin Dubois, P.Eng.
#pragma once

// ===== C++ ================================================================
#include <exception>

// Constants
/////////////////////////////////////////////////////////////////////////////

#define UTL_CALLER_ERROR "CALLER ERROR"
#define UTL_CONFIG_ERROR "CONFIG ERROR"
#define UTL_ERROR        "ERROR"
#define UTL_FILE_ERROR   "FILE ERROR"
#define UTL_PARSE_ERROR  "PARSE ERROR"

// Functions
/////////////////////////////////////////////////////////////////////////////

extern void Utl_DisplayError(int aCode, const std::exception & aException);
extern void Utl_DisplayError(const char * aErrorType, int aCode, const char * aMessage);

extern void Utl_ThrowError(const char * aErrorType, int aCode, const char * aMessage);
extern void Utl_ThrowErrorIfNeeded(int aCode, const char * aElement, const char * aName, unsigned int aErrorCount);
