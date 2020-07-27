
// Author     KMS - Martin Dubois, P.Eng.
// Copyrights (C) 2020 KMS. All rights reserved.
// Product    NetworkHelper
// File       NHLib/Checks.h

// CODE REVIEW 2020-07-27 KMS - Martin Dubois, P.Eng.

#pragma once

// ===== NHLib ==============================================================
#include "Check.h"

// Classes
/////////////////////////////////////////////////////////////////////////////

class Check_Enabled : public Check
{

public:

    Check_Enabled(const char * aType, int aCode, const char * aMessage);

    // ===== Check ==========================================================

    virtual ~Check_Enabled();

    virtual unsigned int Verify(const NH::Interface & aElement) const;

};

class Check_Private : public Check
{

public:

    Check_Private(const char * aType, int aCode, const char * aMessage);

    // ===== Check ==========================================================

    virtual ~Check_Private();

    virtual unsigned int Verify(const NH::Interface & aElement) const;

};

class Check_Public : public Check
{

public:

    Check_Public(const char * aType, int aCode, const char * aMessage);

    // ===== Check ==========================================================

    virtual ~Check_Public();

    virtual unsigned int Verify(const NH::Interface & aElement) const;

};

class Check_Reach : public Check
{

public:

    Check_Reach(const char * aType, int aCode, const char * aMessage, uint32_t aAddr);
    Check_Reach(const char * aType, int aCode, const char * aMessage, const NH::SubNet * aSubNet);

    // ===== Check ==========================================================

    virtual ~Check_Reach();

    virtual unsigned int Verify(const NH::Router & aElement) const;

private:

    uint32_t           mAddr  ;
    const NH::SubNet * mSubNet;

};
