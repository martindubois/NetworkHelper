
// License https://creativecommons.org/licenses/by-nc-nd/4.0/legalcode
// Product NetworkHelper

/// \author    KMS - Martin Dubois, P.Eng.
/// \copyright Copyright &copy; 2020 KMS. All rights reserved.
/// \file      Includes/NH/Network.h
/// \brief     NH::Network

#pragma once

// ===== C++ ================================================================
#include <list>

// ===== Import/Includes ====================================================
#include <HI/Types.h>

// ===== Includes ===========================================================
#include <NH/Device.h>
#include <NH/Object.h>
#include <NH/Router.h>
#include <NH/SubNetList.h>

namespace NH
{

    /// \brief NH::Network
    class Network : public Object
    {

    public:

        static const unsigned int FLAG_DEBUG;

        Network();

        ~Network();

        /// \param aDevice The Device to add
        void AddDevice(Device * aDevice);

        void AddKnownRouters();

        /// \param aRouter The router to add
        void AddRouter(Router * aRouter);

        void DeleteDevices();

        void DeleteRouters();

        /// \param aFolder See FOLDER_...
        /// \param aName   File name without the extension
        /// \param aTitle  Title
        /// \param aFlags  See FLAG_...
        /// \exception std::exception
        void Generate_HTML(HI::FolderId aFolder, const char * aName, const char * aTitle, unsigned int aFlags = 0);

        void Verify() const;

        SubNetList mSubNets;

    private:

        Network(const Network &);

        const Network & operator = (const Network &);

        const Router * FindRouter(uint32_t aAddr) const;

        typedef std::list<Device *> DeviceList;
        typedef std::list<Router *> RouterList;

        DeviceList mDevices;
        RouterList mRouters;

    };

}
