#include "router_json_parser.h"

std::vector< std::vector<int> > parseNetworkTopology(std::string json)
{
    // initialize return parameter
    std::vector< std::vector<int> > networkTopology(6, std::vector<int>(3,0));

    // ParseInsitu expects dynamically allocated char*
    char buffer[json.size()];
    std::memcpy(buffer, json.c_str(), json.size());

    // parse JSON
    rapidjson::Document document;
    document.ParseInsitu(buffer);

    const rapidjson::Value& nested_array = document["networkTopology"];

    for (rapidjson::SizeType i = 0; i < nested_array.Size(); ++i)
    {   
        const rapidjson::Value& link = nested_array[i];
        for (rapidjson::SizeType j = 0; j < nested_array[i].Size(); ++j)
        {
            networkTopology.at(i).at(j) = link[j].GetUint();
        }
    }

    return networkTopology;
}

std::string composeForwardingTable(std::vector< std::vector<int> > forwardingTable)
{
    rapidjson::StringBuffer sb;
    rapidjson::Writer<rapidjson::StringBuffer> json_writer(sb);

    	json_writer.StartObject();
	json_writer.Key("forwardingTable");
	json_writer.StartArray();
	for (int i = 0; i < forwardingTable.size(); ++i)
	{
		json_writer.StartArray();
		for (int j = 0; j < forwardingTable.at(0).size(); ++j)
		{
			json_writer.Uint(forwardingTable.at(i).at(j));
		}
		json_writer.EndArray();
	}
	json_writer.EndArray();
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