//----------------------------------------------------------------------
// NAME: Benjamin Reilly
// FILE: graph.cpp
// DATE: 2/16/21
// DESC: Implements graph.h
//----------------------------------------------------------------------

#include "graph.h"

#include <iostream>

#include <stack>
#include <map>
#include <vector>
#include <algorithm>
#include <set>

//----------------------------------------------------------------------
// HW-3 graph operations

Graph::~Graph() {}

void Graph::bfs(bool dir, int src, Map &tree) const
{
  std::unordered_map<int, bool> visted;
  std::list<int> frontier_verts;

  // initialize all verts as unvisted
  for (int i = 0; i < vertex_count(); i++)
    visted[i] = false;

  // add source vert into queue
  frontier_verts.push_back(src);

  // intialize root of search tree
  tree[src] = -1;
  visted[src] = true;

  while (!frontier_verts.empty())
  {
    const int cur_vert = frontier_verts.front();
    std::list<int> adjacent_verts;

    // get all verts adjacent to the cur_vert
    if (dir)
      connected_to(cur_vert, adjacent_verts);
    else
      adjacent(cur_vert, adjacent_verts);

    // I could possible add adjacent_verts.sort() and .unique()
    // but I don't think that is needed for adjacent();

    for (int adjacent_vert : adjacent_verts)
    {
      if (!visted[adjacent_vert])
      {
        // mark vert as visted
        visted[adjacent_vert] = true;

        // add adjacent vert to search tree
        tree[adjacent_vert] = cur_vert;

        // adds another vert to explore from
        frontier_verts.push_back(adjacent_vert);
      }
    }
    frontier_verts.pop_front();
  }
}

void Graph::shortest_path_length(bool dir, int src, int dst, std::list<int> &path) const
{
  // I'm going to do a lazy implementation b/c im gonna to guess that how Bowers
  // wants it done. Also its less work :P

  int cur_vert = dst;
  Map tree;

  // Gets the search tree
  bfs(dir, src, tree);

  while (cur_vert != -1)
  {
    path.push_front(cur_vert);

    // set the next iteration to the parent of the current
    if (tree.count(cur_vert) == 0)
    {
      // if path does not exist
      path.clear();
      return;
    }

    cur_vert = tree[cur_vert];
  }
}

void Graph::bfs_connected_components(Map &components) const
{
  for (int root = 0; root < vertex_count(); root++)
  {
    if (components.count(root) > 0)
      continue;

    std::list<int> frontier_verts;
    frontier_verts.push_back(root);
    components[root] = root;

    while (!frontier_verts.empty())
    {
      const int cur_vert = frontier_verts.front();
      std::list<int> adjacent_verts;
      adjacent(cur_vert, adjacent_verts);

      for (int adjacent_vert : adjacent_verts)
      {
        if (components.count(adjacent_vert) == 0)
        {
          components[adjacent_vert] = root;
          frontier_verts.push_back(adjacent_vert);
        }
      }
      frontier_verts.pop_front();
    }
  }
}

bool Graph::bipartite_graph(Map &coloring) const
{
  enum color
  {
    blue = 0,
    green = 1
  };

  for (int start_vert = 0; start_vert < vertex_count(); start_vert++)
  {
    // skip the verts which already have been colored
    if (coloring.count(start_vert) > 0)
      continue;

    // implement a basic bfs for coloring

    // initialize the graph for searching
    std::list<int> frontier_verts;
    coloring[start_vert] = blue;
    frontier_verts.push_back(start_vert);

    while (!frontier_verts.empty())
    {
      const int cur_vert = frontier_verts.front();
      color cur_color = (color)coloring[cur_vert];

      std::list<int> adjacent_verts;

      // get all verts adjacent to the cur_vert
      adjacent(cur_vert, adjacent_verts);

      for (int adjacent_vert : adjacent_verts)
      {
        if (coloring.count(adjacent_vert) > 0 && coloring[adjacent_vert] == cur_color)
        {
          return false;
        }
        else
        {
          // this code is kinda garbage but it checks that the coloring of this vert
          // will not clash with any of its neighbors
          color new_color = cur_color == blue ? green : blue;
          std::list<int> adj_colors;

          adjacent(adjacent_vert, adj_colors);

          for (int adj_color : adj_colors)
          {
            if (coloring.count(adj_color) > 0 && coloring[adj_color] == new_color)
              return false;
          }

          // color the vert the opposite color of the current vert
          coloring[adjacent_vert] = new_color;
        }
      }
      frontier_verts.pop_front();
    }
  }

  return true;
}

//----------------------------------------------------------------------
// HW-4 graph operations
//----------------------------------------------------------------------

void Graph::dfs(bool dir, int src, Map &tree) const
{
  enum status
  {
    unvisted,
    visted,
    deadend
  };

  std::stack<int> cur_path;
  Map status;

  // initialize search status for all verts
  for (int i = 0; i < vertex_count(); i++)
    status[i] = unvisted;

  // initialize the search tree
  tree[src] = -1;

  // prepare the first step
  cur_path.push(src);
  status[src] = visted;

  while (!cur_path.empty())
  {
    std::list<int> neighbors;
    bool all_visited = true;

    if (dir)
      connected_to(cur_path.top(), neighbors);
    else
      adjacent(cur_path.top(), neighbors);

    for (int vert : neighbors)
    {
      if (status[vert] == unvisted)
      {
        // add hop to the search tree
        tree[vert] = cur_path.top();

        status[vert] = visted;
        cur_path.push(vert);
        all_visited = false;
        break;
      }
    }

    if (all_visited)
    {
      status[cur_path.top()] = deadend;
      cur_path.pop();
    }
  }
}

bool Graph::acyclic(bool dir) const
{
  enum status
  {
    unvisted = 0,
    visted = 1,
    deadend = 2
  };

  Map status;

  // initialize search status for all verts
  for (int i = 0; i < vertex_count(); i++)
    status[i] = unvisted;

  for (int start_vert = 0; start_vert < vertex_count(); start_vert++)
  {
    // skip already searched sub graphs
    if (status[start_vert] == visted || status[start_vert] == deadend)
      continue;

    std::stack<int> cur_path;
    cur_path.push(start_vert);
    status[start_vert] = visted;

    while (!cur_path.empty())
    {
      std::list<int> neighbors;
      bool all_visited = true;

      if (dir)
      {
        connected_to(cur_path.top(), neighbors);
      }
      else
      {
        adjacent(cur_path.top(), neighbors);

        if (cur_path.size() > 1)
        {
          // saves the top
          int top = cur_path.top();
          cur_path.pop();

          // removes edge from prev path
          neighbors.remove(cur_path.top());

          // restore cur_path to how it was
          cur_path.push(top);
        }
      }

      //std::cout << "Vert " << cur_path.top() << ": ";
      //print_list(neighbors, status);

      for (int vert : neighbors)
      {
        if (status[vert] == unvisted)
        {
          //std::cout << vert << "\n";
          status[vert] = visted;
          cur_path.push(vert);
          all_visited = false;
          break;
        }
        else if (status[vert] == visted)
        {
          return false;
          // need to check for case of src selfloop
        }
      }

      if (all_visited)
      {
        status[cur_path.top()] = deadend;
        cur_path.pop();
      }
    }
  }

  return true;
}

void Graph::unweighted_transitive_closure(bool dir, Graph &closed_graph) const
{
  for (int vert = 0; vert < vertex_count(); vert++)
  {
    Map search_tree;
    dfs(dir, vert, search_tree);

    for (std::pair<int, int> edge : search_tree)
    {
      if (edge.second >= 0)
        closed_graph.set_edge(vert, -69, edge.first);
    }
  }
}

bool Graph::dfs_topological_sort(Map &vertex_ordering) const
{
  enum status
  {
    unvisted,
    visted,
    deadend
  };

  Map status;
  int order = vertex_count();

  // initialize search status for all verts
  for (int i = 0; i < vertex_count(); i++)
    status[i] = unvisted;

  for (int start_vert = 0; start_vert < vertex_count(); start_vert++)
  {
    if (status[start_vert] == visted || status[start_vert] == deadend)
      continue;

    std::stack<int> cur_path;
    cur_path.push(start_vert);
    status[start_vert] = visted;

    while (!cur_path.empty())
    {
      std::list<int> neighbors;
      bool all_visited = true;

      connected_to(cur_path.top(), neighbors);

      for (int vert : neighbors)
      {
        if (status[vert] == unvisted)
        {
          status[vert] = visted;
          cur_path.push(vert);
          all_visited = false;
          break;
        }
        else if (status[vert] == visted)
        {
          return false;
          // need to check for case of src selfloop
        }
      }

      if (all_visited)
      {
        status[cur_path.top()] = deadend;
        vertex_ordering[cur_path.top()] = order;
        order--;

        cur_path.pop();
      }
    }
  }

  return true;
}

//----------------------------------------------------------------------
// HW-5 graph operations
//----------------------------------------------------------------------

void Graph::strongly_connected_components(Map &components) const
{
  enum status
  {
    unvisted = 0,
    visted = 1,
    deadend = 2
  };

  Map status;
  std::stack<int> post_time;

  for (int i = 0; i < vertex_count(); i++)
    status[i] = unvisted;

  for (int start_vert = 0; start_vert < vertex_count(); start_vert++)
  {
    if (status[start_vert] == visted || status[start_vert] == deadend)
      continue;

    std::stack<int> cur_path;
    cur_path.push(start_vert);
    status[start_vert] = visted;

    while (!cur_path.empty())
    {
      std::list<int> neighbors;
      bool all_visited = true;
      connected_from(cur_path.top(), neighbors);

      for (int vert : neighbors)
      {
        if (status[vert] == unvisted)
        {
          status[vert] = visted;
          cur_path.push(vert);
          all_visited = false;
          break;
        }
      }

      if (all_visited)
      {
        status[cur_path.top()] = deadend;
        post_time.push(cur_path.top());
        cur_path.pop();
      }
    }
  }

  for (int i = 0; i < vertex_count(); i++)
    status[i] = unvisted;

  int label = 0;

  while (!post_time.empty())
  {
    const int start_vert = post_time.top();

    if (components.count(start_vert) == 0)
    {
      // Only start search on verts that haven't been labeled yet
      std::stack<int> cur_path;
      cur_path.push(start_vert);
      status[start_vert] = visted;

      while (!cur_path.empty())
      {
        std::list<int> neighbors;
        bool all_visited = true;
        connected_to(cur_path.top(), neighbors);

        for (int vert : neighbors)
        {
          if (status[vert] == unvisted)
          {
            status[vert] = visted;
            cur_path.push(vert);
            all_visited = false;
            break;
          }
        }

        if (all_visited)
        {
          components[cur_path.top()] = label;
          status[cur_path.top()] = deadend;
          cur_path.pop();
        }
      }
      label++;
    }

    post_time.pop();
  }
}

void Graph::transitive_reduction(Graph &reduced_graph) const
{
  Map component_labels;
  std::vector<std::list<int>> component_groups(vertex_count());
  std::map<std::pair<int, int>, std::pair<int, int>> connection_bridges;

  // Strongly connected components
  strongly_connected_components(component_labels);
  for (std::pair<int, int> label : component_labels)
    component_groups[label.second].push_back(label.first);

  // Add all non-bridges to the reduced graph(just component edges), save bridges
  for (int label = 0; label < vertex_count(); label++)
  {
    if (component_groups[label].empty())
      break;

    std::list<int> *src_verts = &component_groups[label];

    for (int src_vert : *src_verts)
    {
      std::list<int> dest_verts;
      connected_to(src_vert, dest_verts);

      for (int dest_vert : dest_verts)
      {
        if (component_labels[dest_vert] == component_labels[src_vert])
        {
          /*
          // Was the previous solution find the true minimal solution but
          // not the minimum
          int edge = 0;
          get_edge(src_vert, dest_vert, edge);
          reduced_graph.set_edge(src_vert, edge, dest_vert);
          */
        }
        else
        {
          std::pair<int, int>
              connection(component_labels[src_vert], component_labels[dest_vert]),
              bridge(src_vert, dest_vert);

          connection_bridges[connection] = bridge;
        }
      }
    }
  }

  // Create a cycle for each component
  for (int label = 0; label < vertex_count(); label++)
  {
    if (component_groups[label].empty())
      break;
    std::list<int> *verts = &component_groups[label];

    if (verts->size() < 2)
      continue;

    std::list<int>::iterator iter_next = verts->begin();
    iter_next++;

    // Creates a cycle
    for (std::list<int>::iterator iter = verts->begin(); iter != verts->end(); iter++)
    {
      if (iter_next == verts->end())
        iter_next = verts->begin();

      reduced_graph.set_edge(*iter, -1, *iter_next);
      iter_next++;
    }
  }

  /*
  // Was the previous solution find the true minimal solution but
  // not the minimum
  // Reduce each component
  for (int label = 0; label < vertex_count(); label++)
  {
    if (component_groups[label].empty())
      break;

    std::list<int> *src_verts = &component_groups[label];

    for (int src_vert : *src_verts)
    {
      Map vert_span;
      std::list<int> dest_verts;

      reduced_graph.connected_to(src_vert, dest_verts);

      if (dest_verts.size() <= 1)
        continue;

      reduced_graph.dfs(true, src_vert, vert_span);

      for (int dest_vert : dest_verts)
      {
        bool match = true;
        reduced_graph.remove_edge(src_vert, dest_vert);

        for (int vert : component_groups[label])
        {
          Map removed_vert_span;
          reduced_graph.dfs(true, vert, removed_vert_span);

          if (vert_span.size() != removed_vert_span.size())
          {
            match = false;
            break;
          }
        }

        if (!match)
        {
          int edge = 0;
          get_edge(src_vert, dest_vert, edge);
          reduced_graph.set_edge(src_vert, edge, dest_vert);
        }
      }
    }
  }
  */

  // Add back reduced bridges
  for (std::pair<std::pair<int, int>, std::pair<int, int>> connection : connection_bridges)
  {
    std::pair<int, int> bridge = connection.second;

    int edge = 0;
    get_edge(bridge.first, bridge.second, edge);
    reduced_graph.set_edge(bridge.first, edge, bridge.second);
  }
}

bool Graph::directed_eulerian_path(std::list<int> &path) const
{
  enum status
  {
    unvisted,
    visted,
    deadend
  };

  int src_vert = -1, sink_vert = -1;

  // Check if Eulerian path exists and finds src and sink verts
  for (int vert = 0; vert < vertex_count(); vert++)
  {
    std::list<int> out_edges, in_edges;
    connected_to(vert, out_edges);
    connected_from(vert, in_edges);

    if (out_edges.size() == in_edges.size() + 1)
    {
      if (src_vert == -1)
        src_vert = vert;
      else
        return false;
    }
    else if (in_edges.size() == out_edges.size() + 1)
    {
      if (sink_vert == -1)
        sink_vert = vert;
      else
        return false;
    }
    else if ((out_edges.size() > in_edges.size() + 1) ||
             (in_edges.size() > out_edges.size() + 1))
    {
      return false;
    }
  }

  // Initialize the src_vert
  if (src_vert == -1)
    src_vert = 0;

  std::map<std::pair<int, int>, status> edge_status;

  // Initialize all the edges
  for (int vert = 0; vert < vertex_count(); vert++)
  {
    std::list<int> dest_verts;
    connected_to(vert, dest_verts);
    for (int dest_vert : dest_verts)
      edge_status[std::make_pair(vert, dest_vert)] = unvisted;
  }

  std::stack<int> cur_path;
  cur_path.push(src_vert);

  while (!cur_path.empty())
  {
    std::list<int> neighbors;
    bool all_visited = true;
    const int vert = cur_path.top();
    connected_to(vert, neighbors);

    for (int neighbor : neighbors)
    {
      std::pair<int, int> cur_edge(vert, neighbor);

      if (edge_status[cur_edge] == unvisted)
      {
        edge_status[cur_edge] = visted;
        cur_path.push(neighbor);
        all_visited = false;
        break;
      }
    }

    if (all_visited)
    {
      path.push_front(vert);
      cur_path.pop();
    }
  }

  return true;
}