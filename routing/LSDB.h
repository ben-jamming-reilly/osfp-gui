#ifndef LSDB_H
#define LSDB_H

#include "LSA.h"
#include <vector>
#include <map>
#include <algorithm>
#include <ostream>

class LSDB
{
public:
    // Adds the new LSA to the database
    // Inputs: Link State Advertisement (LSA)
    // Outputs: None
    void add_router_lsa(RouterLSA lsa);

    // Checks if this LSA already exists in the database
    // Inputs: Link State Advertisement (LSA)
    // Outputs: True if it exists, false if not
    bool router_lsa_exists(RouterLSA lsa);

    // Returns all the Link State Advertisements in the database
    // Inputs: None
    // Outputs: Sorted vector list of Link State Advertisements (LSAs).
    std::vector<RouterLSA> advertise_database();

    // Takes advertised Link State Advertisements and updates its database
    // Inputs: Vector of Link State Advertisements (LSAs)
    // Outputs: None
    void update_database(std::vector<RouterLSA> advertised_database);

    // Clears the database of all Link State Advertisements
    // Inputs: None
    // Outputs: None
    void clear();

    // Returns the number of Link State Advertisements in the database
    // Inputs: None
    // Outputs: Size of database
    size_t size();

    // Finds all the unique destinations and returns them
    // Inputs: None
    // Outputs: all destinations
    std::vector<int> get_all_destinations();

    
    // Finds connections with the inputted router and returns them
    // @param int router
    // @return vector list of pair (routerID, cost)
    std::vector<std::pair<int, int>> find_connections_with(int router);

private:
    std::map<Link, RouterLSA> router_lsdb;
};

#endif
