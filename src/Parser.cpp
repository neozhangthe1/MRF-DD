#include "Parser.h"
#include "FactorGraph.h"
#include "Factor.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include "Util.h"

#define MAX_BUF_SIZE 65536

using namespace std;

namespace MRF_DD {
    vector<string> Parser::GetLabels(const char* data_file) {
        char buf[MAX_BUF_SIZE];
        char *eof;
        set<string> set_labels;
        vector<string> tokens;
        
        FILE* fin = fopen(data_file, "r");
        while (true) {
            eof = fgets(buf, MAX_BUF_SIZE, fin);
            if (eof == NULL) {
                break;
            }
            
            tokens = CommonUtil::StringTokenize(buf);
            
            if (tokens[0] == "#") {
                continue;
            }
            set_labels.insert(tokens[0].substr(1));
        }
        fclose(fin);
        vector<string> labels(set_labels.begin(), set_labels.end());
        return labels;
    }
    
    bool Parser::UpdateFactor(const string& key, int& index)
    {
        auto it = factor_index.find(key);
        if(it != factor_index.end()) {
            index = it->second;
            return false;
        } else {
            factor_index[key] = factor_index.size();
            return true;
        }
    }
    
    
    void Parser::LoadData(const char* data_file, FactorGraph *graph) {
        cout << "Loading file" << endl;
        char buf[MAX_BUF_SIZE];
        char* eof;
        vector<string> tokens;

        vector<BinaryVariable*> variables;
        vector<BinaryVariable*> vars;
        
        vector<string> feature_keys = GetLabels(data_file);
        cout << feature_keys.size() << " features " << endl;
        
        for (int i = 0; i < feature_keys.size(); i++) {
            graph->feature_keys.push_back(feature_keys[i]);
            feature_index.insert(make_pair(feature_keys[i], i));
        }
        FILE* fin = fopen(data_file, "r");
        
        while (true) {
            eof = fgets(buf, MAX_BUF_SIZE, fin);
            if (eof == NULL) {
                cout << "Loading file finished" << endl;
                break;
            }
            vars.clear();
            // # Factor Type NUM_VAR Variables
            if (tokens[0] == "#edge") {

                int a = atoi(tokens[1].c_str());
                int b = atoi(tokens[2].c_str());

                vars.push_back(variables[a]);
                vars.push_back(variables[b]);
                
                int index = 0;
                bool created = UpdateFactor(tokens[3], index);
                if (created) {
                    graph->factor_keys.push_back(tokens[3]);
                    graph->factor_type.push_back(FactorTypes::FACTOR_DENSE);
                    graph->factor_type.push_back(2);
                }

                graph->CreateFactorDense(vars, index);
            } else if (tokens[0] == "#atmost1") {
                int num_vars = atoi(tokens[1].c_str());
                vars.resize(num_vars, 0);
                for(int i = 0; i < num_vars; i++) {
                    vars.push_back(variables[atoi(tokens[2+i].c_str())]);
                }
                
                int index = 0;
                bool created = UpdateFactor(tokens[3], index);
                if (created) {
                    graph->factor_keys.push_back(tokens[3]);
                    graph->factor_type.push_back(FactorTypes::FACTOR_DENSE);
                    graph->factor_type.push_back(2);
                }
                
                graph->CreateFactorAtMostOne(vars, index);
            }
            // Read Variables
            // LABEL_TYPE LABEL NUM_FEATURE FEATURES
            else {
                char label_type = tokens[0][0];
                char label_value = tokens[0][1];
                
                auto var = graph->CreateBinaryVariable();
                var->SetId(variables.size());
                // Labeled
                if (label_type == '+') {
                    var->SetType(VariableTypes::VAR_TRAINING);
                }
                // Unlabeled
                else if (label_type == '?') {
                    var->SetType(VariableTypes::VAR_TEST);
                }
                else {
                    fprintf(stderr, "Data format wrong! Label must start with +/?\n");
                    exit(0);
                }
                
                if (label_value == '0') {
                    var->SetValue(-1);
                } else {
                    var->SetValue(1);
                }
                
                for (int i = 1; i < tokens.size(); i++) {
                    vector<string> key_val = CommonUtil::StringSplit(tokens[i], ':');
                    var->features.push_back(feature_index[key_val[0]]);
                    int val = atof(key_val[1].c_str());
                    var->feature_values[i-1] = val;
                }
                variables.push_back(var);
            }
        }
        

        fclose(fin);
    }
}
