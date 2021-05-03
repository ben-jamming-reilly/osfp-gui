#ifndef LSDB_H
#define LSDB_H

#include "LSA.h"
#include <vector>
#include <map>
#include <algorithm>
#include <ostream>
#include <iostream>

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
    bool router_lsa_exists(RouterLSA lsa) const;

    // Returns all the Link State Advertisements in the database
    // Inputs: None
    // Outputs: Sorted vector list of Link State Advertisements (LSAs).
    std::vector<RouterLSA> advertise_database() const;

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
    size_t size() const;

    // Finds all the unique destinations and returns them
    // Inputs: None
    // Outputs: all destinations
    std::vector<int> get_all_destinations() const ;

    // Finds connections with the inputted router and returns them
    // @param int router
    // @return vector list of pair (routerID, cost)
    std::vector<std::pair<int, int>> find_connections_with(int router) const;

    // Returns a list of router IDs that are known neighbors
    // Inputs: Router ID to find neighbors relative to
    // Outputs: Router IDs of adjacent routers
    std::vector<int> neighbors(int) const;

    // Print database in friendly formatting
    // Inputs: None
    // Outputs: None
    void print() const;

    // affirms or denies if two LSDBs are equal
    bool operator== (const LSDB& rhs) const
    {
        if (this->router_lsdb.size() != rhs.router_lsdb.size())
        {
            // no sense checking anything else if
            // the databases aren't the same size
            return false;
        }

        bool retVal = true;

        std::vector<RouterLSA> lhs_lsas = this->advertise_database();
        std::vector<RouterLSA> rhs_lsas = rhs.advertise_database();

        RouterLSA lhs_lsa, rhs_lsa;

        for (size_t i = 0; i < this->router_lsdb.size(); ++i)
        {
            lhs_lsa = lhs_lsas.at(i);
            rhs_lsa = rhs_lsas.at(i);

            if (lhs_lsa.LINK != rhs_lsa.LINK ||
                lhs_lsa.LINK_COST != rhs_lsa.LINK_COST ||
                lhs_lsa.SEQ_NUM != rhs_lsa.SEQ_NUM)
            {
                // links aren't identical, link costs differ,
                // or one LSA is newer than the other
                retVal = false;
                break;
            }
        }

        return retVal;
    }

private:
    std::map<Link, RouterLSA> router_lsdb;
};

#endif
