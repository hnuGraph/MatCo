#ifndef CSM_MATCHING_H
#define CSM_MATCHING_H

#include <vector>

#include "utils/types.h"
#include "graph/graph.h"

class matching
{
protected:
    Graph& query_;
    Graph& data_;

    // config
    const size_t max_num_results_;
    const bool print_preprocessing_results_;
    const bool print_enumeration_results_;
    const bool homomorphism_;

    // execution info
    std::vector<bool> visited_;
    size_t num_initial_results_;
    size_t num_keyvertex_;


public:
    matching(Graph& query_graph, Graph& data_graph,
        size_t max_num_results = ULONG_MAX, 
        bool print_preprocessing_results = true,
        bool print_enumeration_results = false, 
        bool homomorphism = false);
    virtual ~matching() = default;

    virtual void Preprocessing();
    virtual void InitialMatching();

    virtual void AddEdge(uint v1, uint v2, uint label);
    virtual void RemoveEdge(uint v1, uint v2);
    virtual void AddVertex(uint id, uint label);
    virtual void RemoveVertex(uint id);
    

    // get execution info
    void GetNumInitialResults(size_t &num_initial_results);
    void GetNumKeyVertex(size_t &num_keyvertex);


};

#endif 
