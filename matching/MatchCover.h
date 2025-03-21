#ifndef MATCHING_MatCo
#define MATCHING_MatCo

#include <vector>

#include "utils/types.h"
#include "graph/graph.h"
#include "matching/matching.h"
#include <unordered_map>
#include "utils/timer.hpp"
#include <unordered_set>
#include <set>
#include <fstream>
#include <sstream>

class MatCo : public matching
{
private:
    std::vector<std::vector<std::vector<uint>>> CandidateSets; 
    std::vector<std::vector<bool>> CandidateSetFlag; 
    std::vector<std::vector<bool>> adjacency_matrix ; 
    std::vector<bool> KeyVertexSet;
    std::vector<uint> match_order;
    uint mutiexp_depth_;
    uint prune_depth_;

public:
    MatCo(Graph& query_graph, Graph& data_graph, size_t max_num_results,
            bool print_prep, bool print_enum, bool homo);

    ~MatCo() override {};
    
    //brief Generate matching order ,isolate vertices and adj matrix
    void Preprocessing() override;

    void InitialMatching() override;

    void PrintKeyVertexSet();

    bool VerifyCorrectness(const std::string &kv_path);
    


private:
    void GenerateMatchingOrder();

    void BuildCP2LEOrder();

    //brief build adjacency matrix and calculate prune_depth_. 
    void BuildAdjMatrix();

    void BuildCover();
    
    bool ComputeCand(uint depth,std::vector<uint> m);

    // This function checks if the current partial match satisfies the pruning condition.
    bool FullCoveragePrune(const uint depth,const std::vector<uint> &m);

    //Optimization with Linear Enumeration
    void MutiExpansion(std::vector<uint> m);

    //DFS to find whether there is a feasible match limit by same label
    bool MutiExpTest(int depth,const std::vector<uint> &label_same_index, std::vector<uint> &m);

    void CountRes(std::vector<uint> m);
    
    void FlushFlag(uint flush_depth);

    void PrintMatch(const std::vector<uint> &m);

    void FindMatCo(uint depth, std::vector<uint> m);
    
    
};

#endif 
