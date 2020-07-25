
// License https://creativecommons.org/licenses/by-nc-nd/4.0/legalcode
// Product NetworkHelper

/// \author    KMS - Martin Dubois, P.Eng.
/// \copyright Copyright &copy; 2020 KMS. All rights reserved.
/// \file      Includes/NH/NameObject.h
/// \brief     NH::NamedObject

#pragma once

// ===== C++ ================================================================
#include <string>

// ===== Includes ===========================================================
#include <NH/Object.h>

namespace NH
{

    /// \brief NH::NamedObject
    class NamedObject : public Object
    {

    public:

        /// \return This method return a pointer to an internal buffer.
        virtual const char * GetName() const;

        /// \param aOut
        /// \param aOutSize_byte
        virtual void GetName(char * aOut, unsigned int aOutSize_byte) const;

        /// \param aName
        virtual void SetName(const char * aName);

    // Internal

        // ===== Object =====================================================
        virtual void DisplayError(const char * aErrorType, int aCode, const char * aMessage) const;

    protected:

        NamedObject(const char * aTypeName);

        // ===== Object =====================================================
        virtual void ThrowError  (const char * aErrorType, int aCode, const char * aMessage) const;

        virtual void ThrowErrorIfNeeded(int aCode, unsigned int aErrorCount) const;

    private:

        std::string mName;

    };

}
