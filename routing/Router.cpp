#include "Router.h"

std::vector<std::tuple<int, int, unsigned int>> Router::populate_least_cost_destination(std::vector<int> all_dest) {
    std::tuple<int, int, unsigned int> package;
    std::vector<std::tuple<int, int, unsigned int>> lcd;

    // For each destination
    for(int i = 0; i < all_dest.size(); i++) {
        // Make an entry
        package = std::make_tuple(all_dest[i], this->routerID, inf);
        lcd.push_back(package);
    }  
    // TODO grab initial connections
    return lcd;
}

void Router::calculate_dijkstras() {
    this->all_dest = generate_all_destinations();
    this->least_cost_destination = populate_least_cost_destination(this->all_dest);

    std::vector<int> nprime;
    nprime.push_back(this->routerID);

    // For all destinations
    while(nprime.size() < this->all_dest.size()) {
        // Find the next destination to calculate
        int cur_w = compute_lowest_dv(nprime);
        nprime.push_back(cur_w);

        // Find all connections with cur_w that aren't already in nprime
        std::vector<std::pair<int, int>> connected_routers = find_connections_with(cur_w);
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