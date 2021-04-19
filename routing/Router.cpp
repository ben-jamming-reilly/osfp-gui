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
        if(this->all_dest.at(i) == this->routerID)) {
            cost = 0;
        }
        // Else the cost is infinity unless the router is connected with, 
        // the current one, in which case it is that link state cost
        else {
            cost = inf;
            for(int j = 0; j < connections.size(); j++) {
                if(connections.at(j).at(0) == this->all_dest.at(i)) {
                    cost = connections.at(j).at(1);
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

    std::vector<int> nprime;
    nprime.push_back(this->routerID);

    // For all destinations
    while(nprime.size() < this->all_dest.size()) {
        // Find the next destination to calculate
        int cur_w = compute_lowest_dv(nprime);
        nprime.push_back(cur_w);

        // Find all connections with cur_w that aren't already in nprime
        std::vector<std::pair<int, int>> connected_routers = this->networkLSA.find_connections_with(cur_w);
        // Remove ones already in nprime
        for(int i = 0; i < nprime.size(); i++) {
            for(int j = 0; j < connected_routers.size(); j++) {
                if(nprime.at(i) == connected_routers.at(i).first) {
                    connected_routers.erase(j);
                    j--;
                }
            }
        }


        // Find current cost to cur_w
        int dw_cost = 0;
        for(int i = 0; i < this->least_cost_destination; i++) {
            if(cur_w == get<0>(this->least_cost_destination.at(i))) {
                dw_cost = get<2>(this->least_cost_destination);
                break;
            }
        }


        // Update according to the equation D(v) = min(D(v), D(w) + c(w, v))
        for(int i = 0; i < this->least_cost_destination.size(); i++) {
            int v = get<0>(this->least_cost_destination.at(i));
            // Get c(w, v)
            int cwy = this->inf;
            for(int j = 0; j < connected_routers.size(); j++) {
                if(v == connected_routers.at(j).first) {
                    cwy = connected_routers.at(i).second;
                    break;
                }
            }

            if(dw_cost + cwy < get<2>this->least_cost_destination.at(i)) {
                get<2>this->least_cost_destination.at(i) = dw_cost + cwy;
                get<1>this->least_cost_destination.at(i) = cur_w;
            }
        }
    } 
}

std::vector<std::pair<int, int>> Router::find_connections_with(int router) {

}