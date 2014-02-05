#include "Parser.h"
#include "FactorGraph.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include "Parser.h"
#include "Util.h"

#define MAX_BUF_SIZE 65536

using namespace std;

namespace MRF_DD {
    vector<string> DataSet::GetLabels(const char* data_file) {
        char buf[MAX_BUF_SIZE];
        char *eof;
        set<string> set_labels;
        vector<string> tokens;
        
        FILE* fin = fopen(data_file, "r");
        for (;;) {
            eof = fgets(buf, MAX_BUF_SIZE, fin);
            
            tokens = CommonUtil::StringTokenize(buf);
            
            if (tokens[0] == "#")
                continue;
            set_labels.insert(tokens[0].substr(1));
        }
        fclose(fin);
        vector<string> labels(set_labels.begin(), set_labels.end());
        return labels;
    }
    
//    int DataSet::LoadGraph(ifstream &file, FactorGraph *graph) {
//        string line;
//        
//        // Read number of variables.
//        getline(file, line);
//        if (file.eof()) {
//            return -1;
//        }
//        TrimComments("#", &line);
//        int num_vars = atoi(line.c_str());
//        
//        // Read number of factors.
//        getline(file, line);
//        TrimComments("#", &line);
//        int num_factors = atoi(line.c_str());
//        
//        // Read variables
//        vector<BinaryVariable*> variables(num_vars);
//        for (int i = 0; i < num_vars; i++) {
//            getline(file, line);
//            TrimComments("#", &line);
//            BinaryVariable* var = graph->CreateBinaryVariable();
//            
//            char label_type = tokens[0][0];
//            string label_name = tokens[0].substr(1);
//            
//            // Positive
//            if (label_type == "+") {
//                BinaryVariable* var = graph->CreateBinaryVariable();
//                var->SetLabel(1);
//            }
//            // Negative
//            else if (label_type == "-") {
//                var->SetLabel(0);
//            }
//            // Unlabeled
//            else if (label_type == "?") {
//                var->SetLabel(-1);
//            }
//            else {
//                fprintf(stderr, "Data format wrong! Label must start with +/?\n");
//                exit(0);
//            }
//            
//            for (int i = 0; i < tokens.size(); i++) {
//                
//            }
//            
//            variables[i] = var;
//        }
    
        // Read factors.
        
//        for (int i = 0; i < num_factors; i++) {
//            getline(file, line);
//            TrimComments("#", &line);
//            vector<string> fields;
//            StringSplit(line, "\t ", &fields);
//            
//            // Read linked variables.
//            int offset = 1;
//            int num_links = atoi(fields[1].c_str());
//            vector<BinaryVariable*> binary_vars(num_links);
//            vector<bool> labels(num_links, false);
//            offset++;
//            
//            for (int j = 0; j < num_links; j++) {
//                int k = atoi(fields[offset+j].c_str());
//                binary_vars[j] = variables[k];
//            }
//            
//            // Read factor type.
//            Factor *factor;
//            if (fields[0] == "XOR") {
//                factor = graph->CreateFactorXOR(binary_vars);
//            } else if (fields[0] == "ATMOSTONE") {
//                factor = graph->CreateFactorAtMostOne(binary_vars);
//            } else if (fields[0] == "PAIR") {
//                factor = graph->CreateFactorPAIR(binary_vars);
//            }
//        }
//        
//        cout << "Read " << num_vars << " variables and " << num_factors << " factors." << endl;
//        
//        return 0;
//    }
    
    void DataSet::LoadData(const char* data_file, FactorGraph *graph) {
        char buf[MAX_BUF_SIZE];
        char* eof;
        
        vector<string> tokens;
        
        vector<string> labels = GetLabels(data_file);
        for (int i = 0; i < labels.size(); i++) {
            label_dict.GetId(labels[i]);
        }
        FILE* fin = fopen(data_file, "r");
        
        for (;;) {
            eof = fgets(buf, MAX_BUF_SIZE, fin);
            
            // # Factor Type NUM_VAR Variables
            if (tokens[0] == "#PAIR") {
                int a = atoi(tokens[3].c_str());
                int b = atoi(tokens[4].c_str());
                vector<BinaryVariable*> binary_vars;
                vector<bool> negated;
                //binary_vars.push_back(a);
                //binary_vars.push_back(b);
                Factor* factor = graph->CreateFactorXOR(binary_vars, negated);
                factor->name = factor_type_dict.GetId(tokens[1]);
            }
            else if (tokens[0] == "#ATMOST1") {
                vector<int> vars;
                vector<BinaryVariable*> binary_vars;
                vector<bool> negated;
                int num_vars = atoi(tokens[1].c_str());
                vars.resize(num_vars, 0);
                for(int i = 0; i < num_vars; i++) {
                    vars[i] = atoi(tokens[3+i].c_str());
                }
                Factor* factor = graph->CreateFactorAtMostOne(binary_vars, negated);
                factor->name = factor_type_dict.GetId(tokens[1]);
                
            }
            else if (tokens[0] == "#TRAN") {
                int a = atoi(tokens[1].c_str());
                int b = atoi(tokens[2].c_str());
                int c = atoi(tokens[3].c_str());
                //Factor* factor = graph->CreateFactorDense();
                //factor->type = factor_type_dict.GetId(tokens[1]);
                
            }
            // Read Variables
            // LABEL_TYPE LABEL NUM_FEATURE FEATURES
            else {
                char label_type = tokens[0][0];
                string label_name = tokens[0].substr(1);
                BinaryVariable *var = graph->CreateBinaryVariable();
                // Labeled
                if (label_type == '+') {
                    var->IsLabeled(true);
                }
                // Unlabeled
                else if (label_type == '?') {
                    var->IsLabeled(false);
                }
                else {
                    fprintf(stderr, "Data format wrong! Label must start with +/?\n");
                    exit(0);
                }
                
                for (int i = 0; i < tokens.size(); i++) {
                    vector<string> key_val = CommonUtil::StringSplit(tokens[i], ':');
                    var->features.push_back( feature_type_dict.GetId(key_val[0]) );
                    var->values.push_back( atof(key_val[1].c_str()) );
                }
            }
        }
        
        num_label = label_dict.GetSize();
        num_feature_type = feature_type_dict.GetSize();
        num_factor_type = factor_type_dict.GetSize();
        if (num_factor_type == 0) {
            num_factor_type = 1;
        }
        
        fclose(fin);
    }
}
