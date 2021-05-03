#include <napi.h>
#include <string>
#include <iostream>
#include "router_json_parser.h"
#include "../ospf.h"
#include "../Router.h"


// For the both getForwardingTable() and getLeastCostPathsTable(),
// the expected input is given by following network topology,
//			[1]
//		 5 /   \ 1
//		[0]-----[2]
//			 11
//
// Expected stringified JSON input:
//
//  {
//    "networkTopology": [
//      [
//        0,1,5
//      ],
//      [
//        1,0,5
//      ],
//      [
//        0,2,11
//      ],
//      [
//        2,0,11
//      ],
//      [
//        1,2,1
//      ],
//      [
//        2,1,1
//      ]
//    ]
//  }


// Expected stringified JSON output:
//
//  {
//      "forwardingTable": {
//          "0": [
//            [
//              0,0,0
//            ],
//            [
//              1,0,5
//            ],
//            [
//              2,1,6
//            ]
//          ],
//          "1": [
//            [
//              0,0,5
//            ],
//            [
//              1,1,0
//            ],
//            [
//              2,2,1
//            ]
//          ],
//          "2": [
//            [
//              0,1,6
//            ],
//            [
//              1,1,1
//            ],
//            [
//              2,2,0
//            ]
//          ]
//      }
//  }
//
Napi::String getForwardingTable(const Napi::CallbackInfo& info)
{
    Napi::Env env = info.Env();
    std::string json = (std::string) info[0].ToString();

    std::vector< std::vector<int> > network_topology = parseNetworkTopology(json);
    
    
    // step #1: build up network of router objects and simulate
    //          adjacency formation by synchronizing each router's LSDB

    std::vector<Router> routers = synchronize_routers(network_topology);

    // step #2: use Dijkstra's algorithm to calculate the
    //          least cost paths table for each router

    for (int i = 0; i < routers.size(); ++i)
    {
        routers.at(i).calculate_dijkstras();
    }

    // step #3: calculate forwarding table from least cost table
    std::vector<ForwardingTable> forwardingTables;
    std::vector< std::tuple<int,int,unsigned int> > forwardingTable;
    ForwardingTable formattedForwardingTable;
    std::vector<int> entry;

    for (int i = 0; i < routers.size(); ++i)
    {
        forwardingTable = routers.at(i).generate_forwarding_table();

        // change each row from a tupple to an array
        for (int j = 0; j < forwardingTable.size(); ++j)
        {
            entry = {std::get<0>(forwardingTable.at(j)),
                     std::get<1>(forwardingTable.at(j)),
               (int) std::get<2>(forwardingTable.at(j))};
            formattedForwardingTable.push_back(entry);
        }

        // add formatted table to list
        forwardingTables.push_back(formattedForwardingTable);
    }

    // step #4: parse forwarding table and pass back through NAPI

    std::vector<int> routerIDs = parseRouterIDs(network_topology);
    std::string composed_json = composeForwardingTable(forwardingTables, routerIDs);

    return Napi::String::New(env, composed_json);
}


// Expected stringified JSON output:
//  {
//    "lowestCostPaths": {
//      "0": {
//        "0": "0,0",
//        "1": "0,1",
//        "2": "0,1,2"
//      },
//      "1": {
//        "0": "1,0",
//        "1": "1,1",
//        "2": "1,2"
//      },
//      "2": {
//        "0": "2,1,0",
//        "1": "2,1",
//        "2": "2,2"
//      }
//    }
//  }
//
Napi::String getLeastCostPathsTable(const Napi::CallbackInfo& info)
{
    Napi::Env env = info.Env();
    std::string json = (std::string) info[0].ToString();

    std::vector< std::vector<int> > network_topology = parseNetworkTopology(json);

    // step #1: build up network of router objects and simulate
    //          adjacency formation by synchronizing each router's LSDB

    std::vector<Router> routers = synchronize_routers(network_topology);

    // step #2: use Dijkstra's algorithm to calculate the
    //          least cost paths table for each router

    for (int i = 0; i < routers.size(); ++i)
    {
        routers.at(i).calculate_dijkstras();
    }

    // step #3: calculate least cost paths table from least cost table
    std::vector< std::vector<std::string> > leastCostPathsTables;
    std::vector< std::vector<unsigned int> > leastCostPathsTable;
    std::vector<std::string> formattedLeastCostPathsTable;
    std::string buffer = "";
    std::vector<unsigned int> path;
    for (int i = 0; i < routers.size(); ++i)
    {
        leastCostPathsTable = routers.at(i).generate_shortest_paths();

        // change each row from an array to a string
        for (int row = 0; row < leastCostPathsTable.size(); ++row)
        {
            path = leastCostPathsTable.at(row);
            for (int col = 0; col < path.size(); ++col)
            {
                buffer = buffer.append(std::to_string(path.at(col))).append(",");
            }
            
            // remove last extra comma
            buffer = buffer.substr(0, buffer.size() - 1);
    
            formattedLeastCostPathsTable.push_back(buffer);
        }

        leastCostPathsTables.push_back(formattedLeastCostPathsTable);
    }

    std::string composed_json = composeLeastCostPathsTable(leastCostPathsTables);

    return Napi::String::New(env, composed_json);
}


Napi::Object Init(Napi::Env env, Napi::Object exports)
{
    exports.Set(
        Napi::String::New(env, "getForwardingTable"),
        Napi::Function::New(env, getForwardingTable)
    );

    exports.Set(
        Napi::String::New(env, "getLeastCostPathsTable"),
        Napi::Function::New(env, getLeastCostPathsTable)
    );

    return exports;
}

NODE_API_MODULE(native_ospf, Init)