#include <iostream>
#include <vector>
#include "Router.h"
#include "ospf.h"
#include "native_ospf/router_json_parser.h"

std::vector<Router> synchronize_routers(std::vector< std::vector<int> > networkTopology)
{
    // step #1: initialize list of routers sorted by ID with empty LSDBs
    std::vector<int> router_ids = parseRouterIDs(networkTopology);
    std::sort(router_ids.begin(), router_ids.end());
    
    // key = router ID, value = "Router" object with same ID
    std::map<int, Router> routers;

    int router_id;
    for (int i = 0; i < router_ids.size(); ++i)
    {
        router_id = router_ids.at(i);
        routers.insert(std::pair<int, Router>(router_id, Router(router_id)));
    }

    // step #2: add direct neighbors to each router's LSDB
    
    // [src router ID, dst router ID, link cost]
    std::vector<int> link;
    RouterLSA lsa;

    for (int i = 0; i < routers.size(); ++i)
    {
        router_id = routers.at(i).getID();

        for (int link_idx = 0; link_idx < networkTopology.size(); ++link_idx)
        {
            link = networkTopology.at(link_idx);
            if (link.at(0) == router_id || link.at(1) == router_id)
            {
                // link is adjacent to current router, at to database
                lsa = {Link(link.at(0), link.at(1)), INIT_SEQ_NUM, link.at(2)};
                routers.at(i).receive_lsa(lsa);
            }
        }
    }

    // step #3: loop through routers by ID in ascending order, advertising
    //          its LSDB to its neighbors, until all LSDBs are the same. Once
    //          every router has advertised once, the round is over. Routers
    //          continue advertising until ALL databases are synchronized.

    bool synchronized = false;

    while (!synchronized)
    {
        // advertise each router database to its neighbors
        std::vector<int> neighbor_ids;
        int neighbor_id;
        std::vector<RouterLSA> advertised_database;
        
        for (int i = 0; i < routers.size(); ++i)
        {
            // get list of neighbors
            neighbor_ids.clear();
            neighbor_ids = routers.at(i).neighbors();

            // advertise database to all neighbors
            advertised_database = routers.at(i).advertise_database();
            for (int neighbor_idx = 0; neighbor_idx < neighbor_ids.size(); ++neighbor_idx)
            {
                neighbor_id = neighbor_ids.at(neighbor_idx);
                routers.at(neighbor_id).update_database(advertised_database);
            }
        } 

        // start by assuming databases are all the same
        synchronized = true;

        // check if LSDBs are all the same
        for (int i = 0; i < routers.size() - 1; ++i)
        {
            // A bit of a hack, but check for equality by
            // going through routers in order of ID. More
            // faithful emulation would check neighbors to
            // each other.
            if (!routers.at(i).adjacent(routers.at(i+1)))
            {
                // at least two routers are not
                // synchronized, continue advertising
                // another round
                synchronized = false;
                break;
            }
        }
    }

    // copy synchronized routers to a return parameter
    std::vector<Router> retVal;

    for (std::map<int,Router>::const_iterator router = routers.begin();
         router != routers.end(); ++router)
    {
        retVal.push_back(router->second);
    }

    return retVal;
}


