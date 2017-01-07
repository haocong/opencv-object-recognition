//
//  config.cpp
//  MyFirstOpenCV
//
//  Created by haocong on 2017/1/6.
//  Copyright © 2017年 nankai. All rights reserved.
//

#include <iostream>
#include <string>
#include "config.h"

int config()
{
    ConfigFile cfg("/Users/haocong/Desktop/config.cfg");
    
    bool exists = cfg.keyExists("car");
    std::cout << "car key: " << std::boolalpha << exists << "\n";
    exists = cfg.keyExists("fruits");
    std::cout << "fruits key: " << exists << "\n";
    
    std::string someValue = cfg.getValueOfKey<std::string>("mykey", "Unknown");
    std::cout << "value of key mykey: " << someValue << "\n";
    std::string carValue = cfg.getValueOfKey<std::string>("car");
    std::cout << "value of key car: " << carValue << "\n";
    double doubleVal = cfg.getValueOfKey<double>("double");
    std::cout << "value of key double: " << doubleVal << "\n\n";
    
    return 0;
}
