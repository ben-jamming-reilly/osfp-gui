#ifndef OSPF_H
#define OSPF_H

#include <vector>
#include <algorithm>
#include <iostream>
#include "graph.h"

#define INF     unsigned ((int) ~0)

typedef std::pair<unsigned int, unsigned int> Pair;

std::vector<Pair> dijkstra(const Graph& graph, int node)
{
    // intialize return parameter
    std::vector<Pair> lowest_cost_table(graph.num_nodes);
    for (int i = 0; i < graph.num_nodes; ++i)
    {
        lowest_cost_table.at(i) = Pair(INF, INF);
    }

    // cost from node to itself is always zero
    lowest_cost_table.at(node) = Pair(0, node);

    // used to keep track of progression of algorithm
    std::vector<int> visited_nodes;
    std::vector<int> unvisited_nodes;
    int new_neighbor;
    int cost, lowest_cost_node;
    Edge cur_link;

    // we've already "visited" the current node
    visited_nodes.push_back(node);

    // Algorithm is O(n^2) complexity worst-case
    for (int iter = 0; iter < graph.num_nodes - 1; ++iter)
    {
        for (int i = 0; i < graph.edge_list.size(); ++i)
        {
            cur_link = graph.edge_list.at(i);
            // if neighbor to last visited node
            if (cur_link.src == visited_nodes.back())
            {
                new_neighbor = cur_link.dst;
                cost = cur_link.weight + lowest_cost_table.at(cur_link.src).first;
                // check to see if it provides a shorter path
                if (cost < lowest_cost_table.at(new_neighbor).first)
                {
                    // new lowest-cost path, update table
                    lowest_cost_table.at(new_neighbor) = Pair(cost, cur_link.src);
                }
            }
        }

        for (int j = 0; j < graph.num_nodes; ++j)
        {
            if (std::find(visited_nodes.begin(), visited_nodes.end(), j) == visited_nodes.end())
            {
                // node j has not been visited, add to list
                unvisited_nodes.push_back(j);
            }
        }

        lowest_cost_node = unvisited_nodes.at(0);

        for (int k = 0; k < unvisited_nodes.size(); ++k)
        {
            if (lowest_cost_table.at(unvisited_nodes.at(k)).first < 
                lowest_cost_table.at(lowest_cost_node).first)
            {
                // node has minimum cost and is unvisited
                lowest_cost_node = unvisited_nodes.at(k);
            }
        }

        // lowest cost node is finally added to the list
        // of visited nodes and the algorithm repeats
        visited_nodes.push_back(lowest_cost_node);
        unvisited_nodes.clear();
    }

    return lowest_cost_table;
}

void print(const std::vector<Pair>& lowest_cost_table)
{
    std::cout << "| DST | COST | PREV NODE |" << std::endl;
    for (int i = 0; i < lowest_cost_table.size(); ++i)
    {
        std::cout << "   " << static_cast<char>(i+'u') << "     " << (unsigned int) lowest_cost_table.at(i).first
                  << "\t   " << static_cast<char>(lowest_cost_table.at(i).second+'u') << std::endl;
    }
    std::cout << std::endl;
}

void printForwardingTable(const std::vector<Pair>& lowest_cost_table, int node)
{
    int cur_node, prev_node;

    std::cout << "| DST | OUTGOING LINK |" << std::endl;
    for (int i = 0; i < lowest_cost_table.size(); ++i)
    {
        std::cout << "   ";
        std::cout << static_cast<char>(i+'u');
        std::cout << "          ";
        
        cur_node = i;
        prev_node = i;
        while (cur_node != node)
        {
            prev_node = cur_node;
            cur_node = lowest_cost_table.at(cur_node).second;
        }

        std::cout << static_cast<char>(prev_node+'u') << std::endl;
    }
}

void printShortestPaths(const std::vector<Pair>& lowest_cost_table, int node)
{
    int cur_node, prev_node;
    for (int i = 0; i < lowest_cost_table.size(); ++i)
    {
        cur_node = i;
        prev_node = i;
        while (cur_node != node)
        {
            std::cout << " " << static_cast<char>(cur_node+'u') << " <-";
            prev_node = cur_node;
            cur_node = lowest_cost_table.at(cur_node).second;
        }
        std::cout << " " << static_cast<char>(cur_node+'u') << std::endl;
    }
}

#endif