
// License https://creativecommons.org/licenses/by-nc-nd/4.0/legalcode
// Product NetworkHelper

/// \author    KMS - Martin Dubois, P.Eng.
/// \copyright Copyright &copy; 2020 KMS. All rights reserved.
/// \file      Includes/NH/Route.h
/// \brief     NH::Route

#pragma once

// ===== C ==================================================================
#include <stdint.h>

// ===== Includes ===========================================================
#include <NH/Object.h>

namespace NH
{

    class SubNet;

    /// \brief NH::Route
    class Route : public Object
    {

    public:

        /// \param aSubNet
        /// \param aNextRouter
        Route(const SubNet * aSubNet, uint32_t aNextRouter);

        /// \param aSubNet
        /// \param aNextRouter
        Route(const SubNet * aSubNet, const char * aNextRouter);

        ~Route();

        /// \return This method return the address of the router to use to reach the SubNet.
        uint32_t GetNextRouter() const;

        /// \param aOut
        /// \param aOutSize_byte
        void GetNextRouter(char * aOut, unsigned int aOutSize_byte) const;

        /// \param aOut
        /// \param aOutSize_byte
        void GetDescription(char * aOut, unsigned int aOutSize_byte) const;

        /// \return This method return the destination SubNet
        const SubNet * GetSubNet() const;

        /// \param aSubNet
        /// \retval false
        /// \retval true
        bool Match(const SubNet & aSubNet) const;

    private:

        void Init(const SubNet * aSubNet, uint32_t aNextRouter);

        uint32_t mNextRouter;

        const SubNet * mSubNet;

    };

}
