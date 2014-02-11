#pragma once

#include <string>
#include <vector>
#include <map>
#include "Util.h"
#include "FactorGraph.h"

using namespace std;

namespace MRF_DD {
    class Parser {
    public:
        map<string, int> feature_index;
        map<string, int> factor_index;
        
        FactorGraph graph;
        
        void LoadData(const char* data_file, FactorGraph *graph);
        vector<string> GetLabels(const char* data_file);
        int LoadGraph(ifstream &file, FactorGraph *graph);
        bool UpdateFactor(const string& key, int& index);
    };
}
