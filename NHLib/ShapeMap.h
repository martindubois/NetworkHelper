
// Author     KMS - Martin Dubois, P.Eng.
// Copyrights (C) 2020 KMS. All rights reserved.
// Product    NetworkHelper
// File       NHLib/ShapeMap.h

#pragma once

// ===== Import/Includes ====================================================
#include <HI/Shape.h>

// ===== C++ ================================================================
#include <map>

// Class
/////////////////////////////////////////////////////////////////////////////

class ShapeMap
{

public:

    void Add(const void * aKey, const HI::Shape * aShape);

    const HI::Shape * Get(const void * aKey) const;

private:

    typedef std::map<const void *, const HI::Shape *> InternalMap;

    InternalMap mShapes;

};
