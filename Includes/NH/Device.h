
// License https://creativecommons.org/licenses/by-nc-nd/4.0/legalcode
// Product NetworkHelper

/// \author    KMS - Martin Dubois, P.Eng.
/// \copyright Copyright &copy; 2020 KMS. All rights reserved.
/// \File      Includes/NH/Device.h
/// \brief     NH::Device

#pragma once

// ===== Import/Includes ====================================================
#include <HI/Diagram.h>

// ===== Includes ===========================================================
#include <NH/Interface.h>

// ===== NetworkHelperLib ===================================================
class ShapeMap;

namespace NH
{

    class SubNet;

    /// \brief NH::Device
    class Device : public Interface
    {

    public:

        Device();

        // ===== Interface ==================================================

        virtual ~Device();

    // Internal

        void Prepare(HI::Diagram * aDiagram, const ShapeMap & aSubNetMap);

    private:

        Device(const Device &);

        const Device & operator = (const Device &);

    };

}
