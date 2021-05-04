#include <vector>
#include <algorithm>
#include "router_json_parser.h"
#include "../ospf.h"

std::stringstream error_message;

// helper function to be called by getForwardingTable()
// and getLeastCostPathsTable() just so we don't have
// to define the error message more than once
void define_error_message()
{
    error_message << "The network topology should be passed in as stringified";
    error_message << std::endl << "JSON. For the following three-router network, the";
    error_message << std::endl << "topology would be passed in by argument as follows:";
    error_message << std::endl << std::endl;
    error_message << "          [1]" << std::endl;
    error_message << "       5 /   \\ 1" << std::endl;
    error_message << "      [0]-----[2]" << std::endl;
    error_message << "           11" << std::endl << std::endl;
    error_message << "{" << std::endl;
    error_message << "\t\"networkTopology\": [" << std::endl;
    error_message << "\t\t[0,1,5]," << std::endl;
    error_message << "\t\t[1,0,5]," << std::endl;
    error_message << "\t\t[0,2,11]," << std::endl;
    error_message << "\t\t[2,0,11]," << std::endl;
    error_message << "\t\t[1,2,1]," << std::endl;
    error_message << "\t\t[2,1,1]" << std::endl;
    error_message << "\t]" << std::endl;
    error_message << "}" << std::endl;
}

std::vector< std::vector<int> > parseNetworkTopology(std::string json)
{
    // initialize return parameter, where each element is a link:
	//	[src. router ID, dst. router ID, link cost]
    std::vector< std::vector<int> > networkTopology;

    // ParseInsitu expects dynamically allocated char*
    char buffer[json.size()+1];
    std::memcpy(buffer, json.c_str(), json.size()+1);


    // parse JSON
    rapidjson::Document document;
    document.ParseInsitu(buffer);

	std::string error;
	if (!document.IsObject() || !document.HasMember("networkTopology"))
	{
		error = "Incorrectly formatted network topology.\n" + (error_message.str());
		
		// C++ exceptions are disabled, so we just pass
		// back an empty array to signal something went wrong
		std::cout << error;
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

			// C++ exceptions are disabled, so we just pass
			// back an empty array to signal something went wrong
			std::cout << error;
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

// function assumes that routers have been synchronized and
// Dijkstra's algorithm has been called on each router already
std::vector<ForwardingTable> formatForwardingTable(std::vector<Router> routers)
{
	std::vector<ForwardingTable> forwardingTables;
    std::vector< std::tuple<int,int,unsigned int> > forwardingTable;
    ForwardingTable formattedForwardingTable;
    std::vector<int> entry;

	// format data to get ready for composing JSON
    for (size_t i = 0; i < routers.size(); ++i)
    {
        forwardingTable = routers.at(i).generate_forwarding_table();

        // change each row from a tupple to an array
        for (size_t j = 0; j < forwardingTable.size(); ++j)
        {
            entry = {std::get<0>(forwardingTable.at(j)),
                     std::get<1>(forwardingTable.at(j)),
               (int) std::get<2>(forwardingTable.at(j))};
            formattedForwardingTable.push_back(entry);
        }

        // add formatted table to list
        forwardingTables.push_back(formattedForwardingTable);

		// reset variable
		formattedForwardingTable.clear();
		formattedForwardingTable.resize(0);
    }

	return forwardingTables;
}

std::vector<int> parseRouterIDs(std::vector< std::vector<int> > networkTopology)
{
	// note that network topology links are in the following form:
	// [destination ID, source ID, link cost]

	std::vector<int> router_ids;
	int routerID;
	for (size_t i = 0; i < networkTopology.size(); ++i)
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

std::vector< std::vector<std::string> > formatLeastCostPathsTable(std::vector<Router> routers)
{
	std::vector< std::vector<std::string> > leastCostPathsTables;
    std::vector< std::vector<unsigned int> > leastCostPathsTable;
    std::vector<std::string> formattedLeastCostPathsTable;
    std::string buffer = "";
    std::vector<unsigned int> path;

    // format data to get ready for composing JSON
    for (size_t i = 0; i < routers.size(); ++i)
    {
        leastCostPathsTable = routers.at(i).generate_shortest_paths();

        // change each row from an array to a string
        for (size_t row = 0; row < leastCostPathsTable.size(); ++row)
        {
			buffer = "";
            path = leastCostPathsTable.at(row);
            for (size_t col = 0; col < path.size(); ++col)
            {
                buffer = buffer.append(std::to_string(path.at(col))).append(",");
            }
            
            // remove last extra comma
            buffer = buffer.substr(0, buffer.size() - 1);
    
            formattedLeastCostPathsTable.push_back(buffer);
        }

        leastCostPathsTables.push_back(formattedLeastCostPathsTable);

		// reset variable
		formattedLeastCostPathsTable.clear();
		formattedLeastCostPathsTable.resize(0);
    }

	return leastCostPathsTables;
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
	for (size_t i = 0; i < forwardingTables.size(); ++i)
	{
		routerID = routerIDs.at(i);
		snprintf(buffer, BUFFER_SIZE, "%u", routerID);
		json_writer.Key(buffer);
		json_writer.StartArray();

		forwardingTable = forwardingTables.at(i);
		for (size_t j = 0; j < forwardingTable.size(); ++j)
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

bool sortByDestination(const std::vector<int>& v1, const std::vector<int>& v2)
{
	// sort by second to last element (the destination router ID)
	return v1.at(v1.size() - 2) < v2.at(v2.size() - 2);
}

std::string composeLeastCostPathsTable(std::vector< std::vector<std::string> > leastCostPathsTable)
{
    rapidjson::StringBuffer sb;
    rapidjson::Writer<rapidjson::StringBuffer> json_writer(sb);

    json_writer.StartObject();
	json_writer.Key("lowestCostPaths");
	json_writer.StartArray();
	
	std::stringstream buffer;
	std::string substring;
	std::vector< std::vector<int> > paths;
	std::vector<int> path;

	for (size_t i = 0; i < leastCostPathsTable.size(); ++i)
	{
		// reset variables
		paths.clear();
		paths.resize(0);

		for (size_t j = 0; j < leastCostPathsTable.at(i).size(); ++j)
		{
			// reset variables
			path.clear();
			path.resize(0);
			buffer.clear();
			substring = "";
			
			// parse string to get paths
			buffer = (std::stringstream) leastCostPathsTable.at(i).at(j);
			while(buffer.good())
			{
				// split strings up by comma delimiter
				getline(buffer, substring, ',');
				path.push_back(std::stoi(substring));
			}
			paths.push_back(path);
		}

		// sort paths by destination
		std::sort(paths.begin(), paths.end(), sortByDestination);

		// compose JSON for given paths
		json_writer.StartArray();
		for (size_t j = 0; j < paths.size(); ++j)
		{
			json_writer.StartArray();
			for (size_t k = 0; k < paths.at(j).size(); ++k)
			{	
				json_writer.Uint(paths.at(j).at(k));
			}
			json_writer.EndArray();
		}
		json_writer.EndArray();
	}

	json_writer.EndArray();
	json_writer.EndObject();

    return sb.GetString();
}