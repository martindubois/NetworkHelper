
// Author    KMS - Martin Dubois, P.Eng.
// Copyright (C) 2020 KMS. All rights reserved.
// Product   NetworkHelper
// File      NHLib_Test/NHLib_Test.cpp

#include <KmsBase.h>

// ===== Interface ==========================================================
#include <KmsTest.h>

// Tests
/////////////////////////////////////////////////////////////////////////////

KMS_TEST_GROUP_LIST_BEGIN
    KMS_TEST_GROUP_LIST_ENTRY("Base")
KMS_TEST_GROUP_LIST_END

extern int Network_Base     ();
extern int Network_MyNetwork();
extern int Router_Base      ();

KMS_TEST_LIST_BEGIN
    KMS_TEST_LIST_ENTRY(Network_Base     , "Network - Base"      , 0, 0)
    KMS_TEST_LIST_ENTRY(Network_MyNetwork, "Network - My network", 0, 0)
    KMS_TEST_LIST_ENTRY(Router_Base      , "Router - Base"       , 0, 0)
KMS_TEST_LIST_END

KMS_TEST_MAIN
