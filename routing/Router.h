/*
Name: Tristan Call
Date: 4/17/2021
Description: This file contains the Router class, which has all the 
various functions and variables a router would need.
*/

#include <iostream>
#include <vector>
#include <bits/stdc++.h>
#include <tuple>
#include "LSDB.h"


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


        /**
         * This function calculates starting values for the least_cost_destination
         * table
         * @return lcd
         * */
        std::vector<std::tuple<int, int, unsigned int>> populate_least_cost_destination();
    
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
         * This generates the forwarding table and returns it in the
         * form of [(destination router, current router, next node)]
         * */
        std::vector<std::tuple<int, int, unsigned int>> generate_forwarding_table() {}

        

    private:
        int routerID;
        std::vector<int> all_dest;
        // routerLSA;
        LSDB networkLSA;
        // Of form (dest node, prev node, node cost)
        std::vector<std::tuple<int, int, unsigned int>> least_cost_destination;
        const int inf = 65535;
 


};