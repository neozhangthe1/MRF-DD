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
    Parser* parser = new Parser;
    FactorGraph* graph = new FactorGraph;
    parser->LoadData("example.txt", graph);
    graph->Train();
    graph->Inference();
}