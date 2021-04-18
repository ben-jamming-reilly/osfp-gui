#ifndef LSDB_H
#define LSDB_H

#include "LSA.h"
#include <vector>
#include <map>
#include <algorithm>

class LSDB
{
public:
    LSDB();
    ~LSDB();

    // Adds the new LSA to the database
    // Inputs: Link State Advertisement (LSA)
    // Outputs: None
    void add_router_lsa(LSA lsa);

    // Checks if this LSA already exists in the database
    // Inputs: Link State Advertisement (LSA)
    // Outputs: True if it exists, false if not
    bool router_lsa_exists(LSA lsa);

    // Returns all the Link State Advertisements in the database
    // Inputs: None
    // Outputs: List of Link State Advertisements (LSAs).
    vector<RouterLSA> advertise_database();

private:
    map< pair<int, int>, LSA > router_lsdb;
    int num_router_lsa;
};

#endif
