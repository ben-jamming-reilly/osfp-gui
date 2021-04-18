#ifndef LSDB_H
#define LSDB_H

#include "LSA.h"
#include <vector>
#include <map>
#include <algorithm>

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

private:
    std::map<Link, RouterLSA> router_lsdb;
};

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

bool LSDB::router_lsa_exists(RouterLSA lsa)
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
std::vector<RouterLSA> LSDB::advertise_database()
{
    std::vector<RouterLSA> retVal(this->router_lsdb.size());

    int i = 0;
    for (std::map<Link, RouterLSA>::iterator lsa = this->router_lsdb.begin();
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

    for (int i = 0; i < advertised_database.size(); ++i)
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

size_t LSDB::size()
{
    return this->router_lsdb.size();
}

#endif
