#include <iostream>
#include <vector>


class Router {
    public:
        Router(int routerID) {
            this.routerID = routerID;
        }
        int getID() {
            return this.routerID;
        }

        void lsac() {}

        /**
         * This function populates the least_cost_destination table
         * with starting code
         * */
        void populate_least_cost_destination() {
            int inf = 65535;
            std::vector<(int, int, unsigned int)> lcd;
            /*
            for(i = 0; i < this.routerLSA.size(); i++) {
                // For each destination
                
                int id = router.getID();
                package = (id, this.routerID, inf);
                lcd.append(package)
            }
            */
            this.least_cost_destination = lcd;

        }
    
        /**
         * This function calculates Dijkstra's algorithm
         * Note: It modifies the least_cost_destination
         * */
        void calculate_dijkstras() {

        }

        /**
         * This generates the forwarding table and returns it in the
         * form of [(destination router, current router, next node)]
         * */
        [(int, int, unsigned int)] generate_forwarding_table() {}

    private:
        std::vector<(Router, int)> neighbors;
        int routerID;
        // routerLSA;
        // networkLSA;
        std::vector<(int, int, unsigned int)> least_cost_destination;
 


}