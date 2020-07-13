
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
#include <NH/InterfaceList.h>
#include <NH/Route.h>

// ===== NHLib ==============================================================
class ShapeMap;

namespace NH
{

    class SubNetList;

    /// \brief NH::Rooter
    class Router
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

        static const unsigned int FLAG_NO_ECHO;

        Router();

        virtual ~Router();

        /// \param aRoute
        void AddRoute(const Route & aRoute);

        /// \param aSubNet
        /// \param aAddr
        void AddRoute(const SubNet * aSubNet, uint32_t aAddr);

        /// \param aSubNet
        /// \param aAddr
        void AddRoute(const SubNet * aSubNet, const char * aAddr);

        /// \param aAddr
        /// \retval false Cannot reach
        /// \retval true  Can reach
        bool CanReach(uint32_t aAddr) const;

        /// \return This method returns the SubNetList associated to the network the router is in.
        SubNetList * GetSubNetList();

        void SetIpRouting();

        /// \param aName The name
        void SetName(const char * aName);

        /// \param aSubNets The list of subnet associated to the network
        void SetSubNetList(SubNetList * aSubNets);

        /// \param aType  See INFO_...
        /// \param aData  The data associated to the information type
        /// \param aFlags See FLAG_...
        /// \exception std::exception
        virtual void RetrieveInfo(InfoType aType, const char * aData, unsigned int aFlags = 0);

        virtual void Verify() const;

        AccessListMap mAccessLists;
        InterfaceList mInterfaces ;

    // Internal

        typedef std::list<Route> RouteList;

        const RouteList * GetRoutes() const;

        void Prepare(HI::Diagram * aDiagram, HI::CSS_Color aColor, const ShapeMap & aSubNetMap);

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

        std::string  mName   ;
        RouteList    mRoutes ;
        SubNetList * mSubNets;

    };

}
