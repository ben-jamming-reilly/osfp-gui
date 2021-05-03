#include "LSDB.h"

// checks if newer link state advertisement
// already exists for the given link
void LSDB::add_router_lsa(RouterLSA lsa)
{
    if (this->router_lsdb.count(lsa.LINK) == 0)
    {
        // LSDB does not contain LSA, add to database
        router_lsdb.insert(std::pair<Link, RouterLSA>(lsa.LINK, lsa));
    } else if (this->router_lsdb.at(lsa.LINK).SEQ_NUM < lsa.SEQ_NUM)
    {
        // LSDB has an old LSA for this link, update database
        router_lsdb.at(lsa.LINK) = lsa;
    }
}

bool LSDB::router_lsa_exists(RouterLSA lsa) const
{
    bool retVal = false;

    if (this->router_lsdb.count(lsa.LINK) != 0 &&
        this->router_lsdb.at(lsa.LINK).SEQ_NUM == lsa.SEQ_NUM)
    {
        // LSA exists in the database
        retVal = true;
    }

    return retVal;
}

// sort LSAs by link into a vector and return
std::vector<RouterLSA> LSDB::advertise_database() const
{
    std::vector<RouterLSA> retVal(this->router_lsdb.size());

    int i = 0;
    for (std::map<Link, RouterLSA>::const_iterator lsa = (this->router_lsdb).begin();
         lsa != this->router_lsdb.end(); ++lsa)
    {
        retVal.at(i) = lsa->second;
        ++i;
    }

    return retVal;
}

// update database from vector list of LSAs
void LSDB::update_database(std::vector<RouterLSA> advertised_database)
{
    // iterate through database
    RouterLSA new_lsa;

    for (size_t i = 0; i < advertised_database.size(); ++i)
    {
        new_lsa = advertised_database.at(i);

        if (this->router_lsdb.count(new_lsa.LINK) == 0 ||
            this->router_lsdb.at(new_lsa.LINK).SEQ_NUM < new_lsa.SEQ_NUM)
        {
            // new LSA, update database
            this->router_lsdb.insert(std::pair<Link, RouterLSA>(new_lsa.LINK, new_lsa));
        }
    }
}

void LSDB::clear()
{
    this->router_lsdb.clear();
}

size_t LSDB::size() const
{
    return this->router_lsdb.size();
}

// Finds all the unique destinations and returns them
// Inputs: None
// Outputs: all destinations
std::vector<int> LSDB::get_all_destinations() const
{
    std::vector<int> routers;

    // For every connection
    for (std::map<Link, RouterLSA>::const_iterator lsa = (this->router_lsdb).begin();
         lsa != this->router_lsdb.end(); ++lsa)
    {
        int dest = lsa->second.LINK.get_dest_id();
        bool new_dest = true;
        int src = lsa->second.LINK.get_src_id();
        bool new_src = true;
        // Find unique router IDs
        for(size_t i = 0; i < routers.size(); i++) {
            if(dest == routers.at(i)) {
                new_dest = false;
            }
            if(src == routers.at(i)) {
                new_src = false;
            }
        }
        // Add routers if haven't appeared before
        if(new_dest == true) {
            routers.push_back(dest);
        }
        if(new_src == true) {
            routers.push_back(src);
        }
    }
    return routers;
}

// Finds connections with the inputted router and returns them
// @param int router
// @return vector list of pair (routerID, cost)
std::vector<std::pair<int, int>> LSDB::find_connections_with(int router) const
{
    std::vector<std::pair<int, int>> connections;
    int link;
    int cost;

    // For every connection
    for (std::map<Link, RouterLSA>::const_iterator lsa = (this->router_lsdb).begin();
         lsa != this->router_lsdb.end(); ++lsa)
    {
        link = -1;
        cost = 65535;

        // See if one of the routers is the given one
        if(lsa->second.LINK.get_dest_id() == router) {
            link = lsa->second.LINK.get_src_id();
            cost = lsa->second.LINK_COST;
            
        }
        else if(lsa->second.LINK.get_src_id() == router) {
            link = lsa->second.LINK.get_dest_id();
            cost = lsa->second.LINK_COST;
        }
        // Add to the list if it is
        if(link != -1) {
            connections.push_back(std::pair<int, int>(link, cost));
            
        }
    }
    return connections;
}

// Returns a list of router IDs that are known neighbors
// Inputs: None
// Outputs: Router IDs of adjacent routers
std::vector<int> LSDB::neighbors(int router_id) const
{
    std::vector<int> retVal;

    RouterLSA router_lsa;

    for (std::map<Link, RouterLSA>::const_iterator lsa = (this->router_lsdb).begin();
         lsa != (this->router_lsdb).end(); ++lsa)
    {
        router_lsa = lsa->second;
        if (router_lsa.LINK.get_src_id() == router_id)
        {
            // destination is neighbor, add to list
            retVal.push_back(router_lsa.LINK.get_dest_id());
        } else if (router_lsa.LINK.get_dest_id() == router_id)
        {
            // source is neighbor, add to list
            retVal.push_back(router_lsa.LINK.get_src_id());
        }
    }

    return retVal;
}

// Print database in friendly formatting
// Inputs: None
// Outputs: None
void LSDB::print() const
{
    // print headers
    std::cout << "[SRC. ROUTER ID] [DST. ROUTER ID] [SEQUENCE NUMBER] [LINK COST]";
    std::cout << std::endl;
    for (std::map<Link,RouterLSA>::const_iterator lsa = (this->router_lsdb).begin();
         lsa != (this->router_lsdb).end(); ++lsa)
    {
        std::cout << "\t" << (lsa->second).LINK.get_src_id();
        std::cout << "\t\t" << (lsa->second).LINK.get_dest_id();
        std::cout << "\t      0x" << std::hex << (lsa->second).SEQ_NUM;
        std::cout << std::dec;
        std::cout << "\t " << (lsa->second).LINK_COST;
        std::cout << std::endl << std::endl;
    }
}