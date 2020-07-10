
// Author     KMS - Martin Dubois, P.Eng.
// Copyrights (C) 2020 KMS. All rights reserved.
// Product    NetworkHelper
// File       NHLib/Utilities.h

// CODE REVIEW 2020-07-10 KMS - Martin Dubois, P.Eng.

#pragma once

// ===== C++ ================================================================
#include <exception>

// Functions
/////////////////////////////////////////////////////////////////////////////

extern void Utl_DisplayError(int aCode, const std::exception & aException);
extern void Utl_DisplayError(const char * aErrorType, int aCode, const char * aMessage);

extern void Utl_ThrowError(const char * aErrorType, int aCode, const char * aMessage);
extern void Utl_ThrowErrorIfNeeded(int aCode, const char * aElement, const char * aName, unsigned int aErrorCount);
