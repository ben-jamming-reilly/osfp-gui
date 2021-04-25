#include <vector>
#include <cstdlib>
#include <string>
#include <iostream>
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

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