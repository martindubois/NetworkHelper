
// Author     KMS - Martin Dubois, P.Eng.
// Copyrights (C) 2020 KMS. All rights reserved.
// Product    NetworkHelper
// File       NHLib/Parser.h

// CODE REVIEW 2020-07-30 KMS - Martin Dubois, P.Eng.

#pragma once

// ===== Includes ===========================================================
#include <NH/Router.h>

// Class
/////////////////////////////////////////////////////////////////////////////

class Parser
{

public:

    enum
    {
        CODE_ERROR    = 0x7ffffffe,
        CODE_IGNORE   = 0x7ffffffd,
        CODE_NO_MATCH = 0x7ffffffc,
    };

    typedef struct Node_S
    {
        unsigned int mCode;
        const char * mName;

        const struct Node_S * mSubNodes;
    }
    Node;

    static unsigned int Walk(const char **aElements, unsigned int aCount, const Node * aNodes);

    void Parse(const char * aFileName);

    void SetRouter(NH::Router * aRouter);

protected:

    static void Display_Comment(const char * aLine);

    Parser(const Node * aCommands, const char ** aSectionNames);

    NH::Router * GetRouter();

    void SetCommand(const char * aCommand);

    virtual unsigned int ParseLine(const char * aLine);

    virtual bool Process(unsigned int aCode, const char ** aElements, unsigned int aCount) = 0;

    NH::Object * Section_GetContext(unsigned int aType);
    void         Section_Enter(unsigned int aType, NH::Object * aContext);
    void         Section_Exit();

    void ValidateCount(const char * aCommand, unsigned int aCount, unsigned int aMin, unsigned int aMax = 0xffffffff);
    void ValidateCount(                       unsigned int aCount, unsigned int aMin, unsigned int aMax = 0xffffffff);

private:

    const char * mCommand ;
    const Node * mCommands;
    NH::Router * mRouter  ;

    NH::Object  * mSection_Context;
    const char ** mSection_Names;
    unsigned int  mSection_Type;

};
