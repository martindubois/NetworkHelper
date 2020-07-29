
// License https://creativecommons.org/licenses/by-nc-nd/4.0/legalcode
// Product NetworkHelper

/// \author    KMS - Martin Dubois, P.Eng.
/// \copyright Copyright &copy; 2020 KMS. All rights reserved.
/// \File      Includes/NH/Router.h
/// \brief     NH::Router

#pragma once

// ===== C++ ================================================================
#include <list>
#include <string>

// ===== Import/Includes ====================================================
#include <HI/CSS_Colors.h>
#include <HI/Diagram.h>
#include <HI/SVG_Document.h>
#include <HI/Shape.h>

// ===== Includes ===========================================================
#include <NH/AccessListMap.h>
#include <NH/DHCPList.h>
#include <NH/InterfaceList.h>
#include <NH/NamedObject.h>
#include <NH/NATList.h>
#include <NH/Route.h>

// ===== NHLib ==============================================================
class CheckList;
class ShapeMap;

namespace NH
{

    class NAT       ;
    class SubNetList;

    /// \brief NH::Rooter
    class Router : public NamedObject
    {

    public:

        /// \brief NH::Router::InfoType
        typedef enum
        {
            INFO_COM_PORT   ,
            INFO_CONFIG_FILE,
            INFO_LOG_FILE   ,
            INFO_TELNET     ,

            INFO_QTY
        }
        InfoType;

        Router();

        virtual ~Router();

        /// \param aSubNet     The destination subnet
        /// \param aNextRouter The next router
        /// \exception std::exception See Route::Route
        void AddRoute(const SubNet * aDestination, uint32_t aNextRouter);

        /// \param aSubNet     The destination subnet
        /// \param aNextRouter The next router
        /// \exception std::exception See Route::Route
        void AddRoute(const SubNet * aDestination, const char * aNextRouter);

        /// \param aAddr Can the router reach the address?
        /// \retval false No, the router cannot reach the address.
        /// \retval true  Yes, the router can reach the address.
        bool CanReach(uint32_t aAddr) const;

        /// \param aSubNet Can the router reach the subnet?
        /// \retval false No, the router cannot reach the subnet.
        /// \retval true  Yes, the router can reach the subnet.
        bool CanReach(const SubNet & aSubNet) const;

        /// \return This method returns the SubNetList associated to the network the router is in.
        SubNetList * GetSubNetList();

        void SetIpRouting();

        /// \param aSubNets The list of subnet associated to the network
        void SetSubNetList(SubNetList * aSubNets);

        /// \param aType  See InfoType
        /// \param aData  The data associated to the information type
        /// \param aFlags Reserved
        /// \exception std::exception
        virtual void RetrieveInfo(InfoType aType, const char * aData, unsigned int aFlags = 0);

        /// \exception std::exception
        virtual void Verify() const;

        AccessListMap mAccessLists;
        DHCPList      mDHCPs      ;
        InterfaceList mInterfaces ;
        NATList       mNATs       ;

    // Internal

        typedef std::list<Route> RouteList;

        const RouteList * GetRoutes() const;

        void Prepare(HI::Diagram * aDiagram, HI::CSS_Color aColor, const ShapeMap & aSubNetMap);

        unsigned int Verify_Internal() const;

        CheckList * mCheckList;

    private:

        Router(const Router &);

        const Router & operator = (const Router &);

        unsigned int Verify_Routes() const;

        struct
        {
            unsigned mIpRouting : 1;

            unsigned mReserved0 : 31;
        }
        mFlags;

        RouteList    mRoutes ;
        SubNetList * mSubNets;

    };

}
