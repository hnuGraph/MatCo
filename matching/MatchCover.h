#ifndef MATCHING_MatCo
#define MATCHING_MatCo

#include <vector>

#include "utils/types.h"
#include "graph/graph.h"
#include "matching/matching.h"
#include <unordered_map>
#include "utils/timer.hpp"
#include <unordered_set>

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

    void Preprocessing() override;
    void InitialMatching() override;
    


private:
    void GenerateMatchingOrder();
    void BuildMutiExpMOrder();
    void BuildAdjMatrix();

    void BuildCover();
    
    bool ComputeCand(uint depth,std::vector<uint> m);
    bool FullCoveragePrune(const uint depth,const std::vector<uint> &m);

    void MutiExpansion(std::vector<uint> m);
    bool MutiExpTest(int depth,const std::vector<uint> &label_same_index, std::vector<uint> &m);

    void CountRes(std::vector<uint> m);
    void FlushFlag(uint flush_depth);


    void PrintMatch(const std::vector<uint> &m);
    
    void FindMatCo(uint depth, std::vector<uint> m);
    
    
    
};

#endif //MATCHING_GRAPHFLOW
