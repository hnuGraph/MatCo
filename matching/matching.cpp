#include <unordered_set>
#include <vector>

#include "utils/types.h"
#include "graph/graph.h"
#include "matching/matching.h"

matching::matching(Graph& query_graph, Graph& data_graph,
        size_t max_num_results, 
        bool print_prep,
        bool print_enum, 
        bool homo)
: query_(query_graph)
, data_(data_graph)

, max_num_results_(max_num_results)
, print_preprocessing_results_(print_prep)
, print_enumeration_results_(print_enum)
, homomorphism_(homo)

, visited_(data_.NumVertices(), false)
, num_initial_results_(0ul)
, num_keyvertex_(0ul)
{}

void matching::Preprocessing()
{}

void matching::InitialMatching()
{}

void matching::AddEdge(uint v1, uint v2, uint label)
{
    data_.AddEdge(v1, v2, label);
}

void matching::RemoveEdge(uint v1, uint v2)
{
    data_.RemoveEdge(v1, v2);
}

void matching::AddVertex(uint id, uint label)
{
    data_.AddVertex(id, label);
}

void matching::RemoveVertex(uint id)
{
    data_.RemoveVertex(id);
}


void matching::GetNumInitialResults(size_t &num_initial_results)
{
    num_initial_results = num_initial_results_;
}
void matching::GetNumKeyVertex(size_t &num_keyvertex){
    num_keyvertex = num_keyvertex_;
}
