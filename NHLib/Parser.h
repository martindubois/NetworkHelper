
// Author     KMS - Martin Dubois, P.Eng.
// Copyrights (C) 2020 KMS. All rights reserved.
// Product    NetworkHelper
// File       NHLib/Parser.h

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

    static void ValidateCount(const char * aCommand, unsigned int aCount, unsigned int aMin, unsigned int aMax = 0xffffffff);

    static unsigned int Walk(const char **aElements, unsigned int aCount, const Node * aNodes);

    void Parse(const char * aFileName);

    void SetRouter(NH::Router * aRouter);

protected:

    static void Display_Comment(const char * aLine);

    Parser(const Node * aCommands);

    NH::Router * GetRouter();

    virtual unsigned int ParseLine(const char * aLine);

    virtual bool Process(unsigned int aCode, const char ** aElements, unsigned int aCount) = 0;

private:

    const Node * mCommands;
    NH::Router * mRouter  ;

};
