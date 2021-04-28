#include "Router.h"

std::vector<std::tuple<int, int, unsigned int>> Router::populate_least_cost_destination(std::vector<int> all_dest) {
    std::tuple<int, int, unsigned int> package;
    std::vector<std::tuple<int, int, unsigned int>> lcd;
    int cost = 0;
    std::vector<std::pair<int, int>> connections;
    connections = this->networkLSA.find_connections_with(this->routerID);

    // For each destination
    for(int i = 0; i < this->all_dest.size(); i++) {
        // If is this router, distance is 0
        if(this->all_dest.at(i) == this->routerID) {
            cost = 0;
        }
        // Else the cost is infinity unless the router is connected with, 
        // the current one, in which case it is that link state cost
        else {
            cost = inf;
            for(int j = 0; j < connections.size(); j++) {
                if(connections.at(j).first == this->all_dest.at(i)) {
                    cost = connections.at(j).second;
                    break;
                }
            }
        }

        // Create and add the package
        package = std::make_tuple(all_dest.at(i), this->routerID, cost);
        lcd.push_back(package);
    }  
    return lcd;
}

void Router::calculate_dijkstras() {
    this->all_dest = this->networkLSA.get_all_destinations();
    this->least_cost_destination = populate_least_cost_destination(this->all_dest);

    std::vector<int> nprime = {};
    nprime.push_back(this->routerID);

    // For all destinations
    while(nprime.size() < this->all_dest.size()) {
        // Find the next destination to calculate
        int cur_w = compute_lowest_dv(nprime);
        if(cur_w == this->routerID) {
            break;
        }
        nprime.push_back(cur_w);

        // Find all connections with cur_w that aren't already in nprime
        std::vector<std::pair<int, int>> connected_routers;
        std::vector<std::pair<int, int>> connected_routers_all = this->networkLSA.find_connections_with(cur_w);
        
        for(int i = 0; i < nprime.size(); i++) {
            for(int j = 0; j < connected_routers_all.size(); j++) {
    
                if(nprime.at(i) != connected_routers_all.at(j).first) {
                    connected_routers.push_back(connected_routers_all.at(j));

                }
            }
        }
        


        // Find current cost to cur_w
        int dw_cost = 0;
        for(int i = 0; i < this->least_cost_destination.size(); i++) {
            if(cur_w == std::get<0>(this->least_cost_destination.at(i))) {
                dw_cost = std::get<2>(this->least_cost_destination.at(i));
                break;
            }
        }
        


        // Update according to the equation D(v) = min(D(v), D(w) + c(w, v))
        for(int i = 0; i < this->least_cost_destination.size(); i++) {
            int v = std::get<0>(this->least_cost_destination.at(i));
            // Get c(w, v)
            int cwy = this->inf;
            for(int j = 0; j < connected_routers.size(); j++) {
                if(v == connected_routers.at(j).first) {
                    cwy = connected_routers.at(j).second;
                    break;
                }
            }

            if(dw_cost + cwy < std::get<2>(this->least_cost_destination.at(i))) {
                std::get<2>(this->least_cost_destination.at(i)) = dw_cost + cwy;
                std::get<1>(this->least_cost_destination.at(i)) = cur_w;
            }
        }
    } 
}

// returns the ID of a router adjacent to the routers
// in nprime but not itself in nprime
int Router::compute_lowest_dv(std::vector<int> nprime)
{
    int least_cost = this->inf;
    int cur_cost = this->inf;
    bool in_nprime = false;
    int cur_small = this->routerID;
    // For every destination
    for(int i = 0; i < this->all_dest.size(); i++) {
        int cur_val = this->all_dest.at(i);
        // Make sure not in nprime
        in_nprime = false;
        for(int j = 0; j < nprime.size(); j++) {
            if(cur_val == nprime.at(j)) {
                in_nprime = true;
                break;
            }
        }

        if(!in_nprime) {
            // Find the current cost to it
            for(int k = 0; k < this->least_cost_destination.size(); k++) {
                if(cur_val == std::get<0>(this->least_cost_destination.at(k))) {
                    cur_cost = std::get<2>(this->least_cost_destination.at(k));
                }
            }
            // See if its smaller
            if(cur_cost < least_cost) {
                least_cost = cur_cost;
                cur_small = cur_val;
            }
        }

    }
    return cur_small;
}