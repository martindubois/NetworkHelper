
// Author    KMS - Martin Dubois, P.Eng.
// Copyright (C) 2020 KMS. All rights reserved.
// Product   NetworkHelper
// File      NHLib/ShapeMap.cpp

// CODE REVIEW 2020-06-30 KMS - Martin Dubois, P.Eng

// TEST COVERAGE 2020-06-30 KMS - Martin Dubois, P.Eng

// ===== C ==================================================================
#include <assert.h>

// ===== NHLib ==============================================================
#include "ShapeMap.h"

// Public
/////////////////////////////////////////////////////////////////////////////

void ShapeMap::Add(const void * aKey, const HI::Shape * aShape)
{
    assert(NULL != aKey  );
    assert(NULL != aShape);

    mShapes.insert(InternalMap::value_type(aKey, aShape));
}

const HI::Shape * ShapeMap::Get(const void * aKey) const
{
    assert(NULL != aKey);

    InternalMap::const_iterator lIt = mShapes.find(aKey);
    assert(mShapes.end() != lIt);

    assert(NULL != lIt->second);

    return lIt->second;
}
