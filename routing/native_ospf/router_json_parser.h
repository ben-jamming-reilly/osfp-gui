#ifndef ROUTER_JSON_PARSER_H
#define ROUTER_JSON_PARSER_H

#include <vector>
#include <cstdlib>
#include <string>
#include <iostream>
#include "../ospf.h"
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

std::vector< std::vector<int> > parseNetworkTopology(std::string);
std::vector<int> parseRouterIDs(std::vector< std::vector<int> >);
std::string composeForwardingTable(std::vector<ForwardingTable>, std::vector<int>);
std::string composeLeastCostPathsTable(std::vector< std::vector<std::string> >);

#endif
