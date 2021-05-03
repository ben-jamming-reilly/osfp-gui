#include <vector>
#include <algorithm>
#include <napi.h>
#include "router_json_parser.h"
#include "../ospf.h"

std::vector< std::vector<int> > parseNetworkTopology(Napi::Env env, std::string json)
{
    // initialize return parameter, where each element is a link:
	//	[src. router ID, dst. router ID, link cost]
    std::vector< std::vector<int> > networkTopology;

    // ParseInsitu expects dynamically allocated char*
    char buffer[json.size()];
    std::memcpy(buffer, json.c_str(), json.size());

    // parse JSON
    rapidjson::Document document;
    document.ParseInsitu(buffer);

	std::string error;
	if (!document.IsObject() || !document.HasMember("networkTopology"))
	{
		error = "Incorrectly formatted network topology.\n" + error_message.str();
		Napi::Error::New(env, error).ThrowAsJavaScriptException();
		networkTopology.clear();
		networkTopology.resize(0);
		return networkTopology;
	}

    const rapidjson::Value& json_array = document["networkTopology"];

    for (rapidjson::SizeType i = 0; i < json_array.Size(); ++i)
    {   
		// add new vector to return parameter
		networkTopology.push_back(std::vector<int>(3));

        const rapidjson::Value& link = json_array[i];

		if (link.Size() != 3)
		{
			error = "Incorrectly formatted network topology.\n";
			error.append("Link arrays must contain exactly three integers.\n\n");
			error.append("Malformed link:\n\n");
			error.append("\t[");
			for (rapidjson::SizeType j = 0; j < link.Size(); ++j)
			{
				error.append(std::to_string(link[j].GetUint()));
				error.append(",");
			}
			
			// remove last unecessary comma
			error = error.substr(0, error.size() - 1);
			
			error.append("]\n\n");
			error.append(error_message.str());

			Napi::Error::New(env, error).ThrowAsJavaScriptException();
			networkTopology.clear();
			networkTopology.resize(0);
			return networkTopology;
		}

        for (rapidjson::SizeType j = 0; j < link.Size(); ++j)
        {
            networkTopology.at(i).at(j) = link[j].GetUint();
        }
    }

    return networkTopology;
}

std::vector<int> parseRouterIDs(std::vector< std::vector<int> > networkTopology)
{
	// note that network topology links are in the following form:
	// [destination ID, source ID, link cost]

	std::vector<int> router_ids;

	int routerID;
	for (int i = 0; i < networkTopology.size(); ++i)
	{
		routerID = networkTopology.at(i).at(0);	// destination ID for given link
		if (std::count(router_ids.begin(), router_ids.end(), routerID) == 0)
		{
			router_ids.push_back(routerID);
		}

		routerID = networkTopology.at(i).at(1);	// source ID for given link
		if (std::count(router_ids.begin(), router_ids.end(), routerID) == 0)
		{
			router_ids.push_back(routerID);
		}
	}

	return router_ids;

	/*
	// this implementation keeps router_ids in sorted order and uses
	// binary search to check if an ID has already been recorded

	// first router ID is guaranteed to be unique
	router_ids.push_back(networkTopology.at(0).at(0));

	int min_id = router_ids.at(0);
	int max_id = router_ids.at(0);
	for (int i = 0; i < networkTopology.size(); ++i)
	{
		// step #1: check if router id already exists
		while ()
		if (router_ids. networkTopology.at(i).at(0) )
	}
	*/
}

std::string composeForwardingTable(std::vector<ForwardingTable> forwardingTables, std::vector<int> routerIDs)
{
    rapidjson::StringBuffer sb;
    rapidjson::Writer<rapidjson::StringBuffer> json_writer(sb);

    json_writer.StartObject();
	json_writer.Key("forwardingTable");
	json_writer.StartObject();

	char buffer[10];
	int BUFFER_SIZE = 10;
	int routerID;
	ForwardingTable forwardingTable;
	std::vector<int> tableEntry;
	for (int i = 0; i < forwardingTables.size(); ++i)
	{
		routerID = routerIDs.at(i);
		snprintf(buffer, BUFFER_SIZE, "%u", routerID);
		json_writer.Key(buffer);
		json_writer.StartArray();

		forwardingTable = forwardingTables.at(i);
		for (int j = 0; j < forwardingTable.size(); ++j)
		{
			tableEntry = forwardingTable.at(j);
			json_writer.StartArray();

			// destination node
			json_writer.Uint(tableEntry.at(0));
			// first-hop router to destination node
			json_writer.Uint(tableEntry.at(1));
			// cost to destination router from router i
			json_writer.Uint(tableEntry.at(2));

			json_writer.EndArray();
		}
		json_writer.EndArray();
	}

	json_writer.EndObject();
	json_writer.EndObject();

    return sb.GetString();
}

std::string composeLeastCostPathsTable(std::vector< std::vector<std::string> > leastCostPathsTable)
{
    rapidjson::StringBuffer sb;
    rapidjson::Writer<rapidjson::StringBuffer> json_writer(sb);

    json_writer.StartObject();
	json_writer.Key("lowestCostPaths");
	json_writer.StartObject();
	
	char buffer[10];
	int BUFFER_SIZE = 10;

	for (int i = 0; i < leastCostPathsTable.size(); ++i)
	{
		snprintf(buffer, BUFFER_SIZE, "%u", i);
		json_writer.String(buffer);
		json_writer.StartObject();
		for (int j = 0; j < leastCostPathsTable.at(0).size(); ++j)
		{
			snprintf(buffer, BUFFER_SIZE, "%u", j);
			json_writer.Key(buffer);
			json_writer.String(leastCostPathsTable.at(i).at(j).c_str());
		}
		json_writer.EndObject();
	}

	json_writer.EndObject();
	json_writer.EndObject();

    return sb.GetString();
}