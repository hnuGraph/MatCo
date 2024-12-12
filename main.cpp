#include <iostream>
#include <filesystem>
#include "utils/timer.hpp"
#include "matching/MatchCover.h"
#include "utils/globals.h"
#include "utils/CLI11.hpp"
#include <sys/resource.h>
#include <sys/time.h>

int main(int argc, char *argv[]){
    CLI::App app{"App description"};
    bool print_enum = false, print_prep = false, print_key_vertice = false;
    std::string data_graph_path;
    std::string query_graph_path;
    std::string verify_correctness;
    app.add_option("-d,--data_graph_path", data_graph_path, "data_graph_path")->required();
    app.add_option("-q,--query_graph_path", query_graph_path, "query_graph_path")->required();
    app.add_option("-e,--print_enum", print_enum, "print_enum")->default_val(false);
    app.add_option("-p,--print_prep", print_prep, "print_prep")->default_val(false);
    app.add_option("-k,--print_key_vertice", print_key_vertice, "print_key_vertice")->default_val(false);
    app.add_option("-v,--verify_correctness", verify_correctness, "verify_correctness")->default_val("");

    CLI11_PARSE(app, argc, argv);

    Graph data_graph;
    std::cout<<"data graph loading..."<<std::endl;
    data_graph.LoadFromFile(data_graph_path);
    data_graph.PrintMetaData();
    Graph query_graph;
    std::cout<<"query graph loading..."<<std::endl;
    query_graph.LoadFromFile(query_graph_path);
    query_graph.PrintMetaData();

    int mem_start = mem::getMemUsage();
    Timer t;
    t.StartTimer();

    MatCo *matco = nullptr;
    
    matco  = new MatCo  (query_graph, data_graph, ULONG_MAX, print_prep, print_enum, false);
    std::cout<<"MatCo preprocessing..."<<std::endl;

    matco->Preprocessing();

    std::cout<<"MatCo initial matching..."<<std::endl;
   
    auto InitialFun = [&matco]()
    {   
        matco->InitialMatching();
    };
    execute_with_time_limit(InitialFun, 600*12, reach_time_limit);

    int mem_end = mem::getMemUsage();
    std::cout<<"MatCo finished..."<<std::endl;
    size_t MatCo_num_results = 0ul;
    size_t MatCo_num_kv = 0ul;
    matco->GetNumKeyVertex(MatCo_num_kv);
    matco->GetNumInitialResults(MatCo_num_results);
    double time_cost = t.StopTimer_ms();
    std::cout<<"Memory usage: "<<mem_end-mem_start<<" KB"<<std::endl;
    std::cout<<"MatCo time: "<<time_cost<<" ms"<<std::endl;
    std::cout<<"MatCo match cover size: "<<MatCo_num_results<<std::endl;
    std::cout<<"MatCo the number of key vertices: "<<MatCo_num_kv<<std::endl;
    if(print_key_vertice) matco->PrintKeyVertexSet();
    if(verify_correctness!="") {
        if(matco->VerifyCorrectness(verify_correctness)) std::cout<<"Correctness verified!"<<std::endl;
        else std::cout<<"Correctness not verified!"<<std::endl;
    }
    return 0;

}



