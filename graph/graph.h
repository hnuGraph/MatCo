#ifndef GRAPH_GRAPH
#define GRAPH_GRAPH

#include <queue>
#include <tuple>
#include <vector>
#include "utils/types.h"
#include "utils/utils.h"
#include <unordered_map>
#include <bitset>

class Graph
{
protected:
    std::string name_;
    uint edge_count_;
    uint vlabel_count_;
    uint elabel_count_;
    size_t max_degree_;
    uint max_label_frequency_;
    std::vector<std::vector<uint>> neighbors_;
    std::vector<std::vector<uint>> elabels_;
    std::unordered_map<uint,std::vector<uint>> verterbylabel_;

public:
    std::vector<uint> vlabels_;

public:
    Graph();

    virtual uint NumVertices() const { return vlabels_.size(); }
    virtual uint NumEdges() const { return edge_count_; }
    uint NumVLabels() const { return vlabel_count_; }
    uint NumELabels() const { return elabel_count_; }
    uint GetDiameter() const;
    uint GetMaxDegree() const{ return max_degree_; };

    void AddVertex(uint id, uint label);
    void RemoveVertex(uint id);
    void AddEdge(uint v1, uint v2, uint label);
    void RemoveEdge(uint v1, uint v2);
    void SetName(const std::string &name) { name_ = name; }
    std::string GetName() const { return name_; }
    uint GetVertexLabel(uint u) const;
    const std::vector<uint>& GetNeighbors(uint v) const;
    const std::vector<uint>& GetNeighborLabels(uint v) const;
    uint GetDegree(uint v) const;
    const std::vector<uint>& GetVerticesByLabel(uint label) const;
    uint getGraphMaxLabelFrequency() const;
    bool checkEdgeExistence(uint v1, uint v2) const;

    std::tuple<uint, uint, uint> GetEdgeLabel(uint v1, uint v2) const;

    void LoadFromFile(const std::string &path);
    void PrintMetaData() const;
};

#endif //GRAPH_GRAPH
