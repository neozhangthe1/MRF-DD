#pragma once

#include <string>
#include <vector>
#include <map>
#include "Util.h"
#include "FactorGraph.h"

using namespace std;

namespace MRF_DD {
    class DataSet {
    public:
        int num_label;
        int num_sample;
        
        int num_feature_type;
        int num_factor_type;
        
        MappingDict label_dict;
        MappingDict feature_type_dict;
        MappingDict factor_type_dict;
        
        FactorGraph graph;
        
        void LoadData(const char* data_file, FactorGraph *graph);
        vector<string> GetLabels(const char* data_file);
        int LoadGraph(ifstream &file, FactorGraph *graph);
    };
}
