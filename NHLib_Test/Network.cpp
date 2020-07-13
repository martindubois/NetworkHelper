
// Author    KMS - Martin Dubois, P.Eng.
// Copyright (C) 2020 KMS. All rights reserved.
// Product   NetworkHelper
// File      NHLib_Test/Router.h

// ===== Import/Includes ====================================================
#include <KmsTest.h>

// ===== Import/Includes ====================================================
#include <HI/HTML_Document.h>

// ===== Includes ===========================================================
#include <Cisco/Router.h>
#include <NH/Device.h>
#include <NH/Interface.h>
#include <NH/Network.h>
#include <NH/SubNet.h>

// Static function declarations
/////////////////////////////////////////////////////////////////////////////

static void Test(unsigned int aTestIndex, unsigned int aRouterQty);

// Tests
/////////////////////////////////////////////////////////////////////////////

KMS_TEST_BEGIN(Network_Base)

    Test(2, 2);
    Test(3, 3);
    Test(4, 4);
    Test(5, 6);

KMS_TEST_END

KMS_TEST_BEGIN(Network_MyNetwork)

    NH::Network lN;
    NH::Router  lR_ASUS;
    NH::Router  lR_Bell;

    NH::SubNet * lSN_ASUS = lN.mSubNets.FindOrCreate("192.168.0.0", "255.255.255.0");
    NH::SubNet * lSN_Bell = lN.mSubNets.FindOrCreate("192.168.2.0", "255.255.255.0");

    lR_ASUS.SetIpRouting();
    lR_ASUS.SetName("ASUS");

    NH::Interface * lI_ASUS_WAN = lR_ASUS.mInterfaces.FindOrCreate("WAN");
    NH::Interface * lI_ASUS_1   = lR_ASUS.mInterfaces.FindOrCreate("1");

    lI_ASUS_WAN->SetDHCP  ();
    lI_ASUS_WAN->SetSubNet(lSN_Bell);

    lI_ASUS_1->SetAddress("192.168.0.1");
    lI_ASUS_1->SetSubNet (lSN_ASUS);

    lSN_ASUS->SetDHCP(&lR_ASUS, lI_ASUS_1);

    lR_Bell.SetIpRouting();
    lR_Bell.SetName("Bell");

    NH::Interface * lI_Bell_WAN = lR_Bell.mInterfaces.FindOrCreate("WAN");
    NH::Interface * lI_Bell_1   = lR_Bell.mInterfaces.FindOrCreate("1");

    lI_Bell_WAN->SetDHCP();

    lI_Bell_1->SetAddress("192.168.2.1");
    lI_Bell_1->SetSubNet (lSN_Bell);

    lSN_Bell->SetDHCP(&lR_Bell, lI_Bell_1);

    lN.AddRouter(&lR_ASUS);
    lN.AddRouter(&lR_Bell);

    NH::Device lD_Decoder;
    lD_Decoder.SetDHCP  ();
    lD_Decoder.SetName  ("Decoder");
    lD_Decoder.SetSubNet(lSN_Bell);
    lN.AddDevice(&lD_Decoder);

    NH::Device lD_FireStick;
    lD_FireStick.SetDHCP  ();
    lD_FireStick.SetName  ("FireStick");
    lD_FireStick.SetSubNet(lSN_Bell);
    lN.AddDevice(&lD_FireStick);

    NH::Device lD_S;
    lD_S.SetDHCP  ();
    lD_S.SetName  ("S");
    lD_S.SetSubNet(lSN_Bell);
    lD_S.SetWifi  ();
    lN.AddDevice(&lD_S);

    NH::Device lD_TV;
    lD_TV.SetDHCP  ();
    lD_TV.SetName  ("TV");
    lD_TV.SetSubNet(lSN_Bell);
    lN.AddDevice(&lD_TV);

    NH::Device lD_GitLab;
    lD_GitLab.SetAddress("192.168.0.15");
    lD_GitLab.SetName   ("GitLab");
    lD_GitLab.SetSubNet (lSN_ASUS);
    lD_GitLab.SetVirtual();
    lN.AddDevice(&lD_GitLab);

    NH::Device lD_MFC;
    lD_MFC.SetDHCP  ();
    lD_MFC.SetName  ("MFC-9130CW");
    lD_MFC.SetSubNet(lSN_ASUS);
    lD_MFC.SetWifi  ();
    lN.AddDevice(&lD_MFC);

    NH::Device lD_Mo;
    lD_Mo.SetDHCP  ();
    lD_Mo.SetName  ("Mo");
    lD_Mo.SetSubNet(lSN_ASUS);
    lN.AddDevice(&lD_Mo);

    NH::Device lD_PDC;
    lD_PDC.SetAddress("192.168.0.21");
    lD_PDC.SetName   ("PDC");
    lD_PDC.SetSubNet (lSN_ASUS);
    lD_PDC.SetVirtual();
    lN.AddDevice(&lD_PDC);

    NH::Device lD_VMWare;
    lD_VMWare.SetAddress("192.168.0.9");
    lD_VMWare.SetName   ("VMWare");
    lD_VMWare.SetSubNet (lSN_ASUS);
    lN.AddDevice(&lD_VMWare);

    lN.Verify();

    lN.Generate_HTML(HI::FOLDER_CURRENT, "MyNetwork", "My Network");

KMS_TEST_END

// Static functions
/////////////////////////////////////////////////////////////////////////////

void Test(unsigned int aTestIndex, unsigned int aRouterQty)
{
    assert(0 < aRouterQty);

    Cisco::Router * lCR = new Cisco::Router[aRouterQty];
    assert(NULL != lCR);

    char        lFile[256];
    NH::Network lNN;
    int         lRet;

    for (unsigned int i = 0; i < aRouterQty; i++)
    {
        lCR[i].SetSubNetList(&lNN.mSubNets);

        lRet = sprintf_s(lFile, "NHLib_Test/Tests/Test_%u_Router_%u.txt", aTestIndex, i);
        assert(            0 < lRet);
        assert(sizeof(lFile) > lRet);

        lCR[i].RetrieveInfo(NH::Router::INFO_CONFIG_FILE, lFile);

        lNN.AddRouter(lCR + i);
    }

    lNN.Verify();

    char lTitle[32];

    lRet = sprintf_s(lFile , "Test_%u", aTestIndex);
    assert(            0 < lRet);
    assert(sizeof(lFile) > lRet);

    lRet = sprintf_s(lTitle, "Test - %u", aTestIndex);
    assert(            0 < lRet);
    assert(sizeof(lFile) > lRet);

    lNN.Generate_HTML(HI::FOLDER_CURRENT, lFile, lTitle, 4 == aTestIndex ? NH::Network::FLAG_DEBUG : 0);

    delete[] lCR;
}
