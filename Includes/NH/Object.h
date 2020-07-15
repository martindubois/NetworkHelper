
// License https://creativecommons.org/licenses/by-nc-nd/4.0/legalcode
// Product NetworkHelper

/// \author    KMS - Martin Dubois, P.Eng.
/// \copyright Copyright &copy; 2020 KMS. All rights reserved.
/// \file      Includes/NH/Object.h
/// \brief     NH::Object

#pragma once

namespace NH
{

    /// \brief NH::Object
    class Object
    {

    public:

        /// \return This method returns the value passed to the constructor.
        const char * GetObjectType() const;

    protected:

        Object(const char * aObjectType);

        virtual void DisplayError(const char * aErrorType, int aCode, const char * aMessage) const;
        virtual void ThrowError  (const char * aErrorType, int aCode, const char * aMessage) const;

        virtual void ThrowErrorIfNeeded(int aCode, unsigned int aErrorCount) const;

    private:

        const char * mObjectType;

    };

}
