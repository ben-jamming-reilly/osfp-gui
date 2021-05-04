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
//              [1, 1, 5],
//              [0, 0, 0],
//              [2, 1, 6]
//          ],
//          "1": [
//              [1, 1, 0],
//              [0, 0, 5],
//              [2, 2, 1]
//          ],
//          "2": [
//              [1, 1, 1],
//              [0, 1, 6],
//              [2, 2, 0]
//          ]
//      }
//  }
//
Napi::String getForwardingTable(const Napi::CallbackInfo& info)
{
    Napi::Env env = info.Env();

    std::string error;
    define_error_message();
    if (info.Length() == 0)
    {
        error = "Too few arguments.\n" + error_message.str();
        Napi::Error::New(env, error).ThrowAsJavaScriptException();
        return Napi::String::New(env, "");
    }

    if (info.Length() > 1)
    {
        error = "Too many arguments.\n" + error_message.str();
        Napi::Error::New(env, error).ThrowAsJavaScriptException();
        return Napi::String::New(env, "");
    }

    std::string json = (std::string) info[0].ToString();

    std::vector< std::vector<int> > network_topology = parseNetworkTopology(json);

    if (network_topology.size() == 0)
    {
        return Napi::String::New(env, "");
    }
    
    // step #1: build up network of router objects and simulate
    //          adjacency formation by synchronizing each router's LSDB

    std::vector<Router> routers = synchronize_routers(network_topology);
    
    // step #2: use Dijkstra's algorithm to calculate the
    //          least cost paths table for each router

    for (size_t i = 0; i < routers.size(); ++i)
    {
        routers.at(i).calculate_dijkstras();
    }

    // step #3: calculate forwarding table from least cost table
    std::vector<ForwardingTable> forwardingTables = formatForwardingTable(routers);

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

    std::string error;
    define_error_message();
    if (info.Length() == 0)
    {
        error = "Too few arguments.\n" + error_message.str();
        Napi::Error::New(env, error).ThrowAsJavaScriptException();
        return Napi::String::New(env, "");
    }

    if (info.Length() > 1)
    {
        error = "Too many arguments.\n" + error_message.str();
        Napi::Error::New(env, error).ThrowAsJavaScriptException();
        return Napi::String::New(env, "");
    }

    std::string json = (std::string) info[0].ToString();

    std::vector< std::vector<int> > network_topology = parseNetworkTopology(json);

    // step #1: build up network of router objects and simulate
    //          adjacency formation by synchronizing each router's LSDB

    std::vector<Router> routers = synchronize_routers(network_topology);

    // step #2: use Dijkstra's algorithm to calculate the
    //          least cost paths table for each router

    for (size_t i = 0; i < routers.size(); ++i)
    {
        routers.at(i).calculate_dijkstras();
    }

    // step #3: calculate least cost paths table from least cost table
    std::vector< std::vector<std::string> > leastCostPathsTables = formatLeastCostPathsTable(routers);

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