#include <napi.h>
#include <string>
#include "router_json_parser.h"
#include "ospf.h"


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
    
    // TODO: add code generating forwarding table
    std::vector<ForwardingTable> forwardingTable;

    std::vector<int> routerIDs = parseRouterIDs(network_topology);
    std::string composed_json = composeForwardingTable(forwardingTable, routerIDs);

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

    // TODO: add code generating least cost paths table
    std::vector < std::vector<std::string> > leastCostPathsTable;

    std::string composed_json = composeLeastCostPathsTable(leastCostPathsTable);

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