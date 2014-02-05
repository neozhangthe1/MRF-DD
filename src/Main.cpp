//
//  File.cpp
//  
//
//  Created by Yutao Zhang on 14-2-5.
//
//

#include "Parser.h"

using namespace MRF_DD;

int main(int argc, char* argv[]) {
    DataSet* ds;
    
    ds = new DataSet();
    FactorGraph* graph = new FactorGraph;
    ds->LoadData("example.txt", graph);
}