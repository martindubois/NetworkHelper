
// Author     KMS - Martin Dubois, P.Eng.
// Copyrights (C) 2020 KMS. All rights reserved.
// Product    NetworkHelper
// File       NHLib/Color.h

// CODE REVIEW 2020-06-29 KMS - Martin Dubois, P.Eng.

#pragma once

#ifdef _WIN32
    // ===== Windows =========================================================
    #include <Windows.h>
#endif

// Macros
/////////////////////////////////////////////////////////////////////////////

#define BLUE  (0x09)
#define GREEN (0x0a)
#define RED   (0x0c)
#define WHITE (0x07)

#ifdef _WIN32
    #define COLOR(A) SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (A))
#else
    #define COLOR(A)
#endif
