/*
Name: Tristan Call
Date: 4/17/2021
Description: This file contains the Router class, which has all the 
various functions and variables a router would need.
*/

#include <iostream>
#include <vector>
//#include <bits/stdc++.h>
#include <tuple>
#include <iostream>
#include "LSDB.h"

#ifndef ROUTER_H
#define ROUTER_H

class Router {
    public:
        Router(int routerID) {
            this->routerID = routerID;
        }
        int getID() {
            return this->routerID;
        }

        void set_networkLSD(LSDB network) {
            this->networkLSA = network;
        }

        std::vector<std::tuple<int, int, unsigned int>> get_least_cost_dest() {
            return this->least_cost_destination;
        }

        // Adds Link-State Advertisement to database
        // Inputs: RouterLSA (Link object, Seq #, Link Cost)
        // Outputs: None
        void receive_lsa(RouterLSA lsa) {
            this->networkLSA.add_router_lsa(lsa);
        }

        // Test if two routers share the same LSDB
        // Inputs: Router to test against
        // Outputs: True is LSDBs are equal, false otherwise
        bool adjacent(Router router)
        {
            return (this->networkLSA == router.networkLSA);
        }

        // Returns list of known neighbors
        // Inputs: None
        // Outputs: List of router IDs
        std::vector<int> neighbors()
        {
            return (this->networkLSA.neighbors(this->getID()));
        }

        // Allows routers to advertise adjacencies between one another
        // Inputs: None
        // Outputs: Contents of the LSDB
        std::vector<RouterLSA> advertise_database()
        {
            return (this->networkLSA).advertise_database();
        }

        // Allows routers to receive new adjacency information. Doesn't change
        // LSDB is all of the LSAs are older than those already present in LSDB
        // Inputs: List of RouterLSAs (Link between Routers (IDs), Seq #, Cost)
        // Outputs: None
        void update_database(std::vector<RouterLSA> advertised_database)
        {
            (this->networkLSA).update_database(advertised_database);
        }

        // Print the Link-State Database contained in the router in a friendly way
        // Inputs: None
        // Outputs: None
        void print() const
        {
            (this->networkLSA).print();
        }

        /**
         * This function calculates starting values for the least_cost_destination
         * table
         * @return lcd
         * */
        std::vector<std::tuple<int, int, unsigned int>> populate_least_cost_destination(std::vector<int> all_dest);
    
        /**
         * This function calculates Dijkstra's algorithm
         * Note: It modifies the least_cost_destination
         * */
        void calculate_dijkstras();

        /**
         * Computes the destination with the lowest cost not in nprime and returns it
         * @param nprime, list of routerIDs
         * @return next value to add to nprime
         * */
        int compute_lowest_dv(std::vector<int> nprime);

        /**
         * This generates the shortest paths to a given distination
         * @return vector (vector(start, ..., end, cost))
         * */
        std::vector<std::vector<unsigned int>> generate_shortest_paths();

        /**
         * This generates the forwarding table and returns it in the
         * @return [(destination router, first hop router, cost)]
         * */
        std::vector<std::tuple<int, int, unsigned int>> generate_forwarding_table();        

    private:
        int routerID;
        std::vector<int> all_dest;
        // routerLSA;
        LSDB networkLSA;
        // Of form (dest node, prev node, node cost)
        std::vector<std::tuple<int, int, unsigned int>> least_cost_destination;
        const int inf = 65535;
 


};

#endif
