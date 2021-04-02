//----------------------------------------------------------------------
// Name: Benjamin Reilly
// File: adjacency_matrix.cpp
// Date: Spring 2021
// Desc: Implementation of Adjacency Matrix
//----------------------------------------------------------------------

#include "adjacency_matrix.h"

AdjacencyMatrix::AdjacencyMatrix(int vertex_count)
    : number_of_vertices(vertex_count),
      number_of_edges(0)
{
  const int num_nodes = number_of_vertices * number_of_vertices;

  // allocate the appropriate memory
  adj_matrix = new int *[num_nodes];

  for (int i = 0; i < num_nodes; i++)
    adj_matrix[i] = nullptr;
}

AdjacencyMatrix::~AdjacencyMatrix()
{
  make_empty();
}

AdjacencyMatrix::AdjacencyMatrix(const AdjacencyMatrix &rhs)
{
  number_of_vertices = rhs.vertex_count();
  number_of_edges = 0;

  const int num_nodes = number_of_vertices * number_of_vertices;
  adj_matrix = new int *[num_nodes];

  // initialize the matrix
  for (int i = 0; i < num_nodes; i++)
    adj_matrix[i] = nullptr;

  for (int v1 = 0; v1 < number_of_vertices; v1++)
  {
    for (int v2 = 0; v2 < number_of_vertices; v2++)
    {
      int edge = __INT_MAX__;
      if (rhs.get_edge(v1, v2, edge))
      {
        set_edge(v1, edge, v2);
      }
    }
  }

  number_of_edges = rhs.edge_count();
}

AdjacencyMatrix &AdjacencyMatrix::operator=(const AdjacencyMatrix &rhs)
{
  if (this != &rhs)
  {
    make_empty();

    number_of_vertices = rhs.vertex_count();
    number_of_edges = 0;

    const int num_nodes = number_of_vertices * number_of_vertices;
    adj_matrix = new int *[num_nodes];

    // initialize the matrix
    for (int i = 0; i < num_nodes; i++)
      adj_matrix[i] = nullptr;

    for (int v1 = 0; v1 < number_of_vertices; v1++)
    {
      for (int v2 = 0; v2 < number_of_vertices; v2++)
      {
        int edge = __INT_MAX__;
        if (rhs.get_edge(v1, v2, edge))
        {
          set_edge(v1, edge, v2);
        }
      }
    }

    number_of_edges = rhs.edge_count();
  }
  return *this;
}

void AdjacencyMatrix::set_edge(int v1, int edge_label, int v2)
{
  if (!has_edge(v1, v2))
  {
    // edge doesn't exist, so allocate memory for it
    adj_matrix[index(v1, v2)] = new int;
    number_of_edges++;
  }

  // update the edge value for v1 -> v2
  *(adj_matrix[index(v1, v2)]) = edge_label;
}

bool AdjacencyMatrix::has_edge(int v1, int v2) const
{
  return adj_matrix[index(v1, v2)] != nullptr;
}

bool AdjacencyMatrix::get_edge(int v1, int v2, int &edge) const
{
  if (!has_edge(v1, v2))
    return false;

  edge = *(adj_matrix[index(v1, v2)]);
  return true;
}

void AdjacencyMatrix::remove_edge(int v1, int v2)
{
  if (!has_edge(v1, v2))
    return;

  int remove_index = index(v1, v2);

  delete adj_matrix[remove_index];
  adj_matrix[remove_index] = nullptr;

  number_of_edges--;
}

void AdjacencyMatrix::connected_to(int v1, std::list<int> &vertices) const
{
  for (int i = 0; i < number_of_vertices; i++)
  {
    if (has_edge(v1, i))
      vertices.push_back(i);
  }
}

void AdjacencyMatrix::connected_from(int v2, std::list<int> &vertices) const
{
  for (int i = 0; i < number_of_vertices; i++)
  {
    if (has_edge(i, v2))
      vertices.push_back(i);
  }
}

void AdjacencyMatrix::adjacent(int v, std::list<int> &vertices) const
{
  std::list<int> connected_to_lst, connected_from_lst;

  connected_to(v, connected_to_lst);
  connected_from(v, connected_from_lst);

  for (int vert : connected_to_lst)
    vertices.push_back(vert);

  for (int vert : connected_from_lst)
    vertices.push_back(vert);
}

int AdjacencyMatrix::vertex_count() const
{
  return number_of_vertices;
}

int AdjacencyMatrix::edge_count() const
{
  return number_of_edges;
}

int AdjacencyMatrix::index(int x, int y) const
{
  // This is suppose to find the index that represents x -> y
  // in my opinion they should be flipped but whatever ...
  return (x * number_of_vertices) + y;
}

void AdjacencyMatrix::make_empty()
{
  const int num_nodes = number_of_vertices * number_of_vertices;

  for (int i = 0; i < num_nodes; i++)
  {
    if (adj_matrix[i] != nullptr)
    {
      delete adj_matrix[i];
      adj_matrix[i] = nullptr;
    }
  }

  number_of_edges = 0;
  delete[] adj_matrix;
}