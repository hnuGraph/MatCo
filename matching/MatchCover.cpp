#include <algorithm>
#include <iostream>
#include <vector>
#include "utils/types.h"
#include "utils/globals.h"
#include "utils/utils.h"
#include "graph/graph.h"
#include "matching/MatchCover.h"

MatCo::MatCo(Graph& query_graph, Graph& data_graph, 
        size_t max_num_results,
        bool print_prep, 
        bool print_enum, 
        bool homo)
: matching(
    query_graph, data_graph, max_num_results, 
    print_prep, print_enum, homo)
{}


void MatCo::Preprocessing()
{
    
    GenerateMatchingOrder();
    #ifdef CP2LE
    BuildCP2LEOrder();
    #endif
    BuildAdjMatrix();
}

void MatCo::BuildAdjMatrix(){
    
    //build adjacency matrix of query graph
    adjacency_matrix.reserve(query_.NumVertices());
    for(int i = 0; i<query_.NumVertices();i++){
        adjacency_matrix.emplace_back();
        adjacency_matrix[i].reserve(query_.NumVertices());
        adjacency_matrix[i].resize(query_.NumVertices(),false);
    }
    
    for(int i = 0; i<query_.NumVertices();i++){
        auto q_nbrs = query_.GetNeighbors(i);
        for(auto j: q_nbrs){
            adjacency_matrix[i][j] = true;
            adjacency_matrix[j][i] = true;
        }
    }

    //compute prune_depth_, When search depth > prune_depth_, it means no local candidate space is null. 
    std::vector<bool> AllRN(query_.NumVertices(), false);
    for(uint i = 0; i < query_.NumVertices()-1; i++)
    {
        std::fill(AllRN.begin(),AllRN.end(),false);
        for(uint s = 0 ; s <= i ; s++)
        {
            uint uf = match_order[s];
            auto nbrs = query_.GetNeighbors(uf);
            for(auto j: nbrs)
            {
                AllRN[j] = true;
            }
        }
        bool flag = true;
        for(uint j = i +1 ; j < query_.NumVertices(); j++)
        {
            uint ub = match_order[j];
            if(!AllRN[ub]) {
                flag = false;
                break;
            }
        }
        if(flag) {
            prune_depth_ = i;
            break;
        }
    }
    if(print_preprocessing_results_) std::cout<<"prune_depth_: "<<prune_depth_<<std::endl;
}
void MatCo::BuildCP2LEOrder(){
    if(print_preprocessing_results_) std::cout<<"BuildCP2LEOrder...."<<std::endl;
    mutiexp_depth_ = UINT32_MAX;
    int sum = 0;
    for(int i = query_.NumVertices()-1; i>=0; i--){
        uint u = match_order[i];
        bool flag = true;
        for(int j = i+1; j<query_.NumVertices(); j++){
            uint v = match_order[j];
            if(query_.checkEdgeExistence(u,v)){
                flag = false;
                break;
            }
        }
        if(flag) {
            sum++;
            for(int j = i; j<query_.NumVertices()-1; j++){
                match_order[j] = match_order[j+1];
            }
            match_order[query_.NumVertices()-1] = u;
        }
    }
    mutiexp_depth_ = query_.NumVertices()-sum;
    std::reverse(match_order.begin() + mutiexp_depth_,  match_order.end());
    if(print_preprocessing_results_){
        std::cout<<"CP2LEOrder: "<<std::endl;
        for(int i =0 ; i<query_.NumVertices();i++){
            std::cout<<match_order[i]<<" ";
        }
        std::cout<<std::endl;
        std::cout<<"CP2LE_depth_:"<<mutiexp_depth_;
    }
    std::cout<<std::endl;
    
}
void MatCo::GenerateMatchingOrder()
{

    uint max_degree = 0u;
    uint max_degree_id = 0u;
    for (uint i = 0; i < query_.NumVertices(); i++)
    {
        if (query_.GetDegree(i) > max_degree)
        {
            max_degree = query_.GetDegree(i);
            max_degree_id=i;
        }
    }
    match_order.resize(query_.NumVertices());
    std::vector<bool> visited(query_.NumVertices(), false);
    match_order[0] = max_degree_id;
    visited[max_degree_id] = true;

    for (uint i = 1; i < query_.NumVertices(); ++i)
    {
        uint max_adjacent = 0;
        uint max_adjacent_u = NOT_EXIST;

        for (size_t j = 0; j < query_.NumVertices(); j++)
        {
            uint cur_adjacent = 0u;
            if (visited[j]) continue;

            auto& q_nbrs = query_.GetNeighbors(j);
            for (auto& other : q_nbrs)
                if (visited[other])
                    cur_adjacent++;
            if (!cur_adjacent) continue;

            if (
                max_adjacent_u == NOT_EXIST ||
                (cur_adjacent == max_adjacent &&
                    query_.GetDegree(j) > query_.GetDegree(max_adjacent_u)) ||
                cur_adjacent > max_adjacent
            ) {
                max_adjacent = cur_adjacent;
                max_adjacent_u = j;
            }
        }

        match_order[i] = max_adjacent_u;
        visited[max_adjacent_u] = true;
    }
    
    //print order
    if(print_preprocessing_results_){
        std::cout<<"origin match order: "<<std::endl;
        for(uint i = 0; i < query_.NumVertices(); i++)
        {
            std::cout<<match_order[i]<<" ";
        }
        std::cout<<std::endl;
    }
    
}


void MatCo::InitialMatching()
{
    BuildCover();
}

void MatCo::BuildCover()
{
    uint max_degree_ = data_.GetMaxDegree();
    uint qsize_ = query_.NumVertices();
    //initialize local candidate space
    CandidateSets.reserve(qsize_);
    for (int i = 0; i < qsize_; ++i) {
        CandidateSets.emplace_back();
        CandidateSets[i].reserve(qsize_);
        for (int j = 0; j < qsize_; ++j) {
            CandidateSets[i].emplace_back();
            CandidateSets[i][j].reserve(max_degree_); // the candidate set size will not exceed the maximum degree of the data graph
        }
    }

    //initialize flags of local candidate space. 
    //True means the elements of the candidate set are all key vertices.
    CandidateSetFlag.reserve(qsize_);
    for(int i = 0; i<qsize_;i++){
        CandidateSetFlag.emplace_back();
        CandidateSetFlag[i].resize(qsize_,false);
    }

    std::vector<uint> m(query_.NumVertices(), UNMATCHED);
    KeyVertexSet = std::vector<bool>(data_.NumVertices(), false); //initialize key vertex flags

    for (uint i = 0; i < data_.NumVertices(); i++)
    if (data_.GetVertexLabel(i) != NOT_EXIST)
    if (query_.GetVertexLabel(match_order[0]) == data_.GetVertexLabel(i))
    {
        m[match_order[0]] = i;
        visited_[i] = true;

        FindMatCo(1, m);

        visited_[i] = false;
        m[match_order[0]] = UNMATCHED;
    }
}




void MatCo::FindMatCo(uint depth, std::vector<uint> m)
{
    if (reach_time_limit) return;
    if (num_initial_results_ >= max_num_results_) return; //max_num_results_ is set to ULONG_MAX by default 
    if(!ComputeCand(depth,m)) {
        empty_set_num++; // exist empty candidate set, return
        return;
    }
    
    //prune by local candidate space
    #ifdef FullCoverage
    if(depth>=prune_depth_&&FullCoveragePrune(depth,m)) return;
    #endif

    //optimization with linear enumeration
    #ifdef CP2LE 
    if(depth == mutiexp_depth_){
        MutiExpansion(m);
        return;
    }
    #endif

    // normal enumeration
    uint u = match_order[depth];
    if(CandidateSets[depth][depth].size() == 0){
        for (size_t i = 0; i < data_.NumVertices(); i++)
        if (data_.GetVertexLabel(i) != NOT_EXIST)
        if (query_.GetVertexLabel(u) == data_.GetVertexLabel(i))
        {
            CandidateSets[depth][depth].push_back(i);
        }
    }
    if(CandidateSets[depth][depth].size() == 0) return;
    for(uint i =0 ; i< CandidateSets[depth][depth].size();i++){
        uint v = CandidateSets[depth][depth][i];
        if (!homomorphism_ && visited_[v]) continue;
        m[u] = v;
        visited_[v] = true;
        
        if (depth == query_.NumVertices() - 1)
        {   
            num_initial_results_++;
            for(auto j: m) {
                if(KeyVertexSet[j]) continue;
                else{
                    num_keyvertex_++;
                    KeyVertexSet[j] = true;
                }
            }
            if(print_enumeration_results_) PrintMatch(m);
        }
        else
        {
            FindMatCo(depth + 1, m);
        }
        visited_[v] = false;
        m[u] = UNMATCHED;
        if (num_initial_results_ >= max_num_results_) return;
        if (reach_time_limit) return;
    }
}

void MatCo::MutiExpansion(std::vector<uint> m){

    std::vector<uint> label_same_index ;
    std::vector<bool> label_check(query_.NumVertices(),false);
    for(uint i = mutiexp_depth_;i<query_.NumVertices();i++){
        if(label_check[i]) continue;
        uint uf = match_order[i];
        label_check[i] = true;
        label_same_index.clear();
        label_same_index.push_back(i);
        //find the vertices with the same label in isolated vertices
        for(uint j = mutiexp_depth_;j<query_.NumVertices();j++){
            if(label_check[j]) continue;
            uint ub = match_order[j];
            if(query_.GetVertexLabel(uf) == query_.GetVertexLabel(ub)){
                label_same_index.push_back(j);
                label_check[j] = true;
            }  
        }
        bool flag = false;
        uint u_index = label_same_index[0];
        uint u = match_order[u_index];
        //DFS to find whether there is a feasible match limit by same label
        for(int s = 0 ; s <CandidateSets[mutiexp_depth_][u_index].size();s++){
            uint v = CandidateSets[mutiexp_depth_][u_index][s];
            if(visited_[v]) continue;
            visited_[v] = true;
            m[u] = v;
            if(MutiExpTest(1,label_same_index,m)) {
                visited_[v] = false;
                flag = true;
                break;
            }
            m[u] = UNMATCHED;
            visited_[v] = false;
        }
        if(!flag) return; //no feasible match
    }
    //Now, we can reduce Cartesian products into linear enumeration.
    CountRes(m);
    FlushFlag(mutiexp_depth_-1);
}
void MatCo::CountRes(std::vector<uint> m){
    //if current match m exists uncovered vertices, then it is a result.
    bool flag_all_cv = true ;
    for(uint i = 0 ; i<query_.NumVertices();i++){
        uint u = match_order[i];
        if(KeyVertexSet[m[u]]==false){
            KeyVertexSet[m[u]] = true;
            num_keyvertex_++;
            flag_all_cv = false ; 
        }
    }
    if(!flag_all_cv){
        num_initial_results_++;
        if(print_enumeration_results_) PrintMatch(m);
    }
    //Go through local candidate space of and use uncovered vertices to replace m[u]
    for(uint i = mutiexp_depth_;i<query_.NumVertices();i++){
        for(auto cand:CandidateSets[mutiexp_depth_][i]){
            if(KeyVertexSet[cand]==false){
                KeyVertexSet[cand] = true;
                num_keyvertex_++;
                num_initial_results_++;
                if(print_enumeration_results_){
                    uint u = match_order [i] ; 
                    m[u] = cand ;
                    PrintMatch(m);
                }
            }
        }
    }
    for(uint i = mutiexp_depth_;i<query_.NumVertices();i++){
        uint u = match_order [i] ; 
        m[u] = UNMATCHED ;
    }
}

//When backtracking, flush the flag of local candidate space
void MatCo::FlushFlag(uint flush_depth){
    uint u = match_order[flush_depth];
    for(uint i = flush_depth+1;i<query_.NumVertices();i++){
        uint ub = match_order[i];
        if(adjacency_matrix[u][ub]) continue;
        // the vertex,which will backtracking, is not adjacent to the local candidate space
        // so the flag of local candidate space will be true.(all elements of the candidate set are key vertices)
        else{
            CandidateSetFlag[flush_depth][i] = true;
        }
    }
}
void MatCo::PrintMatch(const std::vector<uint> &m){
    for(auto j: m)
    {
        std::cout << j << " ";
    }
    std::cout << std::endl;
}

void MatCo::PrintKeyVertexSet(){
    std::cout<<"Key vertices: ";
    for(uint i = 0 ; i<KeyVertexSet.size();i++){
        if(KeyVertexSet[i]) std::cout<<i<<" ";
    }
    std::cout<<std::endl;
}

bool MatCo::VerifyCorrectness(const std::string& kvPath) {
    std::ifstream inputFile(kvPath);
    if (!inputFile.is_open()) {
        std::cerr << "file path error: " << kvPath << std::endl;
        return false;
    }

    std::set<int> kvSet;
    for(auto i = 0; i < KeyVertexSet.size(); i++){
        if(KeyVertexSet[i]) kvSet.insert(i);
    }

    std::set<int> verifySet;
    std::string line;
    while (std::getline(inputFile, line)) {
        std::stringstream ss(line);
        std::string number;
        
        while (std::getline(ss, number, ',')) {
            try {
                verifySet.insert(std::stoi(number));
            } catch (const std::invalid_argument&) {
                std::cerr << "Invalid: " << number << std::endl;
                return false;
            }
        }
    }

    //VerifyCorrectness
    return kvSet == verifySet;
}

//DFS to find whether there is a feasible match limit by same label
bool MatCo::MutiExpTest(int depth,const std::vector<uint> &label_same_index, std::vector<uint> &m) {
    if(reach_time_limit) return false;
    if(depth==label_same_index.size()) return true;
    uint u_index = label_same_index[depth];
    uint u = match_order[u_index];
    for(int i = 0 ; i < CandidateSets[mutiexp_depth_][u_index].size();i++){
        uint v = CandidateSets[mutiexp_depth_][u_index][i];
        if(visited_[v]) continue;
        visited_[v]=true;
        m[u] = v;
        if(MutiExpTest(depth+1, label_same_index,m)) {
            visited_[v]=false;
            return true;
        }
        m[u] = UNMATCHED;
        visited_[v]=false;
    }
    return false;
    
}


bool MatCo::ComputeCand(uint depth,std::vector<uint> m){
    uint uf = match_order[depth-1]; // uf is the last matched query vertex
    uint vf = m[uf]; // vf is the matched vertex of uf
    auto vf_nbrs = data_.GetNeighbors(vf);
    for(uint i = depth;i<query_.NumVertices();i++){
        uint ub = match_order[i];
        if(adjacency_matrix[uf][ub]){ // uf and ub are adjacent
            CandidateSets[depth][i].clear();
            //local candidate space is null
            if(CandidateSets[depth-1][i].size() == 0){
                for(auto vf_nei : vf_nbrs){
                if (data_.GetVertexLabel(i) != NOT_EXIST)
                if(data_.GetVertexLabel(vf_nei) == query_.GetVertexLabel(ub)){
                        CandidateSets[depth][i].push_back(vf_nei);
                    }
                } 
            }
            //local candidate space is not null, intersect with the neighbors of vf
            else{ 
                std::set_intersection(CandidateSets[depth-1][i].begin(),CandidateSets[depth-1][i].end(),vf_nbrs.begin(),vf_nbrs.end(),std::back_inserter(CandidateSets[depth][i]));
            }
            
            if(CandidateSets[depth][i].size() == 0){
                return false;
            }
        }
        else{ // uf and ub are not adjacent, the local candidate space need not modify
            CandidateSets[depth][i] = CandidateSets[depth-1][i];
        }
    }
    return true;

}


bool MatCo::FullCoveragePrune(uint depth,const std::vector<uint> &m){

    //check if the current matched vertices are all key vertices
    for(uint i = 0 ; i< depth ; i++){
        uint u= match_order[i];
        if(!KeyVertexSet[m[u]]) {
            // flush the flag of local candidate space
            for(uint i = depth; i<query_.NumVertices(); i++){
                CandidateSetFlag[depth][i] = CandidateSetFlag[depth-1][i];
            }
            return false;
        }
    }
    //check if the local candidate space are all key vertices
    for(uint i = depth; i<query_.NumVertices(); i++){
        if(CandidateSets[depth][i].size()==0) {
            for(uint j = i; j<query_.NumVertices(); j++){
                        CandidateSetFlag[depth][j] = CandidateSetFlag[depth-1][j];
                    }
                    return false;
        }
        if(CandidateSetFlag[depth-1][i]){   
            CandidateSetFlag[depth][i] = true;
        }
        else{
            CandidateSetFlag[depth][i] = true;
            for(auto v: CandidateSets[depth][i]){
                if(!KeyVertexSet[v]) {
                    for(uint j = i; j<query_.NumVertices(); j++){
                        CandidateSetFlag[depth][j] = CandidateSetFlag[depth-1][j];
                    }
                    return false;
                }
            }
        }
    }
    //scuccessful prune and flush the flag of local candidate space
    FlushFlag(depth-1);  
    return true;
}




