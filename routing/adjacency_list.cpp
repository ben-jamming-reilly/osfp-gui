//----------------------------------------------------------------------
// Name: Benjamin Reilly
// File: adjacency_list.cpp
// Date: Spring 2021
// Desc: Implementation of Adjacency List
//----------------------------------------------------------------------

#include "adjacency_list.h"

AdjacencyList::AdjacencyList(int vertex_count)
    : number_of_vertices(vertex_count),
      number_of_edges(0)
{
  // allocate arrays of lists
  adj_list_out = new Node *[number_of_vertices];
  adj_list_in = new Node *[number_of_vertices];

  // initialize to nullptr
  for (int i = 0; i < number_of_vertices; i++)
  {
    adj_list_out[i] = nullptr;
    adj_list_in[i] = nullptr;
  }
}

AdjacencyList::~AdjacencyList()
{
  make_empty();
}

AdjacencyList::AdjacencyList(const AdjacencyList &rhs)
{
  number_of_vertices = rhs.vertex_count();
  number_of_edges = 0;

  // Allocate the proper amount of memory
  adj_list_out = new Node *[number_of_vertices];
  adj_list_in = new Node *[number_of_vertices];

  // initialize all values to nullptr
  for (int i = 0; i < number_of_vertices; i++)
  {
    adj_list_out[i] = nullptr;
    adj_list_in[i] = nullptr;
  }

  for (int i = 0; i < number_of_vertices; i++)
  {
    Node *rhs_cur = rhs.adj_list_out[i];

    while (rhs_cur != nullptr)
    {
      set_edge(i, rhs_cur->edge, rhs_cur->vertex);
      rhs_cur = rhs_cur->next;
    }
  }
  // this might not be necessary but keep for rn
  number_of_edges = rhs.edge_count();
}

AdjacencyList &AdjacencyList::operator=(const AdjacencyList &rhs)
{
  if (this != &rhs)
  {
    make_empty();
    number_of_vertices = rhs.vertex_count();
    number_of_edges = 0;

    // Allocate the proper amount of memory
    adj_list_out = new Node *[number_of_vertices];
    adj_list_in = new Node *[number_of_vertices];

    // initialize all values to nullptr
    for (int i = 0; i < number_of_vertices; i++)
    {
      adj_list_out[i] = nullptr;
      adj_list_in[i] = nullptr;
    }

    for (int i = 0; i < number_of_vertices; i++)
    {
      Node *rhs_cur = rhs.adj_list_out[i];

      while (rhs_cur != nullptr)
      {
        set_edge(i, rhs_cur->edge, rhs_cur->vertex);
        rhs_cur = rhs_cur->next;
      }
    }
    // this might not be necessary but keep for rn
    number_of_edges = rhs.edge_count();
  }
  return *this;
}

void AdjacencyList::set_edge(int v1, int edge_label, int v2)
{
  bool found_out_edge = false,
       found_in_edge = false;

  // assumes v1 -> v2 and are between [0, num_verts]
  Node **cur_out_ptr = &adj_list_out[v1];

  while (*cur_out_ptr != nullptr)
  {
    if ((*cur_out_ptr)->vertex == v2)
    {
      found_out_edge = true;
      break;
    }
    // gets the ptr to next ptr. IK, its confusing but it makes the code
    // eloquent as hell
    cur_out_ptr = &((*cur_out_ptr)->next);
  }

  if (found_out_edge)
  {
    // update edge value for the node
    (*cur_out_ptr)->edge = edge_label;
  }
  else
  {
    // add edge to the list
    Node *addedNode = new Node;
    addedNode->edge = edge_label;
    addedNode->vertex = v2;
    addedNode->next = nullptr;

    // appends new node to end of list
    *cur_out_ptr = addedNode;
  }

  Node **cur_in_ptr = &adj_list_in[v2];

  while (*cur_in_ptr != nullptr)
  {
    if ((*cur_in_ptr)->vertex == v1)
    {
      found_in_edge = true;
      break;
    }

    cur_in_ptr = &((*cur_in_ptr)->next);
  }

  if (found_in_edge)
  {
    // update edge value for the node
    (*cur_in_ptr)->edge = edge_label;
  }
  else
  {
    // add edge to the list
    Node *addedNode = new Node;
    addedNode->edge = edge_label;
    addedNode->vertex = v1;
    addedNode->next = nullptr;

    // appends new node to end of list
    *cur_in_ptr = addedNode;
  }

  if (!found_out_edge && !found_in_edge)
  {
    // This should execute every single time a node is added
    number_of_edges++;
    // if there is an discrepancy in edge count then it is likely
    // caused by this function
  }
}

bool AdjacencyList::has_edge(int v1, int v2) const
{
  // only have to check one adj_list cause the lists are redundant
  Node *cur = adj_list_out[v1];

  while (cur != nullptr)
  {
    if (cur->vertex == v2)
    {
      return true;
    }
    cur = cur->next;
  }

  return false;
}

bool AdjacencyList::get_edge(int v1, int v2, int &edge) const
{
  // only have to check one adj_list cause the lists are redundant
  Node *cur = adj_list_out[v1];

  while (cur != nullptr)
  {
    if (cur->vertex == v2)
    {
      edge = cur->edge;
      return true;
    }
    cur = cur->next;
  }

  return false;
}

void AdjacencyList::remove_edge(int v1, int v2)
{
  bool found_in_vert = false, found_out_vert = false;
  Node **cur_out_ptr = &adj_list_out[v1];

  while (*cur_out_ptr != nullptr)
  {
    if ((*cur_out_ptr)->vertex == v2)
    {
      Node *removed = *cur_out_ptr;
      *cur_out_ptr = removed->next;
      delete removed;
      found_out_vert = true;

      break;
    }

    cur_out_ptr = &((*cur_out_ptr)->next);
  }

  Node **cur_in_ptr = &adj_list_in[v2];

  while (*cur_in_ptr != nullptr)
  {
    if ((*cur_in_ptr)->vertex == v1)
    {
      Node *removed = *cur_in_ptr;
      *cur_in_ptr = removed->next;
      delete removed;
      found_in_vert = true;

      break;
    }

    cur_in_ptr = &((*cur_in_ptr)->next);
  }

  if (found_out_vert && found_in_vert)
    number_of_edges--;
}

void AdjacencyList::connected_to(int v1, std::list<int> &vertices) const
{
  Node *cur = adj_list_out[v1];
  while (cur != nullptr)
  {
    vertices.push_back(cur->vertex);
    cur = cur->next;
  }
}

void AdjacencyList::connected_from(int v2, std::list<int> &vertices) const
{
  Node *cur = adj_list_in[v2];
  while (cur != nullptr)
  {
    vertices.push_back(cur->vertex);
    cur = cur->next;
  }
}

void AdjacencyList::adjacent(int v, std::list<int> &vertices) const
{
  std::list<int> connected_to_lst, connected_from_lst;

  connected_to(v, connected_to_lst);
  connected_from(v, connected_from_lst);

  // modern c++ baby
  for (int vert : connected_to_lst)
    vertices.push_back(vert);

  for (int vert : connected_from_lst)
    vertices.push_back(vert);
}

int AdjacencyList::vertex_count() const
{
  return number_of_vertices;
}

int AdjacencyList::edge_count() const
{
  return number_of_edges;
}

void AdjacencyList::make_empty()
{
  for (int i = 0; i < number_of_vertices; i++)
  {
    // pops the adj_list_out until it is empty
    while (adj_list_out[i] != nullptr)
    {
      Node *removedVert = adj_list_out[i];

      adj_list_out[i] = removedVert->next;

      delete removedVert;
    }

    // pops the adj_list_in until it is empty
    while (adj_list_in[i] != nullptr)
    {
      Node *removedVert = adj_list_in[i];
      adj_list_in[i] = removedVert->next;

      delete removedVert;
    }
  }

  // I might move this outside of this method but its fine
  // for rn
  delete[] adj_list_out;
  delete[] adj_list_in;

  number_of_edges = 0;
}