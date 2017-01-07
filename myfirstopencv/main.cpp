//
//  main.cpp
//  MyFirstOpenCV
//
//  Created by haocong on 2017/1/6.
//  Copyright © 2017年 nankai. All rights reserved.
//

#include "json.hpp"
#include <fstream>

using namespace nlohmann;
using namespace std;

int main() {
    ifstream i("/Users/haocong/Desktop/config.json");
    json colorRange;
    i >> colorRange;
    cout << setw(2) << colorRange << endl;
    return 0;
}
