
// Author     KMS - Martin Dubois, P.Eng.
// Copyrights (C) 2020 KMS. All rights reserved.
// Product    NetworkHelper
// File       NHLib/Check.h

// CODE REVIEW 2020-07-26 KMS - Martin Dubois, P.Eng.

#pragma once

// ===== C++ ================================================================
#include <string>

// ===== Includes ===========================================================
#include <NH/Interface.h>
#include <NH/Router.h>

// Class
/////////////////////////////////////////////////////////////////////////////

class Check
{

public:

    virtual ~Check();

    virtual unsigned int Verify(const NH::Interface & aElement) const;
    virtual unsigned int Verify(const NH::Router    & aElement) const;

protected:

    Check(const char * aType, int aCode);
    Check(const char * aType, int aCode, const char * aMessage);

    void SetMessage(const char * aMessage);

private:

    int          mCode   ;
    std::string  mMessage;
    const char * mType   ;

};
