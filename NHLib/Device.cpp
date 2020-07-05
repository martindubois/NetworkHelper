
// Author     KMS - Martin Dubois, P.Eng.
// Copyrights (C) 2020 KMS. All rights reserved.
// Product    NetworkHelper
// File       NHLib/Device.cpp

// CODE REVIEW 2020-06-30 KMS - Martin Dubois, P.Eng.

// TEST COVERAGE 2020-06-30 KMS - Martin Dubois, P.Eng.

// ===== C ==================================================================
#include <assert.h>

// ===== Includes ===========================================================
#include <NH/Device.h>

// Constants
/////////////////////////////////////////////////////////////////////////////

#define NAME_DEFAULT "Device"

namespace NH
{

    // Public
    ////////////////////////////////////////////////////////////////////////

    Device::Device() : Interface(NAME_DEFAULT)
    {
    }

    // ===== HI::Shape ======================================================

    Device::~Device()
    {
    }

    // Internal
    ////////////////////////////////////////////////////////////////////////

    void Device::Prepare(HI::Diagram * aDiagram, const ShapeMap & aSubNetMap)
    {
        HI::Shape * lShape = new HI::Shape("Device", GetName());
        assert(NULL != lShape);

        Interface::Prepare(aDiagram, lShape, aSubNetMap);
    }

}
