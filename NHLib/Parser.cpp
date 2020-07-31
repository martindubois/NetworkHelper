
// Author     KMS - Martin Dubois, P.Eng.
// Copyrights (C) 2020 KMS. All rights reserved.
// Product    NetworkHelper
// File       NHLib/Parser.h

// CODE REVIEW 2020-07-30 KMS - Martin Dubois, P.Eng.

// TEST COVERAGE 2020-07-30 KMS - Martin Dubois, P.Eng.

#include "Component.h"

// ===== C ==================================================================
#include <stdio.h>
#include <string.h>

// ===== C++ ================================================================
#include <exception>

// ===== NHLib ==============================================================
#include "Color.h"
#include "Errors.h"
#include "Utilities.h"

#include "Parser.h"

// Constants
/////////////////////////////////////////////////////////////////////////////

#define TYPE_NONE (0xffffffff)

// Static function declarations
/////////////////////////////////////////////////////////////////////////////

static void Display_Error  (const char * aLine, const char * aError);
static void Display_Ignored(const char * aLine);
static void Display_Line   (const char * aLine);

static unsigned int Split(const char * aIn, char * aOut, unsigned int aOutSize_byte, const char ** aElements, unsigned int aMax);

// Public
/////////////////////////////////////////////////////////////////////////////

unsigned int Parser::Walk(const char **aElements, unsigned int aCount, const Node * aNodes)
{
    assert(NULL != aElements);
    assert(   0 <  aCount   );
    assert(NULL != aNodes   );

    assert(NULL != aElements[0]);

    size_t lLen = strlen(aElements[0]);
    assert(0 < lLen);

    unsigned int lIndex = 0;

    while (NULL != aNodes[lIndex].mName)
    {
        if (0 == _strnicmp(aNodes[lIndex].mName, aElements[0], lLen))
        {
            if ((1 < aCount) && (NULL != aNodes[lIndex].mSubNodes))
            {
                unsigned int lResult = Walk(aElements + 1, aCount - 1, aNodes[lIndex].mSubNodes);
                if (CODE_NO_MATCH != lResult)
                {
                    return lResult;
                }
            }

            return aNodes[lIndex].mCode;
        }

        lIndex++;
    }

    if (CODE_ERROR == aNodes[lIndex].mCode)
    {
        char lMessage[128];

        int lRet = sprintf_s(lMessage, "%s is not an expected command element or enumeration value", aElements[0]);
        assert(               0 < lRet);
        assert(sizeof(lMessage) > lRet);

        Utl_ThrowError(ERROR_PARSE, __LINE__, lMessage);
    }

    return aNodes[lIndex].mCode;
}

// aRouter [-K-;RW-]
void Parser::SetRouter(NH::Router * aRouter)
{
    assert(NULL != aRouter);

    assert(NULL == mRouter);

    mRouter = aRouter;
}

void Parser::Parse(const char * aFileName)
{
    assert(NULL != aFileName);

    FILE * lFile;

    errno_t lErr = fopen_s(&lFile, aFileName, "r");
    if (0 != lErr)
    {
        Utl_ThrowError(ERROR_FILE, __LINE__, "fopen_s( , ,  )  failed");
    }

    unsigned int lErrorCount = 0;

    assert(NULL != lFile);

    char lLine[4096];

    while (NULL != fgets(lLine, sizeof(lLine), lFile))
    {
        lErrorCount += ParseLine(lLine);
    }

    int lRet = fclose(lFile);
    assert(0 == lRet);

    if (0 < lErrorCount)
    {
        char lMessage[1024];

        int lRet = sprintf_s(lMessage, ERROR_001_FMT, aFileName, lErrorCount);

        Utl_ThrowError(ERROR_001, lMessage);
    }
}

// Protected
/////////////////////////////////////////////////////////////////////////////

void Parser::Display_Comment(const char * aLine)
{
    assert(NULL != aLine);

    COLOR(GREEN);
    {
        Display_Line(aLine);
    }
    COLOR(WHITE);
}

// aCommands [-K-;R--]
Parser::Parser(const Node * aCommands, const char ** aSectionNames) : mCommands(aCommands), mRouter(NULL), mSection_Names(aSectionNames), mSection_Type(TYPE_NONE)
{
    assert(NULL != aCommands);
    assert(NULL != aSectionNames);
}

NH::Router * Parser::GetRouter()
{
    assert(NULL != mRouter);

    return mRouter;
}

// aCommand [-K-;R--]
void Parser::SetCommand(const char * aCommand)
{
    assert(NULL != aCommand);

    mCommand = aCommand;
}

unsigned int Parser::ParseLine(const char * aLine)
{
    assert(NULL != aLine);

    try
    {
        const char * lElements[  16];
        char         lLine    [1024];

        unsigned int lCount = Split(aLine, lLine, sizeof(lLine), lElements, sizeof(lElements) / sizeof(lElements[0]));
        assert(0 < lCount);

        unsigned int lCode = Walk(lElements, lCount, mCommands);
        switch (lCode)
        {
        case CODE_ERROR: assert(false);

        case CODE_IGNORE  :
        case CODE_NO_MATCH:
            Display_Ignored(aLine);
            break;

        default:
            if (Process(lCode, lElements, lCount))
            {
                Display_Line(aLine);
            }
            else
            {
                Display_Ignored(aLine);
            }
        }
    }
    catch (std::exception eE)
    {
        Display_Error(aLine, eE.what());
        return 1;
    }

    return 0;
}

NH::Object * Parser::Section_GetContext(unsigned int aType)
{
    assert(TYPE_NONE != aType);

    assert(NULL != mCommand);
    assert(NULL != mSection_Names);

    if ((TYPE_NONE == mSection_Type) || (mSection_Type != aType))
    {
        char lMessage[128];

        int lRet = sprintf_s(lMessage, ERROR_225_FMT, mCommand, mSection_Names[aType]);
        assert(lRet > 0);
        assert(lRet < sizeof(lMessage));

        Utl_ThrowError(ERROR_225, lMessage);
    }

    return mSection_Context;
}

// aContext [-K-;---]
void Parser::Section_Enter(unsigned int aType, NH::Object * aContext)
{
    assert(TYPE_NONE != aType);

    mSection_Context = aContext;
    mSection_Type    = aType;
}

void Parser::Section_Exit()
{
    mSection_Type = TYPE_NONE;
}

// Static functions
/////////////////////////////////////////////////////////////////////////////

void Display_Error(const char * aLine, const char * aError)
{
    assert(NULL != aError);

    COLOR(RED);
    {
        Display_Line(aLine);
        printf(">>>>> %s\n", aError);
    }
    COLOR(WHITE);
}

void Display_Ignored(const char * aLine)
{
    COLOR(BLUE);
    {
        Display_Line(aLine);
    }
    COLOR(WHITE);
}

void Display_Line(const char * aLine)
{
    assert(NULL != aLine);

    printf("%s", aLine);
}

unsigned int Split(const char * aIn, char * aOut, unsigned int aOutSize_byte, const char ** aElements, unsigned int aMax)
{
    assert(NULL != aIn          );
    assert(NULL != aOut         );
    assert(   0 <  aOutSize_byte);
    assert(NULL != aElements    );
    assert(   0 <  aMax         );

    unsigned int lElement  =   0;
    const char * lIn       = aIn;
    unsigned int lOutIndex =   0;
    unsigned int lResult   =   0;

    for (;;)
    {
        switch (*lIn)
        {
        case '\0':
        case '\n':
        case '\r':
            if (0 < lElement)
            {
                aOut[lOutIndex] = '\0';
            }
            return lResult;

        case ' ':
        case '\t':
            if (0 < lElement)
            {
                aOut[lOutIndex] = '\0';
                lElement = 0;
                lOutIndex++;
            }
            break;

        default:
            if (0 >= lElement)
            {
                if (aMax <= lResult)
                {
                    Utl_ThrowError(ERROR_PARSE, __LINE__, "The command contains too many elements");
                }

                aElements[lResult] = aOut + lOutIndex;
                lResult++;
            }

            aOut[lOutIndex] = *lIn;
            lElement++;
            lOutIndex++;
        }

        if (aOutSize_byte <= lOutIndex)
        {
            Utl_ThrowError(ERROR_PARSE, __LINE__, "The command is too long");
        }
        lIn++;
    }
}

void Parser::ValidateCount(const char * aCommand, unsigned int aCount, unsigned int aMin, unsigned int aMax)
{
    SetCommand(aCommand);

    ValidateCount(aCount, aMin, aMax);
}

void Parser::ValidateCount(unsigned int aCount, unsigned int aMin, unsigned int aMax)
{
    assert(0    <  aCount);
    assert(0    <  aMin);
    assert(aMin <= aMax);

    assert(NULL != mCommand);

    char lMessage[128];
    int  lRet;

    if (aMin > aCount)
    {
        lRet = sprintf_s(lMessage, ERROR_401_FMT, mCommand, aMin);
        assert(0 < lRet);
        assert(sizeof(lMessage) > lRet);

        Utl_ThrowError(ERROR_401, lMessage);
    }

    if (aMax < aCount)
    {
        lRet = sprintf_s(lMessage, ERROR_402_FMT, mCommand, aMax);
        assert(0 < lRet);
        assert(sizeof(lMessage) > lRet);

        Utl_ThrowError(ERROR_402, lMessage);
    }
}
